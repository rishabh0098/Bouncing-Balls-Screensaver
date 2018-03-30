#! bin/bash
#------------------------------------------------------------------------------

CC=g++ -w 
BIN=./obj
SRC=./src
EXE=./output
LIBS= -lpthread -lGL -lGLU -lglut

# override NUM_THREADS = 10

#------------------------------------------------------------------------------

execute: CC+= -DCOUNT=$(NUM_THREADS)

execute: $(EXE)/run
	./$(EXE)/run

$(EXE)/run: $(BIN)/main.o $(BIN)/display.o $(BIN)/balls.o $(BIN)/physics.o $(BIN)/terrain.o
	$(CC) $(BIN)/main.o $(BIN)/display.o $(BIN)/balls.o $(BIN)/physics.o $(BIN)/terrain.o -o $(EXE)/run $(LIBS)

run1:
	mkdir obj

run2: 
	mkdir output

$(BIN)/main.o: $(SRC)/main.cpp run1 run2
	$(CC) -c $(SRC)/main.cpp -o $(BIN)/main.o $(LIBS)

$(BIN)/display.o: $(SRC)/display.cpp
	$(CC) -c $(SRC)/display.cpp -o $(BIN)/display.o $(LIBS)

$(BIN)/balls.o: $(SRC)/balls.cpp
	$(CC) -c $(SRC)/balls.cpp -o $(BIN)/balls.o $(LIBS)

$(BIN)/physics.o: $(SRC)/physics.cpp
	$(CC) -c $(SRC)/physics.cpp -o $(BIN)/physics.o $(LIBS)

$(BIN)/terrain.o: $(SRC)/terrain.cpp
	$(CC) -c $(SRC)/terrain.cpp -o $(BIN)/terrain.o $(LIBS)

clean: clean2

clean2: 
	rm -r obj output
