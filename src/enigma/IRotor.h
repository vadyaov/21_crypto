#ifndef _IROTOR_H_
#define _IROTOR_H_

#include <string>
#include <array>
#include <iostream>

template<uint8_t N>
class IRotor {
  public:
    [[nodiscard]] virtual std::pair<uint8_t, bool> get(std::pair<uint8_t, bool> c) = 0;
    virtual void setConfig(const std::string& filename) = 0;

    virtual ~IRotor() {};
};

template<uint8_t N>
class AbstractRotor : public IRotor<N> {
  public:
    friend std::ostream& operator<<(std::ostream& os, const AbstractRotor& r) {
      r.dump(os);
      return os;
    }

    virtual ~AbstractRotor() {};

  protected:
    std::string name_;
    std::array<uint8_t, N> wiring_;

    /* virtual bool isValid(const std::string& s) const = 0; */

  private:
    void dump(std::ostream& os) const {
      os << name_ << ": ";
      for (auto w : wiring_) {
        os << char(w + 'A');
      }
    }
};

#endif // _IROTOR_H_
