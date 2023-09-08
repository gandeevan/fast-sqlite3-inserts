#include <iostream> 
#include <sqlite3.h>
#include <chrono>

#define RAISE_ON_DB_ERROR(rc) \
    if(rc) { \
        if(rc != SQLITE_DONE) { \
            throw std::runtime_error("SQL error at " __FILE__ ":" + std::to_string(__LINE__) + " - " + sqlite3_errmsg(db)); \
        } \
    }

int getRandomNumber(int min, int max) {
    return min + rand()%(max-min+1);
}

bool getRandomBool() {
    return rand()%2;
}

int getRandomAge() {
    int ages[3] = {5, 10, 15};
    return ages[getRandomNumber(0, 2)];
}


bool getRandomActive() {
    return getRandomBool();
}

void getRandomArea(char* area) {
    int areaCode = getRandomNumber(100000, 999999);
    sprintf(area, "%06d", areaCode); 
}


struct Row {
    char area[7];
    int age;
    bool active;
    bool withArea;
};

void generateData(int numRows, Row** rows) {
    *rows = new Row[numRows];
    for(int i=0; i<numRows; i++) {
        getRandomArea((*rows)[i].area);
        (*rows)[i].age = getRandomAge();
        (*rows)[i].active = getRandomActive();
        (*rows)[i].withArea = rand()%2;
    }
}

void initializeDB(sqlite3* db) {
    RAISE_ON_DB_ERROR(sqlite3_exec(db, "PRAGMA journal_mode = OFF", NULL, NULL, NULL));
    RAISE_ON_DB_ERROR(sqlite3_exec(db, "PRAGMA synchronous = 0", NULL, NULL, NULL));
    RAISE_ON_DB_ERROR(sqlite3_exec(db, "PRAGMA locking_mode = EXCLUSIVE", NULL, NULL, NULL));
    RAISE_ON_DB_ERROR(sqlite3_exec(db, "PRAGMA cache_size = 1000000", NULL, NULL, NULL));
    RAISE_ON_DB_ERROR(sqlite3_exec(db, "PRAGMA temp_store = MEMORY", NULL, NULL, NULL));
}

void  createTable(sqlite3* db) {
    const char* sql = 
        "CREATE TABLE IF NOT EXISTS user ("
        "id INTEGER NOT NULL PRIMARY KEY,"
        "area CHAR(6),"
        "age INTEGER NOT NULL,"
        "active INTEGER NOT NULL);";

    RAISE_ON_DB_ERROR(sqlite3_exec(db, sql, NULL, NULL, NULL));
}