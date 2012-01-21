########## Basics. ##########

cmake_minimum_required(VERSION 2.8)
#set(CMAKE_VERBOSE_MAKEFILE on)
project(zar)

########## Configuration options. ##########

option(ENABLE_STRICT_COMPILATION
	"Turns on several extra compiler warnings and turns them into errors."
	ON
)
option(ENABLE_DEBUG_COMPILATION
	"Turns on several extra debug flags."
	ON
)
option(ENABLE_UNIT_TEST
	"Build the unit test programme (has additional dependencies)"
	ON
)


########## Dependencies. ##########

set(boost_components
	system
)

if(ENABLE_UNIT_TEST)
	set(boost_components ${boost_components} unit_test_framework)
endif(ENABLE_UNIT_TEST)

find_package(Boost 1.48.0 REQUIRED COMPONENTS ${boost_components})


########## Setup flags. ##########


if(NOT CONFIGURED)
	# Set our own default flags at first run.
	if(CMAKE_COMPILER_IS_GNUCXX)

		#
		# Test whether gcc is not too old.
		#
		file(WRITE
			"${CMAKE_BINARY_DIR}/gcc_test.c"
			"#if __GNUC__       < 4\n#error\n#endif\n"
			"#if __GNUC_MINOR__ < 7\n#error\n#endif\n"
			"int main(){}"
		)
		try_compile(GCC_TEST
				"${CMAKE_BINARY_DIR}"
				"${CMAKE_BINARY_DIR}/gcc_test.c"
		)

		file(REMOVE "${CMAKE_BINARY_DIR}/gcc_text.c")

		if(NOT GCC_TEST)
			message(FATAL_ERROR "GCC too old, see INSTALL for more information.")
		endif(NOT GCC_TEST)


		#
		# Set strict compilation flags.
		#
		if(ENABLE_STRICT_COMPILATION)
			# Set a bunch of warnings, as the project progresses some might be
			# too strict, but for now try to keep the warnings as strict as
			# possible.
			set(STRICT_CXX_FLAGS "-Werror")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -fstack-protector-all")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wstack-protector")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wformat=2")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wformat-security")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Winit-self")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wmissing-declarations")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wlogical-op")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wnon-virtual-dtor")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wctor-dtor-privacy")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wold-style-cast")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wmissing-field-initializers")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wno-deprecated")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -ffor-scope")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -fshow-column")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -foperator-names")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -fno-gnu-keywords")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wshadow")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wconversion")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wold-style-cast")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Woverloaded-virtual")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wsign-promo")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -pedantic")
			set(STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Weffc++")
		else(ENABLE_STRICT_COMPILATION)
			set(STRICT_CXX_FLAGS "")
		endif(ENABLE_STRICT_COMPILATION)


		#
		# Set debug compilation flags.
		#
		if(ENABLE_DEBUG_COMPILATION)
			set(DEBUG_CXX_FLAGS "-O0 -ggdb3")
			set(DEBUG_CXX_FLAGS "${DEBUG_CXX_FLAGS} -D_GLIBCXX_DEBUG")
			set(DEBUG_CXX_FLAGS "${DEBUG_CXX_FLAGS} -D_GLIBCXX_DEBUG_PEDANTIC")
		else(ENABLE_DEBUG_COMPILATION)
			set(DEBUG_CXX_FLAGS "")
		endif(ENABLE_DEBUG_COMPILATION)

		#
		# Set compiler flags
		#
		set(CMAKE_CXX_FLAGS
			"-O2 -std=c++11 -W -Wall -Wextra -pedantic ${STRICT_CXX_FLAGS} ${DEBUG_CXX_FLAGS} $ENV{CXXFLAGS}"
			CACHE STRING "Flags used by the CXX compiler during normal builds."
			FORCE
		)
	endif(CMAKE_COMPILER_IS_GNUCXX)

	set(CONFIGURED ON CACHE INTERNAL "")
endif(NOT CONFIGURED)


############ Add subdirectories. ###########

add_subdirectory(src)
