cmake_minimum_required(VERSION 3.16.2) 
project(Server VERSION 0.1.0)

IF (WIN32)
  set(CMAKE_TOOLCHAIN_FILE C:\\Users\\Tiago\\Documents\\Projetos\\estudo-cpp\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
  include_directories(C:\\Users\\Tiago\\Documents\\Projetos\\estudo-cpp\\vcpkg\\installed\\x64-windows\\include)
ENDIF()

add_executable(sim_client main.cpp getopt.c)
find_package(cppnetlib CONFIG REQUIRED)
find_package(Boost REQUIRED thread)

IF (WIN32)
  target_link_libraries(sim_client PRIVATE cppnetlib-uri cppnetlib-server-parsers cppnetlib-client-connections Boost::boost Boost::thread)
ENDIF()

