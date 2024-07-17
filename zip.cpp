#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

// Node class to represent each character in the Huffman tree
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparison object for priority queue
struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// Function to generate Huffman codes
void generateCodes(Node* root, const string& str, unordered_map<char, string>& huffmanCode) {
    if (!root) return;

    // Found a leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

// Function to build the Huffman tree and generate codes
Node* buildHuffmanTree(const string& text) {
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        int sum = left->freq + right->freq;
        Node* node = new Node('\0', sum);
        node->left = left;
        node->right = right;
        pq.push(node);
    }

    return pq.top();
}

// Function to encode the input text
string encode(const string& text, unordered_map<char, string>& huffmanCode) {
    string encodedStr;
    for (char ch : text) {
        encodedStr += huffmanCode[ch];
    }
    return encodedStr;
}

// Function to decode the encoded string
string decode(const string& encodedStr, Node* root) {
    string decodedStr;
    Node* curr = root;
    for (char bit : encodedStr) {
        curr = (bit == '0') ? curr->left : curr->right;

        if (!curr->left && !curr->right) {
            decodedStr += curr->ch;
            curr = root;
        }
    }
    return decodedStr;
}

// Main function to demonstrate Huffman encoding and decoding
int main() {
    string text = "this is an example for huffman encoding";

    Node* root = buildHuffmanTree(text);

    unordered_map<char, string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    cout << "Huffman Codes are :\n";
    for (auto pair : huffmanCode) {
        cout << pair.first << " " << pair.second << '\n';
    }

    string encodedStr = encode(text, huffmanCode);
    cout << "\nEncoded string is :\n" << encodedStr << '\n';

    string decodedStr = decode(encodedStr, root);
    cout << "\nDecoded string is :\n" << decodedStr << '\n';

    return 0;
}