cmake_minimum_required(VERSION 3.28)
project(BlairLearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

# 链接 OpenGL 库
if (APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
elseif (WIN32)
    set(OPENGL_LIBRARY opengl32)
else ()
    find_library(OPENGL_LIBRARY GL)
endif ()


# 添加头文件搜索路径，让编译器能找到工程内的头文件
include_directories(
        ${PROJECT_SOURCE_DIR}/BlairLearnOpenGL
)

# 仅收集 BlairLearnOpenGL 根目录下的 .cpp 文件，不递归
file(GLOB MAIN_SOURCES
        "${PROJECT_SOURCE_DIR}/BlairLearnOpenGL/*.cpp"  # 根目录下的 .cpp 文件
)


# ====================== 将Core编译成静态库 ======================
file(GLOB CORE_SOURCES "${PROJECT_SOURCE_DIR}/BlairLearnOpenGL/Core/*.cpp")
# 将 Core 目录的源文件编译成静态库 CoreLib（也可以用 SHARED 改成共享库/动态库）
add_library(CoreLib STATIC ${CORE_SOURCES})
target_link_libraries(CoreLib
        PRIVATE glfw
        PRIVATE glm::glm
        PRIVATE glad
)
# 为 Core 库设置头文件包含路径，这样其他代码包含 Core 头文件时能找到
target_include_directories(CoreLib PUBLIC
        "${PROJECT_SOURCE_DIR}/BlairLearnOpenGL/Core"
)


# ====================== 将glad编译成静态库 ======================
add_library(glad STATIC BlairLearnOpenGL/Core/glad/src/glad.c)
target_include_directories(glad PUBLIC BlairLearnOpenGL/Core/glad/include)


# ====================== 添加依赖库 ======================
find_package(glm REQUIRED)
if(NOT glm_FOUND)
    message(FATAL_ERROR "glm not found!")
else()
    message(STATUS "Found glm: ${glm_DIR}")
    message(STATUS "glm targets: ${TARGETS}")  # 查看可用目标
endif()

find_package(glfw3 REQUIRED)
if(NOT glfw3_FOUND)
    message(FATAL_ERROR "GLFW not found!")
else()
    message(STATUS "Found GLFW: ${glfw3_DIR}")
    message(STATUS "GLFW targets: ${TARGETS}")  # 查看可用目标
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ====================== 复制资源 ======================

add_custom_target(CopyResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/Resource
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resource
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/BlairLearnOpenGL/Shader
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shader
)

# ====================== 生成可执行文件 ======================
# 为每个包含 main 函数的文件创建独立的可执行文件
foreach(main_file ${MAIN_SOURCES})
    # 获取不带路径和扩展名的文件名作为可执行文件名称
    get_filename_component(executable_name ${main_file} NAME_WE)

    # 创建可执行文件，链接公共 Core 代码和依赖库
    add_executable(${executable_name}
            ${main_file}
    )

    add_dependencies(${executable_name} CopyResources)

    # 链接所需的库
    target_link_libraries(${executable_name}
            glfw
            glm::glm
            glad
            CoreLib
    )

endforeach()

