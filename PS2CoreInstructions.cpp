#include "PS2Core.hpp"

void PS2Core::executeInstruction(uint32_t opcode) {
    const uint32_t op = (opcode >> 26) & 0x3F;
    const uint32_t rs = (opcode >> 21) & 0x1F;
    const uint32_t rt = (opcode >> 16) & 0x1F;
    const uint16_t imm = opcode & 0xFFFF;
    const int16_t simm = static_cast<int16_t>(imm);

    switch (op) {
        case 0x04: { // BEQ
            if (gpr[rs] == gpr[rt]) {
                next_pc = pc + (static_cast<int32_t>(simm) << 2);
            }
            break;
        }
        case 0x09: { // ADDIU
            gpr[rt] = static_cast<uint32_t>(static_cast<int32_t>(gpr[rs]) + simm);
            break;
        }
        case 0x0D: { // ORI
            gpr[rt] = gpr[rs] | imm;
            break;
        }
        case 0x0F: { // LUI
            gpr[rt] = static_cast<uint32_t>(static_cast<int32_t>(simm) << 16);
            break;
        }
        case 0x23: { // LW
            const uint32_t addr = gpr[rs] + static_cast<uint32_t>(simm);
            gpr[rt] = read32(addr);
            break;
        }
        case 0x2B: { // SW
            const uint32_t addr = gpr[rs] + static_cast<uint32_t>(simm);
            write32(addr, gpr[rt]);
            break;
        }
        default:
            break;
    }
}
