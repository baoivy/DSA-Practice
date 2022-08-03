#include <iostream>
#include <math.h>
using namespace std;

enum balance {
	LH = -1,
	EH = 0,
	RH = 1,
};

template <class K, class V>
class AVLTree {
public:
	class Node;
private:
	Node* root;
	bool checkPrime(int x) {
		if (x < 2)
			return false;
		for (int i = 2; i <= x / 2; i++) {
			if (x % i == 0)
				return false;
		}
		return true;
	}
	int count;
public:
	AVLTree() : root(nullptr), count(0) {};
	int height(Node* node) {
		if (node == nullptr)
			return 0;
		return node->height;
	}

	Node* rightRotation(Node* root) {
		Node* T1 = root->left;
		Node* T2 = T1->right;
		root->left = T2;
		T1->right = root;
		//Update Height
		root->height = max(height(root->left), height(root->right)) + 1;
		T1->height = max(height(T1->left), height(T1->right)) + 1;
		return T1;
	}

	Node* leftRotation(Node* root) {
		Node* T1 = root->right;
		Node* T2 = T1->left;
		root->right = T2;
		T1->left = root;
		//Update height
		root->height = max(height(root->left), height(root->right)) + 1;
		T1->height = max(height(T1->left), height(T1->right)) + 1;
		return T1;
	}

	Node* createNode(K key, V value) {
		Node* newNode = new Node(key, value);
		return newNode;
	}


	void balanceTree(Node* &root, K key) {
		int balanceFactor = height(root->left) - height(root->right);
		if (balanceFactor > RH) {
			if (key <= root->left->key) {
				root = rightRotation(root);
			}
			else {
				root = leftRotation(root);
				root = rightRotation(root);
			}
		}
		else if (balanceFactor < LH) {
			if (key <= root->left->key) {
				root = leftRotation(root);
			}
			else {
				root = rightRotation(root);
				root = leftRotation(root);
			}
		}
	}

	Node* insertNode(Node* root, K key, V value) {
		if (root == nullptr)
			return createNode(key, value);
		if (root->key > key)
			root->left = insertNode(root->left, key, value);
		else if(root->key <= key)
			root->right = insertNode(root->right, key, value);

		balanceTree(root, key);
		return root;
	}

	void insert(K key, V value) {
		root = insertNode(root, key, value);
	}

	Node* search(Node* root, K key) {
		if (root->key == key)
			return root;
		if (root->key > key)
			root->left = search(root->left, key);
		else if (root->key <= key)
			root->right = search(root->right, key);
		else
			return nullptr;
	}

	int hashing(int value) {
		int tmp = value;
		while (!checkPrime(value))
			value--;
		return (value + tmp) % 15;
	}

	void updateHTree(int hashValue, V value) {
		if (count >= 15)
			return;
		int key = hashing(hashValue);
		Node* node = search(root, key);
		if (node->value == -1) {
			node->value = value;
		}
		else {
			int i = 0;
			while (node->value != -1) {
				node = search(root, (key + 2*i) % 15);
				i++;
			}
			node->value = value;
		}
		count++;
	}

	void printPreOrder(Node* root) {
		if (root != nullptr) {
			cout << root->value << " ";
			printPreOrder(root->left);
			printPreOrder(root->right);
		}
	}

	void print() {
		printPreOrder(root);
	}

	class Node {
	private:
		V value;
		K key;
		Node* left, * right;
		int height;
		friend class AVLTree<K,V>;
	public:
		Node(K key) {
			this->key = key;
			this->height = 0;
			Node* left = nullptr;
			Node* right = nullptr;
		}

		Node(K key, V value) {
			this->value = value;
			this->key = key;
			this->height = 0;
			Node* left = nullptr;
			Node* right = nullptr;
		}
	};

	~AVLTree() {
		
	}
};


int main(int argc, char* argv) {
	AVLTree<int, int>* tree = new AVLTree<int, int>;
	for (int i = 1; i <= 15; i++) {
		tree->insert(i, -1);
	}

	tree->updateHTree(14, 4);
	tree->print();
	delete tree;
}
