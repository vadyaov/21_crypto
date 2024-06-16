#ifndef _ROTOR_H_
#define _ROTOR_H_

#include "IRotor.h"

#include <algorithm>
#include <numeric>
#include <iostream>

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class TextRotor final : public AbstractRotor<26> {
  public:
    using AbstractRotor<26>::wiring_;
    using AbstractRotor<26>::name_;
    // such default ctor which just makes useless rotor with no cipher
    // A-A, B-B, ..., Z-Z
    TextRotor() : AbstractRotor<26>(), turnover_{0} {
      std::iota(std::begin(wiring_), std::end(wiring_), 0);
    }

    TextRotor(const std::string& configfile) : TextRotor() {
      TextRotor::setConfig(configfile);
    }

    [[nodiscard]] std::pair<uint8_t, bool> get(std::pair<uint8_t, bool> c) override {
      // сначала поворот, потом сигнал
      /* bool spinNext = false; */
      /* if (c.second == true) { */
      /*   if (wiring_[0] == turnover_) { */
      /*     spinNext = true; */
      /*     std::cout << "Spin next ROTOR\n"; */
      /*   } */
      /*   makeSpins(1); */
      /* } */

      return std::make_pair(wiring_[c.first], false);
    }

    [[nodiscard]] virtual uint8_t getReverse(uint8_t c) const override {
      return std::distance(std::begin(wiring_),
                           std::find(std::begin(wiring_), std::end(wiring_), c));
    }

    void setConfig(const std::string& filename) override {
      std::ifstream f(filename);
      if (f.rdstate() == std::ios_base::failbit)
        throw std::invalid_argument("Can't open file: " + filename);

      json data = json::parse(f);
      name_ = data["name"];

      std::string w = data["wiring"];

      if (w.length() != 26) throw std::invalid_argument("");

      int i = 0;
      for (char c : w) {
        wiring_[i++] = std::tolower(c) - 'a';
      }

      std::string start = data["start"];
      std::string notch = data["notch"];

      int pos = 26 - (std::tolower(notch[0]) - 'a');
      if (pos == 26) pos = 0;
      turnover_ = wiring_[pos];
      std::cout << char(turnover_ + 'a') << "\n";

      makeSpins(std::tolower(start[0]) - 'a');
    }

  private:
    uint8_t turnover_;

    void makeSpins(int count) {
      if (!count) return;
      std::cout << "\nMAKE " << count << "SPINs FOR " << name_ << "\n";
      std::vector<uint8_t> tmp;
      std::copy(std::rbegin(wiring_), std::rbegin(wiring_) + count, std::back_inserter(tmp));

      for (int i = 25; i >= count; --i) {
        wiring_[i] = wiring_[i - count];
      }

      std::copy(std::begin(tmp), std::end(tmp), std::begin(wiring_));
    }
};

#endif // _ROTOR_H_

