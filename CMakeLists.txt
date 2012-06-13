CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (DWD-CONVERTER)

SET(QT_DONT_USE_QTGUI FALSE)
FIND_PACKAGE(Qt 4.5 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

SET (SOURCES
	main.cpp
	magnitudes.h
	magnitudes.cpp
	station.h
	station.cpp
	timespan.h
	timespan.cpp
	values.h
	values.cpp
	weatherstation.h
	weatherstation.cpp
)

INCLUDE_DIRECTORIES( . )

ADD_EXECUTABLE(dwd-converter ${SOURCES})

TARGET_LINK_LIBRARIES(dwd-converter ${QT_LIBRARIES})