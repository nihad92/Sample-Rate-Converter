#Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.4.1)
project(oboe-sample-converter LANGUAGES CXX)

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add_library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.

# Build the Oboe library
set (OBOE_DIR src/main/cpp/oboe)
add_subdirectory (${OBOE_DIR} ./oboe)


add_library(${PROJECT_NAME} SHARED
        src/main/cpp/common/handle.h
        src/main/cpp/converter.cpp)

# Specify the libraries which our native library is dependent on, including Oboe
target_link_libraries (${PROJECT_NAME} log oboe)


# Make the Oboe public headers available to our app
include_directories (${OBOE_DIR}/include ${OBOE_DIR}/src/flowgraph)
