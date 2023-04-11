#include "systemc_fixture.h"
#include <axi_modules/axi_loopback.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <tlm_utils/simple_initiator_socket.h>

using namespace axi;
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

class AxiLoopbackTest : public SystemCFixture {
protected:
    AxiLoopbackTest()
    {
        // Prevents an error when a test is run that does not start the simulator.
        sc_core::sc_report_handler::set_actions(sc_core::SC_ID_NO_SC_START_ACTIVITY_,
                                                sc_core::SC_DO_NOTHING);
    }

    void BuildTestbenchTop() override { injector.socket.bind(loopback.socket); }

    AxiLoopback  loopback{"TestAxiLoopback"};
    MockInjector injector{"MockInjector"};
};

TEST_F(AxiLoopbackTest, CanInstantiate)
{
    EXPECT_TRUE(&loopback);
}

TEST_F(AxiLoopbackTest, TestEvent)
{
    AdvanceSimulationTime();
}

TEST_F(AxiLoopbackTest, TestNbTransportFw)
{
    tlm::tlm_generic_payload payload;
    tlm::tlm_phase           phase;
    sc_core::sc_time         time;
    EXPECT_EQ(tlm::TLM_COMPLETED, loopback.nb_transport_fw(payload, phase, time));
}

TEST_F(AxiLoopbackTest, TestNbTransportBw)
{
    tlm::tlm_generic_payload payload;
    tlm::tlm_phase           phase;
    sc_core::sc_time         time;
    EXPECT_CALL(injector, nb_transport_bw(_, _, _));
    loopback.socket->nb_transport_bw(payload, phase, time);
    injector.socket->nb_transport_fw(payload, phase, time);
}
