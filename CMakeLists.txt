cmake_minimum_required(VERSION 3.30)
project(pong)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML root folder
set(SFML_ROOT "C:/SFML")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")
set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/modules" ${CMAKE_MODULE_PATH})

# Executable sources
add_executable(pong
        src/main.cpp
)

# Include headers
target_include_directories(pong PRIVATE
        headers
        ${SFML_ROOT}/include
)

# Find SFML components
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)

if(SFML_FOUND)
    target_link_libraries(pong PRIVATE sfml-system sfml-window sfml-graphics)
else()
    message(FATAL_ERROR "SFML not found!")
endif()

# Copy assets folder to build dir
#file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Copy SFML DLLs to build dir on Windows
if(WIN32)
    file(GLOB SFML_DLLS "${SFML_ROOT}/bin/*.dll")
    file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_BINARY_DIR})

    # Copy MinGW DLLs (adjust path to your MSYS2 mingw64 bin)
    file(GLOB MINGW_DLLS "C:/msys64/mingw64/bin/*.dll")
    file(COPY ${MINGW_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()
