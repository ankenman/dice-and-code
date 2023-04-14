#pragma once

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

namespace example {

class ExampleModule : public sc_core::sc_module {
public:
    explicit ExampleModule(sc_core::sc_module_name name);
    auto something() -> int;
    auto start_of_simulation() -> void override;
    auto nb_transport_fw(tlm::tlm_generic_payload& payload, tlm::tlm_phase& phase,
                         sc_core::sc_time& time) -> tlm::tlm_sync_enum;
    auto nb_transport_bw(tlm::tlm_generic_payload& payload, tlm::tlm_phase& phase,
                         sc_core::sc_time& time) -> tlm::tlm_sync_enum;

    tlm_utils::simple_initiator_socket<ExampleModule> initiator_socket;
    tlm_utils::simple_target_socket<ExampleModule>    target_socket;

private:
    sc_core::sc_event event;

    auto event_handler() -> void;
};
} // namespace example
