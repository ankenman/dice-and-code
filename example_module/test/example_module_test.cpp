#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "example_module/example_module.h"
#include "systemc_fixture.h"

using namespace example;

using testing::_;


class MockInjector : public sc_core::sc_module {
public:
    tlm_utils::simple_initiator_socket<MockInjector> socket;
    explicit MockInjector(sc_core::sc_module_name name) : sc_core::sc_module(name)
    {
        socket.register_nb_transport_bw(this, &MockInjector::nb_transport_bw);
    }
    MOCK_METHOD(tlm::tlm_sync_enum, nb_transport_bw,
                (tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_core::sc_time&) );
};

class MockLoopback : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<MockLoopback> socket;
    explicit MockLoopback(sc_core::sc_module_name name) : sc_core::sc_module(name)
    {
        socket.register_nb_transport_fw(this, &MockLoopback::nb_transport_fw);
    }
    MOCK_METHOD(tlm::tlm_sync_enum, nb_transport_fw,
                (tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_core::sc_time&) );
};

class ExampleModuleTest : public SystemCFixture {
protected:
    ExampleModuleTest() {
        // Prevents an error when a test is run that does not start the simulator.
        sc_core::sc_report_handler::set_actions(sc_core::SC_ID_NO_SC_START_ACTIVITY_,
                                                sc_core::SC_DO_NOTHING);
    }

    void BuildTestbenchTop() override {
        injector.socket.bind(e.target_socket);
        e.initiator_socket(loopback.socket);
    }

    example::ExampleModule e{"Example"};
    MockInjector injector{"MockInjector"};
    MockLoopback loopback{"MockLoopback"};
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

TEST_F(ExampleModuleTest, TestNbTransport)
{
    tlm::tlm_generic_payload payload;
    tlm::tlm_phase           phase;
    sc_core::sc_time         time;
    EXPECT_CALL(loopback, nb_transport_fw(_, _, _));
    EXPECT_CALL(injector, nb_transport_bw(_, _, _));
    e.initiator_socket->nb_transport_fw(payload, phase, time);
    e.target_socket->nb_transport_bw(payload, phase, time);
}
