# @ref http://blog.hwrd.me/2014/05/07/beginning-opengl-development-on-os-x/ - 08.01.2018

# OBJS specifies which files to compile as part of the project
OBJS = main.cpp

# CC specifies which compiler we're using
CC = g++

# INCLUDE_PATHS specifies the additional include paths we'll need
#  for linux -I/opt/X11/include
INCLUDE_PATHS = -I/usr/local/include 

# LIBRARY_PATHS specifies the additional library paths we'll need 
#  for linux -I/opt/X11/lib
LIBRARY_PATHS = -L/usr/local/lib 

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -std=c++14

# LINKER_FLAGS specifies the libraries we're linking against
# Cocoa, IOKit, and CoreVideo are needed for static GLFW3.
LINKER_FLAGS = -framework OpenGL -lSDL2 -lGLEW

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o build/$(OBJ_NAME)