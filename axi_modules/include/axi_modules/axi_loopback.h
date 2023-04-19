#pragma once

#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include "axi_extension.h"

namespace axi {


class AxiLoopback : public sc_core::sc_module {
public:
    explicit AxiLoopback(sc_core::sc_module_name name);
    auto start_of_simulation() -> void override;
    auto nb_transport_fw(tlm::tlm_generic_payload& payload, tlm::tlm_phase& phase,
                         sc_core::sc_time& time) -> tlm::tlm_sync_enum;
    tlm_utils::simple_target_socket<AxiLoopback> socket;

private:
    sc_core::sc_event event;
    sc_core::sc_time period{1, sc_core::SC_NS};
    auto              event_handler() -> void;
    tlm::tlm_generic_payload* okay_payload;
};
} // namespace axi
