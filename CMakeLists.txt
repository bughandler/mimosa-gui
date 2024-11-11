# The main build script for Mimosa-GUI.

cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

if(POLICY CMP0069)
  cmake_policy(SET CMP0069 NEW)
endif()

project(mimosa-gui LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Define module names here
set(MAIN_MODULE_NAME "mimosa")

# Build settings
set(BUILD_TYPE "library" CACHE STRING "Choose the build type")
set_property(CACHE BUILD_TYPE PROPERTY STRINGS library extension)
option(STATIC_BUILD "Build mimosa-gui statically" OFF)

# Check compilers
set( compiler_is_clang "$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>" )
set( compiler_is_gnu "$<CXX_COMPILER_ID:GNU>" )
set( compiler_is_msvc "$<CXX_COMPILER_ID:MSVC>" )
set( compiler_less_than_v8 "$<VERSION_LESS:$<CXX_COMPILER_VERSION>,8>" )
set( compiler_greater_than_or_equal_v9 "$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,9>" )
set( compiler_greater_than_or_equal_v11 "$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,11>" )
set( compiler_less_than_v11 "$<VERSION_LESS:$<CXX_COMPILER_VERSION>,11>" )
set( compiler_greater_than_or_equal_v12 "$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,12>" )

# Detect target bits
if(NOT DEFINED BITS)
	set(BITS 32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(BITS 64)
	endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
endif()

# Unify platform name
if(WIN32)
	set(PLATFORM_NAME "windows")
elseif(ANDROID)
	set(PLATFORM_NAME "android")
elseif(LINUX)
	set(PLATFORM_NAME "linux")
elseif(IOS)
	set(PLATFORM_NAME "ios")
elseif(APPLE)
	set(PLATFORM_NAME "osx")
endif()

# Source files
function(_expand_files OUT_VAR EXPR_LIST)
	foreach(expr in ${EXPR_LIST})
		file(GLOB_RECURSE expand_files CONFIGURE_DEPENDS ${expr})
		list(APPEND OUT_VAR ${expand_files})
	endforeach()
endfunction()
file(GLOB_RECURSE MAIN_CPP_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/main/*.c**)
file(GLOB_RECURSE MAIN_CPP_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/main/*.h**)
_expand_files(CORE_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/core/*.c**
							   ${CMAKE_CURRENT_SOURCE_DIR}/core/base/*.c**
							   ${CMAKE_CURRENT_SOURCE_DIR}/core/${PLATFORM_NAME}/*.c**)
_expand_files(CORE_CPP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/core/*.h**
							   ${CMAKE_CURRENT_SOURCE_DIR}/core/base/*.h**
							   ${CMAKE_CURRENT_SOURCE_DIR}/core/base/${PLATFORM_NAME}/*.h**)

# Include thirdparty compiling script
include(thirdparty/thirdparty.cmake)

# Build target
add_library(${MAIN_MODULE_NAME} SHARED ${MAIN_CPP_SOURCES}
									   ${MAIN_CPP_HEADERS}
									   ${CORE_CPP_SOURCES}
									   ${CORE_CPP_HEADERS}
									   ${THIRDPARTY_SOURCES}
									   ${THIRDPARTY_HEADERS})
target_include_directories(${MAIN_MODULE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
													   ${CMAKE_CURRENT_SOURCE_DIR}/thidparty)
target_compile_options(${MAIN_MODULE_NAME} PRIVATE
    # MSVC only
    $<${compiler_is_msvc}:
        # /W4
        -D_TAGET_PY_VERSION=${BUILD_PY_VERSION}
        -D_HAS_EXCEPTIONS=0 # Disable C++ exceptions.
        /utf-8 # Indicate source code is encoding with utf-8
    >

    $<$<NOT:${compiler_is_msvc}>:
        -fno-exceptions # Disable C++ exceptions.
        -fno-unwind-tables
        -fno-rtti # Disable RTTI.
    >)
if(WIN32)
	target_link_libraries(${MAIN_MODULE_NAME} PRIVATE user32.lib shlwapi.lib shell32.lib delayimp.lib)
	if(STATIC_BUILD)
		set_property(TARGET ${MAIN_MODULE_NAME} PROPERTY CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()
endif()

# Enable LTO if available for "MinSizeRel" build
if(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    # Check if LTO is supported
    include(CheckIPOSupported)
    check_ipo_supported(RESULT lto_supported OUTPUT error)

	if(lto_supported)
		message(STATUS "IPO /LTO enabled")
		set_property(TARGET ${MAIN_MODULE_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
	else()
		message(STATUS "IPO /LTO not supported: <${error}>")
	endif()
endif()