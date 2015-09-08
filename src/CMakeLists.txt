# Allow the source files to find headers in src/
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include/goosepp)

# Define the source files
set(SOURCE_FILES
    "goosepp.cpp"
    "GooseExtractor.cpp"
    "contentExtraction/ContentExtractor.cpp"
    "contentExtraction/NodeTextCleaner.cpp"
    "contentExtraction/NodeScorer.cpp"
    "contentExtraction/TextNodeCollector.cpp"
    "contentExtraction/BoostChecker.cpp"
    "TitleExtractor.cpp"
    "PublishDateExtractor.cpp"
    "OpenGraphExtractor.cpp"
    "stopwords/englishStopwordSet.cpp"
    "stopwords/stopwords.cpp"
    "tokenizer/WhitespaceTokenizer.cpp"
    "util/fileUtils.cpp"
    "util/gooseUtil.cpp"
    "util/gumboUtils.cpp"
)

add_library(goosepp SHARED ${SOURCE_FILES})

set_target_properties(goosepp PROPERTIES
  OUTPUT_NAME "goosepp"
  VERSION ${GENERIC_LIB_VERSION}
  SOVERSION ${GENERIC_LIB_SOVERSION}
)

# Link threads.
target_link_libraries(goosepp gumbo)

# Expose public API
target_include_directories(goosepp PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Install target (will install the library to specified CMAKE_INSTALL_PREFIX variable)
install(
  TARGETS goosepp
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT library
)

install(
  DIRECTORY "${PROJECT_SOURCE_DIR}/include/goosepp"
  DESTINATION include
)

include_directories(${PROJECT_SOURCE_DIR}/external/gtest-1.7.0-min/include)
include_directories(${PROJECT_SOURCE_DIR}/external/gmock-1.7.0/include)
include_directories(${PROJECT_SOURCE_DIR}/src/test/unit)
set(UNIT_SOURCE_FILES
  "test/unit/test_BoostChecker.cpp"
  "test/unit/test_OpenGraphExtractor.cpp"
  "test/unit/test_PublishDateExtractor.cpp"
  "test/unit/test_TitleExtractor.cpp"
  "test/unit/test_WhitespaceTokenizer.cpp"
  "test/unit/runner.cpp"
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
  "test/functional/testContentExtraction.cpp"
)

target_link_libraries(
  ftest_content_extraction ${TEST_LINK}
)

add_executable(
  memory_leak_test
  "test/memory/runner.cpp"
)

target_link_libraries(
  memory_leak_test ${TEST_LINK}
)

add_executable(
  benchmark_runner
  "test/benchmark/runner.cpp"
)

target_link_libraries(
  benchmark_runner goosepp gumbo benchmark pthread
)
