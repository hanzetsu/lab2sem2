cmake_minimum_required(VERSION 3.16)
project(LAB2SEM2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Widgets)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
endif()

set(HEADERS_COMMON
    include/BitSequence.hpp
    include/DynamicArray.hpp
    include/ImmutableArraySequence.hpp
    include/ImmutableListSequence.hpp
    include/LinkedList.hpp
    include/MutableArraySequence.hpp
    include/MutableListSequence.hpp
    include/Sequence.hpp
    include/Exceptions.hpp
)

set(HEADERS_GUI
    include/mainwindow.hpp
)

set(SOURCES_GUI
    source/main.cpp
    source/mainwindow.cpp
)

set(UI_FILES
    source/mainwindow.ui
)

qt_add_executable(LAB2SEM2
    ${SOURCES_GUI}
    ${HEADERS_COMMON}
    ${HEADERS_GUI}
    ${UI_FILES}
)

target_include_directories(LAB2SEM2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(Qt6_FOUND)
    target_link_libraries(LAB2SEM2 PRIVATE Qt6::Core Qt6::Widgets)
else()
    target_link_libraries(LAB2SEM2 PRIVATE Qt5::Core Qt5::Widgets)
endif()

qt_add_executable(tests
    source/test.cpp
    ${HEADERS_COMMON}          
)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(Qt6_FOUND)
    target_link_libraries(tests PRIVATE Qt6::Core)
else()
    target_link_libraries(tests PRIVATE Qt5::Core)
endif()