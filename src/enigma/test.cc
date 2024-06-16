#include "Rotor.h"
#include "Reflector.h"
#include <list>

int main() {
  std::list<AbstractRotor<26>*> rotors;
  rotors.push_back(new TextRotor("enigma/config/rotor3.json"));
  rotors.push_back(new TextRotor("enigma/config/rotor2.json"));
  rotors.push_back(new TextRotor("enigma/config/rotor1.json"));
  rotors.push_back(new TextReflector("enigma/config/reflector.json"));
  std::cout << "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
  for (const AbstractRotor<26>* r : rotors)
    std::cout << *r << "\n";
  char c = 0;
  while (c != '1') {
    std::cin >> c;
    std::pair<uint8_t, bool> start = {c - 'a', true};
    std::cout << "VPERED\n";
    for (auto& rot : rotors) {
      std::cout << "Letter " << (char)(start.first + 'a') << " encodes to ";
      start = rot->get(start);
      std::cout << (char)(start.first + 'a') << "\n";
      std::cout << *rot << "\n";
    }

    std::cout << "NAZAD\n";
    for (auto rit = ++rotors.rbegin(); rit != rotors.rend(); ++rit) {
      std::cout << "Letter " << (char)(start.first + 'a') << " encodes to ";
      start.first = (*rit)->getReverse(start.first);
      std::cout << (char)(start.first + 'a') << "\n";
    }
  }

  for (auto& ptr : rotors) {
    delete(ptr);
  }
  
  return 0;
}

// ABCDEFGHIJKLMNOPQRSTUVWXYZ
// BDFHJLCPRTXVZNYEIWGAKMUSQO
// ^
