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

# Try and enable C++11. Don't use C++14 because it doesn't work in some
# configurations.
add_cxx_compiler_flag(-std=c++11)
if (NOT HAVE_CXX_FLAG_STD_CXX11)
  add_cxx_compiler_flag(-std=c++0x)
endif()

# # Turn compiler warnings up to 11
# add_cxx_compiler_flag(-Wall)
# add_cxx_compiler_flag(-Wextra)
# add_cxx_compiler_flag(-Wshadow)
# add_cxx_compiler_flag(-Werror RELEASE)
# add_cxx_compiler_flag(-pedantic)
# add_cxx_compiler_flag(-pedantic-errors)
# add_cxx_compiler_flag(-Wshorten-64-to-32)
# add_cxx_compiler_flag(-Wfloat-equal)
# add_cxx_compiler_flag(-Wzero-as-null-pointer-constant)
# add_cxx_compiler_flag(-fstrict-aliasing)
# if (HAVE_CXX_FLAG_FSTRICT_ALIASING)
#   add_cxx_compiler_flag(-Wstrict-aliasing)
# endif()
# add_cxx_compiler_flag(-Wthread-safety)
# if (HAVE_WTHREAD_SAFETY)
#   add_definitions(-DHAVE_WTHREAD_SAFETY)
#   cxx_feature_check(THREAD_SAFETY_ATTRIBUTES)
# endif()

# # Link time optimisation
# if (BENCHMARK_ENABLE_LTO)
#   add_cxx_compiler_flag(-flto)
#   if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
#     find_program(GCC_AR gcc-ar)
#     if (GCC_AR)
#       set(CMAKE_AR ${GCC_AR})
#     endif()
#     find_program(GCC_RANLIB gcc-ranlib)
#     if (GCC_RANLIB)
#       set(CMAKE_RANLIB ${GCC_RANLIB})
#     endif()
#   endif()
# endif()

# Coverage build type
# set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING
#   "Flags used by the C++ compiler during coverage builds."
#   FORCE)
# set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
#   "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" CACHE STRING
#   "Flags used for linking binaries during coverage builds."
#   FORCE)
# set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
#   "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" CACHE STRING
#   "Flags used by the shared libraries linker during coverage builds."
#   FORCE)
# mark_as_advanced(
#   CMAKE_CXX_FLAGS_COVERAGE
#   CMAKE_EXE_LINKER_FLAGS_COVERAGE
#   CMAKE_SHARED_LINKER_FLAGS_COVERAGE)
# set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
#   "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Coverage."
#   FORCE)
# add_cxx_compiler_flag(--coverage COVERAGE)

# C++ feature checks
# cxx_feature_check(STD_REGEX)
# cxx_feature_check(GNU_POSIX_REGEX)
# cxx_feature_check(POSIX_REGEX)
# cxx_feature_check(STEADY_CLOCK)

# Ensure we have pthreads
# find_package(Threads REQUIRED)

# Set up directories
include_directories(${PROJECT_SOURCE_DIR}/src)

# Build the targets
add_subdirectory(src)