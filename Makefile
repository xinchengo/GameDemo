CXX       := g++
CXX_FLAGS := -std=c++17 -O3

BIN := bin
SRC := src
INCLUDE := include

LIBRARIES  := -lsfml-graphics -lsfml-window -lsfml-system
EXECUTABLE := game

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*