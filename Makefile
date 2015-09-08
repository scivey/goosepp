clean:
	rm -rf build build2

.PHONY: clean test test-unit test-functional test-mem bench test-build

test-unit: test-build
	cd build2 && make unit_test_runner
	./build2/src/test/unit_test_runner

test: test-unit test-functional test-mem

test-build:
	mkdir -p build2
	cd build2 && cmake -DGOOSEPP_ENABLE_TESTING=ON ../

test-functional: test-build
	cd build2 && make ftest_content_extraction
	./build2/src/test/ftest_content_extraction

test-mem: test-build
	cd build2 && make memory_leak_test
	valgrind --tool=memcheck --show-leak-kinds=definite,possible,indirect ./build2/src/test/memory_leak_test

bench: test-build
	cd build2 && make benchmark_runner
	./build2/src/test/benchmark_runner

deps:
	cd external/gtest-1.7.0-min && make libgtest_main.a
	cd external/gmock-1.7.0 && ./configure && make src/gmock-all.o

