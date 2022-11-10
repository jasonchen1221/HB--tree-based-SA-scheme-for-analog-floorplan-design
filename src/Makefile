CXX = g++
EXE = PLACE.exe

CFLAGS = -g -Wall -O3

OBJS = 

HEADERS = 

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