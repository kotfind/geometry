add_library(euclidian STATIC)
target_include_directories(euclidian PRIVATE "${PROJECT_SOURCE_DIR}")

target_sources(euclidian
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES
        EGeometry.h

    PRIVATE
        EGeometry.cpp
        EFunctionList.cpp

        ETransformation.cpp

        Point.cpp
        Line.cpp
        Circle.cpp
)

target_link_libraries(euclidian PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Svg

    core
    util
)

qt_add_resources(euclidian "imgs"
    PREFIX "/"
    BASE "img/"
    FILES
        "img/none.svg"
)
