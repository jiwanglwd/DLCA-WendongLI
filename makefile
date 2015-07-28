RM := rm -f
MKDIR = mkdir -p

CC := g++
STD := -std=c++11
OPTIMIZE := -O3
LD_FLAGS := $(STD) $(OPTIMIZE)
CC_FLAGS := $(STD) $(OPTIMIZE)

DLCA := dlca
BIN_DIR = bin
SRC_DIR := DLCA
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(SRC_FILES:.cpp=.o)))

$(BIN_DIR)/$(DLCA): $(OBJ_FILES) $(BIN_DIR)
	$(CC) $(LD_FLAGS) -o $@ $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CC) $(CC_FLAGS) -c -o $@ $<

$(BIN_DIR):
	$(MKDIR) $@

$(OBJ_DIR):
	$(MKDIR) $@

clean:
	$(RM) $(OBJ_FILES)
