#include "systemc_fixture.h"
#include <axi_modules/axi_injector.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <tlm_utils/simple_target_socket.h>

using namespace axi;
using namespace tlm;

using testing::_;
using testing::Sequence;

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

    void BuildTestbenchTop() override
    {
        if (injector)
            injector.reset(new AxiInjector("TestInjector"));
        else
            injector = std::make_unique<AxiInjector>("TestInjector");
        injector->socket.bind(loopback.socket);
    }

    std::unique_ptr<AxiInjector>      injector;
    testing::StrictMock<MockLoopback> loopback{"MockLoopback"};

    auto send_phase(tlm_phase phase) -> void
    {
        tlm_generic_payload payload;
        sc_core::sc_time    time;
        loopback.socket->nb_transport_bw(payload, phase, time);
    }
};

TEST_F(AxiInjectorTest, TestNbTransportBw)
{
    tlm::tlm_generic_payload payload;
    tlm::tlm_phase           phase;
    sc_core::sc_time         time;
    EXPECT_EQ(tlm::TLM_COMPLETED, injector->nb_transport_bw(payload, phase, time));
}

TEST_F(AxiInjectorTest, TestNbTransportFw)
{
    tlm::tlm_generic_payload payload;
    tlm::tlm_phase           phase;
    sc_core::sc_time         time;
    EXPECT_CALL(loopback, nb_transport_fw(_, _, _));
    injector->socket->nb_transport_fw(payload, phase, time);
    loopback.socket->nb_transport_bw(payload, phase, time);
}

TEST_F(AxiInjectorTest, WriteSequenceWithoutStartingReady)
{
    injector->create_write();

    AdvanceSimulationTime(sc_core::sc_time(2, sc_core::SC_NS));

    Sequence s;
    auto     data_phase = axi::DATA;
    EXPECT_CALL(loopback, nb_transport_fw(_, data_phase, _)).Times(3).InSequence(s);
    auto data_last_phase = axi::DATA_LAST_CHUNK;
    EXPECT_CALL(loopback, nb_transport_fw(_, data_last_phase, _)).Times(1).InSequence(s);
    send_phase(WRITE_READY);

    AdvanceSimulationTime();

    send_phase(B_VALID);
    EXPECT_FALSE(injector->has_write_in_progress());
}

TEST_F(AxiInjectorTest, WriteSequenceStartingReady)
{
    injector->create_write();

    Sequence s;
    auto     data_phase = axi::DATA;
    EXPECT_CALL(loopback, nb_transport_fw(_, data_phase, _)).Times(3).InSequence(s);
    auto data_last_phase = axi::DATA_LAST_CHUNK;
    EXPECT_CALL(loopback, nb_transport_fw(_, data_last_phase, _)).Times(1).InSequence(s);
    send_phase(WRITE_READY);

    AdvanceSimulationTime();

    send_phase(B_VALID);
    EXPECT_FALSE(injector->has_write_in_progress());
}

TEST_F(AxiInjectorTest, AssertWriteReadyWithNoWrite)
{
    send_phase(WRITE_READY);
    AdvanceSimulationTime(sc_core::sc_time{5, sc_core::SC_NS});
}

TEST_F(AxiInjectorTest, TwoWritesInSequence)
{
    injector->create_write();

    Sequence s;
    auto     data_phase = axi::DATA;
    EXPECT_CALL(loopback, nb_transport_fw(_, data_phase, _)).Times(3).InSequence(s);
    auto data_last_phase = axi::DATA_LAST_CHUNK;
    EXPECT_CALL(loopback, nb_transport_fw(_, data_last_phase, _)).Times(1).InSequence(s);
    send_phase(WRITE_READY);

    AdvanceSimulationTime();
    injector->create_write();
    send_phase(B_VALID);
    EXPECT_FALSE(injector->has_write_in_progress());
    AdvanceSimulationTime(sc_core::sc_time{1, sc_core::SC_NS});
    send_phase(WRITE_READY);
    Sequence s1;
    EXPECT_CALL(loopback, nb_transport_fw(_, data_phase, _)).Times(3).InSequence(s1);
    EXPECT_CALL(loopback, nb_transport_fw(_, data_last_phase, _)).Times(1).InSequence(s1);
    AdvanceSimulationTime();
}
