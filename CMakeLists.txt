cmake_minimum_required (VERSION 2.8.11)
project (goosepp)

# Make sure we can import out CMake functions
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Read the git tags to determine the project version
include(GetGitVersion)
get_git_version(GIT_VERSION)

# Tell the user what versions we are using
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" VERSION ${GIT_VERSION})
message("-- Version: ${VERSION}")

# The version of the libraries
set(GENERIC_LIB_VERSION ${VERSION})
string(SUBSTRING ${VERSION} 0 1 GENERIC_LIB_SOVERSION)

# Import our CMake modules
include(CheckCXXCompilerFlag)
include(AddCXXCompilerFlag)
include(CXXFeatureCheck)

# Try and enable C++11.
add_cxx_compiler_flag(-std=c++11)
if (NOT HAVE_CXX_FLAG_STD_CXX11)
  add_cxx_compiler_flag(-std=c++0x)
endif()


# Ensure we have gumbo and mitie
find_library(gumbo REQUIRED)
find_library(mitie REQUIRED)


# Set up directories
include_directories(${PROJECT_SOURCE_DIR}/src)

# Build the targets
add_subdirectory(src)
