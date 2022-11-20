#include "ConcatStringTree.h"

//ReducedConcatStringTree::LitStringHash litStringHash = ReducedConcatStringTree::LitStringHash();

ConcatStringTree::ConcatStringTree(const char* s) {
	string tmp;
	stringstream ss(s); ss >> tmp;
	BSTNode* newNode = new BSTNode(tmp , 0);
	this->root = newNode;
	this->size = 1;
	root->key = keyGenerator();
	newNode->parent_node->insertNode(newNode);
}

int ConcatStringTree::length() const {
	return this->root->lengthStr;
}

char ConcatStringTree::get(int index) {
	if(index < 0 || index >= root->lengthStr)
		throw std::out_of_range("Index of string is invalid!");
	BSTNode* node = root;
	while (node != nullptr) {
		if (node->left == nullptr && node->right == nullptr)
			break;
		if (node->leftLength <= index)
			node = node->left;
		else if (node->leftLength > index) {
			index -= node->leftLength;
			node = node->right;
		}
	}
	return node->data[index];
}

void ConcatStringTree::inOrdertoString(BSTNode* root, string &data) const {
	if (root != nullptr) {
		inOrdertoString(root->left, data);
		if (root->left == nullptr && root->right == nullptr)
			data += root->data;
		inOrdertoString(root->right, data);
	}
}

int ConcatStringTree::indexOf(char c) {
	string res = "";
	inOrdertoString(root, res);
	return res.find(c);
}

void ConcatStringTree::preOrderString(BSTNode* root, string &res) const{
	if (root != nullptr) {
		res += "(LL=" + to_string(root->leftLength); res += ";";
		res += "L=" + to_string(root->lengthStr); res += ";";
		root->data.empty() ? res += "<NULL>" : res += root->data; res += ");";
		preOrderString(root->left, res);
		preOrderString(root->right, res);
	}
}

string ConcatStringTree::toStringPreOrder() const {
	string res = "ConcatStringTree[";
	preOrderString(root, res);
	if (size > 0)
		res.pop_back();
	res += "]";
	return res;
}

string ConcatStringTree::toString() const {
	string tmp = "ConcatStringTree[";
	inOrdertoString(root, tmp);
	tmp += "]";
	return tmp;
}

ConcatStringTree ConcatStringTree::concat(const ConcatStringTree& otherS) const {
	BSTNode* newTree = new BSTNode();
	newTree->left = root;
	newTree->right = otherS.root;
	newTree->lengthStr = otherS.root->lengthStr + this->root->lengthStr;
	newTree->leftLength = otherS.root->lengthStr;
	newTree->key = keyGenerator();
	newTree->parent_node->insertNode(newTree);
	newTree->left->parent_node->insertNode(newTree);
	newTree->right->parent_node->insertNode(newTree);
	return ConcatStringTree(newTree, this->size + otherS.size + 1);
}

ConcatStringTree::BSTNode* ConcatStringTree::inOrderReverse(BSTNode* root) const {
	BSTNode* newRoot = nullptr;
	if (root != nullptr) {
		string tmp = string(root->data.rbegin(), root->data.rend());
		if(root->right != nullptr)
			newRoot = new BSTNode(tmp, root->right->lengthStr);
		else 
			newRoot = new BSTNode(tmp, 0);
		newRoot->key = keyGenerator();
		newRoot->left = inOrderReverse(root->right);
		newRoot->right = inOrderReverse(root->left);
	}
	return newRoot;
}

ConcatStringTree ConcatStringTree::reverse() const {
	BSTNode* newRoot = inOrderReverse(this->root);
	newRoot->parent_node->insertNode(newRoot);
	return ConcatStringTree(newRoot, this->size);
}

ConcatStringTree::BSTNode* ConcatStringTree::preOrderSubString(BSTNode* root, int from, int to) const {
	if (root == nullptr)
		return nullptr;
	BSTNode* newRoot = new BSTNode();
	if (root->left == nullptr && root->right == nullptr)
		newRoot->data = root->data.substr(from, to - from);
	else
		newRoot->data = root->data;
	newRoot->key = keyGenerator();
	if (from <= root->leftLength - 1) {
		if (to <= root->leftLength - 1)
			newRoot->left = preOrderSubString(root->left, from, to);
		else
			newRoot->left = preOrderSubString(root->left, from, root->leftLength);
	}
	else
		newRoot->left = nullptr;
	if (to >= root->leftLength - 1) {
		if (from >= root->leftLength - 1)
			newRoot->right = preOrderSubString(root->right, from - root->leftLength, to - root->leftLength);
		else
			newRoot->right = preOrderSubString(root->right, 0, to - root->leftLength);
	}
	else
		newRoot->right = nullptr;
	
	return newRoot;
}

