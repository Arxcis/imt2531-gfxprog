# @doc http://blog.hwrd.me/2014/05/07/beginning-opengl-development-on-os-x/ - 08.01.2018

CC = g++

INCLUDE_PATHS = -I/usr/local/include
LIBRARY_PATHS = -L/usr/local/lib 
LINKER_FLAGS = -framework OpenGL -lsdl2 -lglew 

COMPILER_FLAGS = -Wall -std=c++14

# OBJS specifies which files to compile as part of the project
OBJS = main.cpp

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o build/$(OBJ_NAME)