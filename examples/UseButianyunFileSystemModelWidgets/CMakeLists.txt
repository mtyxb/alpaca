###################################################################################
################## Butianyun QT Video Lesson V2 ###################################
########### BUTIANYUN， QT Programming Trainning Professional ######################
####################################################################################


cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0071 NEW)

project(UseButianyunFileSystemModelWidgets VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/cmake)
find_package(BUTIANYUN_ALPACA REQUIRED)
message(STATUS {${BUTIANYUN_ALPACA_FOUND}})
message(STATUS ${BUTIANYUN_ALPACA_INCLUDE_DIR})
message(STATUS ${BUTIANYUN_ALPACA_LIBRARY})



set(PROJECT_SOURCES
        butianyun.cpp
        butianyunwidget.cpp
        butianyunwidget.h
)

qt_add_executable(UseButianyunFileSystemModelWidgets
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_include_directories(UseButianyunFileSystemModelWidgets
    PRIVATE ${BUTIANYUN_ALPACA_INCLUDE_DIR})

target_link_libraries(UseButianyunFileSystemModelWidgets
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE ${BUTIANYUN_ALPACA_LIBRARY}
)

set_target_properties(UseButianyunFileSystemModelWidgets PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set(PROJECT_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../modules)

install(TARGETS UseButianyunFileSystemModelWidgets
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${PROJECT_MODULE_DIR}/lib
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UseButianyunFileSystemModelWidgets)
endif()