ConcatStringTree ConcatStringTree::subString(int from, int to) const {
	if (from < 0 || from >= this->root->lengthStr || to < 0 || to > this->root->lengthStr)
		throw std::out_of_range("Index of string is invalid");
	else if (from >= to)
		throw std::logic_error("Invalid range");
	BSTNode* root = preOrderSubString(this->root, from, to);
	return ConcatStringTree(root, this->size);
}

void ConcatStringTree::clear(BSTNode* root) {
	if (root != nullptr) {
		if (root->parent_node->size() == 0) {
			if (root->left != nullptr) 
				root->left->parent_node->deleteNode(root->key);
			if (root->right != nullptr)
				root->right->parent_node->deleteNode(root->key);
		}
		clear(root->left);
		clear(root->right);
		if (root->parent_node->size() == 0) {
			delete root;
			root = nullptr;
		}
	}
}

ConcatStringTree::~ConcatStringTree() {
	root->parent_node->deleteNode(root->key);
	if (root->parent_node->size() == 0) {
		if (root->left != nullptr) 
			root->left->parent_node->deleteNode(root->key);
		if (root->right != nullptr) 
			root->right->parent_node->deleteNode(root->key);
		clear(root->left);
		clear(root->right);
		delete root;
		this->root = nullptr;
	}
}

int ConcatStringTree::ParentsTree::size() const {
	return count;
}

int ConcatStringTree::ParentsTree::height(AVLNode* node) {
	if (node == nullptr)
		return 0;
	return node->height;
}

ConcatStringTree::ParentsTree::AVLNode* ConcatStringTree::ParentsTree::rightRotate(AVLNode* node) {
	AVLNode* tmp_left = node->left;
	AVLNode* tmp_right = tmp_left->right;
	tmp_left->right = node;
	node->left = tmp_right;
	//Update Parent
	tmp_left->parent = node->parent;
	if (node != root) {
		if (node->parent->left == node)
			node->parent->left = tmp_right;
		else
			node->parent->right = tmp_right;
	}
	if (node == root)
		root = tmp_left;
	node->parent = tmp_left;
	if(tmp_right != nullptr)
		tmp_right->parent = node;
	
	node->height = max(height(node->left), height(node->right)) + 1;
	tmp_left->height = max(height(tmp_left->left), height(tmp_left->right)) + 1;

	return tmp_left;
}

ConcatStringTree::ParentsTree::AVLNode* ConcatStringTree::ParentsTree::leftRotate(AVLNode* node) {
	AVLNode* tmp_right = node->right;
	AVLNode* tmp_left = tmp_right->left;
	tmp_right->left = node;
	node->right = tmp_left;
	//Update parent
	tmp_right->parent = node->parent;
	if (node != root) {
		if (node->parent->left == node)
			node->parent->left = tmp_right;
		else
			node->parent->right = tmp_right;
	}
	if (node == root)
		root = tmp_right;
	node->parent = tmp_right;
	if(tmp_left != nullptr)
		tmp_left->parent = node;

	node->height = max(height(node->left), height(node->right)) + 1;
	tmp_right->height = max(height(tmp_right->left), height(tmp_right->right)) + 1;
	return tmp_right;
}

void ConcatStringTree::ParentsTree::balance(AVLNode* node) {
	while (node != nullptr) {
		if (height(node->left) - height(node->right) > 1) {
			if (height(node->left->left) >= height(node->left->right))
				node = rightRotate(node);
			else {
				node = leftRotate(node);
				node = rightRotate(node);
			}
		}
		else if (height(node->right) - height(node->left) > 1) {
			if (height(node->right->right) >= height(node->right->left))
				node = leftRotate(node);
			else {
				node = rightRotate(node);
				node = leftRotate(node);
			}
		}
		updateHeight(node);
		node = node->parent;
	}
}

int ConcatStringTree::keyGenerator() const {
	static int key = 0;
	if (key > 1E7)
		throw std::overflow_error("Id is overflow!");
	return ++key;
}

void ConcatStringTree::ParentsTree::updateHeight(AVLNode* node) {
	while (node != nullptr) {
		node->height = max(height(node->left), height(node->right)) + 1;
		node = node->parent;
	}
}

void ConcatStringTree::ParentsTree::insertNode(ConcatStringTree::BSTNode* node) {
	if (node == nullptr)
		return;
	if (root == nullptr) 
		root = new AVLNode(node);
	else {
		AVLNode* ptr = root;
		while (ptr != nullptr) {
			if (ptr->left == nullptr && ptr->right == nullptr) {
				if (ptr->key > node->key) {
					ptr->left = new AVLNode(node);
					ptr->left->parent = ptr;
					updateHeight(ptr->left);
					balance(ptr->left);
				}
				else {
					ptr->right = new AVLNode(node);
					ptr->right->parent = ptr;
					updateHeight(ptr->right);
					balance(ptr->right);
				}
				break;
			}
			if (ptr->key > node->key)
				ptr = ptr->left;
			else if (ptr->key < node->key)
				ptr = ptr->right;
		}
	}
	this->count++;
}

