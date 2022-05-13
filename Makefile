CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread -lm
INCLUDE_HEADERS_DIRECTORY=-Iheaders

fec:  src/system.c src/process_block.c src/get_info.c src/main.c    # add your other object files needed to compile your program here. !! The ordering is important !! if file_a.o depends on file_b.o, file_a.o must be placed BEFORE file_b.o in the list !
	@$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -O3 -o $@ $^ $(LIBS)    # this will run the following command: gcc -Wall -Werror -g -o kmeans src/distance.o other_object_filespresent_above.o ... -lcunit -lpthread
	@ echo "pogram compiled"
%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
	@$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

clean :
	@rm -f src/*.o
	@rm -f fec
	@rm -f test_tinymt32

	@rm -f test_processe_block
	@rm -f test_get_info
	@rm -f test_system
	@echo "directory cleaned from .c ,test and txt files "

tests: 
	$(CC) -o test_system tests/test_system.c -lcunit
	./test_system
	$(CC) -o test_get_info tests/test_get_info.c -lcunit
	./test_get_info
	$(CC) -o test_processe_block tests/test_processe_block.c -lcunit
	./test_processe_block



# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests

