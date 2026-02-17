#include "embedding_generator.h"
#include <fstream>
#include <models/semantic_info.h>

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

 std::vector<float> embedding_generator::generate_embedding(const std::string& input) {
    std::vector<llama_token> tokens(input.size() +1);

    //tokenize input (text)
    int n_tokens = llama_tokenize(llama_model_get_vocab(model), input.c_str(), input.length(), tokens.data(), tokens.size(), true, false);
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
sem::sem_info_file embedding_generator::chunk_text(const FileInfo& fi, size_t chunk_size){
    std::ifstream file(fi.path);
    if (!file.is_open()) {
        return {};
    }
    size_t n_chunks = (fi.size + chunk_size - 1) / chunk_size;
    std::vector<sem::sem_info_chunk> sems(n_chunks);
    char* buffer = new char[chunk_size*n_chunks]; //Assuming 384 is the embedding size | allocated mem will be deleted in sem_info_file destructor
    file.read(buffer, fi.size);
    file.close();
    for(size_t i = 0; i < n_chunks; ++i) {
        sems[i] = (sem::sem_info_chunk{
            .chunk = buffer + i * chunk_size,
            .chunk_size = std::min(chunk_size, fi.size - i * chunk_size),
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