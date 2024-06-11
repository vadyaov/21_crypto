#ifndef _IROTOR_H_
#define _IROTOR_H_

#include <string>

class IRotor {
  public:
    virtual bool makeStep() = 0;
    virtual void setConfig(const std::string& filename) = 0;
    virtual void setOffset(int offset) = 0;

    virtual ~IRotor();
};

#endif // _IROTOR_H_
