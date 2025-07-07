cmake_minimum_required(VERSION 3.31)
project(Globulus)

set(CMAKE_CXX_STANDARD 20)

add_executable(Globulus main.cpp
        Particle.cpp
        Particle.h
        Vec.cpp
        Vec.h
        Node.cpp
        Node.h
        BHtree.cpp
        BHtree.h
        NodePool.cpp
        NodePool.h)
