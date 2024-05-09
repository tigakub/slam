cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

project(slam VERSION 0.1)

configure_file(slamConfig.h.in ../slamConfig.h)

add_custom_target(embeddedShaderData 
    COMMAND cd .. && ./generateEmbeddedShaderData.sh
)

add_executable(slam 
    slam.cpp 
    gl.c 
    lidar.cpp 
    visualizer.cpp 
    vis/container.cpp 
    vis/link.cpp 
    vis/occupancyGrid.cpp
    vis/quaternion.cpp
    vis/vertex.cpp
    vis/aabb.cpp
    vis/uniformBuffer.cpp
    vis/vertexBuffer.cpp
    vis/elementBuffer.cpp
    vis/vertexArray.cpp
    vis/box.cpp
    vis/triangle.cpp
    vis/pointCloud.cpp
    vis/pointCloudAccumulator.cpp
    vis/camera.cpp
    vis/light.cpp
    vis/framebuffer.cpp
    vis/context.cpp
    vis/node.cpp
    vis/geometry.cpp
)

add_dependencies(slam embeddedShaderData)

target_include_directories(slam PUBLIC 
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/unitree_lidar_sdk/include"
    "${PROJECT_SOURCE_DIR}/unitree_lidar_sdk/include*")

link_directories("${PROJECT_SOURCE_DIR}/unitree_lidar_sdk/lib/aarch64")
add_library(libunitree_lidar_sdk STATIC IMPORTED)
set_target_properties(libunitree_lidar_sdk PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/unitree_lidar_sdk/lib/aarch64/libunitree_lidar_sdk.a")

target_link_libraries(slam libunitree_lidar_sdk dl glfw)
