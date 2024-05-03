cmake_minimum_required(VERSION 3.28)
project(CTVM C)

set(CMAKE_C_STANDARD 17)

include_directories(include)

add_executable(CTVM
        src/main.c
        include/defines.h
        include/core/vm.h
        include/core/register.h
        include/core/value.h
        include/memory/object.h
        include/core/string.h
        include/bytecode/bytecodeheader.h
        include/bytecode/subroutine.h)
