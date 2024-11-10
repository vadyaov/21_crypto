#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <functional>

#include <fstream>

using std::map;
using std::unordered_map;
using std::string;
using std::vector;
using std::priority_queue;

using std::ifstream;
using std::ofstream;

// string-weight pair
using sw_pair = std::pair<string, size_t>;

// Creates binary alphabet from weighted alphabet
class Huffman {
  public:
    Huffman() : tree_{nullptr} {}

    void Init(const unordered_map<char, size_t>& alphabet) {
      for (const auto& [c, freq] : alphabet) {
        sw_pair new_pair;
        new_pair.first = string(1, c);
        new_pair.second = freq;

        state_.push(new_pair);
      }
    }

    // Creates tree while number of active elements in queue > 1
    // TODO
    void CreateTree() {
      if (state_.size() <= 1) {
        // what should i do ?
      }

      while (state_.size() != 1) {
        auto first_min = state_.top();
        state_.pop();
        auto second_min = state_.top();
        state_.pop();

        // dangerous string because of possible (size_t + size_t) overflow
        sw_pair new_val(first_min.first + second_min.first,
            first_min.second + second_min.second);
        // first insertion
        if (tree_ == nullptr) {
          Node* left_child = new Node(first_min);
          Node* right_child = new Node(second_min);
          Node* parent = new Node(new_val, left_child, right_child);
          tree_ = parent;
        } else {
        }
      }
    }

    // Считаем, что Init() был вызван
    // TODO
    void GetBinaryAlphabet(unordered_map<char, string>& out) {

    }

  private:
    struct Node {
      Node() : value_{}, left_{nullptr}, right_{nullptr} {}
      Node(const sw_pair& p, Node* left = nullptr, Node* right = nullptr) :
        value_{p}, left_{left}, right_{right} {}

      sw_pair value_;
      Node* left_;
      Node* right_;
    };

    // current huffman state
    // Все таки я уверен что поле tree_ не нужно, все можно сделать с помощью одной
    // структуры state_ - очередь с приоритеом, но нужно вместо пар хранить узлы Node*
    // и переопределить оператор сравнения для Node, потому что затем эти узлы будут
    // сливаться и в итоге останется один, так что поддерживать доп структуру дерева не нужно,
    // она будет встроена внутрь state_
    priority_queue<sw_pair, vector<sw_pair>, std::greater<sw_pair>> state_;
    Node* tree_;
};

// read file and use Huffman:
// - takes file from user
// - reads file and makes alphabet
// - give to huffman correct alphabet
// - takes from huffman binary alphabet
// - encodes file using binary alphabet
class HuffmanEncoder {
  public:
    HuffmanEncoder(const string& input, const string& output) :
      istr_(input), ostr_(output) {
      if (!istr_) throw std::runtime_error(input + " does not exist.");
      if (!ostr_) throw std::runtime_error("Can't open file " + output);
    }

    ~HuffmanEncoder() {
      istr_.close();
      ostr_.close();
    }
    
    void Encode() {
      // read file and create alphabet
      unordered_map<char, size_t> mp;
      char ch;
      while (istr_.get(ch)) {
        mp[ch]++;
      }

      if (istr_.fail())
        throw std::runtime_error("Unexpected error while reading ");
      
      // init huffman with alphabet
      huffman_.Init(mp);
      huffman_.CreateTree();

      unordered_map<char, string> binary_alphabet;
      huffman_.GetBinaryAlphabet(binary_alphabet);
      
      istr_.clear();  // reset stream state
      istr_.seekg(0); // set pointer to the start

      while (istr_.get(ch)) {
        auto it = binary_alphabet.find(ch);
        if (it == binary_alphabet.end()) {
          throw std::runtime_error("Unexpected exception. Abort.");
        }
        ostr_ << it->second;
      }
    }
  private:
    ifstream istr_;
    ofstream ostr_;
    Huffman huffman_;
};

bool operator<(const sw_pair& lhs, const sw_pair& rhs) {
  return lhs.second < rhs.second;
}

