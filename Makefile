CXX=clang++-3.5
INC=-I./src -I./external/gtest-1.7.0-min/include -I./external/gmock-1.7.0/include

CXXFLAGS=-stdlib=libstdc++ --std=c++14 $(INC)
LINK=-lgumbo -lglog -lfolly -latomic -pthread
COMP = $(CXX) $(CXXFLAGS)

LIB_OBJ = $(addprefix ./src/, \
		fileUtils.o \
		goosepp.o \
		contentExtraction/ContentExtractor.o \
		contentExtraction/NodeTextCleaner.o \
		contentExtraction/TextNodeCollector.o \
		contentExtraction/NodeScorer.o \
		contentExtraction/BoostChecker.o \
		gumboUtils.o \
		TitleExtractor.o \
		PublishDateExtractor.o \
		stopwords/englishStopwordSet.o \
		tokenizer/WhitespaceTokenizer.o \
		gooseUtil.o \
		stopwords/stopwords.o \
		OpenGraphExtractor.o \
	)

UNIT_TEST_OBJ = $(addprefix ./src/test/unit/, \
		test_BoostChecker.o \
		test_WhitespaceTokenizer.o \
		test_OpenGraphExtractor.o \
		test_TitleExtractor.o \
		test_PublishDateExtractor.o \
	)

./src/main.cpp: $(LIB_OBJ)

runner: ./src/main.cpp $(LIB_OBJ)
	$(COMP) -o $@ ./src/main.cpp $(LIB_OBJ) $(LINK)

%.o:%.cpp
	$(COMP) -o $@ -c $<

run: runner
	./runner


clean-tests:
	rm -f src/test/unit/*.o unit_test_runner src/test/functional/*.o func_test_runner

clean: clean-tests
	rm -f runner src/*.o src/stopwords/*.o src/tokenizer/*.o

.PHONY: run clean clean-tests

GTEST_LIB = ./external/gtest-1.7.0-min/gtest-all.o
GMOCK_LIB = ./external/gmock-1.7.0/src/gmock-all.o

TEST_LINK = $(GMOCK_LIB) $(GTEST_LIB) $(LINK)
UNIT_RUNNER_SRC = ./src/test/unit/runner.cpp
unit_test_runner: $(UNIT_RUNNER_SRC) $(UNIT_TEST_OBJ) $(LIB_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(UNIT_TEST_OBJ) $(UNIT_RUNNER_SRC) $(LIB_OBJ) $(TEST_LINK)

test-unit: unit_test_runner
	./unit_test_runner

test: unit_test_runner
	./unit_test_runner

.PHONY: test-unit

FUNC_TEST_NAMES = testContentExtraction

FUNC_TESTS = $(addprefix ./build/func/, $(FUNC_TEST_NAMES))

$(FUNC_TESTS): build/func

./build/func/%: ./src/test/functional/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIB_OBJ) $(LINK)

test-func: $(FUNC_TESTS)
	./build/func/testContentExtraction


build/func:
	mkdir -p build/func

build:
	mkdir -p build

