cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)
project(DateTimeCPP VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_executable(DateTimeCPP
		DateTime.hpp
		DateTimeFormat.hpp
		DateTimeFormatter.hpp
		DateTimeParser.hpp
		LocalDateTime.hpp
		Timespan.hpp
		Timestamp.hpp
		Timezone.hpp
		DateTime.cpp
		DateTimeFormatter.cpp
		DateTimeParser.cpp
		LocalDateTime.cpp
		Main.cpp
		Timespan.cpp
		Timestamp.cpp
		)

if(WIN32)
	target_sources(DateTimeCPP PRIVATE TimezoneWin32.cpp)
else()
	target_sources(DateTimeCPP PRIVATE TimezoneUnix.cpp)
endif()

target_include_directories(DateTimeCPP PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_features(DateTimeCPP PUBLIC cxx_std_17)
