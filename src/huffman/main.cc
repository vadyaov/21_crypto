#include "huffman.h"
#include <iostream>

int main() {
  try {
  HuffmanEncoder encoder("input.txt", "output.txt");
  encoder.Encode();
  } catch (const std::exception& e) {
    std::cout << e.what() << "\n";
  }
}