# Current machine: MacOS Ventura 13.6.7

SERVER_EXECUTABLE=proxy-server
CLIENT_EXECUTABLE=client
CLIENT_MANUAL_EXECUTABLE=client-manual
TEST_EXECUTABLE=tests
SRC_DIR=src
BUILD_DIR=build-debug

clean:
	rm -rf ${BUILD_DIR}

build:
	cmake -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

rebuild: clean build

test: rebuild
	./${BUILD_DIR}/${TEST_EXECUTABLE}

run-server:
	./${BUILD_DIR}/${SERVER_EXECUTABLE}
run-client:
	./${BUILD_DIR}/${CLIENT_EXECUTABLE}
run-client-manual:
	./${BUILD_DIR}/${CLIENT_MANUAL_EXECUTABLE}
run-test:
	./${BUILD_DIR}/${TEST_EXECUTABLE}

# For PostgreSQL usage
initdb:
	@echo "Initializing the database..."
	@psql -h $(DB_HOST) -p $(DB_PORT) -U $(DB_ADMIN_USER) -f $(SQL_INIT_SCRIPT)