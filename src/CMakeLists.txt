########## Include dirs. ##########

include_directories( ${CMAKE_SOURCE_DIR}/src/ )


########## Library dirs. ##########

link_directories(${Boost_LIBRARY_DIRS})


########## Libraries. ##########

### Exception

add_library(exception STATIC
	lib/exception/exception.cpp
	lib/exception/exit.cpp
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
	modules/communication/detail/acceptor.cpp
	modules/communication/detail/connection.cpp
	modules/communication/detail/connector.cpp
	modules/communication/detail/receiver.cpp
	modules/communication/detail/sender.cpp
	modules/communication/file.cpp
	modules/communication/message.cpp
	modules/communication/tcp_socket.cpp
	modules/communication/types.cpp
)

target_link_libraries(communication
	exception
	logging
	strand
	${Boost_SYSTEM_LIBRARIES}
)

### Game

add_library(game STATIC
       modules/game/game.cpp
       modules/game/detail/player.cpp
)

target_link_libraries(game
       logging
)

### Lobby

add_library(lobby STATIC
       modules/lobby/lobby.cpp
       modules/lobby/session.cpp
       modules/lobby/detail/session_reaper.cpp
)

target_link_libraries(lobby
       communication
       game
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


########## Programmes. ##########

### czar

# add another connection class in a library
# This class does the basic connection stuff
# currently part of the sesion in the lobby
set(czar_sources
	czar/czar.cpp
	czar/application.cpp
)

add_executable(czar
	${czar_sources}
)

target_link_libraries(czar
	communication
	${Boost_DATE_TIME_LIBRARIES}
	${Boost_SYSTEM_LIBRARIES}
)

### Zard

set(zard_sources
	zard/options.cpp
	zard/zard.cpp
)

add_executable(zard
	${zard_sources}
)

target_link_libraries(zard
	lobby
	${Boost_SYSTEM_LIBRARIES}
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
