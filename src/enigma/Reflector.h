#ifndef _REFLECTOR_H_
#define _REFLECTOR_H_

#include "IRotor.h"

#include <fstream>
#include <nlohmann/json.hpp>

template<uint8_t N>
class Reflector : public AbstractRotor<N> {
  using AbstractRotor<N>::wiring_;
  using AbstractRotor<N>::name_;

  public:
    virtual ~Reflector() {}

    Reflector(const std::string& configfile) {
      Reflector::setConfig(configfile);
    }

    [[nodiscard]] std::pair<uint8_t, bool> get(std::pair<uint8_t, bool> c) override {
      return std::make_pair(wiring_[c.first], false);
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

class Reflector26 final : public Reflector<26> {
  public:
    Reflector26(const std::string& configfile) : Reflector<26>(configfile) {}

  /* protected: */
  /*   bool isValid(const std::string& s) const override { */
  /*     uint8_t mp[26] = {0}; */
  /*     int i = 0; */
  /*     for (char c : s) { */
  /*       if ((mp[tolower(c) - 'a'] == 1 && mp[i] == 0) || */
  /*           (mp[tolower(c) - 'a'] == 0 && mp[i] == 1)) { */
  /*         return false; */
  /*       } */

  /*       mp[tolower(c) - 'a'] = mp[i++] = 1; */
  /*     } */
  /*     return true; */
  /*   } */
};

#endif // _REFLECTOR_H_
