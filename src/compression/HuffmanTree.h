#pragma once
#include <iosfwd>
#include <unordered_map>
#include <queue>
#include <istream>



class HuffmanTree {
public:
    struct Node {
        unsigned char value;       // only valid if leaf
        int freq;                  // optional, helps with building tree
        Node* left = nullptr;
        Node* right = nullptr;

        [[nodiscard]] bool isLeaf() const noexcept{
            return !left && !right;
        }

        explicit Node(unsigned char v, int f = 0) : value(v), freq(f) {}
        Node(Node* l, Node* r) : value(0), freq((l ? l->freq : 0) + (r ? r->freq : 0)), left(l), right(r) {}
    };

    Node* root = nullptr;

    // Methods
    [[nodiscard]] Node* getRoot() const noexcept{ return root; }
    void build(const std::unordered_map<unsigned char, int>& freqMap);
    void serialize(std::ostream& out) const;
    void serializeNode(Node* node, std::ostream& out) const;
    void deserialize(std::istream& in);
    Node* deserializeNode(std::istream& in);
    ~HuffmanTree();

    private:
    void deleteSubTree(Node* n);
};



