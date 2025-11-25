# ===========================
#  Emscripten Card Game Build
# ===========================

# Compiler
EMCC = em++

# Output
OUTPUT = index.html

# Source files
SRC = CardGameUI.cpp \
      UIDrawer.cpp \
      RegularGame.cpp \
      Buttons.cpp \
      Card.cpp \
      Deck.cpp \
      StateVariables.cpp \
      Globals.cpp

# Include and library paths
INCLUDES = -I include/
LIBS     = -L lib/ -lraylib

# Emscripten flags
EM_FLAGS = -s USE_GLFW=3 \
           -s FULL_ES2=1 \
           -s WASM=1 \
           -s ALLOW_MEMORY_GROWTH=1 \
           -s ASYNCIFY=1 \
           -s ASYNCIFY_STACK_SIZE=1048576

# Preload assets
PRELOAD = --preload-file textures@/textures \
          --preload-file fonts@/fonts

# Compiler flags
CFLAGS = -Wall

# ===========================

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(EMCC) $(CFLAGS) $(SRC) -o $(OUTPUT) $(INCLUDES) $(LIBS) $(EM_FLAGS) $(PRELOAD)

clean:
	rm -f $(OUTPUT) index.js index.wasm index.data

.PHONY: all clean

run:
	emrun --port 8080 index.html
