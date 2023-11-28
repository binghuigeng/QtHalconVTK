find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(Config SysConfig.h SysConfig.cpp)

# 声明任何链接到我们的项目都需要包含当前源目录才能找到头文件，而我们不需要
target_include_directories(Config INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           )

target_link_libraries(Config PRIVATE Qt${QT_VERSION_MAJOR}::Core)
