#include "systemc_fixture.h"
#include <axi_modules/axi_injector.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <tlm_utils/simple_target_socket.h>

using namespace axi;

using testing::_;

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
class AxiInjectorTest : public SystemCFixture {
protected:
    AxiInjectorTest()
    {
        // Prevents an error when a test is run that does not start the simulator.
        sc_core::sc_report_handler::set_actions(sc_core::SC_ID_NO_SC_START_ACTIVITY_,
                                                sc_core::SC_DO_NOTHING);
    }

    void BuildTestbenchTop() override { injector.socket.bind(loopback.socket); }

    AxiInjector  injector{"TestAxiInjector"};
    MockLoopback loopback{"MockLoopback"};
};

TEST_F(AxiInjectorTest, CanInstantiate)
{
    EXPECT_TRUE(&injector);
}

TEST_F(AxiInjectorTest, TestEvent)
{
    AdvanceSimulationTime();
}

TEST_F(AxiInjectorTest, TestNbTransportBw)
{
    tlm::tlm_generic_payload payload;
    tlm::tlm_phase           phase;
    sc_core::sc_time         time;
    EXPECT_EQ(tlm::TLM_COMPLETED, injector.nb_transport_bw(payload, phase, time));
}

TEST_F(AxiInjectorTest, TestNbTransportFw)
{
    tlm::tlm_generic_payload payload;
    tlm::tlm_phase           phase;
    sc_core::sc_time         time;
    EXPECT_CALL(loopback, nb_transport_fw(_, _, _));
    injector.socket->nb_transport_fw(payload, phase, time);
    loopback.socket->nb_transport_bw(payload, phase, time);
}
