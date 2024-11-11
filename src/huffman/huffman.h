#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <functional>

#include <fstream>

#include <iostream>

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
    Huffman() = default;

    void Init(const unordered_map<char, size_t>& alphabet) {
      for (const auto& [c, freq] : alphabet) {
        sw_pair new_pair;
        new_pair.first = string(1, c);
        new_pair.second = freq;

        Node* new_node = new Node(new_pair);
        state_.push(new_node);
      }
      std::cout << "Init Success\n";
      std::cout << "state_.size() = " << state_.size() << "\n";
    }

    // Creates tree while number of active elements in queue > 1
    // TODO
    void CreateTree() {
      if (state_.size() <= 1) {
        // what should i do ?
      }

      while (state_.size() != 1) {
        Node* first_min_node = state_.top();
        state_.pop();
        Node* second_min_node = state_.top();
        state_.pop();

        std::cout << "First Min Node: " << first_min_node->value_.first << ": " << first_min_node->value_.second << "\n";
        std::cout << "Secon Min Node: " << second_min_node->value_.first << ": " << second_min_node->value_.second << "\n";

        // dangerous string because of possible (size_t + size_t) overflow
        sw_pair new_val(first_min_node->value_.first + second_min_node->value_.first,
            first_min_node->value_.second + second_min_node->value_.second);
        Node* new_node = new Node(new_val, first_min_node, second_min_node);
        state_.push(new_node);
        std::cout << "New Node: " << new_node->value_.first << ": " << new_node->value_.second << "\n";
      }
    }

    // Считаем, что Init() был вызван
    // TODO
    void GetBinaryAlphabet(unordered_map<char, string>& out) {
      Node* root = state_.top();
      string bits;
      auto inorderTraversal = [&out](const Node* root, const Node* parent, string& bits) -> void {
        if (root == nullptr) {
          return;
        }
        inorderTraversal(root->left, root, bits);
        inorderTraversal(root->right, root, bits);
        // TODO
      };
      inorderTraversal(root, nullptr, bits);
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

    struct GreaterCmp {
      bool operator()(const Node* lhs, const Node* rhs) {
        if (lhs->value_.second > rhs->value_.second) return true;
        else if (lhs->value_.second < rhs->value_.second) return false;
        return lhs->value_.first < rhs->value_.first;
      }
    };

    // current huffman state
    priority_queue<Node*, vector<Node*>, GreaterCmp> state_;
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

      if (!istr_.eof())
        throw std::runtime_error("Unexpected error while reading ");
      
      // init huffman with alphabet
      huffman_.Init(mp);
      huffman_.CreateTree();

      unordered_map<char, string> binary_alphabet;
      huffman_.GetBinaryAlphabet(binary_alphabet);
      
      istr_.clear();  // reset stream state
      istr_.seekg(0); // set pointer to the start

      while (istr_.get(ch)) {
        std::cout << "ch = " << ch << "\n";
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

