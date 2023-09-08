#include <iostream> 
#include <sqlite3.h>
#include <chrono>

#include "timer.hpp"
#include "common.hpp"

#define NUM_ROWS 100*1000*1000
#define BATCH_SIZE 50


int main() {
    Timer t;

    Row* dataset;
    generateData(NUM_ROWS, &dataset);
    // std::cout << "Generated " << NUM_ROWS << " rows in " << t.lap() << "s" << std::endl;

    sqlite3 *db;
    RAISE_ON_DB_ERROR(sqlite3_open("test.db", &db));
    // std::cout << "Opened database successfully" << std::endl;

    initializeDB(db);
    // std::cout << "Initialized database successfully in " << t.lap() << "s" << std::endl;

    createTable(db);
    // std::cout << "Table created successfully in " << t.lap() << "s" << std::endl; 
    
    RAISE_ON_DB_ERROR(sqlite3_exec(db, "BEGIN", NULL, NULL, NULL));
    // std::cout << "Transaction started successfully in " << t.lap() << "s" << std::endl;

    sqlite3_stmt *insertStmt;
    std::string insertQuery;
    insertQuery.reserve(32*1000*1000);
    insertQuery = "INSERT INTO user VALUES ";
    for(int i=0; i<BATCH_SIZE; i++) {
        insertQuery += "(NULL, ?, ?, ?)";
        if(i != BATCH_SIZE-1) {
            insertQuery += ",";
        } else {
            insertQuery += ";";
        }
    }
    // std::cout << "Generated insert statement in " << t.lap() << "s" << std::endl;

    RAISE_ON_DB_ERROR(sqlite3_prepare_v2(db, insertQuery.c_str(), -1, &insertStmt, NULL));
    // std::cout << "Prepared insert statement in " << t.lap() << "s" << std::endl;

    int numBatches = NUM_ROWS/BATCH_SIZE;
    for(int i=0; i<numBatches; i++) {
        for(int j=0; j<BATCH_SIZE; j++) {
            Row& currRow = dataset[i*BATCH_SIZE + j];
            const char* currArea = currRow.withArea ? currRow.area : nullptr;
            int currAreaLen = currRow.withArea ? 6 : -1;
            
            RAISE_ON_DB_ERROR(sqlite3_bind_text(insertStmt, (j*3)+1, currArea, currAreaLen, SQLITE_STATIC));
            RAISE_ON_DB_ERROR(sqlite3_bind_int(insertStmt, (j*3)+2, currRow.age));
            RAISE_ON_DB_ERROR(sqlite3_bind_int(insertStmt, (j*3)+3, currRow.active));
        }
        // std::cout << "Bound batch " << i+1 << "/" << numBatches << " in " << t.lap() << "s" << std::endl;
        RAISE_ON_DB_ERROR(sqlite3_step(insertStmt));
        // std::cout << "Inserted batch " << i+1 << "/" << numBatches << " in " << t.lap() << "s" << std::endl;
        RAISE_ON_DB_ERROR(sqlite3_reset(insertStmt));
    }
    RAISE_ON_DB_ERROR(sqlite3_exec(db, "COMMIT", NULL, NULL, NULL));
    std::cout << "Transaction committed successfully in " << t.lap() << "s" << std::endl;
    sqlite3_finalize(insertStmt);
    sqlite3_close(db);
}