find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(CommonDialog commondialog.h commondialog.cpp commondialog.ui)

target_link_libraries(CommonDialog PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
