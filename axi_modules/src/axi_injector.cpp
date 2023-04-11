#include "axi_modules/axi_injector.h"

using namespace axi;

AxiInjector::AxiInjector(sc_core::sc_module_name name) : sc_core::sc_module(name)
{
    SC_HAS_PROCESS(AxiInjector);
    SC_METHOD(event_handler);
    sensitive << event;
    dont_initialize();
    socket.register_nb_transport_bw(this, &AxiInjector::nb_transport_bw);
}

auto
AxiInjector::event_handler() -> void
{
    std::cout << "Hello from event handler : " << sc_core::sc_time_stamp() << std::endl;
}

auto
AxiInjector::start_of_simulation() -> void
{
    event.notify(2, sc_core::SC_NS);
}
auto
AxiInjector::nb_transport_bw(tlm::tlm_generic_payload& payload, tlm::tlm_phase& phase,
                             sc_core::sc_time& time) -> tlm::tlm_sync_enum
{
    return tlm::TLM_COMPLETED;
}
