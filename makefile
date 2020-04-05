# for Windows use Mingw-w64: 	mingw32-make
# for UNIX:						make

# Compiler [gcc / g++]
CC = g++

# Name
NAME = dasm

# Flags
FLAGS = -Wall -Wno-unused-function

# Paths
PATH_SRC = ./src/
PATH_OUT = ./

all:
	$(CC) $(PATH_SRC)*.cpp $(FLAGS) -o $(PATH_OUT)$(NAME)

run:
	./$(NAME)