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

    [[nodiscard]] RotorData& get(RotorData& c) override {
      // !! need to do double stepping !!

      /*
       *  The middle rotor will advance on the next step of the first rotor
       *  a second time in a row, if the middle rotor is in its own turnover position.
       *  This is called the double-step.
       *
       * I - II - III
       * KDO KDP, KDQ, KER, LFS, LFT, LFU
       */

      if (ring_[0] == turnover_) c.currSpin = true; // double stepping
      bool spinNext = false;
      if (c.currSpin == true) {
        c.prevSpin = true;
        if (ring_[0] == turnover_) spinNext = true;

        spin(entry_, LEFT);
        spin(wiring_, LEFT);
        spin(ring_, LEFT);
      }

      // can be upgraded to one line return
      uint8_t signalToRing = wiring_[c.encoded];
      auto where = std::find(std::begin(ring_), std::end(ring_), signalToRing);
      uint8_t dist = std::distance(std::begin(ring_), where);

      c.encoded = (dist + r_) % N;
      c.currSpin = spinNext;

      return c;
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
      std::string ring = data["ring"];
      std::string turn = data["turnover"];

      r_ = toOffset(ring[0]);

      int s = toOffset(start[0]);

      nSpin(ring_, LEFT, s);
      nSpin(wiring_, LEFT, s);
      nSpin(entry_, LEFT, s);

      nSpin(wiring_, RIGHT, r_);
      nSpin(entry_, RIGHT, r_);

      /* if (s > N / 2) { */
      /*   for (int i = 0; i != N - s; ++i) { */
      /*     spin(ring_, RIGHT); */
      /*     spin(wiring_, RIGHT); */
      /*     spin(entry_, RIGHT); */
      /*   } */
      /* } else { */
      /*   for (int i = 0; i != s; ++i) { */
      /*     spin(ring_, LEFT); */
      /*     spin(wiring_, LEFT); */
      /*     spin(entry_, LEFT); */
      /*   } */
      /* } */

      /* for (int i = 0; i != r_; ++i) { */
      /*   spin(wiring_, RIGHT); */
      /*   spin(entry_, RIGHT); */
      /* } */

      turnover_ = toOffset(turn[0]);
    }

    char position() const noexcept {
      return toUpper(ring_[0]);
    }

  private:
    uint8_t turnover_;
    uint8_t r_;
    std::array<uint8_t, N> ring_;

  private:
    void leftShift(std::array<uint8_t, N>& a) noexcept {
      uint8_t tmp = a[0];
      for (int i = 0; i != N - 1; ++i) {
        a[i] = a[i + 1];
      }
      a[N - 1] = tmp;
    }

    void rightShift(std::array<uint8_t, N>& a) noexcept {
      uint8_t tmp = a[N - 1];
      for (int i = N - 1; i >= 1; --i) {
        a[i] = a[i - 1];
      }
      a[0] = tmp;
    }

    void spin(std::array<uint8_t, N>& arr, SpinSide side) {
      side == RIGHT ? rightShift(arr) : leftShift(arr);
    }

    void nSpin(std::array<uint8_t, N>& arr, SpinSide side, int n) {
      if (n > N / 2) {
        side = (side == LEFT) ? RIGHT : LEFT;
        n = N - n;
      }

      for (int i = 0; i != n; ++i) {
        spin(arr, side);
      }
    }
};

#endif // _ROTOR_H_

