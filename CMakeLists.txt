cmake_minimum_required(VERSION 3.16)

project(Iconizer
        VERSION 0.1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Qml
        Quick
        QuickControls2
)

qt_standard_project_setup(REQUIRES 6.11)

# ---------------------------------------------------------------------------
# Core application logic & QML module
# ---------------------------------------------------------------------------

qt_add_library(IconizerCore STATIC
        src/models/iconmodel.h          src/models/iconmodel.cpp
        src/models/iconfiltermodel.h    src/models/iconfiltermodel.cpp
)

target_link_libraries(IconizerCore
        PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
)

target_include_directories(IconizerCore PUBLIC
        src
        src/models
)

# ---------------------------------------------------------------------------
# QML UI module
# ---------------------------------------------------------------------------

set_source_files_properties(
        qml/Main.qml
        PROPERTIES QT_RESOURCE_ALIAS "Main.qml"
)

set_source_files_properties(
        qml/components/IconCard.qml
        PROPERTIES QT_RESOURCE_ALIAS "IconCard.qml"
)

set_source_files_properties(
        qml/components/IconGrid.qml
        PROPERTIES QT_RESOURCE_ALIAS "IconGrid.qml"
)

set_source_files_properties(
        qml/views/IconLibrary.qml
        PROPERTIES QT_RESOURCE_ALIAS "IconLibrary.qml"
)

qt_add_qml_module(IconizerCore
        URI Iconizer
        VERSION 1.0
        PLUGIN_TARGET IconizerUiplugin

        QML_FILES
        qml/Main.qml
        qml/components/IconCard.qml
        qml/components/IconGrid.qml
        qml/views/IconLibrary.qml

        RESOURCES
        resources/icons/folder.svg
        resources/icons/documents.svg
        resources/icons/games.svg
        resources/icons/music.svg
        resources/icons/pictures.svg
)

# ---------------------------------------------------------------------------
# Application
# ---------------------------------------------------------------------------

qt_add_executable(Iconizer
        src/main.cpp
)

target_link_libraries(Iconizer
        PRIVATE
        IconizerCore
        IconizerUiplugin
        Qt6::Gui
        Qt6::Quick
        Qt6::QuickControls2
)