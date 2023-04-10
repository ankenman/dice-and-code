#pragma once

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

namespace axi {

    class AxiLoopback : public sc_core::sc_module, public tlm::tlm_fw_nonblocking_transport_if<> {
    public:
        explicit AxiLoopback(sc_core::sc_module_name name);

        auto start_of_simulation() -> void override;

        tlm::tlm_sync_enum
        nb_transport_fw(tlm::tlm_generic_payload &payload, tlm::tlm_phase &phase, sc_core::sc_time &time) override {
            return tlm::TLM_COMPLETED;
        }

    private:
        sc_core::sc_event event;

        auto event_handler() -> void;
    };
} // namespace axi
