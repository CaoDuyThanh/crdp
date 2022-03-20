include(FetchContent)
cmake_minimum_required(VERSION 3.0.0)

# Project
set(PROJECT_NAME crdp)
set(PROJECT_SRC pyrdp.cpp src/rdp.cpp src/rdp.h)
project(${PROJECT_NAME} VERSION 1.0.0)

set(PYBIND11_VERSION 2.9.1)

# Include Pybind11
FetchContent_Declare(
  PYBIND11
  URL https://github.com/pybind/pybind11/archive/refs/tags/v${PYBIND11_VERSION}.zip
)
FetchContent_MakeAvailable(pybind11)

include_directories("src")
pybind11_add_module(${PROJECT_NAME} ${PROJECT_SRC})
