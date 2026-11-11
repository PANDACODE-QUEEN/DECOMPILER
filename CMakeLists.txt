
cmake_minimum_required(VERSION 3.21)


project(DECOMPILER VERSION 1.0 LANGUAGES C)


set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")

    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
      add_compile_options(/W4)
endif()


add_executable(${PROJECT_NAME} src/main.c)

target_include_directories(${PROJECT_NAME} PRIVATE include)