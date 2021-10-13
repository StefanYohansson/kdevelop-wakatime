cmake_minimum_required(VERSION 3.0)

project(kdevwakatime)

find_package(ECM "5.68.0" REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)
include(FeatureSummary)

find_package(KDevPlatform 5.2.40 REQUIRED)
find_package(KF5 5.15.0 REQUIRED COMPONENTS I18n)

add_subdirectory(src)

ecm_qt_install_logging_categories(
    EXPORT KDEVWAKATIME
    FILE kdevwakatime.categories
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
