CXX=clang++-3.5
INC=-I./src -I./external/gtest-1.7.0-min/include -I./external/gmock-1.7.0/include -O2

CXXFLAGS=-stdlib=libstdc++ --std=c++14 $(INC)
LINK=-lgumbo -latomic -pthread
COMP = $(CXX) $(CXXFLAGS)

LIB_OBJ = $(addprefix ./src/, \
		goosepp.o \
		GooseExtractor.o \
		contentExtraction/ContentExtractor.o \
		contentExtraction/NodeTextCleaner.o \
		contentExtraction/TextNodeCollector.o \
		contentExtraction/NodeScorer.o \
		contentExtraction/BoostChecker.o \
		TitleExtractor.o \
		PublishDateExtractor.o \
		stopwords/englishStopwordSet.o \
		tokenizer/WhitespaceTokenizer.o \
		util/fileUtils.o \
		util/gooseUtil.o \
		util/gumboUtils.o \
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
	rm -f src/test/memory/*.o src/test/benchmark/*.o benchmark_runner mem_test_runner

clean: clean-tests
	rm -f runner src/*.o src/stopwords/*.o src/tokenizer/*.o src/contentExtraction/*.o src/util/*.o

.PHONY: run clean clean-tests

GTEST_LIB = ./external/gtest-1.7.0-min/gtest-all.o
GMOCK_LIB = ./external/gmock-1.7.0/src/gmock-all.o

TEST_LINK = $(GMOCK_LIB) $(GTEST_LIB) -lglog $(LINK)
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

mem_test_runner: ./src/test/memory/runner.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIB_OBJ) $(LINK)

test-mem: mem_test_runner
	valgrind --tool=memcheck --show-leak-kinds=definite,possible,indirect ./mem_test_runner

benchmark_runner: ./src/test/benchmark/runner.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIB_OBJ) -lbenchmark $(LINK)

benchmark: benchmark_runner
	./benchmark_runner

.PHONY: test-mem benchmark

build/func:
	mkdir -p build/func

build:
	mkdir -p build

