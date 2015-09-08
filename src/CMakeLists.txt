# Allow the source files to find headers in src/
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include/goosepp)

# Turn compiler warnings up to 11
add_cxx_compiler_flag(-Wall)
add_cxx_compiler_flag(-Wextra)
add_cxx_compiler_flag(-Wshadow)
add_cxx_compiler_flag(-Werror RELEASE)
add_cxx_compiler_flag(-pedantic)
add_cxx_compiler_flag(-pedantic-errors)
add_cxx_compiler_flag(-Wshorten-64-to-32)
add_cxx_compiler_flag(-Wfloat-equal)
add_cxx_compiler_flag(-Wzero-as-null-pointer-constant)
add_cxx_compiler_flag(-fstrict-aliasing)
if (HAVE_CXX_FLAG_FSTRICT_ALIASING)
  add_cxx_compiler_flag(-Wstrict-aliasing)
endif()
add_cxx_compiler_flag(-Wthread-safety)
if (HAVE_WTHREAD_SAFETY)
  add_definitions(-DHAVE_WTHREAD_SAFETY)
  cxx_feature_check(THREAD_SAFETY_ATTRIBUTES)
endif()

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

