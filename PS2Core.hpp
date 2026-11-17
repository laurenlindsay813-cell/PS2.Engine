#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

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
    std::array<uint32_t, 32> gpr{};
    uint32_t hi;
    uint32_t lo;

    std::vector<uint8_t> ram;
    std::vector<uint8_t> bios;
    std::vector<uint8_t> scratchpad;
    std::vector<uint32_t> framebuffer;

    uint32_t read32(uint32_t vaddr);
    void write32(uint32_t vaddr, uint32_t value);
    void executeInstruction(uint32_t opcode);
};
