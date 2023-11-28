find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(SetDialog setdialog.h setdialog.cpp setdialog.ui)

target_link_libraries(SetDialog PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# 声明任何链接到我们的项目都需要包含当前源目录才能找到头文件，而我们不需要
target_include_directories(SetDialog PRIVATE
                           ${CMAKE_SOURCE_DIR}/Config
                           )
