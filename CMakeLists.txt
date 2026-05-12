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

set(HEADERS_GUI include/mainwindow.hpp)
set(SOURCES_GUI source/main.cpp source/mainwindow.cpp)
set(UI_FILES source/mainwindow.ui)

qt_add_executable(LAB2SEM2
    ${SOURCES_GUI}
    ${HEADERS_COMMON}
    ${HEADERS_GUI}
    ${UI_FILES}
)
target_include_directories(LAB2SEM2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(LAB2SEM2 PRIVATE Qt6::Core Qt6::Widgets)


set(TEST_COMMON_HEADERS
    ${HEADERS_COMMON}
    tests/test_common.hpp
)

function(add_test_executable TARGET_NAME SOURCE_FILE)
    qt_add_executable(${TARGET_NAME}
        ${SOURCE_FILE}
        ${TEST_COMMON_HEADERS}
    )
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
    target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Core)
endfunction()

add_test_executable(test_bit_sequence     tests/test_bit_sequence.cpp)
add_test_executable(test_array_sequence   tests/test_array_sequence.cpp)
add_test_executable(test_list_sequence    tests/test_list_sequence.cpp)
add_test_executable(test_map_reduce       tests/test_map_reduce.cpp)

enable_testing()

add_test(NAME test_bit_sequence COMMAND test_bit_sequence)
add_test(NAME test_array_sequence COMMAND test_array_sequence)
add_test(NAME test_list_sequence COMMAND test_list_sequence)
add_test(NAME test_map_reduce COMMAND test_map_reduce)