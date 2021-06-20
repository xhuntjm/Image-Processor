# source files.
SRC =  image/image.cpp utility/utility.cpp

OBJ = $(SRC:.cpp=.o)

OUT = ../lib/libiptools.a

# include directories
INCLUDES = -I. -I/usr/local/include -Iimage/  -Iutility/ -I/usr/include/c++/4.2.1 

# opencv library for FSprime server
OPENCV = `PKG_CONFIG_PATH=../lib pkg-config --cflags --libs opencv4`
# opencv library for local
#OPENCV = `pkg-config --cflags --libs opencv`

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g -std=c++11

# compiler
CCC = g++


# compile flags
LDFLAGS = -g

.SUFFIXES: .cpp

.cpp.o:
	$(CCC) $(OPENCV) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT) Makefile.bak# DO NOT DELETE
