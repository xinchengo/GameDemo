CXX       := g++
BIN       := bin
SRC       := src
INCLUDE   := include
LIBRARIES  := -lsfml-graphics -lsfml-window -lsfml-system
EXECUTABLE := game

ver = debug
ifeq ($(ver), debug)
CXX_FLAGS := -std=c++17 -O3 -DDEBUG
else
CXX_FLAGS := -std=c++17 -O3
endif

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*