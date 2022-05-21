#include <vector>
#include <string>
#include <fstream>
#include <iterator>

extern std::vector<uint8_t> cartridge;
extern std::string cart_name;

extern std::vector<uint8_t> load_cartridge(const std::string file);