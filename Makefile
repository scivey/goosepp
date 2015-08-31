CXX=clang++-3.5
INC=-I./src
CXXFLAGS=-stdlib=libstdc++ --std=c++14 $(INC)
LINK=-lgumbo -lglog -lfolly -latomic
COMP = $(CXX) $(CXXFLAGS)

LIB_OBJ = $(addprefix ./src/, \
		fileUtils.o \
		gooseFuncs.o \
		gumboUtils.o \
		stopwords.o \
		stopwordSet.o \
		WhitespaceTokenizer.o \
	)

MAIN_OBJ = $(addprefix ./src/, \
		main.o \
	)

$(MAIN_OBJ): $(LIB_OBJ)

runner: $(MAIN_OBJ) $(LIB_OBJ)
	$(COMP) -o $@ $(MAIN_OBJ) $(LIB_OBJ) $(LINK)

%.o:%.cpp
	$(COMP) -o $@ -c $<

run: runner
	./runner

clean:
	rm -f runner src/*.o

.PHONY: run clean
