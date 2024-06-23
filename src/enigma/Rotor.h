#ifndef _ROTOR_H_
#define _ROTOR_H_

#include "IRotor.h"

#include <algorithm>
#include <numeric>
#include <iostream>

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

constexpr const uint8_t N = 26;

class TextRotor final : public AbstractRotor<N> {
  public:
    using AbstractRotor<N>::wiring_;
    using AbstractRotor<N>::name_;
    enum SpinSide {LEFT, RIGHT};
    // such default ctor which just makes useless rotor with no cipher
    // A-A, B-B, ..., Z-Z
    TextRotor() : turnover_{0}, r_{0} {
      std::iota(std::begin(entry_), std::end(entry_), 0);
      std::iota(std::begin(wiring_), std::end(wiring_), 0);
      std::iota(std::begin(ring_), std::end(ring_), 0);
    }

    TextRotor(const std::string& configfile) : TextRotor() {
      TextRotor::setConfig(configfile);
    }

    TextRotor(const TextRotor& other) : AbstractRotor<N>(other),
                                        turnover_{other.turnover_},
                                        r_{other.r_} {}

    [[nodiscard]] std::pair<uint8_t, bool> get(std::pair<uint8_t, bool> c) override {
      // сначала поворот, потом сигнал
      bool spinNext = false;
      if (c.second == true) {
        if (wiring_[0] == turnover_) {
          spinNext = true;
          std::cout << "Spin next ROTOR\n";
        }
        spin(entry_, LEFT);
        spin(wiring_, LEFT);
        spin(ring_, LEFT);
      }

      /* std::cout << "ROTORS CURR:\n"; */
      /* std::cout << *this << "\n"; */
      /* std::cout << "r_ = " << (int)r_ << "\n"; */
      /* std::cout << " RING:"; */
      /* for (uint8_t r : ring_) { */
      /*   std::cout << toUpper(r); */
      /* } */
      /* std::cout << "\n"; */

      /* std::cout << "wiring[c.first] = " << wiring_[c.first] << "\n"; */
      uint8_t signalToRing = wiring_[c.first];
      auto where = std::find(std::begin(ring_), std::end(ring_), signalToRing);
      uint8_t dist = std::distance(std::begin(ring_), where);
      /* std::cout << "dist = " << (int)dist << "\n"; */

      return std::make_pair((dist + r_) % N, spinNext);
    }

    [[nodiscard]] virtual uint8_t getReverse(uint8_t c) const override {
      uint8_t signalToRotor = ring_[c];
      return std::distance(std::begin(wiring_),
                           std::find(std::begin(wiring_), std::end(wiring_), signalToRotor));
    }

    void setConfig(const std::string& filename) override {
      std::ifstream f(filename);
      if (f.rdstate() == std::ios_base::failbit)
        throw std::invalid_argument("Can't open file: " + filename);

      json data = json::parse(f);
      name_ = data["name"];

      std::string w = data["wiring"];

      if (w.length() != N) throw std::invalid_argument("Bad wiring");

      for (int i = 0; i != N; ++i) {
        wiring_[i] = toOffset(w[i]);
      }

      std::string start = data["start"];
      std::string ring = data["ring"]; // меняется таблица подстановок -- leftSpin
      std::string turn = data["turnover"]; // pos to advance next rotor

      r_ = toOffset(ring[0]);

      int s = toOffset(start[0]);
      /* std::cout << "START = " << toUpper(s) << "\n"; */
      if (s > N / 2) {
        for (int i = 0; i != N - s; ++i) {
          /* std::cout << "HERE\n"; */
          spin(ring_, RIGHT);
          spin(wiring_, RIGHT);
          spin(entry_, RIGHT);
        }
      } else {
        for (int i = 0; i != s; ++i) {
          spin(ring_, LEFT);
          spin(wiring_, LEFT);
          spin(entry_, LEFT);
        }
      }

      for (int i = 0; i != r_; ++i) {
        spin(wiring_, RIGHT);
        spin(entry_, RIGHT);
      }

      int turnover_pos = (std::tolower(turn[0]) - 'a') ? N - (std::tolower(turn[0]) - 'a') : 0;
      turnover_ = wiring_[turnover_pos];
    }

  private:
    uint8_t turnover_;
    uint8_t r_;
    std::array<uint8_t, N> ring_;

    void spin(std::array<uint8_t, N>& arr, SpinSide side) {
      std::cout << "\nMAKE ";
      if (side == RIGHT) std::cout << " right ";
      else std::cout << " left ";

      std::cout << "SPIN FOR " << name_ << "\n";

      uint8_t tmp;

      if (side == RIGHT) {
        tmp = arr[N - 1];
        for (int i = N - 1; i >= 1; --i) {
          arr[i] = arr[i - 1];
        }
        arr[0] = tmp;
      } else {
        tmp = arr[0];
        for (int i = 0; i != N - 1; ++i) {
          arr[i] = arr[i + 1];
        }
        arr[N - 1] = tmp;
      }
    }
};

#endif // _ROTOR_H_

