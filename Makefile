clean:
	rm -rf build build2

.PHONY: clean test test-unit test-functional test-mem bench build

test-unit:
	mkdir -p build
	cd build && cmake ../ && make unit_test_runner
	./build/src/unit_test_runner

test: test-unit test-functional test-mem

build:
	mkdir -p build
	cd build && cmake ../

test-functional: build
	cd build && make ftest_content_extraction
	./build/src/ftest_content_extraction

test-mem: build
	cd build && make memory_leak_test
	valgrind --tool=memcheck --show-leak-kinds=definite,possible,indirect ./build/src/memory_leak_test

bench: build
	cd build && make benchmark_runner
	./build/src/benchmark_runner

