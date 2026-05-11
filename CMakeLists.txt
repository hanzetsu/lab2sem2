cmake_minimum_required(VERSION 3.16)
project(LAB2SEM2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Widgets)

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
target_link_libraries(LAB2SEM2 PRIVATE Qt6::Core Qt6::Widgets)


file(GLOB TEST_SOURCES "tests/*.cpp")
file(GLOB TEST_HEADERS "tests/*.hpp")

qt_add_executable(tests
    ${TEST_SOURCES}
    ${TEST_HEADERS}
    ${HEADERS_COMMON}
)
target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(tests PRIVATE Qt6::Core)