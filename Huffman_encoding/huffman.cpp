#include <iostream>
#include <queue>
#include <unordered_map>
using namespace std;

struct Node {
	char c;
	int freq;
	Node* left;
	Node* right;
	Node(char _c, int _freq, Node* _left = nullptr, Node* _right = nullptr):
	c(_c), freq(_freq),
	left(_left), right(_right) {}
};

struct comp {
	bool operator() (const Node* l, const Node* r) const {
		return l->freq > r->freq;
	}
};

bool isLeaf(Node* node) {
	return (!node->left) && (!node->right);
}

void initMapping(unordered_map<char, string>& mp, Node* root, string cur) {
	if (!root) return;
	if (isLeaf(root)) {
		// base case
		mp[root->c] = cur;
		return;
	}
	initMapping(mp, root->left, cur + '0');
	initMapping(mp, root->right, cur + '1');
}

string encode(unordered_map<char, string>& mp, string& str) {
	string res = "";
	for (char c : str) res += mp[c];
	return res;
}

string decode(Node* root, string& str) {
	Node* cur = root;
	string res = "";
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == '0') cur = cur->left;
		else cur = cur->right;

		if (isLeaf(cur)) {
			// append value and reset for new search/traversel
			res.push_back(cur->c);
			cur = root;
		}
	}
	return res;
}

void huffmanEncodeDecode(string& str) {
	if (str.empty()) return;
	// Construct count map: char - freq
	unordered_map<char, int> count;
	for (char c : str) {
		count[c]++;
	}

	// Each entry in the hash map now is a char-freq pair
	// Construct a node for each of them; push to priority_queue
	priority_queue<Node*, vector<Node*>, comp> pq;
	for (auto it = count.begin(); it != count.end(); ++it) {
		Node* nn = new Node(it->first, it->second);
		pq.push(nn);
	}
	// While the size is > 1, take the two least frequent out
	// and merge them to be 1 and push to the pq (as one node) again
	while (pq.size() > 1) {
		Node* left = pq.top(); pq.pop();
		Node* right = pq.top(); pq.pop();
		Node* nn = new Node('*', left->freq + right->freq, left, right);
		pq.push(nn);
	}
	// Now the only remaining node in pq is teh root of the tree
	// We traverse the tree, find the leaves and construct the 
	// char-string (letter-binary) mapping.
	Node* root = pq.top();
	unordered_map<char, string> mp;

	initMapping(mp, root, "");

	cout << "the original String is: " << str << endl;
	// Now use the mp to encode the str
	string encoded = encode(mp, str);
	cout << "the encoded String is: " << encoded << endl;

	// Then decode it again to see if it matches the original string.
	string decoded = decode(root, encoded);
	cout << "the decoded String is: " << decoded << endl;
}

int main() {
	string str = "Huffman coding is a lossless ~ compression algorithm.";
	huffmanEncodeDecode(str);
	return 0;
}