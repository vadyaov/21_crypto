#ifndef _IROTOR_H_
#define _IROTOR_H_

#include <string>
#include <array>
#include <iostream>
#include <iomanip>

inline char toUpper(uint8_t offset) noexcept {
  return static_cast<char>(offset + 'A');
}

inline char toLower(uint8_t offset) noexcept {
  return static_cast<char>(offset + 'a');
}

inline uint8_t toOffset(char c) noexcept {
  return static_cast<uint8_t>(std::tolower(c) - 'a');
}

struct RotorData {
  uint8_t encoded;
  bool prevSpin;
  bool currSpin;
};

class IRotor {
  public:
    [[nodiscard]] virtual RotorData& get(RotorData& c) = 0;
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
    }

    virtual ~AbstractRotor() {};

  protected:
    std::string name_;
    std::array<uint8_t, N> entry_;
    std::array<uint8_t, N> wiring_;

  private:
    void dump(std::ostream& os) const {
      os << std::setw(5) << name_ << ':';
      for (uint8_t e : entry_) {
        os << toUpper(e);
      }
      std::cout << "\n";
      os << std::setw(5) << name_ << ':';
      for (uint8_t w : wiring_) {
        os << toUpper(w);
      }
    }
};

#endif // _IROTOR_H_
