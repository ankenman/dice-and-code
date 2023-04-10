#pragma once

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

namespace axi {

    class AxiInjector : public sc_core::sc_module, public tlm::tlm_bw_nonblocking_transport_if<> {
    public:
        explicit AxiInjector(sc_core::sc_module_name name);

        auto start_of_simulation() -> void override;

        tlm::tlm_sync_enum
        nb_transport_bw(tlm::tlm_generic_payload &payload, tlm::tlm_phase &phase, sc_core::sc_time &time) override {
            return tlm::TLM_COMPLETED;
        }

    private:
        sc_core::sc_event event;

        auto event_handler() -> void;
    };
} // namespace axi
