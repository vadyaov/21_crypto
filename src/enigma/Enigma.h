#ifndef _ENIGMA_H_
#define _ENIGMA_H_

#include "Rotor.h"
#include "Reflector.h"

class IEnigma {
  public:
    virtual ~IEnigma() {}

    virtual void encode(const std::string& filename) const = 0;
};

// abstract enigma machine for N ASCII symbols
// rotor and reflector have size N
class TextEnigma : public IEnigma {
  public:
    // default enigma with 3 rotors and reflector
    TextEnigma() : reflector_{nullptr} {
      rotors_.push_back(new TextRotor("enigma/config/rotor1.json"));
      rotors_.push_back(new TextRotor("enigma/config/rotor2.json"));
      rotors_.push_back(new TextRotor("enigma/config/rotor3.json"));

      reflector_ = new TextReflector("enigma/config/reflector.json");
    }

    TextEnigma(uint8_t n) {
      rotors_.reserve(n);
      /* while (n--) { */
      /*   rotors_.push_back(TextRotor::createRotor()); */
      /* } */
      /* reflector_.push_back(TextReflector::createReflector()); */
    }

    TextEnigma(const TextEnigma& other) {
      for (auto& ptr : other.rotors_) {
        rotors_.push_back(new TextRotor(*ptr));
      }
      reflector_ = new TextReflector(*other.reflector_);
    }

    TextEnigma(TextEnigma&& other) noexcept {
      for (auto& ptr : other.rotors_) {
        rotors_.push_back(ptr);
        ptr = nullptr;
      }

      reflector_ = other.reflector_;
      other.reflector_ = nullptr;
    }

    TextEnigma& operator=(const TextEnigma& other) {
      if (this != &other) {
        destroy();
        rotors_.clear();

        for (auto& ptr : other.rotors_) {
          rotors_.push_back(new TextRotor(*ptr));
        }
        reflector_ = new TextReflector(*other.reflector_);
      }

      return *this;
    }

    TextEnigma& operator=(TextEnigma&& other) noexcept {
      if (this != &other) {
        rotors_.swap(other.rotors_);
        std::swap(reflector_, other.reflector_);
      }

      return *this;
    }

    ~TextEnigma() {
      destroy();
    }

    void encode(const std::string& filename) const override {
      std::ifstream f(filename);
      if (f.rdstate() == std::ios_base::failbit)
        throw std::invalid_argument("Can't open file: " + filename);

      std::string fname = filename.substr(0, filename.find_last_of('.'));

      std::ofstream o(fname + "_encoded");
      if (o.rdstate() == std::ios_base::failbit)
        throw std::runtime_error("Creating file error");

      char c;
      int placeSpace = 0;
      while (f >> c) {
        if (std::isalpha(c)) {
          c = encodeChar(c);
        }
        o << c;
        placeSpace++;
        if (placeSpace == 5) {
          o << ' ';
          placeSpace = 0;
        }
      }
    }


    friend std::ostream& operator<<(std::ostream& os, const TextEnigma& enigma) {
      if (enigma.reflector_) {
        os << *enigma.reflector_;
      }
      os << "\n";

      for (const auto& pr : enigma.rotors_) {
        os << pr->position();
      }
      os << "\n";
      for (const auto& pr : enigma.rotors_) {
        os << *pr << "\n";
      }

      return os;
    }


  protected:
    std::vector<TextRotor*> rotors_;
    TextReflector* reflector_;

  private:
    void destroy() noexcept {
      for (auto& ptr : rotors_)
        delete(ptr);
      delete reflector_;
    }

    char encodeChar(char c) const {
      /* std::cout << "ROTORS before right->left way:\n"; */
      std::cout << *this << "\n";

      /* c = std::tolower(c); */
      RotorData p = {toOffset(c), false, true};

      for (auto rPtr = rotors_.rbegin(); rPtr != rotors_.rend(); ++rPtr) {
        std::cout << toUpper(p.encoded) << "-->";
        p = (*rPtr)->get(p);
      }
      std::cout << toUpper(p.encoded) << "-->";

      p = reflector_->get(p);

      /* p.second = false; */

      for (auto& ptr : rotors_) {
        std::cout << toUpper(p.encoded) << "-->";
        p.encoded = ptr->getReverse(p.encoded);
      }
      std::cout << toUpper(p.encoded) << "\n";

      c = p.encoded;

      return toLower(c);
    }
};

#endif // _ENIGMA_H_
