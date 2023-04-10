#pragma once

#include <tlm>

namespace axi {

class AxiInjector : public sc_core::sc_module {
public:
    explicit AxiInjector(sc_core::sc_module_name name);
    auto start_of_simulation() -> void override;
private:
    sc_core::sc_event event;
    auto event_handler() -> void;
};
} // namespace axi
