#ifndef _REFLECTOR_H_
#define _REFLECTOR_H_

#include "IRotor.h"

#include <fstream>
#include <nlohmann/json.hpp>

class TextReflector : public AbstractRotor<26> {
  using AbstractRotor<26>::wiring_;
  using AbstractRotor<26>::name_;

  public:
    TextReflector(const std::string& configfile) {
      TextReflector::setConfig(configfile);
    }

    [[nodiscard]] std::pair<uint8_t, bool> get(std::pair<uint8_t, bool> c) override {
      return std::make_pair(wiring_[c.first], false);
    }

    [[nodiscard]] virtual uint8_t getReverse(uint8_t c) const override {
      return wiring_[c];
    }

    void setConfig(const std::string& filename) override {
      std::ifstream f(filename);
      if (f.rdstate() == std::ios_base::failbit)
        throw std::invalid_argument("Can't open file: " + filename);

      json data = json::parse(f);
      name_ = data["name"];

      std::string w = data["wiring"];

      assert(w.length() == N);

      /* if (!isValid(w)) */
      /*   throw std::invalid_argument("Incorrect " + name_ + " config: " + w); */

      int i = 0;
      for (char c : w) {
        wiring_[i++] = std::tolower(c) - 'a';
      }
    }
};

#endif // _REFLECTOR_H_
