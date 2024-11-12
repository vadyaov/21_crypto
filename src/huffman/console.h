#pragma once

#include "huffman.h"

#include <algorithm>
#include <thread>
#include <chrono>

class Console {
  public:
    enum Mode {ENCODE, DECODE};

    Console() = default;

    void run() {
      Mode mode;
      string m, p;
      while (true) {
        try {
          std::cout << "Mode(Encode/Decode): ";
          std::cin >> m;
          std::transform(m.begin(), m.end(), m.begin(), [](unsigned char c) { return std::tolower(c); });
          if (m == "encode") {
            mode = ENCODE;
          } else if (m == "decode") {
            mode = DECODE;
          } else {
            throw std::runtime_error("Incorrect mode.");
          }
          std::cout << "Absolute path to source file: ";
          std::cin >> p;
          encoder.InitIO(p);
          if (mode == DECODE) {
            std::cout << "Absolute path to config file: ";
            std::cin >> p;
            encoder.InitConfigFile(p);
          }

          string outpath;
          std::cout << "Absolute path to output: ";
          std::cin >> outpath;
        } catch (const std::runtime_error& e) {
          std::cout << "Error: " << e.what() << "\n";
          std::this_thread::sleep_for(std::chrono::seconds(1));
          continue;
        }
      }
    }
    

  private:
    HuffmanEncoder encoder;
};