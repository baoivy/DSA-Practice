#include <iostream>
#include <queue>

using namespace std;

class Node {
private:
	int value;
	Node* left, * right;
	friend class BSTree;
public:
	Node() {
		this->left = nullptr;
		this->right = nullptr;
	}

	Node(int value)  {
		this->value = value;
		this->left = nullptr;
		this->right = nullptr;
	}
};

class BSTree {
private:
	Node* root = nullptr;
	queue<int> q;
	int sum = 0;
private:
	Node* createNode(int x) {
		Node* newNode = new Node(x);
		return newNode;
	}

	Node* insertNode(Node* root, int x) {
		if (root == nullptr) {
			return createNode(x);
		}
		
		if (root->value > x) {
			root->left = insertNode(root->left, x);
		}
		else if (root->value < x) {
			root->right = insertNode(root->right, x);
		}
		return root;
	}

	Node* minimum(Node* root) {
		while (root != nullptr && root->left != nullptr) {
			root = root->left;
		}
		return root;
	}

	Node* deleteNode(Node* root, int x) {
		if (root == nullptr) {
			return nullptr;
		}
		
		if (root->value > x) {
			root->left = deleteNode(root->left, x);
		}
		else if (root->value < x) {
			root->right = deleteNode(root->right, x);
		}
		else {
			if (root->left == nullptr) {
				Node* tmp = root->right;
				delete root;
				return tmp;
			}
			else if (root->left == nullptr) {
				Node* tmp = root->left;
				delete root;
				return tmp;
			}
			else {
				root->value = minimum(root->right)->value;
				root->right = deleteNode(root->right, root->value);
			}
		}
		return root;
	}
public:
	void insertFIFO(int data) {
		if (q.size() <= 10) {
			root = insertNode(root, data);
			q.push(data);
		}
		else {
			root = deleteNode(root, q.front());
			q.pop();
			root = insertNode(root, data);
			q.push(data);
		}
	}

	void InorderSum(Node* rootL) {
		if (rootL != nullptr) {
			InorderSum(rootL->left);
			sum += rootL->value;
			InorderSum(rootL->right);
		}
	}
	
	int sumLeft() {
		InorderSum(root->left);
		return sum + root->value;
	}

	void SupportPrint(Node* root) {
		if (root != nullptr) {
			SupportPrint(root->left);
			cout << root->value << " ";
			SupportPrint(root->right);
		}
	}

	void print() {
		SupportPrint(root);
	}
};

int main() {
	BSTree tree;
	int arr[] = {30,19,31,6,18,20,28,16,11,13,14,7};
	for (int i = 0; i < (int)sizeof(arr) / sizeof(arr[0]); i++) {
		tree.insertFIFO(arr[i]);
	}
	cout << tree.sumLeft() << "\n";
	tree.print();
}
