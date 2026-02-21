#include "llama.h"
#include <string>
#include <vector>
#include <models/FileInfo.h>

class embedding_generator {
    private:
    llama_model* model = nullptr;
    llama_context* ctx = nullptr;

    public:
    embedding_generator() = default;
    bool initialize(const std::string& model_path);
    std::vector<float> generate_embedding(const std::string& input);
    float cosine_similiarity(const sem::sem_info_chunk& c1, const sem::sem_info_chunk& c2, size_t n_dims);
    sem::sem_info_file chunk_text(const FileInfo& fi, size_t chunk_size, size_t overlap);

    bool add_text_semantics_to_database(const FileInfo& fi, size_t chunk_size=512);
    
    ~embedding_generator() {};
};