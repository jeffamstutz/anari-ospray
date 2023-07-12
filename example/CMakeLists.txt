## Copyright 2022 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(tutorial LANGUAGES CXX)
project_add_executable(main.cpp)
project_link_libraries(PRIVATE anari_library_ospray)
