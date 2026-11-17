#include "PS2Core.hpp"
#include <cstring>

namespace {
bool isAligned32(std::size_t offset) {
    return (offset & 3u) == 0u;
}
}

uint32_t PS2Core::read32(uint32_t vaddr) {
    const uint32_t paddr = vaddr & 0x1FFFFFFF;

    if (paddr < 0x02000000) {
        if (paddr + sizeof(uint32_t) > ram.size() || !isAligned32(paddr)) {
            return 0;
        }

        uint32_t value = 0;
        std::memcpy(&value, &ram[paddr], sizeof(value));
        return value;
    }

    if (paddr >= 0x1FC00000 && paddr < 0x20000000) {
        const uint32_t biosOffset = paddr - 0x1FC00000;
        if (biosOffset + sizeof(uint32_t) > bios.size() || !isAligned32(biosOffset)) {
            return 0;
        }

        uint32_t value = 0;
        std::memcpy(&value, &bios[biosOffset], sizeof(value));
        return value;
    }

    return 0;
}

void PS2Core::write32(uint32_t vaddr, uint32_t value) {
    const uint32_t paddr = vaddr & 0x1FFFFFFF;

    if (paddr < 0x02000000) {
        if (paddr + sizeof(uint32_t) > ram.size() || !isAligned32(paddr)) {
            return;
        }

        std::memcpy(&ram[paddr], &value, sizeof(value));
    }
}
