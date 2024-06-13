#include "Rotor.h"
#include "Reflector.h"
#include <list>

#define N 26

using tRotor = Rotor26;
using tReflector = Reflector26;

int main() {
  std::list<AbstractRotor<N>*> rotors;
  rotors.push_back(new tRotor("enigma/config/rotor1.json"));
  rotors.push_back(new tRotor("enigma/config/rotor2.json"));
  rotors.push_back(new tRotor("enigma/config/rotor3.json"));
  rotors.push_back(new tReflector("enigma/config/reflector.json"));
  std::cout << "Alphabet ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
  for (const AbstractRotor<N>* r : rotors)
    std::cout << *r << "\n";

  std::pair<uint8_t, bool> start = {'X' - 'A', true};
  for (auto& rot : rotors) {
    std::cout << "Letter " << (char)(start.first + 'A') << " encodes to ";
    start = rot->get(start);
    std::cout << (char)(start.first + 'A') << "\n";
  }
  for (auto rit = ++rotors.rbegin(); rit != rotors.rend(); ++rit) {
    std::cout << "Letter " << (char)(start.first + 'A') << " encodes to ";
    start = (*rit)->get(start);
    std::cout << (char)(start.first + 'A') << "\n";
  }

  for (auto& ptr : rotors) {
    delete(ptr);
  }
  
  return 0;
}
