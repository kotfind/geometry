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
)

target_link_libraries(geometry
    Qt6::Core
    Qt6::Widgets
)
