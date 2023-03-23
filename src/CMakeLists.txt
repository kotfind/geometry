cmake_minimum_required(VERSION 3.16)

project(geometry LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=undefined,address -g3 -O0")

find_package(Qt6 REQUIRED
    Core
    Widgets
)

qt_standard_project_setup()

add_executable(geometry
    main.cpp
    global.cpp

    Function.cpp
    functionList.cpp

    Generator.cpp
    FreeGenerator.cpp
    DependantGenerator.cpp

    Point.cpp
    Line.cpp
    Circle.cpp

    MainWindow.cpp

    GeometryItem.cpp

    Scene.cpp
    View.cpp

    Geometry.cpp

    getOrThrow.cpp

    Object.cpp

    Type.cpp

    ToolInfoWidget.cpp

    ArgumentInfo.cpp
    ArgumentInfoModel.cpp

    EditMode.cpp

    cramer.cpp

    Section.cpp

    ToolModel.cpp
)

target_link_libraries(geometry
    Qt6::Core
    Qt6::Widgets
)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.out)

    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    add_custom_target(
        docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )
else (DOXYGEN_FOUND)
    message("Doxygen not found")
endif (DOXYGEN_FOUND)
