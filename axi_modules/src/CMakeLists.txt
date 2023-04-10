add_library(axi_modules axi_injector.cpp)
target_include_directories(axi_modules PUBLIC ../include)
target_link_libraries(axi_modules PRIVATE  SystemC::systemc )
set_target_properties(axi_modules PROPERTIES VERSION ${CONAN_PACKAGE_VERSION})

