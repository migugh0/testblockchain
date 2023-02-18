#Copyright (c) 2016-2023 Knuth Project developers.
#Distributed under the MIT software license, see the accompanying
#file LICENSE or http://www.opensource.org/licenses/mit-license.php.
#SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.25)

#Project name
project(testblockchain
		VERSION 0.1.0
		LANGUAGES CXX C
)

#List of project external dependecies
#(This project do not have external dependencies)

#Set project defines
message("${PROJECT_NAME} message: Starting compiling '${PROJECT_NAME}' version ${PROJECT_VERSION}")

#Set project binary dir correctly in case do not call cmake with -B build/out
message("${PROJECT_NAME} message: CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
message("${PROJECT_NAME} message: CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
set(PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build/out") #build directory
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/../bin") #executable files
set(CMAKE_PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/pdb") #debug files .pdb
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../lib") #.a
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../lib") #.lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin") #.dll

#Check systems defines
string(REGEX MATCH "ucrt64" _UCRT64_COMMAND_PARSED "${CMAKE_COMMAND}")
if(${_UCRT64_COMMAND_PARSED} MATCHES "ucrt64")
	set(_IS_UCRT64 1)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(_IS_WIN 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(_IS_MAC 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	file(READ "/proc/version" _SYS_VERSION)
	string(REGEX MATCH "Microsoft" _SYSTEM_VERSION_PARSED "${_SYS_VERSION}")
	if(${_SYSTEM_VERSION_PARSED} MATCHES "Microsoft")
		set(_IS_WSL 1)
    else()
		set(_IS_LINUX 1)
    endif()
endif()

#Compiler options
#https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category
#https://gcc.gnu.org/onlinedocs/gcc-3.4.6/gcc/Option-Summary.html
#https://www.man7.org/linux/man-pages/man1/g++.1.html
#https://devblogs.microsoft.com/cppblog/introducing-the-universal-crt/
if(MSVC)
	#Warning level 4 and all warnings as errors
	add_compile_options(/W4 /WX)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O3" CACHE INTERNAL "" FORCE)
else()
	#static, lots of warnings, all warnings as errors
	add_compile_options(-pthread -static -static-libgcc -static-libstdc++ -Wextra -Wall -Werror) # -pedantic
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors -Wno-unknown-pragmas -Wno-unused-variable -Wno-unused-parameter" CACHE INTERNAL "" FORCE)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g" CACHE INTERNAL "" FORCE)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3" CACHE INTERNAL "" FORCE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -static" CACHE INTERNAL "" FORCE)
endif()

#We use C++20 (beta) Estandar Debug
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/contrib/cmake")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines -std=c++20") # -std=gnu++20
endif()
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_VERBOSE_MAKEFILE ON) #get more verbose output from Makefile builds and show each command line

#Changes for using diferent compilers and systems
if(_IS_UCRT64)
	#C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\SDK\ScopeCppSDK\SDK\include\ucrt
	#C:\Program Files (x86)\Windows Kits\10\Redist\ucrt
	add_definitions("-D _UCRT")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D __USE_MINGW_ANSI_STDIO")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -l ucrt") #Needs -D _UCRT
endif()
if(_IS_MAC)
	add_definitions("-D MAC_OSX")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk -mmacosx-version-min=10.7")
endif()

#Use ccache if installed to speed up build times
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
	message("${PROJECT_NAME} message: Program ccache found = ${CCACHE_PROGRAM}")
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
	#Set up wrapper scripts
	set(C_LAUNCHER   "${CCACHE_PROGRAM}")
	set(CXX_LAUNCHER "${CCACHE_PROGRAM}")
else()
	message("${PROJECT_NAME} message: Program ccache not found")
endif()

#Common sources
set(COMMON_SOURCES
	./src/main.cpp
	./src/app/block_test.cpp
)

# add_library(COMMON_LIBS
# )
# target_link_libraries(COMMON_LIBS)

#go to the CMakeLists.txt in this subdirectory execute it and continue
#add_subdirectory(src)

#List all current cmake properties
# get_cmake_property(_variableNames VARIABLES)
# list(SORT _variableNames)
# foreach(_variableName ${_variableNames})
    # message("${_variableName}=${${_variableName}}")
# endforeach()

#Executable
add_executable(${PROJECT_NAME} ${COMMON_SOURCES})
#extra defines:
target_compile_definitions(${PROJECT_NAME} PUBLIC -D _DEBUG=1)
#extra include paths:
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src)