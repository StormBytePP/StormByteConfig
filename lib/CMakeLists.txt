include(GNUInstallDirs)

# Sources
file(GLOB_RECURSE STORMBYTE_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cxx")

# Library
add_library(StormByte-Config SHARED ${STORMBYTE_SOURCES})
add_library(StormByte::Config ALIAS StormByte-Config)
target_link_libraries(StormByte-Config PUBLIC StormByte)
set_target_properties(StormByte-Config PROPERTIES
	LINKER_LANGUAGE CXX
	SOVERSION		${CMAKE_PROJECT_VERSION}
	VERSION 		${CMAKE_PROJECT_VERSION}
)

# Compile options
if(MSVC)
	target_compile_options(StormByte-Config PRIVATE /wd4251 /EHsc /O2 /Gs)
	target_link_options(StormByte-Config PRIVATE /LTCG /GL)
else()
	set(CMAKE_CXX_FLAGS_DEBUG "-pipe -g -ggdb -Wall -Wextra -Wnon-virtual-dtor -pedantic -pedantic-errors -O0")
	target_compile_options(StormByte-Config PRIVATE -fvisibility=hidden $<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden>)
endif()

# Include directories
target_include_directories(StormByte-Config
	SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_LIST_DIR}"
)

# Install
if (NOT STORMBYTE_AS_DEPENDENCY)
	install(TARGETS StormByte-Config
		ARCHIVE 		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		LIBRARY 		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME 		DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
	install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/StormByte"
		FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hxx"
	)
endif()
