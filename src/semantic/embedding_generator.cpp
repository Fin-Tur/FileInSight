#include "embedding_generator.h"

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
