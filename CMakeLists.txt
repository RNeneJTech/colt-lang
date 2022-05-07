cmake_minimum_required(VERSION 3.4)

project(Colt-Interpreter VERSION 0.0.0.0 LANGUAGES C)

file(GLOB_RECURSE ColtiHeaders "colti/src/*.h")
file(GLOB_RECURSE ColtiUnits "colti/src/*.c")

add_executable(colti
	${ColtiHeaders} ${ColtiUnits}
)

target_include_directories(colti PUBLIC "${CMAKE_SOURCE_DIR}/colti/src/util" "${CMAKE_SOURCE_DIR}/colti/src/byte-code")

set(VS_STARTUP_PROJECT colti)

configure_file("${CMAKE_SOURCE_DIR}/colti/src/util/cmake/cmake_colti_config.in"
	"${CMAKE_SOURCE_DIR}/colti/src/util/colti_config.h")

target_precompile_headers(colti PUBLIC 
	"$<$<COMPILE_LANGUAGE:C>:${PROJECT_SOURCE_DIR}/colti/src/util/precomph.h>")

# Define COLT_DEBUG_BUILD for debug config
target_compile_definitions(colti PRIVATE $<$<CONFIG:DEBUG>:COLTI_DEBUG_BUILD>)

# Testing for CTest
enable_testing()
# Run the executable
add_test(NAME RunColt
	COMMAND colti)