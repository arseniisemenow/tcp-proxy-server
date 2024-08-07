# Current machine is MacOS Ventura 13.6.7

SERVER_EXECUTABLE=proxy-server
CLIENT_EXECUTABLE=client
TEST_EXECUTABLE=tests
SRC_DIR=src
BUILD_DIR=build-debug

clean:
	rm -rf ${BUILD_DIR}

build:
	cmake -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) -j 8

rebuild: clean build

test: rebuild
	./${BUILD_DIR}/${TEST_EXECUTABLE}

run-server: rebuild
	./${BUILD_DIR}/${SERVER_EXECUTABLE}

run-client:rebuild
	./${BUILD_DIR}/${CLIENT_EXECUTABLE}

# For PostgreSQL usage
initdb:
	@echo "Initializing the database..."
	@psql -h $(DB_HOST) -p $(DB_PORT) -U $(DB_ADMIN_USER) -f $(SQL_INIT_SCRIPT)