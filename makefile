all: test_server

build_lib:
	gcc -c stuffchiks_lib.c -Wall -Wextra -fno-common -std=c90 -o stuffchiks_lib.o
clean_lib:
	rm stuffchiks_lib.o

build_server: build_lib
	gcc server.c stuffchiks_lib.o -std=c90 -Wall -Wextra -fno-common -o server.o
run_server:
	@echo ===
	@echo ====== TEST SERVER ======
	./server.o server -h=localhost -p=12345
	@echo ====== END TEST SERVER ======
	@echo ===
clean_server: clean_lib
	rm server.o
test_server: build_server run_server clean_server

build_client: build_lib
	gcc client.c stuffchiks_lib.o -std=c90 -Wall -Wextra -fno-common -o client.o
run_client:
	@echo ===
	@echo ====== TEST CLIENT ======
	./client.o client -a=123.123.123.123 -p=12345
	@echo ====== END TEST CLIENT ======
	@echo ===
clean_client: clean_lib
	rm client.o
test_client: build_client run_client clean_client
