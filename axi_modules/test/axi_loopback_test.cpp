#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <axi_modules/axi_loopback.h>
#include "systemc_fixture.h"

using namespace axi;

class AxiLoopbackTest : public SystemCFixture {
protected:
    AxiLoopbackTest() {
        // Prevents an error when a test is run that does not start the simulator.
        sc_core::sc_report_handler::set_actions(sc_core::SC_ID_NO_SC_START_ACTIVITY_,
                                                sc_core::SC_DO_NOTHING);
    }

    void BuildTestbenchTop() override {
    }

    AxiLoopback loopback{"TestAxiLoopback"};
};

TEST_F(AxiLoopbackTest, CanInstantiate) {
    EXPECT_TRUE(&loopback);
}

TEST_F(AxiLoopbackTest, TestEvent) {
    AdvanceSimulationTime();
}

TEST_F(AxiLoopbackTest, TestNbTransportBw) {
    tlm::tlm_generic_payload payload;
    tlm::tlm_phase phase;
    sc_core::sc_time time;
    EXPECT_EQ(tlm::TLM_COMPLETED, loopback.nb_transport_fw(payload, phase, time));
}

