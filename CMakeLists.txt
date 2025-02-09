cmake_minimum_required(VERSION 3.5)

project(QuantumLeads VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
set(Qt_DIR_HINT "${Qt${QT_VERSION_MAJOR}_DIR}")
message("Qt found in path: ${Qt_DIR_HINT}")
find_package(QXlsxQt6 REQUIRED HINTS "${Qt_DIR_HINT}/../../../../lib/cmake/QXlsxQt6")

set(PROJECT_VERSION_MAJOR 3)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_RELEASE 0)
set(PROJECT_VERSION_BETA 0)
set(PROJECT_VERSION_DESC "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_RELEASE}.${PROJECT_VERSION_BETA}")


add_definitions(-DPROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_definitions(-DPROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR})
add_definitions(-DPROJECT_VERSION_RELEASE=${PROJECT_VERSION_RELEASE})
add_definitions(-DPROJECT_VERSION_BETA=${PROJECT_VERSION_BETA})
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

include(../common/workingdirectory/workingdirectory.cmake)
include(../common/utils/utils.cmake)
include(model/model.cmake)
include(gui/gui.cmake)

set(PROJECT_SOURCES
        main.cpp
        "../common/types/types.h"
        "../common/types/types.cpp"
        ${MODEL_FILES}
        ${GUI_FILES}
        ${UTILS_FILES}
        ${WORKING_DIR_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QuantumLeads
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QuantumLeads APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QuantumLeads SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QuantumLeads
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QuantumLeads PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    QXlsx::QXlsx
)

set_target_properties(QuantumLeads PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS QuantumLeads
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QuantumLeads)
endif()
