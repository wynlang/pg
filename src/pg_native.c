// pg_native.c — C glue between Wyn and libpq
// Compile: cc -c pg_native.c -I$(pg_config --includedir) -o pg_native.o
// Link:    -lpq

#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Connection pool (simple array of PGconn pointers)
static PGconn* connections[64] = {0};
static int conn_count = 0;

long long pg_native_connect(const char* conninfo) {
    if (conn_count >= 64) return -1;
    PGconn* conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "pg: connection failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return -1;
    }
    int id = conn_count++;
    connections[id] = conn;
    return id;
}

const char* pg_native_query(long long id, const char* sql) {
    if (id < 0 || id >= conn_count || !connections[id]) return "";
    PGresult* res = PQexec(connections[id], sql);
    ExecStatusType status = PQresultStatus(res);
    
    if (status == PGRES_COMMAND_OK) {
        PQclear(res);
        return "OK";
    }
    if (status != PGRES_TUPLES_OK) {
        const char* err = PQerrorMessage(connections[id]);
        static char errbuf[1024];
        snprintf(errbuf, sizeof(errbuf), "ERROR: %s", err);
        PQclear(res);
        return errbuf;
    }
    
    // Build result string: rows as comma-separated values
    int nrows = PQntuples(res);
    int ncols = PQnfields(res);
    static char result[65536];
    int pos = 0;
    
    if (nrows == 1 && ncols == 1) {
        // Single value — return directly
        const char* val = PQgetvalue(res, 0, 0);
        snprintf(result, sizeof(result), "%s", val);
    } else {
        for (int r = 0; r < nrows && pos < 65000; r++) {
            if (r > 0) pos += snprintf(result + pos, sizeof(result) - pos, "\n");
            for (int c = 0; c < ncols && pos < 65000; c++) {
                if (c > 0) pos += snprintf(result + pos, sizeof(result) - pos, ",");
                pos += snprintf(result + pos, sizeof(result) - pos, "%s", PQgetvalue(res, r, c));
            }
        }
    }
    
    PQclear(res);
    return result;
}

void pg_native_close(long long id) {
    if (id >= 0 && id < conn_count && connections[id]) {
        PQfinish(connections[id]);
        connections[id] = NULL;
    }
}
