cmake_minimum_required(VERSION 3.17)
project(VulkanTesting)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
include_directories(/Users/hamzalah/VulkanSDK/macOS/include)

add_executable(VulkanTesting main.cpp vertStruct.h shapes/Cube.h shapes/Triangle.h shapes/Plane.h shapes/Node.h shapes/Shape.h newVulkanApp.cpp vulkanWindow.cpp vulkanWindow.h newVulkanApp.h main.cpp vulkanPipeline.cpp vulkanPipeline.h vulkanDevice.cpp vulkanDevice.h)

target_link_libraries(VulkanTesting "/usr/local/lib/libglfw.3.4.dylib")
target_link_libraries(VulkanTesting "/Users/hamzalah/VulkanSDK/macOS/lib/libvulkan.1.2.162.dylib")
#target_link_libraries(VulkanTesting "/Users/hamzalah/VulkanSDK/macOS/lib/libvulkan.1.dylib")
