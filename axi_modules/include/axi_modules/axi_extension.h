#pragma once

#include <tlm>

namespace axi {
DECLARE_EXTENDED_PHASE(WRITE_VALID);
DECLARE_EXTENDED_PHASE(READ_VALID);
DECLARE_EXTENDED_PHASE(WRITE_READY);
DECLARE_EXTENDED_PHASE(READ_READY);
DECLARE_EXTENDED_PHASE(B_VALID);
DECLARE_EXTENDED_PHASE(B_READY);
DECLARE_EXTENDED_PHASE(DATA);
DECLARE_EXTENDED_PHASE(DATA_LAST_CHUNK);

enum class BRESP { OKAY = 0, EXOKAY = 1, SLVERR = 2, DECERR = 3 };

class AxiExtension : public tlm::tlm_extension<AxiExtension> {
public:
    explicit AxiExtension(std::size_t num_data_beats) : num_data_beats{num_data_beats} {}
    //    AxiExtension(AxiExtension&) = default;
    [[nodiscard]] auto clone() const -> tlm_extension_base* override
    {
        return new AxiExtension(*this);
    }
    auto copy_from(const tlm_extension_base& other) -> void override
    {
        *this = static_cast<const AxiExtension&>(other);
    }
    auto get_next_phase() -> tlm::tlm_phase
    {
        ++sent_data_beats;
        if (sent_data_beats == num_data_beats)
            return DATA_LAST_CHUNK;
        return DATA;
    }
    std::size_t num_data_beats;
    std::size_t sent_data_beats = 0;

    BRESP bresp;
};
} // namespace axi