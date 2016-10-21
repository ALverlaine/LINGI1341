# Compiler
CC = gcc
# See gcc/clang manual to understand all flags
CFLAGS += -std=c99 # Define which version of the C standard to use
CFLAGS += -Wall # Enable the 'all' set of warnings
CFLAGS += -Werror # Treat all warnings as error
CFLAGS += -Wshadow # Warn when shadowing variables
CFLAGS += -Wextra # Enable additional warnings
CFLAGS += -O2 -D_FORTIFY_SOURCE=2 # Add canary code, i.e. detect buffer overflows
CFLAGS += -fstack-protector-all # Add canary code to detect stack smashing

# We have no libraries to link against except libc
LDFLAGS= -lz

# Directory
DIR = src

# DESPS
DEPS = src/packet_interface.h src/create_socket.h src/read_write_loop.h src/real_address.h src/wait_for_client.h

# Object
OBJ = src/packet_implem.o src/create_socket.o src/read_write_loop.o src/real_address.o src/wait_for_client.o 

# Default target
all: clean sender receiver

# If we run `make debug` instead, keep the debug symbols for gdb
# and define the DEBUG macro.
debug: CFLAGS += -g -DDEBUG -Wno-unused-parameter
debug: clean sender

# We use an implicit rule: look for the files record.c/database.c,
# compile them and link the resulting *.o's into an executable named database
%.o: $(DIR)/%.c  $(DEPS)
	$(CC) -c $@ $< $(CFLAGS)
sender: $(OBJ) src/sender.o 
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
receiver: $(OBJ) src/receiver.o 
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(DIR)/*.o server receiver test
