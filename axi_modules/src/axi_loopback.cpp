#include "axi_modules/axi_loopback.h"

using namespace axi;
using namespace tlm;
using namespace sc_core;

AxiLoopback::AxiLoopback(sc_core::sc_module_name name) : sc_core::sc_module(name)
{
    SC_HAS_PROCESS(AxiLoopback);
    SC_METHOD(event_handler);
    sensitive << event;
    dont_initialize();
    socket.register_nb_transport_fw(this, &AxiLoopback::nb_transport_fw);
}

auto
AxiLoopback::event_handler() -> void
{
    std::cout << "Hello from loopback event: " << sc_core::sc_time_stamp() << std::endl;
    tlm_phase phase = B_VALID;
    okay_payload->get_extension<AxiExtension>()->bresp = BRESP::OKAY;
    sc_time zero_time{};
    socket->nb_transport_bw(*okay_payload, phase, zero_time);
    okay_payload->release();
    okay_payload = nullptr;
}

auto
AxiLoopback::start_of_simulation() -> void
{
    tlm_generic_payload payload;
    tlm_phase           phase = axi::WRITE_READY;
    sc_time             time;
    socket->nb_transport_bw(payload, phase, time);
}
auto
AxiLoopback::nb_transport_fw(tlm::tlm_generic_payload& payload, tlm::tlm_phase& phase,
                             sc_core::sc_time& time) -> tlm::tlm_sync_enum
{
    std::cout << "Hello from nb_transport_fw : " << sc_core::sc_time_stamp() << std::endl;
    if (phase == DATA_LAST_CHUNK) {
        okay_payload = &payload;
        okay_payload->acquire();
        event.notify(period);
    }
    return tlm::TLM_COMPLETED;
}
