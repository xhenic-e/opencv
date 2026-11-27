# ===========================================================================
#  OpenCV 视觉第一次作业 —— 任务一
#
#  构建方法（在项目根目录执行）：
#      cmake -B build_cmake -DOpenCV_DIR="D:/opencv/opencv/build/x64/vc16/lib"
#      cmake --build build_cmake --config Release
#
#  或者用自带的助手脚本：
#      tools\cmake_build.cmd Release
#
#  生成的可执行文件：
#      build_cmake/ColorBandLocator.exe   任务一：框出蓝色 / 红色矩形
#      build_cmake/text.exe               课程练习汇总
# ===========================================================================
cmake_minimum_required(VERSION 3.16)
project(OpenCVHomework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# 中文版 MSVC 的 /showIncludes 前缀是本地化文本，Ninja 默认按英文匹配。
# 不设置的话每次构建都会刷一屏「注意: 包含文件」。
# 必须在 project() 之后设置，否则此时还没做编译器检测。
# ---------------------------------------------------------------------------
if(MSVC AND CMAKE_GENERATOR MATCHES "Ninja")
    set(CMAKE_CL_SHOWINCLUDES_PREFIX "注意: 包含文件: " CACHE STRING "MSVC showIncludes 前缀" FORCE)
endif()

# ---------------------------------------------------------------------------
# OpenCV 位置：默认用 D:\opencv 里的官方预编译包（x64 / vc16，MSVC 专用）
# 换版本或换目录时改这里，或者 cmake 时传 -DOpenCV_DIR=...
# 注意：指向的是 OpenCVConfig.cmake 所在的 lib 目录
# ---------------------------------------------------------------------------
if(NOT DEFINED OpenCV_DIR)
    set(OpenCV_DIR "D:/opencv/opencv/build/x64/vc16/lib" CACHE PATH "OpenCVConfig.cmake 所在目录")
endif()

find_package(OpenCV REQUIRED)

# 官方预编译包只带 Release / Debug 两种配置，把其余构建类型映射过去，
# 否则 RelWithDebInfo / MinSizeRel 找不到对应配置。
if(TARGET opencv_world)
    set_property(TARGET opencv_world PROPERTY MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)
    set_property(TARGET opencv_world PROPERTY MAP_IMPORTED_CONFIG_MINSIZEREL Release)
endif()

# ---------------------------------------------------------------------------
# 小工具函数：建一个可执行文件 = 加源码 + 头文件路径 + 链接库 + 收尾拷贝
# 这样下面每加一个程序只要一行，不用把配置重复三遍
# ---------------------------------------------------------------------------
function(add_opencv_app target_name)
    add_executable(${target_name} ${ARGN})

    target_include_directories(${target_name} PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${target_name} PRIVATE opencv_world)

    if(MSVC)
        # 源码是 UTF-8，不告诉 MSVC 的话中文注释和字符串会按 GBK 解析出错
        target_compile_options(${target_name} PRIVATE /utf-8)
    endif()

    # 把 opencv_world4xxx.dll 拷到 exe 旁边，双击运行不用再配 PATH
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:opencv_world>"
                "$<TARGET_FILE_DIR:${target_name}>"
        COMMENT "复制 OpenCV 运行库到 ${target_name} 输出目录"
        VERBATIM)

    # 把测试图片也拷过去：程序里用的是相对路径 "2.png" / "shenzi.png"
    file(GLOB _test_images
         "${CMAKE_CURRENT_SOURCE_DIR}/*.png"
         "${CMAKE_CURRENT_SOURCE_DIR}/*.jpg")
    if(_test_images)
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_test_images}
                    "$<TARGET_FILE_DIR:${target_name}>"
            COMMENT "复制测试图片到 ${target_name} 输出目录"
            VERBATIM)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# 两个程序
# ---------------------------------------------------------------------------
add_opencv_app(ColorBandLocator ColorBandLocator.cpp)   # 任务一
add_opencv_app(LightBarLocator  LightBarLocator.cpp)    # 任务二
add_opencv_app(text             text.cpp)               # 课程练习

message(STATUS "OpenCV ${OpenCV_VERSION} from ${OpenCV_DIR}")
message(STATUS "Build: cmake --build build_cmake --config Release")
