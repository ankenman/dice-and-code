add_library(axi_modules axi_injector.cpp axi_loopback.cpp)
target_include_directories(axi_modules PUBLIC ../include)
target_link_libraries(axi_modules PRIVATE  SystemC::systemc )
set_target_properties(axi_modules PROPERTIES VERSION ${CONAN_PACKAGE_VERSION})

add_executable(axi_main main.cpp axi_injector.cpp axi_loopback.cpp)
#target_include_directories(axi_main PUBLIC ../include)
target_link_libraries(axi_main PRIVATE  SystemC::systemc axi_modules)
set_target_properties(axi_main PROPERTIES VERSION ${CONAN_PACKAGE_VERSION})

