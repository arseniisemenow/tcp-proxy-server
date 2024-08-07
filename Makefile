# Current machine is MacOS Ventura 13.6.7

EXECUTABLE=TCPProxyServer
SRC_DIR=src
BUILD_DIR=build
SQL_INIT_SCRIPT=init_db.sql
DB_HOST=localhost
DB_PORT=5432
DB_ADMIN_USER=postgres

# Not required now. May took some time...
install_boost:
	brew install boost

install_libpqxx:
	brew install libpqxx


build:
	cmake -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)


# Initialize database
initdb:
	@echo "Initializing the database..."
	@psql -h $(DB_HOST) -p $(DB_PORT) -U $(DB_ADMIN_USER) -f $(SQL_INIT_SCRIPT)