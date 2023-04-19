#include "axi_modules/axi_injector.h"

using namespace axi;
using namespace tlm;
using namespace sc_core;

AxiInjector::AxiInjector(sc_core::sc_module_name name) : sc_core::sc_module(name)
{
    SC_HAS_PROCESS(AxiInjector);

    SC_METHOD(write_event_handler);
    sensitive << write_event;
    dont_initialize();

    SC_METHOD(read_event_handler);
    sensitive << read_event;
    dont_initialize();

    socket.register_nb_transport_bw(this, &AxiInjector::nb_transport_bw);
}

auto
AxiInjector::start_of_simulation() -> void
{
}
auto
AxiInjector::read_event_handler() -> void
{
}

auto
AxiInjector::write_event_handler() -> void
{
    std::cout << "Hello from write_event handler : " << sc_time_stamp() << " " << __LINE__ << " " << this->name()
              << std::endl;
    if (has_write_in_progress())
        send_write_data(*write_in_progress);
    if (write_ready && write_valid) {
        send_write_data(*write_to_start);
        write_in_progress = write_to_start;
        write_to_start    = nullptr;
        write_ready       = false;
        write_valid       = false;
    }
    if (have_write_to_set_valid()) {
        write_valid = true;
        if (write_ready)
            write_event.notify(period);
    }
}

auto
AxiInjector::nb_transport_bw(tlm_generic_payload& payload, tlm_phase& phase, sc_time& time)
    -> tlm::tlm_sync_enum
{
    std::cout << "Hello from nb_transport_bw : " << sc_time_stamp() << " " << this->name()
              << std::endl;
    if (phase == axi::WRITE_READY) {
        write_ready = true;
        write_event.notify(period);
    }
    if (phase == B_VALID) {
        write_in_progress->release();
        write_in_progress = nullptr;
    }
    return tlm::TLM_COMPLETED;
}
void
AxiInjector::send_write_data(tlm_generic_payload& payload)
{
    auto phase = payload.get_extension<AxiExtension>()->get_next_phase();
    if (phase != axi::DATA_LAST_CHUNK)
        write_event.notify(period);
    socket->nb_transport_fw(payload, phase, zero_time);
}
auto
AxiInjector::have_write_to_set_valid() -> bool
{
    return write_to_start && !write_valid;
}
void
AxiInjector::create_write(std::size_t num_chunks)
{
    if (write_to_start)
        return;
    write_to_start = TlmMemoryManager::getInstance().new_acquired_payload();
    auto ext       = new AxiExtension(num_chunks);
    write_to_start->set_auto_extension(ext);
    write_event.notify(period);
}
auto
AxiInjector::has_write_in_progress() -> bool
{
    return write_in_progress;
}
