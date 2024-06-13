#ifndef _ROTOR_H_
#define _ROTOR_H_

#include "IRotor.h"

#include <algorithm>
#include <numeric>
#include <iostream>

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

template<uint8_t N>
class Rotor : public AbstractRotor<N> {
  public:
    using AbstractRotor<N>::wiring_;
    using AbstractRotor<N>::name_;
    // such default ctor which just makes useless rotor with no cipher
    // A-A, B-B, ..., Z-Z
    Rotor() : offset_{0} {
      std::iota(std::begin(wiring_), std::end(wiring_), 0);
    }

    Rotor(const std::string& configfile) {
      Rotor::setConfig(configfile);
    }

    virtual ~Rotor() {};

    [[nodiscard]] std::pair<uint8_t, bool> get(std::pair<uint8_t, bool> c) override {
      std::pair<uint8_t, bool> pinned = {wiring_[c.first], false};
      
      if (c.second == true) {
        makeSpins(1);
        ++offset_;

        // DEBUG
        std::cout << name_ << " made 1 spin\n";

        if (N == offset_) {
          offset_ = 0;
          pinned.second = true;
        }
      }

      return pinned;
    }

    void setConfig(const std::string& filename) override {
      std::ifstream f(filename);
      if (f.rdstate() == std::ios_base::failbit)
        throw std::invalid_argument("Can't open file: " + filename);

      json data = json::parse(f);
      name_ = data["name"];

      std::string w = data["wiring"];

      assert(w.length() == N); // IMPORTANT

      /* if (!isValid(w)) */
      /*   throw std::invalid_argument("Incorrect " + name_ + " config: " + w); */

      int i = 0;
      for (char c : w) {
        wiring_[i++] = std::tolower(c) - 'a';
      }
      
      offset_ = static_cast<uint8_t>(data["offset"]) % N;
      makeSpins(offset_);
    }

  private:
    uint8_t offset_ ;

    void makeSpins(int count) {
      std::vector<uint8_t> tmp;
      std::copy(std::begin(wiring_), std::begin(wiring_) + count, std::back_inserter(tmp));

      for (int i = 0; i != N - count; ++i) {
        wiring_[i] = wiring_[i + count];
      }

      std::copy(std::begin(tmp), std::end(tmp), std::end(wiring_) - count);
    }
};

class Rotor26 final : public Rotor<26> {
  public:
    Rotor26(const std::string& configfile) : Rotor<26>(configfile) {}

  /* protected: */
  /*   bool isValid(const std::string& s) const override { */
  /*     uint8_t mp[26] = {0}; */
  /*     for (char c : wiring_) { */
  /*       if (mp[tolower(c) - 'a'] == 1) return false; */

  /*       mp[tolower(c) - 'a'] = 1; */
  /*     } */
  /*     return true; */
  /*   } */
};

#endif // _ROTOR_H_

