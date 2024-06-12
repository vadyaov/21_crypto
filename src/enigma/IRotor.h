#ifndef _IROTOR_H_
#define _IROTOR_H_

#include <string>

class IRotor {
  public:
    [[nodiscard]] virtual std::pair<uint8_t, bool> get(std::pair<uint8_t, bool> c) = 0;
    virtual void setConfig(const std::string& filename) = 0;

    virtual ~IRotor() {};
};

#endif // _IROTOR_H_
