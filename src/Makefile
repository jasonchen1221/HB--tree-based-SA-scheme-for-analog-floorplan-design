CXX = g++
EXE = Place.exe

CFLAGS = -g -Wall -O3

OBJS = main.o block.o net.o parser.o \
	   placer.o helper.o bTree.o contourline.o \
	   results.o

HEADERS = placer.hpp globalParam.hpp block.hpp \
          net.hpp pin.hpp parser.hpp recoverMsg.hpp \
		  bTree.hpp contourline.hpp hashSet.hpp

$(EXE): $(OBJS)
	@echo ">> Building $(EXE) ..."
	@$(CXX) -o $@ $(OBJS) $(CFLAGS)

%.o: %.cpp $(HEADERS)
	@echo "> compiling $< ..."
	@$(CXX) $< -c $(CFLAGS)

clean:
	@echo "> Removing $(EXE) ..."
	@echo "> Removing *.o ..."
	@rm -f $(EXE) *.o *.O *.gch