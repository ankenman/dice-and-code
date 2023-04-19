#include <axi_modules/axi_loopback.h>
#include <axi_modules/axi_injector.h>

#include <systemc>

int sc_main(int argc, char** argv) {
    axi::AxiInjector injector{"injector"};
    axi::AxiLoopback loopback{"loopback"};

    injector.socket.bind(loopback.socket);

    injector.create_write();

    sc_core::sc_start(10, sc_core::SC_NS);

    return 0;
}
