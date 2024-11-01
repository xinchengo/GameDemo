CXX       := g++
BIN       := bin
SRC       := src
INCLUDE   := include
LIBRARIES  := -lsfml-graphics -lsfml-window -lsfml-system
EXECUTABLE := game
FISHTRAINER := fishTrainer

ver = debug
ifeq ($(ver), debug)
CXX_FLAGS := -std=c++17 -O3 -DDEBUG
else
CXX_FLAGS := -std=c++17 -O3
endif

all: game fishtrainer

game: $(BIN)/$(EXECUTABLE)

fishtrainer: $(BIN)/$(FISHTRAINER)

run: clean game
	clear
	./$(BIN)/$(EXECUTABLE)

train: clean fishtrainer
	clear
	./$(BIN)/$(FISHTRAINER)

$(BIN)/$(EXECUTABLE): $(shell find $(SRC)/* -not -name $(FISHTRAINER).cpp)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)
$(BIN)/$(FISHTRAINER): $(shell find $(SRC)/* -not -name $(EXECUTABLE).cpp)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*