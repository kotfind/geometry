add_library(core STATIC)
target_include_directories(core PRIVATE "${PROJECT_SOURCE_DIR}")

target_sources(core
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES
        run.h

        Object.h
        GeometryObject.h

        AbstractGeometry.h
        AbstractTransformation.h

        AbstractPoint.h

    PRIVATE
        run.cpp
        MainWindow.cpp
        Engine.cpp
        AbstractGeometry.cpp
        EditMode.cpp

        Calculator.cpp
        FreeCalculator.cpp
        DependantCalculator.cpp
        RestrictedCalculator.cpp

        SectionMaster.cpp
        Section.cpp
        Function.cpp

        Generator.cpp
        GeometryGenerator.cpp
        RealGenerator.cpp

        GeometryItem.cpp

        Object.cpp
        Real.cpp
        AbstractPoint.cpp

        Scene.cpp
        View.cpp

        ToolWidget.cpp
        ToolInfoWidget.cpp
        ArgumentInfo.cpp
        ArgumentInfoModel.cpp

        VariableDialog.cpp
        VariableModel.cpp
        VariableWidget.cpp
)

target_link_libraries(core PRIVATE
    Qt6::Core
    Qt6::Widgets
)

qt_add_resources(core "coreImgs"
    PREFIX "/"
    BASE "img/"
    FILES
        "img/none.svg"
        "img/create_point.svg"
        "img/remove.svg"
        "img/move.svg"
        "img/hide.svg"
)
