cmake_minimum_required(VERSION 3.16)
project(LAB2SEM2)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Widgets)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
endif()

qt_add_executable(LAB2SEM2
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
    Bit.hpp
    BitSequence.hpp
    DynamicArray.hpp
    ImmutableArraySequence.hpp
    ImmutableListSequence.hpp
    LinkedList.hpp
    MutableArraySequence.hpp
    MutableListSequence.hpp
    Sequence.hpp
    mainwindow.ui
)


if(Qt6_FOUND)
    target_link_libraries(LAB2SEM2 PRIVATE Qt6::Core Qt6::Widgets)
else()
    target_link_libraries(LAB2SEM2 PRIVATE Qt5::Core Qt5::Widgets)
endif()