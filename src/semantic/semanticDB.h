#include "sqlite3.h"
#include <string>
#include <vector>
#include "../models/FileInfo.h"
#include "../models/semantic_info.h"
#include <cstring>

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

    int64_t insert_file(const FileInfo& fi){
        const char* sql = "INSERT OR IGNORE INTO files(path, size, modified) VALUES (?, ?, ?);";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return -1;
        std::string p = fi.path.string();
        sqlite3_bind_text(stmt, 1, p.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int64(stmt, 2, static_cast<int64_t>(fi.size));
        sqlite3_bind_int64(stmt, 3, static_cast<int64_t>(fi.lastUsed));

        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) return -1;
        return sqlite3_last_insert_rowid(db);
    }

    bool insert_chunks(int64_t file_id, const std::vector<sem::sem_info_chunk>& chunks){
        const char* sql = "INSERT INTO chunks(file_id, offset, size, embedding) VALUES(?, ?, ?, ?);";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;

        sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

        int offset = 0;
        for (const auto& c : chunks) {
            sqlite3_bind_int64(stmt, 1, file_id);
            sqlite3_bind_int(stmt, 2, offset);
            sqlite3_bind_int(stmt, 3, static_cast<int>(c.chunk_size));
            sqlite3_bind_blob(stmt, 4, c.embedding, sizeof(c.embedding), SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
                return false;
            }
            sqlite3_reset(stmt);
            offset += static_cast<int>(c.chunk_size);
        }

        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        return true;
    }
    std::vector<sem::sem_info_chunk> get_chunks_by_file_id(int64_t file_id, float* embeddings){
        std::vector<sem::sem_info_chunk> result;
        const char* sql = "SELECT offset, size, embedding FROM chunks WHERE file_id = ? ORDER BY offset;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return result;

        sqlite3_bind_int64(stmt, 1, file_id);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            sem::sem_info_chunk chunk{};
            chunk.chunk_size = static_cast<size_t>(sqlite3_column_int(stmt, 1));

            const void* blob = sqlite3_column_blob(stmt, 2);
            int blob_size = sqlite3_column_bytes(stmt, 2);
            if (blob && blob_size == sizeof(chunk.embedding)) {
                std::memcpy(chunk.embedding, blob, sizeof(chunk.embedding));
                if (embeddings) {
                    std::memcpy(chunk.embedding, blob, sizeof(chunk.embedding));
                }
            }

            result.push_back(chunk);
        }

        sqlite3_finalize(stmt);
        return result;
    }
    
    bool file_exists(const FileInfo& fi){
        const char* sql = "SELECT 1 FROM files WHERE path = ? LIMIT 1;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;

        std::string p = fi.path.string();
        sqlite3_bind_text(stmt, 1, p.c_str(), -1, SQLITE_TRANSIENT);

        bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        return exists;
    }

    int64_t get_file_id(const FileInfo& fi) {
    const char* sql = "SELECT id FROM files WHERE path = ? LIMIT 1;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return -1;
        std::string p = fi.path.string();
        sqlite3_bind_text(stmt, 1, p.c_str(), -1, SQLITE_TRANSIENT);
        int64_t id = -1;
        if (sqlite3_step(stmt) == SQLITE_ROW)
            id = sqlite3_column_int64(stmt, 0);
        sqlite3_finalize(stmt);
        return id;
    }

};