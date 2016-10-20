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
CFLAGS += -I$(HOME)/local/include

# We have no libraries to link against except libc
LDFLAGS += -lz 
LDFLAGS += -L$(HOME)/local/lib -lcunit

# Directory
DIR = src
DIR2 = tests

# DESPS
DEPS = src/packet_interface.h

# Object
OBJ = src/packet_implem.o src/sender.o 
OBJ2 = tests/test.o

# Default target
all: clean sender

# If we run `make debug` instead, keep the debug symbols for gdb
# and define the DEBUG macro.
debug: CFLAGS += -g -DDEBUG -Wno-unused-parameter
debug: clean database

# We use an implicit rule: look for the files record.c/database.c,
# compile them and link the resulting *.o's into an executable named database
%.o: $(DIR)/%.c  $(DEPS)
	$(CC) -c $@ $< $(CFLAGS)
%.o: $(DIR2)/%.c  $(DEPS)
	$(CC) -c $@ $< $(CFLAGS)
test : $(DIR)/packet_implem.o $(DIR2)/test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
sender: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(DIR)/*.o sender $(DIR2)/*.o test
