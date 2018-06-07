cmake_minimum_required(VERSION 3.1)

project(vulkan_embassy)

find_package(glfw3)

if(glfw3_FOUND)
  include_directories(
    $ENV{VULKAN_SDK}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

  link_directories($ENV{VULKAN_SDK}/lib)

  add_subdirectory(examples/01_hello_vulkan)
endif()
