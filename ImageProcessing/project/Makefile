#all:
	#cd ../iptools; make
#	g++ -o bin/test test.cpp  -L ../lib -l iptools 
#	g++ -o bin/test2 test2.cpp  -L ../lib -l iptools -l mpeg 

	# source files.

BIN_DIR=bin/

EXEC = iptool

# opencv library for FSprime server
OPENCV = `PKG_CONFIG_PATH=../lib pkg-config --cflags --libs opencv4`
# opencv library for local
#OPENCV = `pkg-config --cflags --libs opencv`


all: $(EXEC)

iptools:
	(cd ../iptools; make)

clean:
	$(RM) $(BIN_DIR)$(EXEC); make all

DO_EXEC = g++ -std=c++11 $@.cpp $(OPENCV) -o $(BIN_DIR)$@ -L ../lib -l iptools

#------ now compile
$(EXEC):   		iptools
				$(DO_EXEC)
