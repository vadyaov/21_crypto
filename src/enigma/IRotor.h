#ifndef _IROTOR_H_
#define _IROTOR_H_

#include <string>

class IRotor {
  public:
    [[nodiscard]] virtual bool makeStep() = 0;
    [[nodiscard]] virtual uint8_t get(uint8_t c) const = 0;

    virtual void setConfig(const std::string& filename) = 0;
    virtual void setOffset(int offset) = 0;

    virtual ~IRotor() = default;
};

#endif // _IROTOR_H_
