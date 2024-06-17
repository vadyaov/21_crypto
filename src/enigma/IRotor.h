#ifndef _IROTOR_H_
#define _IROTOR_H_

#include <string>
#include <array>
#include <iostream>

class IRotor {
  public:
    [[nodiscard]] virtual std::pair<uint8_t, bool> get(std::pair<uint8_t, bool> c) = 0;
    [[nodiscard]] virtual uint8_t getReverse(uint8_t c) const = 0;
    virtual void setConfig(const std::string& filename) = 0;

    virtual ~IRotor() {};
};

template<uint8_t N>
class AbstractRotor : public IRotor {
  public:
    friend std::ostream& operator<<(std::ostream& os, const AbstractRotor& r) {
      r.dump(os);
      return os;
    }

    AbstractRotor() = default;

    AbstractRotor(const AbstractRotor& other) : name_{other.name_} {
      for (int i = 0; i != N; ++i)
        wiring_[i] = other.wiring_[i];
    }

    virtual ~AbstractRotor() {};

  protected:
    std::string name_;
    std::array<uint8_t, N> wiring_;

  private:
    void dump(std::ostream& os) const {
      for (auto w : wiring_) {
        os << char(w + 'A');
      }
    }
};

#endif // _IROTOR_H_
