add_subdirectory(AboutDialog)
add_subdirectory(CommonDialog)

add_library(CustomizeDialog)

# 声明任何链接到我们的项目都需要包含当前源目录才能找到头文件，而我们不需要
target_include_directories(CustomizeDialog INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}/AboutDialog
                           ${CMAKE_CURRENT_SOURCE_DIR}/CommonDialog
                           )

target_link_libraries(CustomizeDialog PRIVATE AboutDialog CommonDialog)
