
#include "common.hpp"
#include "timer.hpp"
#include "logger.hpp"

#define NUM_ROWS 100*1000*1000
#define BATCH_SIZE 50

int main() {
    spdlog::set_level(spdlog::level::info);

    Timer t;

    sqlite3 *db;
    RAISE_ON_DB_ERROR(sqlite3_open("test.db", &db));
    LOG_INFO("Opened database successfully");

    initializeDB(db);
    LOG_DEBUG("Initialized database successfully in {}ms", t.lap());

    createTable(db);
    LOG_DEBUG("Table created successfully in {}ms", t.lap());
    
    RAISE_ON_DB_ERROR(sqlite3_exec(db, "BEGIN", NULL, NULL, NULL));
    LOG_DEBUG("Transaction started successfully in {}ms", t.lap());

    sqlite3_stmt *insertWithAreaStmt;
    std::string insertWithAreaQuery;
    insertWithAreaQuery = "INSERT INTO user VALUES ";
    for(int i=0; i<BATCH_SIZE; i++) {
        insertWithAreaQuery += "(NULL, ?, ?, ?)";
        if(i != BATCH_SIZE-1) {
            insertWithAreaQuery += ",";
        } else {
            insertWithAreaQuery += ";";
        }
    }
    LOG_DEBUG("Generated insert statement in {}ms", t.lap());
    RAISE_ON_DB_ERROR(sqlite3_prepare_v2(db, insertWithAreaQuery.c_str(), -1, &insertWithAreaStmt, NULL));
    LOG_DEBUG("Prepared insert statement in {}ms", t.lap());

    sqlite3_stmt *insertWithoutAreaStmt;
    std::string insertWithoutAreaQuery;
    insertWithoutAreaQuery = "INSERT INTO user VALUES ";
    for(int i=0; i<BATCH_SIZE; i++) {
        insertWithoutAreaQuery += "(NULL, NULL, ?, ?)";
        if(i != BATCH_SIZE-1) {
            insertWithoutAreaQuery += ",";
        } else {
            insertWithoutAreaQuery += ";";
        }
    }
    LOG_DEBUG("Generated insert statement in {}ms", t.lap());
    RAISE_ON_DB_ERROR(sqlite3_prepare_v2(db, insertWithoutAreaQuery.c_str(), -1, &insertWithoutAreaStmt, NULL));
    LOG_DEBUG("Prepared insert statement in {}ms", t.lap());

    char area[7];
    int numBatches = NUM_ROWS/BATCH_SIZE;
    for(int i=0; i<numBatches; i++) {
        bool withArea = getRandomBool();
        sqlite3_stmt *stmt = withArea ? insertWithAreaStmt : insertWithoutAreaStmt;
        for(int j=0; j<BATCH_SIZE; j++) {
            int age = getRandomAge();
            bool active = getRandomActive();
            if(withArea) {
                getRandomArea(area);
                RAISE_ON_DB_ERROR(sqlite3_bind_text(stmt, (j*3)+1, area, 6, SQLITE_STATIC));
                RAISE_ON_DB_ERROR(sqlite3_bind_int(stmt, (j*3)+2, age));
                RAISE_ON_DB_ERROR(sqlite3_bind_int(stmt, (j*3)+3, active));
            } else {
                RAISE_ON_DB_ERROR(sqlite3_bind_int(stmt, (j*2)+1, age));
                RAISE_ON_DB_ERROR(sqlite3_bind_int(stmt, (j*2)+2, active));
            }
        }
        LOG_DEBUG("Bound batch {}/{} in {}ms", i+1, numBatches, t.lap());
        RAISE_ON_DB_ERROR(sqlite3_step(stmt));\
        LOG_DEBUG("Inserted batch {}/{} in {}ms", i+1, numBatches, t.lap());
        RAISE_ON_DB_ERROR(sqlite3_reset(stmt));
    }
    RAISE_ON_DB_ERROR(sqlite3_exec(db, "COMMIT", NULL, NULL, NULL));
    LOG_DEBUG("Transaction committed successfully in {}ms", t.lap());
    LOG_INFO("Total time taken: {}ms", t.stop().second);
    sqlite3_finalize(insertWithAreaStmt);
    sqlite3_finalize(insertWithoutAreaStmt);
    sqlite3_close(db);
}