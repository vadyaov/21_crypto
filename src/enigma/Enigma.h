#ifndef _ENIGMA_H_
#define _ENIGMA_H_

#include "Rotor.h"
#include "Reflector.h"

class IEnigma {
  public:
    virtual ~IEnigma() {}

    virtual void encode(const std::string& filename) = 0;
};

// abstract enigma machine for N ASCII symbols
// rotor and reflector have size N
template <uint8_t N>
class AbstractEnigma : public IEnigma {
  public:
    using rotPtr = Rotor<N>*;
    using refPtr = Rotor<N>*;

    virtual ~AbstractEnigma() {}

    friend std::ostream& operator<<(std::ostream& os, const AbstractEnigma& enigma) {
      os << "ROTORS:\n";
      for (auto pr : rotors_) {
        os << *pr;
      }

      os << "REFLECTOR:\n";
      os << *reflector;

      return os;
    }


  protected:
    vector<rotPtr> rotors_;
    refPtr reflector_;
};

#endif // _ENIGMA_H_
