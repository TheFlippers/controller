TARGET_EXEC ?= fdd

DEST ?= /var/www/html/fdd
USR ?= www-data
GRP ?= www-data

LIBS ?= wiringPi bcm2835 png pthread

MKDIR_P ?= mkdir -p

OBJ_DIR ?= ./obj
SRC_DIR ?= ./src
INC_DIR ?= ./lib

SRCS := $(shell find $(SRC_DIR) -name *.c)
OBJS := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

INC_FLAGS := $(addprefix -I,$(INC_DIR))
LD_FLAGS := $(addprefix -l,$(LIBS))
DEFs :=

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(INC_FLAGS) $(DEFS) -c $< -o $@ $(LD_FLAGS)

$(TARGET_EXEC): $(OBJS)
	$(CC) $(INC_FLAGS) $(DEFS) $(OBJS) -o $@ $(LD_FLAGS)

.PHONY: clean

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(TARGET_EXEC)

update: clean $(TARGET_EXEC)

install: $(TARGET_EXEC)
	cp $(TARGET_EXEC) $(DEST)
	chown $(USR) $(DEST)
	chgrp $(GRP) $(DEST)
