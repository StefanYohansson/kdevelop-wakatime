set(kdevwakatime_SRCS
    kdevwakatime.cpp
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
)
