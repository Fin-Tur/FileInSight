#include "llama.h"
#include <string>
#include <vector>

class embedding_generator {
    private:
    llama_model* model = nullptr;
    llama_context* ctx = nullptr;

    public:
    embedding_generator() = default;
    bool initialize(const std::string& model_path);
    std::vector<float> generate_embedding(const std::string& input);
    ~embedding_generator() {};
};