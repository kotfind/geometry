add_library(util STATIC)
target_include_directories(util PRIVATE "${PROJECT_SOURCE_DIR}")

target_sources(util
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES
        TR.h

        math.h
        cramer.h

        getOrThrow.h
        IOError.h

    PRIVATE
        math.cpp
        cramer.cpp

        getOrThrow.cpp
)

target_link_libraries(util PRIVATE
    Qt6::Core
)
