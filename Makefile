CXX := g++
OUTPUT := mega-mario

CXX_FLAGS := -O3 -std=c++20 -Wno-unused-result
INCLUDES := -I ./src
LDFLAGS := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL

SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

all:$(OUTPUT)

$(OUTPUT):$(OBJ_FILES) Makefile 
		$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

.cpp.o: 
		$(CXX) -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

clean:
		rm -f $(OBJ_FILES) ./bin/mega-mario

run: $(OUTPUT) 
		cd bin && ./mega-mario && cd ../
