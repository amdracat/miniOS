CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c99
LDFLAGS = -lpthread

.PHONY: all clean

all:
	$(CC) $(CFLAGS) -c ./miniOS/MiniOS.c -o ./miniOS/MiniOS.o
	$(CC) $(CFLAGS) -c ./User/main.c -o ./User/main.o
	$(CC) $(CFLAGS) -c ./User/Module.c -o ./User/Module.o
	$(CC) $(CFLAGS) -c ./User/test_frame.c -o ./User/test_frame.o
	$(CC) $(CFLAGS) -c ./User/Test_Module.c -o ./User/Test_Module.o
	$(CC) $(CFLAGS) -o ./minios ./miniOS/MiniOS.o ./User/main.o ./User/Module.o ./User/test_frame.o ./User/Test_Module.o $(LDFLAGS)

clean:
	rm -f ./minios.exe ./miniOS/MiniOS.o ./User/main.o ./User/Module.o ./User/test_frame.o ./User/Test_Module.o
