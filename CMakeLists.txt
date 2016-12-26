cmake_minimum_required(VERSION 3.6)
project(ServerGameOfLife)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
set(SOURCE_FILES main.cpp src/Serveur.cpp src/Serveur.h src/RequestManager.cpp src/RequestManager.h
        GamingModeGameOfLife/src/Mode.h GamingModeGameOfLife/src/Mode.cpp GamingModeGameOfLife/src/SimpleMode.cpp
        GamingModeGameOfLife/src/SimpleMode.h GamingModeGameOfLife/src/ModeConquest.cpp GamingModeGameOfLife/src/ModeConquest.h
        GamingModeGameOfLife/HashlifeGameOfLife/src/TreeNode.cpp GamingModeGameOfLife/HashlifeGameOfLife/src/TreeNode.h
        GamingModeGameOfLife/HashlifeGameOfLife/src/TreeUniverse.cpp
        GamingModeGameOfLife/HashlifeGameOfLife/src/TreeUniverse.h GamingModeGameOfLife/HashlifeGameOfLife/src/TreeHashMap.cpp
        GamingModeGameOfLife/HashlifeGameOfLife/src/TreeHashMap.h
        GamingModeGameOfLife/HashlifeGameOfLife/src/CanonicalTreeNode.cpp
        GamingModeGameOfLife/HashlifeGameOfLife/src/CanonicalTreeNode.h
        GamingModeGameOfLife/HashlifeGameOfLife/src/HashLifeTreeNode.cpp
        GamingModeGameOfLife/HashlifeGameOfLife/src/HashLifeTreeNode.h
        GamingModeGameOfLife/src/Zone.cpp
        GamingModeGameOfLife/src/Zone.h
        GamingModeGameOfLife/src/Point.cpp GamingModeGameOfLife/src/Point.h)
add_executable(ServerGameOfLife ${SOURCE_FILES})


if (WIN32)
    target_link_libraries(ServerGameOfLife wsock32 ws2_32)
endif ()

target_link_libraries(ServerGameOfLife pthread)