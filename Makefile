# FileName and CC, FLAGS
SRC_DIR := src
BIN_DIR := bin
DATA_DIR := data
CC := gcc
FLAGS := -Wall -pthread -o 

# executable
Tar_lazy := $(BIN_DIR)/lazy.run
Tar_normal := $(BIN_DIR)/normal.run
Tar_parallel := $(BIN_DIR)/parallel.run
Tar_datagen := $(BIN_DIR)/datagen.run

# source file
Src_lazy := $(SRC_DIR)/LazyBT.c
Src_normal  := $(SRC_DIR)/NormalBT.c
Src_parallel := $(SRC_DIR)/ParallelBT.c
Src_datagen := $(SRC_DIR)/data_gen.c

# Build all the executable
normal: $(Tar_lazy) $(Tar_normal) $(Tar_parallel) $(Tar_datagen)
$(Tar_lazy): $(Src_lazy)
	$(CC) $(FLAGS) $@ $^
$(Tar_normal): $(Src_normal)
	$(CC) $(FLAGS) $@ $^
$(Tar_parallel): $(Src_parallel)
	$(CC) $(FLAGS) $@ $^
$(Tar_datagen): $(Src_datagen)
	$(CC) $(FLAGS) $@ $^

# Clean up
clean:
	rm -f $(BIN_DIR)/* $(DATA_DIR)/*
