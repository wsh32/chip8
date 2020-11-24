CXX = g++
CXXFLAGS = -Wall -Werror -std=c++14

CHIP8_OBJS = chip8.o emulator.o formatted_exception.o
CHIP8_BINARY = emulator

SOURCE_DIR = ./src/
BIN_DIR = ./bin/
DOCS_DIR = ./docs/
OBJS_LIST = $(addprefix $(BIN_DIR), $(CHIP8_OBJS))
BINARY = $(addprefix $(BIN_DIR), $(CHIP8_BINARY))

ifeq ($(PREFIX),)
	PREFIX := /usr/local/bin
endif

all: $(OBJS_LIST) $(BINARY) docs

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)%.o: $(SOURCE_DIR)%.cpp
	$(CXX) -c -g $(CXXFLAGS) -x c++ $< -o $@

$(OBJS_LIST): | $(BIN_DIR)

$(BINARY): $(OBJS_LIST)
	$(CXX) $(CXXFLAGS) $(OBJS_LIST) -o $(BINARY) $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(DOCS_DIR)


install: $(BINARY)
	cp $(BINARY) $(PREFIX)/

docs: Doxyfile
	@doxygen
	@open docs/html/index.html

.PHONY: all clean install

