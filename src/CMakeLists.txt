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

add_library(goosepp ${SOURCE_FILES})

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
