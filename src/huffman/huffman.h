#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <functional>

#include <fstream>
#include <filesystem>

#include <iostream>
#include <cassert>

using std::unordered_map;
using std::string;
using std::vector;
using std::priority_queue;

using std::ifstream;
using std::ofstream;

namespace fs = std::filesystem;

// string-weight pair
using sw_pair = std::pair<string, size_t>;

// Creates binary alphabet from weighted alphabet
class Huffman {
  public:
    Huffman() = default;
    ~Huffman() {
      assert(state_.size() == 1);
      destroy(state_.top());
    }

    void Init(const unordered_map<char, size_t>& alphabet) {
      for (const auto& [c, freq] : alphabet) {
        sw_pair new_pair;
        new_pair.first = string(1, c);
        new_pair.second = freq;

        Node* new_node = new Node(new_pair);
        state_.push(new_node);
      }
      std::cout << "Init Success\n";
      // std::cout << "state_.size() = " << state_.size() << "\n";
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

        // std::cout << "First Min Node: " << first_min_node->value_.first << ": " << first_min_node->value_.second << "\n";
        // std::cout << "Secon Min Node: " << second_min_node->value_.first << ": " << second_min_node->value_.second << "\n";

        // dangerous string because of possible (size_t + size_t) overflow
        sw_pair new_val(first_min_node->value_.first + second_min_node->value_.first,
            first_min_node->value_.second + second_min_node->value_.second);
        Node* new_node = new Node(new_val, first_min_node, second_min_node);
        state_.push(new_node);

        std::cout << "New Node: " << new_node->value_.first << ": " << new_node->value_.second << "\n";
      }
      assert(state_.size() == 1);
    }

    // Считаем, что Init() был вызван
    unordered_map<char, string> GetBinaryAlphabet() const {
      unordered_map<char, string> alphabet;
      inorderTraversal(state_.top(), "", alphabet);
      return alphabet;
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

    void inorderTraversal(const Node* root, string bits, unordered_map<char, string>& out) {
      if (root == nullptr) {
        return;
      }
      if (root->left_ == nullptr && root->right_ == nullptr) {
        out[root->value_.first[0]] = bits;
      }
      inorderTraversal(root->left_, bits + '0', out);
      inorderTraversal(root->right_, bits + '1', out);
    };

    void destroy(Node* root) {
      if (root == nullptr) return;
      destroy(root->left_);
      destroy(root->right_);
      delete root;
    }

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
    HuffmanEncoder() = default;

    ~HuffmanEncoder() {
      istr_.close();
      config_.close();
      ostr_.close();
    }
    
    void InitIO(const string& in_path) {
      in_ = in_path; // already absolute path
      istr_.open(in_);
      if (!istr_) throw std::runtime_error(in_ + " does not exists.");

      // create name for encoded file
      const auto name = in_path.substr(in_path.find_last_of('/') + 1);
      auto const pos = name.find_last_of('.');
      if (pos != name.npos) {
        out_ = name.substr(0, pos) + "_encoded" + name.substr(pos);
      } else {
        out_ = name + "_encoded";
      }
    }

    // init path for future config file
    // path should be a directory
    void InitConfigDir(const string& path) { 
      InitDir(path, cfg_dir_);
    }

    void InitOutDir(const string& path) { 
      InitDir(path, out_);
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

      unordered_map<char, string> binary_alphabet = huffman_.GetBinaryAlphabet();

      for (const auto& [ch, code] : binary_alphabet) {
        std::cout << ch << ": " << code << "\n";
      }
      
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
    string in_;
    string out_;
    string cfg_dir_;

    // TODO убрать, вынести внутрь методов
    ifstream istr_;
    ifstream config_;
    ofstream ostr_;

    Huffman huffman_;

    void InitDir(const string& path, string& s) {
      fs::path p(path);
      if (!fs::exists(p)) throw std::runtime_error(path + " does not exist.");
      if (!fs::is_directory(p)) throw std::runtime_error(path + " is not a directory.");

      s = path;
    }
};

bool operator<(const sw_pair& lhs, const sw_pair& rhs) {
  return lhs.second < rhs.second;
}

