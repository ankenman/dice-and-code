#include "example_module/example_module.h"

using namespace example;

auto ExampleModule::something() -> int {
    return 42;
}

auto ExampleModule::event_handler() -> void {
    std::cout << "Hello from event handler : " << sc_core::sc_time_stamp() << std::endl;
}

auto ExampleModule::start_of_simulation() -> void {
    e.notify(2, sc_core::SC_NS);
}

ExampleModule::ExampleModule(sc_core::sc_module_name name) :
        sc_core::sc_module(name) {
    SC_HAS_PROCESS(ExampleModule);
    SC_METHOD(event_handler);
    sensitive << e;
    dont_initialize();
}