void ConcatStringTree::ParentsTree::deleteNode(int key) {
	AVLNode* ptr = root;
	while (ptr != nullptr) {
		if (ptr->key > key)
			ptr = ptr->left;
		else if (ptr->key < key)
			ptr = ptr->right;
		else {
			AVLNode* parent = nullptr;
			if (ptr->left == nullptr && ptr->right == nullptr) {
				AVLNode* tmp_ptr = ptr->parent;
				parent = tmp_ptr;
				if (this->count == 1) {
					delete ptr;
					root = nullptr;
					break;
				}
				else if (tmp_ptr->left == ptr)
					tmp_ptr->left = nullptr;
				else
					tmp_ptr->right = nullptr;
				delete ptr;
			}
			else if (ptr->left == nullptr || ptr->right == nullptr) {
				AVLNode* tmp_ptr = ptr->parent;
				parent = tmp_ptr;
				AVLNode* tmp_ptr_1 = (ptr->left == nullptr) ? ptr->right : ptr->left;
				if (ptr == root) {
					tmp_ptr_1->parent = nullptr;
					root = tmp_ptr_1;
				}
				else {
					tmp_ptr_1->parent = tmp_ptr;
					if (tmp_ptr->left == ptr)
						tmp_ptr->left = tmp_ptr_1;
					else if (tmp_ptr->left == ptr)
						tmp_ptr->right = tmp_ptr_1;
				}
				delete ptr;
			}
			else {
				AVLNode* node_max = ptr->left;
				while (node_max->right != nullptr) {
					node_max = node_max->right;
				}
				ptr->node = node_max->node;
				ptr->key = node_max->key;
				//Delete Leaf
				AVLNode* tmp_ptr = node_max->parent;
				parent = tmp_ptr;
				if (tmp_ptr->left == node_max)
					tmp_ptr->left = nullptr;
				else
					tmp_ptr->right = nullptr;
				delete node_max;
			}
			balance(parent);
			break;
		}
	}
	this->count--;
}

void ConcatStringTree::ParentsTree::PreOrderTraversal(AVLNode* root, string& res) const {
	if (root != nullptr) {
		res += "(id=" + to_string(root->key);
		res += ");";
		PreOrderTraversal(root->left, res);
		PreOrderTraversal(root->right, res);
	}
}

string ConcatStringTree::ParentsTree::toStringPreOrder() const {
	string res = "ParentsTree[";
	PreOrderTraversal(root, res);
	if (this->count >= 1)
		res.pop_back();
	res += "]";
	return res;
}

int ConcatStringTree::getParTreeSize(const string& query) const {
	BSTNode* node = root;
	for (int i = 0; i < (int)query.length(); i++) {
		if (query[i] == 'l')
			node = node->left;
		else if (query[i] == 'r')
			node = node->right;
		if (node == nullptr)
			throw std::runtime_error("Invalid query: reaching NULL");
	}
	return node->parent_node->size();
}

string ConcatStringTree::getParTreeStringPreOrder(const string& query) const {
	BSTNode* node = root;
	for (int i = 0; i < (int)query.length(); i++) {
		if (query[i] == 'l')
			node = node->left;
		else if (query[i] == 'r')
			node = node->right;
		if (node == nullptr)
			throw std::runtime_error("Invalid query: reaching NULL");
	}
	return node->parent_node->toStringPreOrder();
}

void ConcatStringTree::ParentsTree::clear(AVLNode* root) {
	if (root != nullptr) {
		clear(root->left);
		clear(root->right);
		delete root;
		root = nullptr;
	}
}

ConcatStringTree::ParentsTree::~ParentsTree() {
	if (root != nullptr)
		clear(root);
}

LitStringHash::LitStringHash(const HashConfig& hashConfig) {
	this->config = hashConfig;
	this->arr = new HashItem[this->config.initSize];
	this->size = 0;
	this->capacity = config.initSize;
	this->idx_last = -1;
	for (int i = 0; i < capacity; i++) {
		this->arr[i].node = nullptr;
		this->arr[i].numRef = 0;
		this->arr[i].isEmpty = true;
	}
}

int LitStringHash::hashFunc(string s, int size, int p) {
	int sum = 0;
	for (int i = 0; i < (int)s.length(); i++) {
		sum += (((int)s[i] - (int)'0') * power(p, i)) % size;
	}
	return sum;
}

