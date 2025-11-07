cmake_minimum_required(VERSION 3.17)

project(Trees LANGUAGES CXX)

find_package(GTest REQUIRED)
enable_testing()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# option(NDEBUG "Enable debug mode" ON)

set(LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/log)

#--- CREATE TESTS DIRECTORY --------------------------------------------
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests CACHE PATH "Directory for generated tests")
file(MAKE_DIRECTORY ${TESTS_DIR})
message(STATUS "Tests directory: ${TESTS_DIR}")
#------------------------------------------------------------------------

add_subdirectory(RedBlackTree)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(libs/RLogSU)
endif()

add_executable(main
    main.cpp
)

target_link_libraries(main PRIVATE
    RLogSU
    RedBlackTree
    # GTest::gtest
    # GTest::gtest_main
)



target_compile_definitions(main PRIVATE MODULE_NAME="main")

set(EXECS main)
set(LIBS  RLogSU)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    set(DEBUG_SANITIZERS
        -fsanitize=address
        -fsanitize=alignment
        -fsanitize=bool
        -fsanitize=bounds
        -fsanitize=enum
        -fsanitize=float-cast-overflow
        -fsanitize=undefined
        -fsanitize=unreachable
        -fsanitize=vla-bound
        -fsanitize=vptr
        -fsanitize=float-divide-by-zero
        -fsanitize=integer-divide-by-zero
        -fsanitize=leak
        -fsanitize=nonnull-attribute
        -fsanitize=null
        -fsanitize=return
        -fsanitize=returns-nonnull-attribute
        -fsanitize=shift
        -fsanitize=signed-integer-overflow
        # -fsanitize=object-size
    )

    set(DEBUG_FLAGS
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Weffc++
        -Wno-reorder
        # experimental flags:
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wconversion
        -Wdouble-promotion
        -Wformat=2
        -Wimplicit-fallthrough
    )

    foreach(target IN ITEMS ${EXECS})
        target_compile_options(${target} PRIVATE
            ${DEBUG_SANITIZERS}
            ${DEBUG_FLAGS}
        )
        target_link_options(${target} PRIVATE
            ${DEBUG_SANITIZERS}
        )
    endforeach()

    foreach(target IN ITEMS ${LIBS})
        target_compile_options(${target} PRIVATE
            ${DEBUG_SANITIZERS}
            ${DEBUG_FLAGS}
        )
    endforeach()

endif()
