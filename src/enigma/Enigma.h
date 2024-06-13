#ifndef _ENIGMA_H_
#define _ENIGMA_H_

#include "Rotor.h"
#include "Reflector.h"

#include <list>

class Enigma final {
  public:

  private:
    std::list<IRotor*> rotors_;
    /* Reflector reflector_; */
};

#endif // _ENIGMA_H_
