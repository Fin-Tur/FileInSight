#include "embedding_generator.h"
#include <fstream>
#include <models/semantic_info.h>
#include "semanticDB.h"
#include <assert.h>

embedding_generator::~embedding_generator() {
    if (ctx) llama_free(ctx);
    if (model) llama_free_model(model);
}

bool embedding_generator::initialize(const std::string& model_path){
    llama_model_params model_params = llama_model_default_params();

    model = llama_load_model_from_file(model_path.c_str(), model_params);
    if (!model) {
        return false;
    }

    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = 512; 
    ctx_params.n_batch = 512;
    ctx_params.embeddings = true; // Enable embedding extraction

    ctx = llama_new_context_with_model(model, ctx_params);
    return ctx != nullptr;
}

 std::vector<float> embedding_generator::generate_embedding(const char* input, size_t input_len) {
    std::vector<llama_token> tokens(input_len +1);
    

    //tokenize input (text)
    int n_tokens = llama_tokenize(llama_model_get_vocab(model), input, input_len, tokens.data(), tokens.size(), true, false);
    tokens.resize(n_tokens);

    //prepare batch
    llama_batch batch = llama_batch_init(tokens.size(), 0, 1);
    for(size_t i = 0; i < tokens.size(); ++i) {
        batch.token   [batch.n_tokens] = tokens[i];
        batch.pos     [batch.n_tokens] = i;
        batch.n_seq_id[batch.n_tokens] = 1;
        batch.seq_id  [batch.n_tokens][0] = 0;
        batch.logits  [batch.n_tokens] = false;
        batch.n_tokens++;
    }

    //calc embedding
    if (llama_decode(ctx, batch) != 0) {
        llama_batch_free(batch);
        return {};
    }

    //extract embed
    int n_embd = llama_n_embd(model);
    float* embd = llama_get_embeddings(ctx);
    
    std::vector<float> result(embd, embd + n_embd);
    llama_batch_free(batch);
    
    return result;
}

//TODO: overlapping chunks to ensure semantic meaning is kept across chunk boundaries
sem::sem_info_file embedding_generator::chunk_text(const FileInfo& fi, size_t chunk_size, size_t overlap) {
    std::ifstream file(fi.path);
    if (!file.is_open()) {
        return {};
    }
    size_t n_chunks = fi.size <= chunk_size ? 1 : ((fi.size - overlap) + (chunk_size - overlap) - 1) / (chunk_size - overlap);
    std::vector<sem::sem_info_chunk> sems(n_chunks);
    char* buffer = new char[chunk_size*n_chunks]; //Assuming 384 is the embedding size | allocated mem will be deleted in sem_info_file destructor
    file.read(buffer, fi.size);
    file.close();
    for(size_t i = 0; i < n_chunks; ++i) {
        sems[i] = (sem::sem_info_chunk{
            .chunk = buffer + i * (chunk_size - overlap),
            .chunk_size = std::min(chunk_size, fi.size - (i * (chunk_size - overlap))),
            .embedding = {0}
        });
    }
    return sem::sem_info_file{
        .content = buffer,
        .path = fi.path,
        .size = fi.size,
        .chunks = std::move(sems)
    };
}

float cosine_similiarity(const sem::sem_info_chunk& c1, const sem::sem_info_chunk& c2, size_t n_dims){
    float denom = sem::len(c1.embedding, n_dims) * sem::len(c2.embedding, n_dims);
    if (denom == 0.0f) return 0.0f;
    return sem::dot(c1.embedding, c2.embedding, n_dims) / denom;
}

bool embedding_generator::add_text_semantics_to_database(const FileInfo& fi, size_t chunk_size){
    SemanticDB db;
    if (!db.open("file_semantics.db")) return false;
    if (!db.init_schema()) return false;
    if (db.file_exists(fi)) return true; 

    sem::sem_info_file sem_file = chunk_text(fi, chunk_size, chunk_size/6);
    for(auto& chunk : sem_file.chunks) {
        std::vector<float> embedding = generate_embedding(chunk.chunk, chunk.chunk_size);
        std::copy(embedding.begin(), embedding.end(), chunk.embedding);
    }

    int64_t file_id = db.insert_file(fi);
    if (file_id < 0) return false;

    return db.insert_chunks(file_id, sem_file.chunks);
}

float embedding_generator::compare_file_semantics(const FileInfo& fi1, const FileInfo& fi2){
    SemanticDB db;
    if (!db.open("file_semantics.db")) return 0.0f;

    int64_t file_id1 = db.get_file_id(fi1);
    int64_t file_id2 = db.get_file_id(fi2);
    if (file_id1 < 0 || file_id2 < 0) return 0.0f;

    auto chunks1 = db.get_chunks_by_file_id(file_id1, nullptr);
    auto chunks2 = db.get_chunks_by_file_id(file_id2, nullptr);

    if (chunks1.empty() || chunks2.empty()) return 0.0f;

    // Kleinere Datei als Quelle
    const auto& src = chunks1.size() <= chunks2.size() ? chunks1 : chunks2;
    const auto& tgt = chunks1.size() <= chunks2.size() ? chunks2 : chunks1;

    float sum = 0.0f;
    for (const auto& s : src) {
        float best = -1.0f;
        for (const auto& t : tgt)
            best = std::max(best, cosine_similiarity(s, t, 384));
        sum += best;
    }
    return sum / static_cast<float>(src.size());
}