# pg - Official Wyn Package

PostgreSQL driver for Wyn. Wraps libpq.

## Install

```bash
# Install libpq first
brew install libpq        # macOS
apt install libpq-dev     # Ubuntu/Debian

# Then install the package
wyn pkg install github.com/wynlang/pg
```

## Usage

```wyn
import pg

var db = pg.Pg_connect("host=localhost dbname=mydb user=postgres")
pg.Pg_exec(db, "INSERT INTO users (name) VALUES ($1)", ["Alice"])
var rows = pg.Pg_query(db, "SELECT name FROM users")
print(rows)
pg.Pg_close(db)
```

## API

| Function | Description |
|----------|-------------|
| `Pg_connect(conninfo)` | Connect (libpq conninfo string), returns handle |
| `Pg_query(conn, sql)` | Run SQL, returns rows as newline/tab-separated string |
| `Pg_exec(conn, sql, params)` | Run SQL with `$1, $2, ...` placeholders substituted from `params` |
| `Pg_ping(conn)` | True if the connection answers `SELECT 1` |
| `Pg_close(conn)` | Close connection |

## Test

```bash
wyn run tests/test_pg.wyn
```
