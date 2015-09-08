# Allow the source files to find headers in src/

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include/goosepp)
include_directories(${PROJECT_SOURCE_DIR}/external/gtest-1.7.0-min/include)
include_directories(${PROJECT_SOURCE_DIR}/external/gmock-1.7.0/include)
include_directories(${PROJECT_SOURCE_DIR}/src/test/unit)

set(UNIT_SOURCE_FILES
  "unit/test_BoostChecker.cpp"
  "unit/test_OpenGraphExtractor.cpp"
  "unit/test_PublishDateExtractor.cpp"
  "unit/test_TitleExtractor.cpp"
  "unit/test_WhitespaceTokenizer.cpp"
  "unit/runner.cpp"
)

add_executable(
  unit_test_runner
  ${UNIT_SOURCE_FILES}
)

set(TEST_LINK
  goosepp gumbo glog pthread
)

target_link_libraries(
  unit_test_runner
  "${PROJECT_SOURCE_DIR}/external/gmock-1.7.0/src/gmock-all.o"
  "${PROJECT_SOURCE_DIR}/external/gtest-1.7.0-min/libgtest_main.a"
  ${TEST_LINK}
)

add_executable(
  ftest_content_extraction
  "functional/testContentExtraction.cpp"
)

target_link_libraries(
  ftest_content_extraction ${TEST_LINK}
)

add_executable(
  memory_leak_test
  "memory/runner.cpp"
)

target_link_libraries(
  memory_leak_test ${TEST_LINK}
)

add_executable(
  benchmark_runner
  "benchmark/runner.cpp"
)

target_link_libraries(
  benchmark_runner goosepp gumbo benchmark pthread
)
