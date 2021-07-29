#############################################
# User configuration
# #############################################
# # (1) COMPILER SELECTION : Choose "g++" or "aarch64-linux-gnu-g++"
CC = gcc
CXX = g++
#
# # (2) DEFAULT BEHAVIOR : default behavior for make
# .PHONY:: all create_dir
#
# # (3) TARGET NAME : file name of build result
TARGET = exec
#
# # (4) DIRECTORY PATH : path to contain build result
# SRC_PATH = ./src
# OUT_PATH = ./bin
# INC_PATH = ./include
# LIB_PATH = ./lib
#
# # (5) OPTIONS FLAGS : flags for build
# FLAGS = -fPIC -DUSE_NEON -std=gnu++0x -fpermissive
CFLAGS = -g -Wall
#
# # (6) LIBRARY DEPENDENCY : add library dependency
# LIBS  = -lsslengine
# LIBS += -lasound
# LIBS += -lpthread
# LIBS += -pthread
# LIBS += -ldl
# LIBS += -lstdc++
LIBS = -lcurl
#
#
# #############################################
# RPATH = $(PWD)/$(LIB_PATH)
# SRCS := $(wildcard ${SRC_PATH}/*.cpp)
# OBJS := $(addprefix ./, ${SRCS:.c=.o})
OBJS = main.o azure.o fr.o
#
# ALL_BUILD_DIR:=$(OUT_PATH)
#
# all: create_dir $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

main.o: azure.hpp fr.hpp main.cpp
azure.o: azure.hpp azure.cpp
fr.o: fr.hpp fr.cpp

clean:
	rm -f *.o
	rm -f $(TARGET)
