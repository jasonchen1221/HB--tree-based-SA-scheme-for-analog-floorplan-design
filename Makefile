.PHONY: clean, test

#CXX = g++
#EXE = Placer.exe
#CFLAGS = -g -Wall -O3

SOURCE = src
SOURCE_FILE_NAME = _Placer

PYBIND_INCLUDEDS = -I$(shell python3 -m pybind11 --includes) $(shell python3-config --includes)
TARGET = $(SOURCE_FILE_NAME)$(shell python3-config --extension-suffix)

OBJS = main.o block.o net.o parser.o \
	   placer.o helper.o bTree.o contourline.o \
	   results.o

#HEADERS = placer.hpp globalParam.hpp block.hpp \
          net.hpp pin.hpp parser.hpp recoverMsg.hpp \
		  bTree.hpp contourline.hpp hashSet.hpp

#$(EXE): $(OBJS)
#	@echo ">> Building $(EXE) ..."
#	@$(CXX) -o $@ $(OBJS) $(CFLAGS)

#%.o: %.cpp $(HEADERS)
#	@echo "> compiling $< ..."
#	@$(CXX) $< -c $(CFLAGS)

#clean:
#	@echo "> Removing $(EXE) ..."
#	@echo "> Removing *.o ..."
#	@rm -f $(EXE) *.o *.O *.gch

TARGET: $(OBJS) 
	g++ $(OBJS) ${PYBIND_INCLUDEDS} -shared -fPIC -o $(SOURCE_FILE_NAME)$(shell python3-config --extension-suffix) -O3

main.o: main.cpp
	g++ -c -fPIC main.cpp ${PYBIND_INCLUDEDS} -o main.o

block.o: ${SOURCE}/block.cpp
	g++ -c -fPIC ${SOURCE}/block.cpp ${PYBIND_INCLUDEDS} -o block.o

net.o: ${SOURCE}/net.cpp
	g++ -c -fPIC ${SOURCE}/net.cpp ${PYBIND_INCLUDEDS} -o net.o

parser.o: ${SOURCE}/parser.cpp
	g++ -c -fPIC ${SOURCE}/parser.cpp ${PYBIND_INCLUDEDS} -o parser.o

placer.o: ${SOURCE}/placer.cpp
	g++ -c -fPIC ${SOURCE}/placer.cpp ${PYBIND_INCLUDEDS} -o placer.o

helper.o: ${SOURCE}/helper.cpp
	g++ -c -fPIC ${SOURCE}/helper.cpp ${PYBIND_INCLUDEDS} -o helper.o

bTree.o: ${SOURCE}/bTree.cpp
	g++ -c -fPIC ${SOURCE}/bTree.cpp ${PYBIND_INCLUDEDS} -o bTree.o

contourline.o: ${SOURCE}/contourline.cpp
	g++ -c -fPIC ${SOURCE}/contourline.cpp ${PYBIND_INCLUDEDS} -o contourline.o

results.o: ${SOURCE}/results.cpp
	g++ -c -fPIC ${SOURCE}/results.cpp ${PYBIND_INCLUDEDS} -o results.o

clean:
	rm -rf *.o *.so
