#pragma once

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

#include "tlm_memory_manager.h"
#include "axi_extension.h"

namespace axi {

class AxiInjector : public sc_core::sc_module {
public:
    explicit AxiInjector(sc_core::sc_module_name name);
    auto start_of_simulation() -> void override;
    auto nb_transport_bw(tlm::tlm_generic_payload& payload, tlm::tlm_phase& phase,
                         sc_core::sc_time& time) -> tlm::tlm_sync_enum;
    tlm_utils::simple_initiator_socket<AxiInjector> socket;

    auto write_event_handler() -> void;
    auto read_event_handler() -> void;

    void create_write(std::size_t num_chunks = 4);

    auto has_write_in_progress() -> bool;

private:
    sc_core::sc_time          period{1.0, sc_core::SC_NS};
    sc_core::sc_event         write_event;
    sc_core::sc_event         read_event;
    bool                      write_ready{};
    bool                      write_valid{};
    tlm::tlm_generic_payload* write_in_progress{};
    tlm::tlm_generic_payload* write_to_start{};
    sc_core::sc_time          zero_time = sc_core::SC_ZERO_TIME;

    void send_write_data(tlm::tlm_generic_payload& payload);
    auto have_write_to_set_valid() -> bool;
};
} // namespace axi
