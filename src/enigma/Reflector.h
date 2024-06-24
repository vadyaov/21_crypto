#ifndef _REFLECTOR_H_
#define _REFLECTOR_H_

#include "IRotor.h"

#include <fstream>
#include <nlohmann/json.hpp>

extern const uint8_t N;

class TextReflector : public AbstractRotor<N> {
  using AbstractRotor<N>::wiring_;
  using AbstractRotor<N>::name_;

  public:
    TextReflector() {
      std::iota(std::begin(entry_), std::end(entry_), 0);
      std::iota(std::begin(wiring_), std::end(wiring_), 0);
    }

    TextReflector(const std::string& configfile) : TextReflector() {
      TextReflector::setConfig(configfile);
    }

    [[nodiscard]] RotorData& get(RotorData& c) override {
      c.encoded = wiring_[c.encoded];
      c.prevSpin = false;
      c.currSpin = false;
      return c;
      /* return std::make_pair(wiring_[c.first], false); */
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
