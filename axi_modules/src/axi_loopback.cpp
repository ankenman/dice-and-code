#include "axi_modules/axi_loopback.h"

using namespace axi;

AxiLoopback::AxiLoopback(sc_core::sc_module_name name) :
        sc_core::sc_module(name) {
    SC_HAS_PROCESS(AxiLoopback);
    SC_METHOD(event_handler);
    sensitive << event;
    dont_initialize();

//    socket.bind(*this);
}

auto AxiLoopback::event_handler() -> void {
    std::cout << "Hello from event handler : " << sc_core::sc_time_stamp() << std::endl;
}

auto AxiLoopback::start_of_simulation() -> void {
    event.notify(2, sc_core::SC_NS);
}


