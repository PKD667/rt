# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#               thanks to http://stackoverflow.com/users/128940/beta
#   2017-04-24 - changed order of linker params
# ------------------------------------------------

# -------------------
# CCCP Makefile
# Modified by: PKD
#--------------------

# General settings
CC = gcc
CPP = g++

# CUDA settings
CUDA_PATH ?= /usr/local/cuda
NVCC = $(CUDA_PATH)/bin/nvcc
CUDA_LIBS = -L$(CUDA_PATH)/lib64 -lcudart

# Directories
ODIR = obj
SDIR = src
EXEOUT = rt
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include

CFLAGS = -Wall -g -fPIC -O2 -Wextra -L./bin -Iinclude 
CUDAFLAGS = -arch=sm_60 -I$(CUDA_PATH)/include
LIBS = lib/* -lm -lSDL2 -lcjson $(CUDA_LIBS)

# File lists
SOURCES  := $(wildcard $(SRCDIR)/*.c)
CUDA_SOURCES := $(wildcard $(SRCDIR)/cuda/*.cu)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CUDA_OBJECTS := $(CUDA_SOURCES:$(SRCDIR)/cuda/%.cu=$(OBJDIR)/cuda/%.o)

all: $(BINDIR)/$(EXEOUT)
	@echo $(CUDA_SOURCES)
	@echo $(CUDA_OBJECTS) 
	@echo "BUILD SUCCESSFUL"

$(BINDIR)/$(EXEOUT): $(OBJECTS) $(CUDA_OBJECTS)
	@$(NVCC) $(OBJECTS) $(CUDA_OBJECTS) $(CUDA_LIBS) $(LIBS) $(LFLAGS) -o $@ 
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
	@if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR); fi

	$(CC) $(CFLAGS) -D OBJ_COUNT=$(OBJ_COUNT) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(CUDA_OBJECTS): $(OBJDIR)/cuda/%.o : $(SRCDIR)/cuda/%.cu
	@if [ ! -d $(OBJDIR)/cuda ]; then mkdir -p $(OBJDIR)/cuda; fi

	$(NVCC) -rdc=true -c $< -o $@ $(CUDAFLAGS)
	@echo "Compiled "$<" successfully!"


clean:
	rm -f $(ODIR)/*.o $(ODIR)/cuda/*.o $(BINDIR)/*
