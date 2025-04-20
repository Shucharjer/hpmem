# cmake
cmake_minimum_required(VERSION 3.24)

if(NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "${CMAKE_SOURCE_DIR}/install"
      CACHE PATH "..." FORCE)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# project
set(CMAKE_CXX_STANDARD 17)

project(
  hpmem
  DESCRIPTION "A static library with hyper performance memory operations."
  LANGUAGES C CXX)

set(HPMEM_INCLUDE_DIR ${hpmem_SOURCE_DIR}/include)
set(HPMEM_SRC_DIR ${hpmem_SOURCE_DIR}/src)
add_library(hpmem STATIC ${HPMEM_SRC_DIR}/cpy.cpp)

target_include_directories(hpmem PUBLIC $<BUILD_INTERFACE:${HPMEM_INCLUDE_DIR}>)

target_compile_options(
  hpmem PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>, $<CXX_COMPILER_ID:Clang>>:-O3>
                $<$<CXX_COMPILER_ID:MSVC>:/O2>)
# vector support
include(CheckCXXSourceCompiles)
include(CMakePushCheckState)

cmake_push_check_state()
set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -mavx")
check_cxx_source_compiles(
  "
#include <immintrin.h>
int main() {
    __m256i a = _mm256_set_ps(8, 7, 6, 5, 4, 3, 2, 1);
    __m256i b = _mm256_set_ps(1, 2, 3, 4, 5, 6, 7, 8);
    __m256i c = _mm256_add_ps(a, b);
    return 0;
}
"
  HPMEM_AVX_AVAILABLE)
cmake_pop_check_state()
if(HPMEM_AVX_AVAILABLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
endif()

cmake_push_check_state()
set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -mavx2")
check_cxx_source_compiles(
  "
#include <immintrin.h>
int main() {
    __m256i a = _mm256_set_epi32(-3, -2, -1, 0, 1, 2, 3, 4);
    __m256i b = _mm256_abs_epi32(a);
    return 0;
}
"
  HPMEM_AVX2_AVAILABLE)
cmake_pop_check_state()
if(HPMEM_AVX2_AVAILABLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
endif()

if(HPMEM_AVX_AVAILABLE)
  target_compile_options(hpmem PUBLIC $<IF:$<CXX_COMPILER_ID:MSVC>, /arch:AVX,
                                      -mavx>)
endif()
if(HPMEM_AVX2_AVAILABLE)
  target_compile_options(hpmem PUBLIC $<IF:$<CXX_COMPILER_ID:MSVC>, /arch:AVX2,
                                      -mavx2>)
endif()

option(BUILD_HPMEM_BENCHMARK OFF)
set(BUILD_HPMEM_BENCHMARK ON)
if(BUILD_HPMEM_BENCHMARK)
  include(FetchContent)
  FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG main
    GIT_SHALLOW 1)
  set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
  FetchContent_MakeAvailable(googlebenchmark)

  set(HPMEM_BENCHMARK_DIR "${hpmem_SOURCE_DIR}/benchmark")
  add_executable(benchmark-hpcpy "${HPMEM_BENCHMARK_DIR}/cpy.cpp")
  target_link_libraries(benchmark-hpcpy PRIVATE hpmem benchmark)
endif()
