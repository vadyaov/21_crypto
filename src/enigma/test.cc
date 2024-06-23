#include "Enigma.h"

int main() {
  TextEnigma enigma;
  enigma.encode("text.txt");

  std::cout << enigma << "\n";

  return 0;
}
