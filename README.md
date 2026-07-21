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
var db = Pg_connect("host=localhost dbname=mydb user=postgres")
var rows = Pg_query(db, "SELECT name FROM users")
Pg_exec(db, "INSERT INTO users (name) VALUES ($1)", ["Alice"])
Pg_close(db)
```

## Test

```bash
wyn run tests/test_pg.wyn
```
