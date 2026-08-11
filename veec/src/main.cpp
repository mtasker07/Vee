#include "veec/compilation/CompilerDriver.hpp"

int main(int argc, const char** argv) {
    veec::compilation::CompilerDriver driver;
    return driver.run(argc, argv);
}
