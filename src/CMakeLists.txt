########## Include dirs. ##########

include_directories( ${CMAKE_SOURCE_DIR}/src/ )


########## Library dirs. ##########

link_directories(${Boost_LIBRARY_DIRS})


########## Libraries. ##########

### Exception

add_library(exception STATIC
	lib/exception/exception.cpp
)

### Strand

add_library(strand STATIC
	lib/strand/strand.cpp
)

### String

# No files


########## Modules. ##########

### Communication

add_library(communication STATIC
	modules/communication/connection.cpp
	modules/communication/connector.cpp
	modules/communication/acceptor.cpp
	modules/communication/sender.cpp
	modules/communication/receiver.cpp
	modules/communication/file.cpp
	modules/communication/socket.cpp
)

# Disable -Weffc++ since fails due to inheritance.
set_target_properties(communication
	PROPERTIES
		COMPILE_FLAGS "-Wno-effc++"
)

target_link_libraries(communication
	exception
	logging
	${Boost_SYSTEM_LIBRARIES}
)

### Logging

add_library(logging
	modules/logging/level.cpp
	modules/logging/logger.cpp
	modules/logging/log.cpp
	modules/logging/module_logger.cpp
)

target_link_libraries(logging
	exception
)


########## Unit tests. ##########

if(ENABLE_UNIT_TEST)

	set(unit_test_sources
		unit_test/unit_test.cpp
		unit_test/lib/string.cpp
	)

	add_executable(unit_test
		${unit_test_sources}
	)

	set_target_properties(unit_test
		PROPERTIES
			COMPILE_FLAGS "-Wno-effc++"
			COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
	)

	target_link_libraries(unit_test
		exception
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)

endif(ENABLE_UNIT_TEST)
