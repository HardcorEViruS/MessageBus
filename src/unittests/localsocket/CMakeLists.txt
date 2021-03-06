find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Test REQUIRED)

# Test: LocalSocket
set(APPNAME "test_localsocket")

set(SOURCES
	testlocalsocket.cpp
	testlocalsocket_peer.cpp
	testlocalsocketpeerthread.cpp
	../logger.cpp

	# Actual sources of tested classes
	../../localserver.cpp
	../../localsocket.cpp
	../../localsocketprivate.cpp
	../../implementations/localsocketprivate_unix.cpp
	../../tools.cpp
	../../variant.cpp
)

set(HEADERS
	testlocalsocket.h
	testlocalsocket_peer.h
	testlocalsocketpeerthread.h

	# Actual headers of tested classes
	../../localserver.h
	../../localsocket.h
	../../localsocketprivate.h
	../../implementations/localsocketprivate_unix.h
	# 	../tools.h
	# 	../variant.h
)

set(MOC_SRCS)
qt5_wrap_cpp(MOC_SRCS ${HEADERS})

add_executable(${APPNAME} ${SOURCES} ${MOC_SRCS})
target_link_libraries(${APPNAME} Qt5::Core Qt5::Network Qt5::Test)
add_test(NAME ${APPNAME} COMMAND ${APPNAME} -xunitxml -o "${APPNAME}.xunit.xml")

set(SOURCES
	testlocalsocket_external_peer.cpp
	testlocalsocket_external_peer_object.cpp
	../logger.cpp

	# Actual sources of tested classes
	../../localsocket.cpp
	../../localsocketprivate.cpp
	../../implementations/localsocketprivate_unix.cpp
	../../tools.cpp
	../../variant.cpp
)

set(HEADERS
	testlocalsocket_external_peer_object.h

	# Actual headers of tested classes
	../../localsocket.h
	../../localsocketprivate.h
	../../implementations/localsocketprivate_unix.h
	# 	../tools.h
	# 	../variant.h
)

set(MOC_SRCS)
qt5_wrap_cpp(MOC_SRCS ${HEADERS})

add_executable("${APPNAME}_external_peer" ${SOURCES} ${MOC_SRCS})
target_link_libraries("${APPNAME}_external_peer" Qt5::Core Qt5::Network Qt5::Test)
add_dependencies(${APPNAME} "${APPNAME}_external_peer")
