#include "cartridge.h"

std::vector<uint8_t> cartridge;
std::string cart_name("");

// Write this with one less vector later.
std::vector<uint8_t> load_cartridge(const std::string file)
{
    std::ifstream in(file);
    std::vector<uint8_t> contents((std::istreambuf_iterator<char>(in)),
                                  std::istreambuf_iterator<char>());

    return contents;
}