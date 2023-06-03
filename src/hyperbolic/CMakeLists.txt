add_library(hyperbolic STATIC)
target_include_directories(hyperbolic PRIVATE "${PROJECT_SOURCE_DIR}")

target_sources(hyperbolic
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES
        Geometry.h

    PRIVATE
        Geometry.cpp
        functionList.cpp

        Transformation.cpp

        Point.cpp
        Line.cpp
        Circle.cpp
)

target_link_libraries(hyperbolic PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Svg

    core
    util
    euclidian
)

qt_add_resources(hyperbolic "hyperbolicImgs"
    PREFIX "/"
    BASE "img/"
    FILES
        "img/point/Middle.svg"
        "img/point/Intersect.svg"

        "img/line/ByTwoPoints.svg"
        "img/line/Perpendicular.svg"
        "img/line/Hyperparallel.svg"
        "img/line/Horoparallel.svg"

        "img/circle/ByCenterAndPoint.svg"
        "img/circle/ByCenterAndRadius.svg"
)
