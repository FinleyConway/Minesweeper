cmake_minimum_required(VERSION 3.28)
project(Minesweeper LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(sfml GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.1)
FetchContent_MakeAvailable(sfml)

add_executable(Minesweeper
        src/main.cpp
        src/Minesweeper/Grid.h
        src/Minesweeper/Tile.h
        src/Minesweeper/Minesweeper.h
        src/Minesweeper/Minesweeper.cpp
)

target_link_libraries(Minesweeper PRIVATE
        sfml-graphics
)

target_compile_features(Minesweeper PRIVATE cxx_std_20)

if(WIN32)
    add_custom_command(
            TARGET main
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
            VERBATIM)
endif()

add_custom_command(TARGET Minesweeper POST_BUILD
        COMMENT "Move assets to bin"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/bin/assets
)