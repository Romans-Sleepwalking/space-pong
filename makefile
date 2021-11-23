all: test_server

build_server:
	gcc server.c -std=c90 -Wall -Wextra -fno-common -o server.o
run_server:
	@echo ====== TEST SERVER ======
	./server.o -h=localhost -p=12345
	@echo ====== END TEST SERVER ======
clean_server:
	rm server.o
test_server: build_server run_server clean_server

build_client:
	gcc client.c -std=c90 -Wall -Wextra -fno-common -o client.o
run_client:
	@echo ====== TEST CLIENT ======
	./client.o client -a=123.123.123.123 -p=12345
	@echo ====== END TEST CLIENT ======
clean_client:
	rm client.o
test_client: build_client run_client clean_client
