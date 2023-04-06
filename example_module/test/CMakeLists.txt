set(tests
        example_module_test.cpp
)

foreach(test ${tests})
    string(REPLACE _test.cpp _test target ${test})
    add_executable(${target} ${test} systemc_fixture.cc)
    add_test(NAME ${target} COMMAND ${target})
    target_link_libraries(${target} PRIVATE GTest::gtest GTest::gtest_main SystemC::systemc example_module)
endforeach()
