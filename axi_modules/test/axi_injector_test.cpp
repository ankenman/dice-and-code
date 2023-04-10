#include <gtest/gtest.h>
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
