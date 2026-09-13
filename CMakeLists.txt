cmake_minimum_required(VERSION 3.20)

project(CarDealership)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PISTACHE REQUIRED IMPORTED_TARGET libpistache)

file(GLOB_RECURSE SOURCE src/*.cpp src/*.h)

add_executable(CarDealership ${SOURCE}

)

target_include_directories(CarDealership PRIVATE src src/controllers src/models 
            src/services src/repositories src/Database)

target_link_libraries(CarDealership PRIVATE PkgConfig::PISTACHE pq)