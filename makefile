all: test_client

build_lib:
	gcc -c gui_lib.c  -Wall -Wextra -fno-common -std=c90 -o gui_lib.o
	gcc -c utility_lib.c -Wall -Wextra -fno-common -std=c90 -o utility_lib.o
	gcc -c game_lib.c -Wall -Wextra -fno-common -std=c90 -o game_lib.o
clean_lib:
	rm utility_lib.o
	rm game_lib.o
	rm gui_lib.o

build_server: build_lib
	gcc server.c utility_lib.o game_lib.o -std=c90 -Wall -Wextra -fno-common -o server.o
run_server:
	@echo ===
	@echo ====== TEST SERVER ======
	./server.o server -p=6900
	@echo ====== END TEST SERVER ======
	@echo ===
clean_server: clean_lib
	rm server.o
test_server: build_server run_server clean_server

build_client: build_lib
	gcc client.c gui_lib.o utility_lib.o -std=c90 -lglut -lGLU -lGL -lncurses -Wall -Wextra -fno-common -o client.o
run_client:
	@echo ===
	@echo ====== TEST CLIENT ======
	./client.o client -a=localhost -p=6900
	@echo ====== END TEST CLIENT ======
	@echo ===
clean_client: clean_lib
	rm client.o
test_client: build_client run_client clean_client

clean: clean_lib clean_client clean_server
