cmake_minimum_required(VERSION 3.5)

project(QtHalconVTK VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 若是MSVC编译器，则使用UTF-8编码
if(MSVC)
    set(CMAKE_C_FLAGS /utf-8)
    set(CMAKE_CXX_FLAGS /utf-8)
endif()

# 确保调试和发布版本对将安装的可执行文件和库使用不同的名称。让我们使用 d 作为调试可执行文件和库的后缀。
set(CMAKE_DEBUG_POSTFIX d)

# 在 Windows 上设置应用程序图标
if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS ${CMAKE_SOURCE_DIR}/Resource/App/myApp.rc)
endif()

# 配置头文件以将一些 CMake 设置传递给源代码
configure_file(${CMAKE_SOURCE_DIR}/Config/ProjectConfig.h.in ProjectConfig.h)

# VTK
find_package(VTK COMPONENTS
    vtkCommonCore
    vtkGUISupportQt
    vtkGUISupportQtOpenGL
    vtkRenderingOpenGL2
    vtkIOImage
    vtkInteractionImage
)
include(${VTK_USE_FILE})

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

# Halcon
include_directories(D:/MVTec/HALCON-21.11-Progress/include)
include_directories(D:/MVTec/HALCON-21.11-Progress/include/halconcpp)
link_directories(D:/MVTec/HALCON-21.11-Progress/lib/x64-win64)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# 添加子目录
add_subdirectory(Config)
add_subdirectory(CustomizeDialog)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Resource/Image/image.qrc
        Resource/File/file.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
            ${APP_ICON_RESOURCE_WINDOWS}
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# 链接 VTK
target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
# 链接 HALCON
target_link_libraries(${PROJECT_NAME} PRIVATE halconcpp.lib halcon.lib)
# 链接自定义对话框
target_link_libraries(${PROJECT_NAME} PUBLIC Config CustomizeDialog)

# 将二进制文件目录添加到包含文件的搜索路径中，以便我们找到 ProjectConfig.h
target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

# 可执行文件上的 DEBUG_POSTFIX 属性
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    # 通过设置 WIN32_EXECUTABLE 属性来取消命令行窗口。这样，你的应用程序在启动时就不会再打开命令行窗口了。
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
