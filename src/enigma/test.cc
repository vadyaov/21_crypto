#include "Rotor.h"

int main() {
  tRotor rotor;
  std::cout << rotor << "\n";
  try {
    rotor.setConfig("enigma/conf.json");
  } catch (std::exception& e) {
    std::cout << e.what() << "\n";
  }
  std::cout << rotor << "\n";

  std::cout << (char)(rotor.get('G' - 'A') + 'A');
  rotor.makeStep();
  std::cout << (char)(rotor.get('G' - 'A') + 'A');
  
  return 0;
}
