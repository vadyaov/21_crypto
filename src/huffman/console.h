#pragma once

#include "huffman.h"
#include <iostream>

using std::cin;
using std::cout;

class Console {
public:
  void Run() {
    HuffmanProcessor processor;
    while (true) {
      cout << "Choose mode: \n1. Encode\n2. Decode\n3. Exit\n";
      int choice;
      cin >> choice;
      if (choice == 1) {
        EncodeMode(processor);
      } else if (choice == 2) {
        DecodeMode(processor);
      } else {
        break;
      }
    }
  }

private:
  void EncodeMode(HuffmanProcessor& processor) {
    string inputPath, outputPath, cfgPath;
    cout << "Enter path to input file: ";
    cin >> inputPath;
    cout << "Enter path for encoded output: ";
    cin >> outputPath;
    cout << "Enter path for Huffman configuration (cfg): ";
    cin >> cfgPath;

    Status status = processor.Encode(inputPath, outputPath, cfgPath);
    if (!status.ok()) {
      std::cout << status.message() << std::endl;
    } else {
      std::cout << "Encoding completed.\n";
    }
  }

  void DecodeMode(HuffmanProcessor& processor) {
    string encodedPath, cfgPath, outputPath;
    cout << "Enter path to encoded file: ";
    cin >> encodedPath;
    cout << "Enter path to configuration file: ";
    cin >> cfgPath;
    cout << "Enter path for decoded output: ";
    cin >> outputPath;

    Status status = processor.Decode(encodedPath, cfgPath, outputPath);
    if (!status.ok()) {
      std::cout << status.message() << std::endl;
    } else {
      cout << "Decoding completed.\n";
    }
  }
};