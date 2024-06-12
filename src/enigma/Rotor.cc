#include "Rotor.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void tRotor::setConfig(const std::string& filename) {
  std::ifstream f(filename);
  if (f.rdstate() == std::ios_base::failbit)
    throw std::invalid_argument("Can't open file: " + filename);

  json data = json::parse(f);
  std::string w = data["wiring"];
  assert(w.length() == N);

  int i = 0;
  for (char c : w) {
    wiring_[i++] = std::tolower(c) - 'a';
  }
  
  setOffset(data["offset"]);
  spin(offset_);
}

void tRotor::setOffset(int offset) {
  offset_ = offset % N;
}

bool tRotor::makeStep() {
  spin(1);
  offset_++;

  if (offset_ == N) {
    offset_ = 0;
    return true;
  }

  return false;
}

uint8_t tRotor::get(uint8_t c) const {
  return wiring_[c];
}

void tRotor::spin(int count) {
  std::vector<uint8_t> tmp;
  std::copy(std::begin(wiring_), std::begin(wiring_) + count, std::back_inserter(tmp));

  for (int i = 0; i != N - count; ++i) {
    wiring_[i] = wiring_[i + count];
  }

  std::copy(std::begin(tmp), std::end(tmp), std::end(wiring_) - count);
}
