
CC = clang++ -I /usr/include/eigen3

CXXFLAGS  = -std=c++11 -Wall -msse2 -O2 #-fopenmp=libomp #-msse2

INC_DIR = ./source/include/
SRC_DIR = ./source/
BLD_DIR = ./build/
EXE_DIR = ./bin/

default: $(EXE_DIR)main

$(EXE_DIR)main:  $(BLD_DIR)main.o $(BLD_DIR)Classifier.o $(BLD_DIR)DatasetReader.o $(BLD_DIR)Neurons.o
	$(CC) $(CXXFLAGS) -o $(EXE_DIR)main $(BLD_DIR)main.o $(BLD_DIR)Classifier.o $(BLD_DIR)DatasetReader.o $(BLD_DIR)Neurons.o

$(BLD_DIR)main.o:  $(SRC_DIR)main.cpp  $(INC_DIR)Classifier.h $(INC_DIR)DatasetReader.h
	$(CC) $(CXXFLAGS) -o $(BLD_DIR)main.o -c $(SRC_DIR)main.cpp

$(BLD_DIR)Classifier.o: $(SRC_DIR)Classifier.cpp $(INC_DIR)Classifier.h $(INC_DIR)DatasetReader.h $(INC_DIR)Neurons.h
	$(CC) $(CXXFLAGS) -o $(BLD_DIR)Classifier.o -c $(SRC_DIR)Classifier.cpp

$(BLD_DIR)DatasetReader.o: $(SRC_DIR)DatasetReader.cpp $(INC_DIR)DatasetReader.h
	$(CC) $(CXXFLAGS) -o $(BLD_DIR)DatasetReader.o -c $(SRC_DIR)DatasetReader.cpp

$(BLD_DIR)Neurons.o: $(SRC_DIR)Neurons.cpp $(INC_DIR)Neurons.h
	$(CC) $(CXXFLAGS) -o $(BLD_DIR)Neurons.o -c $(SRC_DIR)Neurons.cpp

clean:
	$(RM) count $(BLD_DIR)*.o *~
