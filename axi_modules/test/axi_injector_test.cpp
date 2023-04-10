#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <axi_modules/axi_injector.h>
#include "systemc_fixture.h"

using namespace axi;

class AxiInjectorTest : public SystemCFixture {
protected:
    AxiInjectorTest() {
        // Prevents an error when a test is run that does not start the simulator.
        sc_core::sc_report_handler::set_actions(sc_core::SC_ID_NO_SC_START_ACTIVITY_,
                                                sc_core::SC_DO_NOTHING);
    }

    void BuildTestbenchTop() override {
    }

    AxiInjector injector{"TestAxiInjector"};
};

TEST_F(AxiInjectorTest, CanInstantiate) {
    EXPECT_TRUE(&injector);
}

TEST_F(AxiInjectorTest, TestEvent) {
    AdvanceSimulationTime();
}

TEST_F(AxiInjectorTest, TestNbTransportBw) {
    tlm::tlm_generic_payload payload;
    tlm::tlm_phase phase;
    sc_core::sc_time time;
    EXPECT_EQ(tlm::TLM_COMPLETED, injector.nb_transport_bw(payload, phase, time));
}

