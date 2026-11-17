#include "PS2Core.hpp"
#include <fstream>
#include <cstdio>

PS2Core::PS2Core()
    : pc(0xBFC00000),
      next_pc(0xBFC00004),
      in_delay_slot(false),
      gpr{},
      ram(32 * 1024 * 1024, 0),
      bios(4 * 1024 * 1024, 0),
      framebuffer(FB_WIDTH * FB_HEIGHT, 0xFF101020) {
    gpr.fill(0);
}

bool PS2Core::loadBIOS(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;

    file.read(reinterpret_cast<char*>(bios.data()), static_cast<std::streamsize>(bios.size()));
    return file.gcount() == static_cast<std::streamsize>(bios.size());
}

uint32_t PS2Core::read32(uint32_t vaddr) {
    uint32_t paddr = vaddr & 0x1FFFFFFF;
    if (paddr < 0x02000000) {
        if (paddr + sizeof(uint32_t) > ram.size() || (paddr & 3u) != 0u) {
            return 0;
        }
        return *reinterpret_cast<uint32_t*>(&ram[paddr]);
    } else if (paddr >= 0x1FC00000 && paddr < 0x20000000) {
        const uint32_t biosOffset = paddr - 0x1FC00000;
        if (biosOffset + sizeof(uint32_t) > bios.size() || (biosOffset & 3u) != 0u) {
            return 0;
        }
        return *reinterpret_cast<uint32_t*>(&bios[biosOffset]);
    }
    return 0;
}

void PS2Core::write32(uint32_t vaddr, uint32_t value) {
    uint32_t paddr = vaddr & 0x1FFFFFFF;
    if (paddr < 0x02000000) {
        if (paddr + sizeof(uint32_t) > ram.size() || (paddr & 3u) != 0u) {
            return;
        }
        *reinterpret_cast<uint32_t*>(&ram[paddr]) = value;
    }
}

void PS2Core::stepCPU() {
    uint32_t opcode = read32(pc);
    pc = next_pc;
    next_pc += 4;

    executeInstruction(opcode);
    gpr[0] = 0; // $zero is always 0
}

void PS2Core::executeInstruction(uint32_t opcode) {
    uint32_t op = (opcode >> 26) & 0x3F;
    uint32_t rs = (opcode >> 21) & 0x1F;
    uint32_t rt = (opcode >> 16) & 0x1F;
    uint16_t imm = opcode & 0xFFFF;
    int16_t simm = static_cast<int16_t>(imm);

    switch (op) {
        case 0x04: { // BEQ (Branch on Equal)
            if (gpr[rs] == gpr[rt]) {
                next_pc = pc + (static_cast<int32_t>(simm) << 2);
            }
            break;
        }
        case 0x09: { // ADDIU (Add Immediate Unsigned)
            gpr[rt] = static_cast<uint32_t>(static_cast<int32_t>(gpr[rs]) + simm);
            break;
        }
        case 0x0D: { // ORI (OR Immediate)
            gpr[rt] = gpr[rs] | imm;
            break;
        }
        case 0x0F: { // LUI (Load Upper Immediate)
            gpr[rt] = static_cast<uint32_t>(static_cast<int32_t>(simm) << 16);
            break;
        }
        case 0x2B: { // SW (Store Word)
            uint32_t addr = gpr[rs] + static_cast<uint32_t>(simm);
            write32(addr, gpr[rt]);
            break;
        }
        default:
            break;
    }
}

void PS2Core::runFrame() {
    // Run a test batch of cycles per frame
    for (int i = 0; i < 50000; ++i) {
        stepCPU();
    }
    // Draw a live heartbeat pixel bar at top-left so you know the core is running
    static uint32_t frameCount = 0;
    frameCount++;
    for (int x = 0; x < (frameCount % FB_WIDTH); ++x) {
        framebuffer[x + 10 * FB_WIDTH] = 0xFF00FF88;
    }
}
