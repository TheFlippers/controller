TARGET_EXEC ?= fdd

LIBS ?= wiringPi bcm2835

OBJ_DIR ?= ./obj
SRC_DIR ?= ./src
INC_DIR ?= ./lib

SRCS := $(shell find $(SRC_DIR) -name *.c)
OBJS := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

INC_FLAGS := $(addprefix -I,$(INC_DIR))
LD_FLAGS := $(addprefix -l,$(LIBS))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(INC_FLAGS) -c $< -o $@ $(LD_FLAGS)

$(TARGET_EXEC): $(OBJS)
	$(CC) $(INC_FLAGS) $(OBJS) -o $@ $(LD_FLAGS)

.PHONY: clean

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(TARGET_EXEC)

MKDIR_P ?= mkdir -p
