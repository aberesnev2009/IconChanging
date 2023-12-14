cmake_minimum_required(VERSION 3.16)

project(changeIcon VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Widgets)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appchangeIcon
    main.cpp
    App.cpp
    App.h
    qml.qrc
)



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appchangeIcon PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appchangeIcon
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appchangeIcon
    PRIVATE Qt6::Quick Qt6::Core Qt6::Widgets
)
if(QT_VERSION_MAJOR EQUAL 6)
  qt_import_qml_plugins(changeIcon)
  qt_finalize_executable(changeIcon)
endif()
include(GNUInstallDirs)
install(TARGETS appchangeIcon
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
