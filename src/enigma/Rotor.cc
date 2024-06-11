#include "Rotor.h"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void tRotor::setConfig(const std::string& filename) {
  std::ifstream f("example.json");
  json data = json::parse(f);

  // ...
}
