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
        include/bytecode/subroutine.h
        include/bytecode/module.h
        include/util/vector.h
        src/util/vector.c
        include/core/constpool.h
        include/core/stack.h
        include/executor/context.h
        src/core/vm.c
        src/core/stack.c
        src/executor/context.c
        src/bytecode/module.c
        include/util/fileutils.h
        src/util/fileutils.c
    include/executor/code.h
    src/executor/code.c
    src/executor/instruction.c
    src/core/string.c
    include/util/pointer.h
)
