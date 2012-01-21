########## Include dirs. ##########

include_directories( ${CMAKE_SOURCE_DIR}/src/ )


########## Library dirs. ##########

link_directories(${Boost_LIBRARY_DIRS})


########## Libraries. ##########

### String

# No files


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
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)

endif(ENABLE_UNIT_TEST)
