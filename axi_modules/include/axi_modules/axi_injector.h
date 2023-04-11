#pragma once

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

namespace axi {

class AxiInjector : public sc_core::sc_module {
public:
    explicit AxiInjector(sc_core::sc_module_name name);
    auto start_of_simulation() -> void override;
    auto nb_transport_bw(tlm::tlm_generic_payload& payload, tlm::tlm_phase& phase,
                         sc_core::sc_time& time) -> tlm::tlm_sync_enum;
    tlm_utils::simple_initiator_socket<AxiInjector> socket;

private:
    sc_core::sc_event event;
    auto              event_handler() -> void;
};
} // namespace axi
