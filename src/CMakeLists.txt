cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

add_definitions(-DTRANSLATION_DOMAIN=\"kdevwakatime\")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

set(KDEV_MIN_VERSION 5.1.2)

find_package(KDevPlatform ${KDEV_MIN_VERSION} REQUIRED)
find_package(KF5TextEditor)

if(BUILD_TESTING)
	add_subdirectory(tests)
endif()


set(kdevwakatime_SRCS
    wakatime.cpp
)

ecm_qt_declare_logging_category(kdevwakatime_SRCS
    HEADER debug.h
    IDENTIFIER PLUGIN_KDEVWAKATIME
    CATEGORY_NAME "kdevelop.plugins.kdevwakatime"
    DESCRIPTION "KDevelop plugin: kdevwakatime"
    EXPORT KDEVWAKATIME
)

kdevplatform_add_plugin(kdevwakatime
    SOURCES ${kdevwakatime_SRCS}
)

target_link_libraries(kdevwakatime
    KDev::Interfaces
    KF5::I18n
    KF5::TextEditor
)
