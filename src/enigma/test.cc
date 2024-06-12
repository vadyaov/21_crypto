#include "Rotor.h"
#include <list>

int main() {
  std::list<tRotor*> rotors;
  rotors.push_back(new tRotor("enigma/config/rotor1.json"));
  rotors.push_back(new tRotor("enigma/config/rotor2.json"));
  rotors.push_back(new tRotor("enigma/config/rotor3.json"));
  std::cout << "Alphabet ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
  for (const tRotor* r : rotors)
    std::cout << *r << "\n";

  std::pair<uint8_t, bool> start = {'A' - 'A', true};
  for (auto& rot : rotors) {
    std::cout << "Letter " << (char)(start.first + 'A') << " encodes to ";
    start = rot->get(start);
    std::cout << (char)(start.first + 'A') << "\n";
  }

  for (auto& ptr : rotors) {
    delete(ptr);
  }
  
  return 0;
}
