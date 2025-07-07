//
// Created by Admin on 05.07.2025.
//

#include "HuffmanTree.h"

#include <ostream>
#include <bits/fs_fwd.h>

void HuffmanTree::build(const std::unordered_map<unsigned char, int> &freqMap) {
    //Define lambda comparer func
    auto comp = [](Node* a, Node* b) {
        return a->freq > b->freq;
    };
    //Assign priority queue : DataType, (Store-)Container type, compare operatoor
    std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> queue(comp);

    //Assigning NOdes to queue
    for (auto& [ch, freq] : freqMap) {
        queue.push(new Node(ch, freq));
    }

    //Building Tree
    while (queue.size() > 1) {
        Node * n1 = queue.top();
        queue.pop();
        Node * n2 = queue.top();
        queue.pop();

        queue.push(new Node(n1, n2));
    }

    this->root = queue.top();

}

void HuffmanTree::serialize(std::ostream &out) const {
    serializeNode(root, out);
}

void HuffmanTree::serializeNode(Node *node, std::ostream &out) const {

    if (!node) return;

    unsigned char leafBit = 1;
    unsigned char notLeafBit = 0;
    if (node->isLeaf()) {
        out.write(reinterpret_cast<const char *>(&leafBit), 1);
        unsigned char character = node->value;
        out.write(reinterpret_cast<const char *>(&character), sizeof(character));
    }else {
        out.write(reinterpret_cast<const char *>(&notLeafBit), 1);
        serializeNode(node->left, out);
        serializeNode(node->right, out);
    }
}

void HuffmanTree::deserialize(std::istream &in) {
    root = deserializeNode(in);
}

HuffmanTree::Node *HuffmanTree::deserializeNode(std::istream &in) {
    unsigned char bit;
    in.read(reinterpret_cast<char *>(&bit), 1);
    if (bit == 1) {
        unsigned char character;
        in.read(reinterpret_cast<char *>(&character), sizeof(character));
        return new Node(character, 0);
    }else {
        return new Node(deserializeNode(in), deserializeNode(in));
    }
}


//Deleting SubTree to avoid Memory Leak
void HuffmanTree::deleteSubTree(Node* n) {
    if (!n) return;
    deleteSubTree(n->left);
    deleteSubTree(n->right);
    delete n;
}


HuffmanTree::~HuffmanTree() {
    deleteSubTree(root);
}

