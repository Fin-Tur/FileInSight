#include "sqlite3.h"
#include <string>
#include <vector>
#include <FileInfo.h>
#include <semantic_info.h>

class SemanticDB {
    private:
    sqlite3* db = nullptr;

    public:
    bool open(const std::string& path){
        return sqlite3_open(path.c_str(), &db) == SQLITE_OK;
    }

    bool init_schema() {
        const char* sql =
            "CREATE TABLE IF NOT EXISTS files ("
            "  id INTEGER PRIMARY KEY,"
            "  path TEXT UNIQUE NOT NULL,"
            "  size INTEGER,"
            "  modified INTEGER);"
            "CREATE TABLE IF NOT EXISTS chunks ("
            "  id INTEGER PRIMARY KEY,"
            "  file_id INTEGER REFERENCES files(id) ON DELETE CASCADE,"
            "  offset INTEGER,"
            "  size INTEGER,"
            "  embedding BLOB);"
            "CREATE INDEX IF NOT EXISTS idx_chunks_file ON chunks(file_id);";
        return sqlite3_exec(db, sql, nullptr, nullptr, nullptr) == SQLITE_OK;
    }

    ~SemanticDB() {
        if (db) sqlite3_close(db);
    }

    int64_t insert_file(const FileInfo& fi){}
    bool insert_chunks(int64_t file_id, const std::vector<sem::sem_info_chunk>& chunks){}
    std::vector<sem::sem_info_chunk> get_chunks_by_file_id(int64_t file_id, float* embeddings, size_t n_chunks){}
    bool file_exists(const FileInfo& fi){}

};