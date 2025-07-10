cmake_minimum_required(VERSION 3.31)
project(BHTree)

set(CMAKE_CXX_STANDARD 20)

add_executable(BHTree main.cpp
        Particle.cpp
        Particle.h
        Vec.cpp
        Vec.h
        Node.cpp
        Node.h
        BHtree.cpp
        BHtree.h
        NodePool.cpp
        NodePool.h
        Box.cpp
        Box.h)
