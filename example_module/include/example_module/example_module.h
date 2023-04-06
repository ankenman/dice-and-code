#pragma once

#include <tlm>

namespace example {

class ExampleModule : public sc_core::sc_module {
public:
    explicit ExampleModule(sc_core::sc_module_name name);
    auto something() -> int;
    auto start_of_simulation() -> void override;
private:
    sc_core::sc_event e;
    auto event_handler() -> void;
};
} // namespace example
