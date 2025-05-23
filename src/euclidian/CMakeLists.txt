add_library(euclidian STATIC)
target_include_directories(euclidian PRIVATE "${PROJECT_SOURCE_DIR}")

target_sources(euclidian
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
        Segment.cpp
        Arc.cpp
)

target_link_libraries(euclidian PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Svg

    core
    util
)

qt_add_resources(euclidian "euclidianImgs"
    PREFIX "/"
    BASE "img/"
    FILES
        "img/point/Intersect.svg"
        "img/point/Middle.svg"

        "img/line/ByTwoPoints.svg"
        "img/line/Segment.svg"
        "img/line/Perpendicular.svg"
        "img/line/Parallel.svg"
        "img/line/Bisector.svg"
        "img/line/Tangents.svg"
        "img/line/CommonTangents.svg"

        "img/circle/ByCenterAndPoint.svg"
        "img/circle/ByCenterAndRadius.svg"
        "img/circle/ByThreePoints.svg"
        "img/circle/Incircle.svg"

        "img/measurement/Distance.svg"
        "img/measurement/Radius.svg"
        "img/measurement/Length.svg"
        "img/measurement/Angle.svg"
)
