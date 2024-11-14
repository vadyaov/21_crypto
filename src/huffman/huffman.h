#pragma once

#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <functional>

#include <fstream>

#include <iostream>
#include <cassert>

using std::unordered_map;
using std::string;
using std::vector;
using std::priority_queue;

using std::ifstream;
using std::ofstream;

using sw_pair = std::pair<string, size_t>;

[[nodiscard]] class Status {
public:
  enum Code {
    OK = 0,
    ERROR = 1,
  };

  Status(Code code, const std::string& msg) : code_(code), message_(msg) {}

  static Status Success() {
    return Status(OK, "OK");
  }

  bool ok() const {
    return code_ == OK;
  }

  const std::string& message() const {
    return message_;
  }

private:
  Code code_;
  std::string message_;
};

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
    }

    void CreateTree() {
      while (state_.size() != 1) {
        Node* first_min_node = state_.top();
        state_.pop();
        Node* second_min_node = state_.top();
        state_.pop();

        sw_pair new_val(first_min_node->value_.first + second_min_node->value_.first,
            first_min_node->value_.second + second_min_node->value_.second);
        Node* new_node = new Node(new_val, first_min_node, second_min_node);
        state_.push(new_node);
      }
    }

    unordered_map<char, string> GetBinaryAlphabet() const {
      unordered_map<char, string> alphabet;
      // if (state_.size() == 1) {
      //   alphabet[state_.top()->value_.first[0]] = "0";
      // } else {
        inorderTraversal(state_.top(), "", alphabet);
      // }
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

    void inorderTraversal(const Node* root, string bits, unordered_map<char, string>& out) const {
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

    priority_queue<Node*, vector<Node*>, GreaterCmp> state_;
};

class HuffmanProcessor {
  public:
    HuffmanProcessor() = default;

    Status Encode(const std::string& inputPath, const std::string& outputPath, const std::string& cfgPath) {
      ifstream istr(inputPath);
      ofstream ostr(outputPath);
      if (!istr) return Status(Status::ERROR, "Can't open " + inputPath);
      if (!ostr) return Status(Status::ERROR, "Can't open " + outputPath);

      unordered_map<char, size_t> frequency_map;
      char ch;
      while (istr.get(ch)) {
        frequency_map[ch]++;
      }

      istr.clear();
      istr.seekg(0);

      huffman_.Init(frequency_map);
      huffman_.CreateTree();
      auto binaryAlphabet = huffman_.GetBinaryAlphabet();

      Status status = SaveConfiguration(cfgPath, binaryAlphabet);
      if (!status.ok()) return status;

      while (istr.get(ch)) {
        ostr << binaryAlphabet[ch];
      }

      return Status::Success();
    }

    Status Decode(const std::string& encodedPath, const std::string& cfgPath, const std::string& outputPath) {
      ifstream encodedFile(encodedPath);
      if (!encodedFile) return Status(Status::ERROR, "Can't open " + encodedPath);

      ifstream cfgFile(cfgPath);
      if (!cfgFile) return Status(Status::ERROR, "Can't open " + cfgPath);

      ofstream decodedFile(outputPath);
      if (!decodedFile) return Status(Status::ERROR, "Can't open " + outputPath);

      unordered_map<std::string, char> reverseAlphabet;
      Status status = LoadConfiguration(cfgPath, reverseAlphabet);
      if (!status.ok()) return status;

      string bitString;
      char bit;
      while (encodedFile.get(bit)) {
        bitString += bit;
        if (reverseAlphabet.count(bitString)) {
          decodedFile << reverseAlphabet[bitString];
          bitString.clear();
        }
      }

      return Status::Success();
    }
    
  private:
    Huffman huffman_;

    Status SaveConfiguration(const std::string& path, const unordered_map<char, std::string>& binaryAlphabet) {
      ofstream cfgFile(path);
      if (!cfgFile) return Status(Status::ERROR, "Error creating " + path);
      for (const auto& [ch, code] : binaryAlphabet) {
          cfgFile << ch << ' ' << code << '\n';
      }
      return Status::Success();
    }

    Status LoadConfiguration(const std::string& path, unordered_map<std::string, char>& reverseAlphabet) {
      ifstream cfgFile(path);
      if (!cfgFile) return Status(Status::ERROR, "Can't open " + path);
      char ch;
      string code;
      while (cfgFile >> ch >> code) {
        reverseAlphabet[code] = ch;
      }
      return Status::Success();
    }
};

bool operator<(const sw_pair& lhs, const sw_pair& rhs) {
  return lhs.second < rhs.second;
}
