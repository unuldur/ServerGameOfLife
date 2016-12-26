cmake_minimum_required(VERSION 3.6)
project(ServerGameOfLife)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
set(SOURCE_FILES main.cpp src/Serveur.cpp src/Serveur.h src/RequestManager.cpp src/RequestManager.h
        GameOfLifeMode/src/Mode.h GameOfLifeMode/src/SimpleMode.cpp GameOfLifeMode/src/SimpleMode.h
        GameOfLifeMode/src/FactoryMode.h GameOfLifeMode/HashlifeGameOfLife/src/TreeNode.cpp
        GameOfLifeMode/HashlifeGameOfLife/src/TreeNode.h GameOfLifeMode/HashlifeGameOfLife/src/TreeUniverse.cpp
        GameOfLifeMode/HashlifeGameOfLife/src/TreeUniverse.h GameOfLifeMode/src/Player.h GameOfLifeMode/src/Mode.cpp GameOfLifeMode/src/Player.cpp)
add_executable(ServerGameOfLife ${SOURCE_FILES})


if (WIN32)
    target_link_libraries(ServerGameOfLife wsock32 ws2_32)
endif ()

target_link_libraries(ServerGameOfLife pthread)