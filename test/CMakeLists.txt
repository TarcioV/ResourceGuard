set(USE_CLASS_VERSION OFF)

# enable class version in c++17 and latters
if(CMAKE_CXX_STANDARD GREATER 14)
	# skip vs2015 and earlier
	if(NOT MSVC OR MSVC_VERSION GREATER 1900)
		set(USE_CLASS_VERSION ON)
	endif()
endif()

if(USE_CLASS_VERSION)
	message(STATUS "Test using class version of the library")
	set(TEST_FILE testResourceGuardModern.cpp)
	set(TEST_SINGLE_FILE testSingleResourceGuardModern.cpp)
else()
	message(STATUS "Test using macro version of the library")
	set(TEST_FILE testResourceGuard.cpp)
	set(TEST_SINGLE_FILE testSingleResourceGuard.cpp)
endif()

message(STATUS "TEST_FILE - ${TEST_FILE}")
message(STATUS "TEST_SINGLE_FILE - ${TEST_SINGLE_FILE}")

add_executable(testResourceGuard ${TEST_FILE})
target_include_directories(testResourceGuard PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_test(NAME TestResouceGuardTTT COMMAND testResourceGuard 
	--device-pass=true 
	--window-pass=true 
	--context-pass=true
	)
set_tests_properties(TestResouceGuardTTT PROPERTIES FAIL_REGULAR_EXPRESSION "close_window")

add_test(NAME TestResouceGuardTTF COMMAND testResourceGuard 
	--device-pass=true 
	--window-pass=true 
	--context-pass=false
	)
set_tests_properties(TestResouceGuardTTF PROPERTIES PASS_REGULAR_EXPRESSION "close_window")

add_executable(testSingleResourceGuard ${TEST_SINGLE_FILE})
target_include_directories(testSingleResourceGuard PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_test(NAME TestSingleResouceGuardTT COMMAND testSingleResourceGuard 
	--device-pass=true 
	--window-pass=true 
	)
set_tests_properties(TestSingleResouceGuardTT PROPERTIES FAIL_REGULAR_EXPRESSION "close_device")

add_test(NAME TestSingleResouceGuardTF COMMAND testSingleResourceGuard 
	--device-pass=true 
	--window-pass=false 
	)
set_tests_properties(TestSingleResouceGuardTF PROPERTIES PASS_REGULAR_EXPRESSION "close_device")