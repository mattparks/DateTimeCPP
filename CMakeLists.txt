cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)
project(DateTimeCPP VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_executable(DateTimeCPP
		date/date.h
		date/ios.h
		date/tz.h
		date/tz.cpp
		date/tz_private.h
		Date.hpp
		DateFormats.hpp
		DateTime.hpp
		DateTime.inl
		Main.cpp
		Time.hpp
		TimeDelta.hpp
		)

target_include_directories(DateTimeCPP PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_features(DateTimeCPP PUBLIC cxx_std_17)
