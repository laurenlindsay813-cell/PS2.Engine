#include "PS2Core.hpp"
#include <fstream>

PS2Core::PS2Core()
    : pc(0xBFC00000),
      next_pc(0xBFC00004),
      in_delay_slot(false),
      gpr{},
      hi(0),
      lo(0),
      ram(32 * 1024 * 1024, 0),
      bios(4 * 1024 * 1024, 0),
      scratchpad(16 * 1024, 0),
      framebuffer(FB_WIDTH * FB_HEIGHT, 0xFF101020) {
    gpr.fill(0);
}

bool PS2Core::loadBIOS(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }

    file.read(reinterpret_cast<char*>(bios.data()), static_cast<std::streamsize>(bios.size()));
    return file.gcount() == static_cast<std::streamsize>(bios.size());
}

void PS2Core::stepCPU() {
    const uint32_t opcode = read32(pc);
    pc = next_pc;
    next_pc += 4;

    executeInstruction(opcode);
    gpr[0] = 0;
}

void PS2Core::runFrame() {
    for (int i = 0; i < 50000; ++i) {
        stepCPU();
    }

    static uint32_t frameCount = 0;
    frameCount++;
    for (int x = 0; x < (frameCount % FB_WIDTH); ++x) {
        framebuffer[x + 10 * FB_WIDTH] = 0xFF00FF88;
    }
}
