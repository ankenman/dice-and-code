add_library(example_module example_module.cpp)
target_include_directories(example_module PUBLIC ../include)
target_link_libraries(example_module PRIVATE  SystemC::systemc )
set_target_properties(example_module PROPERTIES VERSION ${CONAN_PACKAGE_VERSION})

