# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/common)

project(HTTP)


#Nhung file ben ngoai vao. Vi du: Nhung file html
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "html/index.html" TEXT)