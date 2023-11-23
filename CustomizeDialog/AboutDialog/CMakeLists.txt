find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(AboutDialog aboutdialog.h aboutdialog.cpp aboutdialog.ui)

target_link_libraries(AboutDialog PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# 将根项目的二进制文件目录添加到包含文件的搜索路径中，以便我们找到 ProjectConfig.h
target_include_directories(AboutDialog PUBLIC
                           "${CMAKE_BINARY_DIR}"
                           )
