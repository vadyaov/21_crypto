#ifndef _ROTOR_H_
#define _ROTOR_H_

#include "IRotor.h"

#include <algorithm>

class tRotor final : public IRotor {
  public:
    // such default ctor which just makes useless rotor with no cipher
    // A-A, B-B, ..., Z-Z
    tRotor() {
      std::iota(std::begin(pins_), std::end(pins_), 0);
    }

    bool makeStep() override;
    void setConfig(const std::string& filename) override;
    void setOffset(int offset) override;

  private:
    uint8_t offset_ {0};
    uint8_t pins_[26] = {0};
};

#endif // _ROTOR_H_

