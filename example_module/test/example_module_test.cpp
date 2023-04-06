#include <gtest/gtest.h>
#include "example_module/example_module.h"
#include "systemc_fixture.h"

class ExampleModuleTest : public SystemCFixture {
protected:
    ExampleModuleTest() {
        // Prevents an error when a test is run that does not start the simulator.
        sc_core::sc_report_handler::set_actions(sc_core::SC_ID_NO_SC_START_ACTIVITY_,
                                                sc_core::SC_DO_NOTHING);
    }

    void BuildTestbenchTop() override {
    }

    example::ExampleModule e{"Example"};
};

TEST_F(ExampleModuleTest, CanInstantiate) {
    EXPECT_TRUE(&e);
}

TEST_F(ExampleModuleTest, TestEvent) {
    AdvanceSimulationTime();
}

TEST_F(ExampleModuleTest, Something) {
    EXPECT_EQ(42, e.something());
}
