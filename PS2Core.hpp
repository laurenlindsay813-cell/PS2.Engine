#pragma once
#include <cstdint>
#include <vector>
#include <string>

class PS2Core {
public:
    static constexpr int FB_WIDTH = 640;
    static constexpr int FB_HEIGHT = 448;

    PS2Core();
    bool loadBIOS(const std::string& path);
    void stepCPU();
    void runFrame();
    const uint32_t* getFramebuffer() const { return framebuffer.data(); }

private:
    uint32_t pc;
    uint32_t next_pc;
    bool in_delay_slot;
    uint64_t gpr;

    std::vector<uint8_t> ram;          // 32 MB Main RAM
    std::vector<uint8_t> bios;         // 4 MB BIOS ROM
    std::vector<uint32_t> framebuffer; // 640x448 RGBA Pixels

    uint32_t read32(uint32_t vaddr);
    void write32(uint32_t vaddr, uint32_t value);
    void executeInstruction(uint32_t opcode);
};