ConcatStringTree::BSTNode* LitStringHash::insertHash(string s) {
	if (this->arr == nullptr)
		this->arr = new HashItem[this->config.initSize];
	int idx = hashFunc(s, config.initSize, config.p);
	int i = 0;
	while (this->arr[idx].isEmpty == false) {
		if (this->arr[idx].isEmpty == false) {
			if (this->arr[idx].node->data == s)
				this->arr[idx].numRef++;
			return this->arr[idx].node;
		}
		idx = idx + config.c1 * i + config.c2 * i * i;
		idx = idx % capacity;
		i++;
	}
	this->arr[idx].node = new ConcatStringTree::BSTNode(s, 0);
	this->arr[idx].numRef++;
	this->arr[idx].isEmpty = false;
	this->size++; this->idx_last = idx;
	if ((double)size / capacity > config.lambda)
		reHash();
	return this->arr[idx].node;
}

ConcatStringTree::BSTNode* LitStringHash::searchHash(string s) {
	int idx = hashFunc(s, config.initSize, config.p);
	int i = 1; int first_idx = idx;
	while (this->arr[idx].node->data != s && this->arr[idx].isEmpty == false) {
		idx = idx + config.c1 * i + config.c2 * i * i;
		idx = idx % capacity;
		i++;
		if (i == capacity)
			return nullptr;
	}
	return this->arr[idx].node;
}

void LitStringHash::deleteHash(ConcatStringTree::BSTNode* node) {
	int idx = hashFunc(node->data, config.initSize, config.p);
	int i = 1;
	while (this->arr[idx].node != node) {
		idx = idx + config.c1 * i + config.c2 * i;
		idx = idx % capacity;
		i++;
	}
	this->arr[idx].numRef--;
	if (this->arr[idx].numRef == 0) {
		this->arr[idx].node = nullptr;
		this->arr[idx].isEmpty = true;
		this->size--;
	}

	if (this->size == 0) {
		delete[] arr;
		arr = nullptr;
		this->idx_last = -1;
	}
}

int LitStringHash::power(int p, int i) {
	if (i == 0) return 1;
	else if (i == 1) return p;
	int outcome = p * p;
	for (int a = 3; a <= i; a++) {
		outcome *= p;
	}
	return outcome;
}

void LitStringHash::reHash() {
	int newCapacity = config.alpha * capacity;
	HashItem* newHash = new HashItem[newCapacity];
	for (int i = 0; i < capacity; i++) {
		if (arr[i].isEmpty == false) {
			int idx = hashFunc(arr[i].node->data, config.initSize, config.p);
			int i = 0;
			while (newHash[idx].isEmpty == true) {
				idx = idx + config.c1 * i + config.c2 * i * i;
				idx = idx % newCapacity;
				i++;
			}
			idx_last = idx;
			newHash[idx] = arr[i];
		}
	}
	capacity = newCapacity;
	delete[] arr;
	arr = newHash;
}

int LitStringHash::getLastInsertedIndex() const {
	return this->idx_last;
}

string LitStringHash::toString() const {
	string res = "LitStringHash[";
	for (int i = 0; i < capacity; i++) {
		if (this->arr[i].isEmpty == true)
			res += "(),";
		else {
			res += "(litS=" + this->arr[i].node->data;
			res += "),";
		}
	}
	if (capacity > 0)
		res.pop_back();
	res += "]";
	return res;
}

LitStringHash::~LitStringHash() {
	if (this->arr != nullptr) {
		for (int i = 0; i < capacity; i++) {
			if (arr[i].node != nullptr)
				delete arr[i].node;
		}
		delete[] arr;
	}
}

ReducedConcatStringTree::ReducedConcatStringTree(const char* s, LitStringHash*& litStringHash) {
	string tmp;
	stringstream ss(s); ss >> tmp;
	this->litStringHash = litStringHash;
	this->root = this->litStringHash->insertHash(tmp);
	this->size = 1;
	root->key = keyGenerator();
	root->parent_node->insertNode(root);
}

ReducedConcatStringTree ReducedConcatStringTree::concat(ReducedConcatStringTree& otherS) {
	BSTNode* newTree = new BSTNode();
	newTree->left = root;
	newTree->right = otherS.root;
	newTree->lengthStr = otherS.root->lengthStr + this->root->lengthStr;
	newTree->leftLength = otherS.root->lengthStr;
	newTree->key = keyGenerator();
	newTree->parent_node->insertNode(newTree);
	newTree->left->parent_node->insertNode(newTree);
	newTree->right->parent_node->insertNode(newTree);
	return ReducedConcatStringTree(newTree, this->size + otherS.size + 1);
}

ReducedConcatStringTree::~ReducedConcatStringTree() {
	this->litStringHash->deleteHash(root);
}

