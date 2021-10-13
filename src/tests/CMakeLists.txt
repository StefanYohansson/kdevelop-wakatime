find_program(WAKATIME_FOUND NAMES wakatime)

include(ECMAddTests)
find_package(Qt5 REQUIRED COMPONENTS Test)
find_package(Qt5Test ${QT_MIN_VERSION} CONFIG REQUIRED)


if (WAKATIME_FOUND)
	remove_definitions(
		-DQT_NO_CAST_FROM_ASCII
		-DQT_NO_CAST_TO_ASCII
		-DQT_NO_CAST_FROM_BYTEARRAY
	)

	ecm_add_test(test_kdevwakatime.cpp
			LINK_LIBRARIES
			Qt5::Test Qt5::Core
			KDev::Tests KDev::Interfaces)
endif ()
