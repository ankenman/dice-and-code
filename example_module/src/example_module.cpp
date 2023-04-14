#include "example_module/example_module.h"

using namespace example;

ExampleModule::ExampleModule(sc_core::sc_module_name name) : sc_core::sc_module(name)
{
    SC_HAS_PROCESS(ExampleModule);
    SC_METHOD(event_handler);
    sensitive << event;
    dont_initialize();
    target_socket.register_nb_transport_fw(this, &ExampleModule::nb_transport_fw);
    initiator_socket.register_nb_transport_bw(this, &ExampleModule::nb_transport_bw);
}

auto
ExampleModule::something() -> int
{
    return 42;
}

auto
ExampleModule::event_handler() -> void
{
    std::cout << "Hello from event handler : " << sc_core::sc_time_stamp() << std::endl;
}

auto
ExampleModule::start_of_simulation() -> void
{
    event.notify(2, sc_core::SC_NS);
}
auto
ExampleModule::nb_transport_fw(tlm::tlm_generic_payload& payload, tlm::tlm_phase& phase,
                               sc_core::sc_time& time) -> tlm::tlm_sync_enum
{
    return tlm::TLM_COMPLETED;
}
auto
ExampleModule::nb_transport_bw(tlm::tlm_generic_payload& payload, tlm::tlm_phase& phase,
                               sc_core::sc_time& time) -> tlm::tlm_sync_enum
{
    return tlm::TLM_COMPLETED;
}
