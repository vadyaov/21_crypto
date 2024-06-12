#ifndef _ROTOR_H_
#define _ROTOR_H_

#include "IRotor.h"

#include <algorithm>
#include <numeric>
#include <iostream>

class tRotor : public IRotor {
  public:
    constexpr static uint8_t N = 26;
    // such default ctor which just makes useless rotor with no cipher
    // A-A, B-B, ..., Z-Z
    tRotor() {
      std::iota(std::begin(wiring_), std::end(wiring_), 0);
    }

    tRotor(const std::string& configfile) {
      tRotor::setConfig(configfile);
    }

    virtual ~tRotor() = default;

    [[nodiscard]] std::pair<uint8_t, bool> get(std::pair<uint8_t, bool> c) override;
    void setConfig(const std::string& filename) override;

    friend std::ostream& operator<<(std::ostream& os, const tRotor& r) {
      r.dump(os);
      return os;
    }

  private:
    std::string name_;
    uint8_t offset_ {0};
    uint8_t wiring_[N] = {0};

    void makeSpins(int count);
    void dump(std::ostream& os) const {
      os << name_ << ": ";
      for (auto w : wiring_) {
        os << char(w + 'A');
      }
    }
};

#endif // _ROTOR_H_

