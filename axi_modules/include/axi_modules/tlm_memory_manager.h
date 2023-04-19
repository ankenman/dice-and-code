#pragma once

#include <tlm>

class TlmMemoryManager : public tlm::tlm_mm_interface {
public:
    static TlmMemoryManager& getInstance()
    {
        static TlmMemoryManager instance; // Guaranteed to be destroyed.
                                          // Instantiated on first use.
        return instance;
    }
    void                      free(tlm::tlm_generic_payload* payload) override { delete payload; }
    tlm::tlm_generic_payload* new_acquired_payload()
    {
        auto payload = new tlm::tlm_generic_payload{this};
        payload->acquire();
        return payload;
    }

private:
    TlmMemoryManager() {}

public:
    TlmMemoryManager(TlmMemoryManager const&) = delete;
    void operator=(TlmMemoryManager const&)   = delete;
};