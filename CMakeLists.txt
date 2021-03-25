cmake_minimum_required(VERSION 3.17)
project(VulkanTesting)

set(CMAKE_CXX_STANDARD 17)

IF (APPLE)
    SET(GUI_TYPE MACOSX_BUNDLE shapes/vert.h)
ENDIF (APPLE)

#for optimization
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

include_directories(/usr/local/include)
include_directories(/Users/hamzalah/VulkanSDK/macOS/include)

#add ${GUI_TYPE} after VulkanTesting (ie target) to compile into app
add_executable(VulkanTesting main.cpp shapes/vert.h shapes/Cube.h shapes/Triangle.h shapes/Plane.h shapes/Node.h newVulkanApp.cpp vulkanWindow.cpp vulkanWindow.h newVulkanApp.h main.cpp vulkanPipeline.cpp vulkanPipeline.h vulkanDevice.cpp vulkanDevice.h vulkanSwapChain.cpp vulkanSwapChain.h vulkanModel.cpp vulkanModel.h shapes/Icosahedron.h stb_image.h)

target_link_libraries(VulkanTesting "/usr/local/lib/libglfw.3.4.dylib")
target_link_libraries(VulkanTesting "/Users/hamzalah/VulkanSDK/macOS/lib/libvulkan.1.2.162.dylib")
#target_link_libraries(VulkanTesting "/Users/hamzalah/VulkanSDK/macOS/lib/libvulkan.1.dylib")