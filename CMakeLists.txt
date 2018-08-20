cmake_minimum_required(VERSION 3.2)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
   project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
   project(allocator VERSION 0.0.1)
endif()

if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(NDEBUG 1)
endif()

configure_file(ndebug.h.in ${PROJECT_SOURCE_DIR}/ndebug.h)

file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/*.h)

add_executable(allocator main.cpp ${HEADER_FILES})

set_target_properties(allocator PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set_target_properties(allocator PROPERTIES COMPILE_OPTIONS -W3)
else()
	set_target_properties(allocator PROPERTIES COMPILE_OPTIONS -Wpedantic -Wall -Wextra)
endif()


install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT a@a.a)

include (CPack)

