load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")

# Compile the SQLite3 library
cc_library(
    name = "sqlite3",
    srcs = ["sqlite/sqlite3.c"],
    hdrs = ["sqlite/sqlite3.h"],  # Assuming you have a header file named sqlite3.h
    copts = [
        "-O3",
        "-ffunction-sections",
        "-fdata-sections",
        "-fPIC",
        "-m64",
        "-DSQLITE_CORE",
        "-DSQLITE_DEFAULT_FOREIGN_KEYS=1",
        "-DSQLITE_ENABLE_API_ARMOR",
        "-DSQLITE_ENABLE_COLUMN_METADATA",
        "-DSQLITE_ENABLE_DBSTAT_VTAB",
        "-DSQLITE_ENABLE_FTS3",
        "-DSQLITE_ENABLE_FTS3_PARENTHESIS",
        "-DSQLITE_ENABLE_FTS5",
        "-DSQLITE_ENABLE_JSON1",
        "-DSQLITE_ENABLE_LOAD_EXTENSION=1",
        "-DSQLITE_ENABLE_MEMORY_MANAGEMENT",
        "-DSQLITE_ENABLE_RTREE",
        "-DSQLITE_ENABLE_STAT2",
        "-DSQLITE_ENABLE_STAT4",
        "-DSQLITE_SOUNDEX",
        "-DSQLITE_THREADSAFE=0",
        "-DSQLITE_USE_URI",
        "-DHAVE_USLEEP=1",
        "-D_POSIX_THREAD_SAFE_FUNCTIONS",
        "-DHAVE_ISNAN",
        "-DHAVE_LOCALTIME_R",
        "-DSQLITE_ENABLE_UNLOCK_NOTIFY",
    ],
)

cc_library(
    name="commons", 
    hdrs=["common.hpp", "timer.hpp", "logger.hpp"],
    copts=[
        "-O3",
        "-ffunction-sections",
        "-fdata-sections",
        "-fPIC",
        "-m64",
    ],
    deps=[
        "@spdlog//:spdlog",
    ],
)

# Compile the naive binary
cc_binary(
    name = "naive",
    srcs = ["naive.cpp"],
    deps = [
        "commons",
        "sqlite3", 
    ],
    copts = [
        "-O3",
        "-ffunction-sections",
        "-fdata-sections",
        "-fPIC",
        "-m64",
    ],
    linkopts = ["-lpthread", "-ldl"],
)

# Compile the naive_with_locality binary
cc_binary(
    name = "naive_with_locality",
    srcs = ["naive_with_locality.cpp"],
    deps = [
        "commons",
        ":sqlite3",
    ],
    copts = [
        "-O3",
        "-ffunction-sections",
        "-fdata-sections",
        "-fPIC",
        "-m64",
        "-DLOG_LEVEL_INFO",
    ],
    linkopts = ["-lpthread", "-ldl"],
)
