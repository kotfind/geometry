cmake_minimum_required(VERSION 3.23)
project(geometry LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS_DEBUG "-fdiagnostics-color=always -fsanitize=undefined,address -g3 -O0")

find_package(Qt6 REQUIRED
    Core
    Widgets
    Svg
)

qt_standard_project_setup()

add_subdirectory(core)
add_subdirectory(util)
add_subdirectory(euclidian)
add_subdirectory(hyperbolic)

add_executable(geometry)

target_sources(geometry PRIVATE
    main.cpp
)

target_link_libraries(geometry PRIVATE
    core
    euclidian
    hyperbolic
)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.out)

    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    add_custom_target(
        docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} -q ${DOXYFILE_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )
else (DOXYGEN_FOUND)
    message("Doxygen not found")
endif (DOXYGEN_FOUND)
