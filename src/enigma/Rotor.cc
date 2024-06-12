#include "Rotor.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void tRotor::setConfig(const std::string& filename) {
  std::ifstream f(filename);
  if (f.rdstate() == std::ios_base::failbit)
    throw std::invalid_argument("Can't open file: " + filename);

  json data = json::parse(f);
  name_ = data["name"];

  std::string w = data["wiring"];

  assert(w.length() == N); // IMPORTANT
  int i = 0;
  for (char c : w) {
    wiring_[i++] = std::tolower(c) - 'a';
  }
  
  offset_ = static_cast<uint8_t>(data["offset"]) % N;
  makeSpins(offset_);
}

std::pair<uint8_t, bool> tRotor::get(std::pair<uint8_t, bool> c) {
  std::pair<uint8_t, bool> pinned = {wiring_[c.first], false};
  
  if (c.second == true) {
    makeSpins(1);
    ++offset_;

    // DEBUG
    std::cout << name_ << " made 1 spin\n";

    if (N == offset_) {
      offset_ = 0;
      pinned.second = true;
    }
  }

  return pinned;
}

void tRotor::makeSpins(int count) {
  std::vector<uint8_t> tmp;
  std::copy(std::begin(wiring_), std::begin(wiring_) + count, std::back_inserter(tmp));

  for (int i = 0; i != N - count; ++i) {
    wiring_[i] = wiring_[i + count];
  }

  std::copy(std::begin(tmp), std::end(tmp), std::end(wiring_) - count);
}
