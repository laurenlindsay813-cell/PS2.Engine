#include "PS2Core.hpp"

int main() {
    PS2Core core;

    if (core.loadBIOS("/tmp/does-not-exist.bin")) {
        return 1;
    }

    core.runFrame();
    return 0;
}
