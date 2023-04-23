###################################################################################
################## Butianyun QT Video Lesson V2 ###################################
########### BUTIANYUN， QT Programming Trainning Professional ######################
####################################################################################


cmake_minimum_required(VERSION 3.5)
project(butianyun-alpaca VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        models/ButianyunFileSystemModel/butianyunfilesystemmodel.cpp
)

set(PROJECT_HEADERS
    common/butianyun_alpaca_common_api.h
    models/ButianyunFileSystemModel/butianyunfilesystemmodel.h
)

add_library(butianyun-alpaca
    SHARED
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_compile_definitions(butianyun-alpaca PRIVATE BUTIANYUN_ALPACA_COMMON_API_EXPORTS)
target_include_directories(butianyun-alpaca PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common)

target_link_libraries(butianyun-alpaca
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(butianyun-alpaca PROPERTIES
   PUBLIC_HEADER "${PROJECT_HEADERS}"
)

set_target_properties(butianyun-alpaca PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER alpaca.qt.butianyun.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set(PROJECT_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../modules)
set(PROJECT_LIB_DIR ${PROJECT_MODULE_DIR}/lib)

install(TARGETS butianyun-alpaca
        RUNTIME DESTINATION ${PROJECT_LIB_DIR}
        LIBRARY DESTINATION ${PROJECT_LIB_DIR}
        ARCHIVE DESTINATION ${PROJECT_LIB_DIR}
        PUBLIC_HEADER DESTINATION ${PROJECT_MODULE_DIR}/include
)
