#include "ConcatStringTree.h"

//ReducedConcatStringTree::LitStringHash litStringHash = ReducedConcatStringTree::LitStringHash();

int keyGenerator() { //KeyGen
	static int key = 0;
	if (key > 1E7)
		throw std::overflow_error("Id is overflow!");
	return ++key;
}

//Class implementation for ConcatStringTree
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
		res += "(LL=" + to_string(root->leftLength); res += ",";
		res += "L=" + to_string(root->lengthStr); res += ",";
		root->data.empty() ? res += "<NULL>" : res += "\"" + root->data + "\"";   res += ");";
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
	string tmp = "ConcatStringTree[\"";
	inOrdertoString(root, tmp);
	tmp += "\"]";
	return tmp;
}

ConcatStringTree ConcatStringTree::concat(const ConcatStringTree& otherS) const {
	BSTNode* newTree = new BSTNode();
	newTree->left = root;
	newTree->right = otherS.root;
	newTree->lengthStr = otherS.root->lengthStr + this->root->lengthStr;
	newTree->leftLength = root->lengthStr;
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
	if (root->left == nullptr && root->right == nullptr) {
		newRoot->data = root->data.substr(from, to - from);
		newRoot->lengthStr = newRoot->data.length();
	}
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
	if (to > root->leftLength - 1) {
		if (from >= root->leftLength - 1)
			newRoot->right = preOrderSubString(root->right, from - root->leftLength, to - root->leftLength);
		else
			newRoot->right = preOrderSubString(root->right, 0, to - root->leftLength);
	}
	else
		newRoot->right = nullptr;
	return newRoot;
}

void ConcatStringTree::updateLeftL(BSTNode* root) const {
	if (root != nullptr) {
		if (root->left != nullptr)
			root->leftLength = root->left->data.length();
		else
			root->leftLength = 0;
		if (root->left != nullptr && root->right != nullptr)
			root->lengthStr = root->leftLength + root->right->data.length();
		updateLeftL(root->left);
		updateLeftL(root->right);
	}
}

ConcatStringTree ConcatStringTree::subString(int from, int to) const {
	if (from < 0 || from >= this->root->lengthStr || to < 0 || to > this->root->lengthStr)
		throw std::out_of_range("Index of string is invalid");
	else if (from >= to)
		throw std::logic_error("Invalid range");
	BSTNode* newRoot = preOrderSubString(this->root, from, to);
	updateLeftL(newRoot);
	return ConcatStringTree(newRoot, this->size);
}

void ConcatStringTree::clearParents(BSTNode* root) {
	if (root != nullptr) {
		if (root->parent_node->size() == 0) {
			if (checkLeaf(root->left) && checkLeaf(root->right)) {
				if (checkLeaf(root->left) && root->left->RCST == true && root->left == root->right) {
					if (root->left->parent_node->size() == 1)
						root->right = nullptr;
				}
			}
			if (root->left != nullptr)
				root->left->parent_node->deleteNode(root);
			if (root->right != nullptr)
				root->right->parent_node->deleteNode(root);
		}
		clearParents(root->left);
		clearParents(root->right);
		if (root->parent_node->size() == 0 && root->numRef == 0) 
			delete root; 
	}
}


ConcatStringTree::~ConcatStringTree() {
	if(root->numRef == 0)
		root->parent_node->deleteNode(root);
	if (root->parent_node->size() == 0) {
		if (checkLeaf(root->left) && root->left->RCST == true) {
			if (root->left->parent_node->size() == 1)
				root->right = nullptr;
		}
		if (root->left != nullptr) {
			root->left->parent_node->deleteNode(root);
		}
		if (root->right != nullptr) {
			root->right->parent_node->deleteNode(root);
		}
		clearParents(root->left);
		clearParents(root->right);
		if (root->numRef == 0) {
			delete root;
			root = nullptr;
		}
	}
}

//Class implementation for ParentsTree
int ConcatStringTree::ParentsTree::size() const {
	return count;
}

int ConcatStringTree::ParentsTree::height(AVLNode* node) {
	if (node == nullptr)
		return 0;
	return node->height;
}

bool ConcatStringTree::checkLeaf(BSTNode* root) {
	if (root == nullptr)
		return false;
	if (root->left == nullptr && root->right == nullptr)
		return true;
	return false;
}

ConcatStringTree::ParentsTree::AVLNode* ConcatStringTree::ParentsTree::rightRotate(AVLNode* node) {
	AVLNode* tmp_left = node->left;
	AVLNode* tmp_right = tmp_left->right;
	tmp_left->right = node;
	node->left = tmp_right;
	//Update Height
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
	node->height = max(height(node->left), height(node->right)) + 1;
	tmp_right->height = max(height(tmp_right->left), height(tmp_right->right)) + 1;

	return tmp_right;
}

int ConcatStringTree::ParentsTree::getBalance(AVLNode* node) {
	if (node == nullptr)
		return 0;
	return height(node->right) - height(node->left);
}

ConcatStringTree::ParentsTree::AVLNode* ConcatStringTree::ParentsTree::insert(AVLNode* root, ConcatStringTree::BSTNode* node) {
	if (root == nullptr)
		return new AVLNode(node);
	if (root->key < node->key)
		root->right = insert(root->right, node);
	else if (root->key > node->key)
		root->left = insert(root->left, node);

	root->height = max(height(root->right), height(root->left)) + 1;

	int balanceFactor = height(root->right) - height(root->left);

	//Balance Factor
	if (balanceFactor < -1) {
		if (getBalance(root->left) <= 0)
			return rightRotate(root);
		else {
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}
	}
	else if (balanceFactor > 1) {
		if (getBalance(root->right) >= 0)
			return leftRotate(root);
		else {
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}
	}
	return root;
}

ConcatStringTree::ParentsTree::AVLNode* ConcatStringTree::ParentsTree::maxValue(AVLNode* root) {
	AVLNode* ptr = root;
	while (ptr->right != nullptr) {
		ptr = ptr->right;
	}
	return ptr;
}

ConcatStringTree::ParentsTree::AVLNode* ConcatStringTree::ParentsTree::remove(AVLNode* root, ConcatStringTree::BSTNode* node, bool &canDelete) {
	if (root == nullptr) {
		canDelete = false;
		return nullptr;
	}
	if (root->key < node->key)
		root->right = remove(root->right, node, canDelete);
	else if (root->key > node->key)
		root->left = remove(root->left, node, canDelete);
	else {
		if (root->right == nullptr || root->left == nullptr) {
			AVLNode* tmp = (root->left) ? root->left : root->right;
			if (tmp == nullptr) {
				tmp = root;
				root = nullptr;
			}
			else
				*root = *tmp;
			delete tmp;
		}
		else {
			AVLNode* tmp = maxValue(root->left);
			root->node = tmp->node;
			root->key = tmp->key;
			root->left = remove(root->left, tmp->node, canDelete);
		}
	}
	if (root == nullptr)
		return root;

	root->height = 1 + max(height(root->left),
		height(root->right));
	int balanceFactor = height(root->right) - height(root->left);

	//Balance Factor
	if (balanceFactor < -1) {
		if (getBalance(root->left) <= 0)
			return rightRotate(root);
		else {
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}
	}
	else if (balanceFactor > 1) {
		if (getBalance(root->right) >= 0)
			return leftRotate(root);
		else {
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}
	}
	return root;
}

void ConcatStringTree::ParentsTree::insertNode(ConcatStringTree::BSTNode* node) {
	if (node == nullptr)
		return;
	root = insert(root, node);
	this->count++;
}

void ConcatStringTree::ParentsTree::deleteNode(ConcatStringTree::BSTNode* node) {
	bool canDelete = true;
	AVLNode* newRoot = remove(root, node, canDelete);
	if (newRoot != nullptr) {
		if (canDelete == false)
			return;
		this->count--;
		this->root = newRoot;
	}
	else {
		this->count = 0;
		this->root = nullptr;
	}
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

void ConcatStringTree::ParentsTree::setNULLRecur(AVLNode* root, ConcatStringTree::BSTNode* node) {
	if (root != nullptr) {
		if (root->node->left == node)
			root->node->left = nullptr;
		if (root->node->right == node)
			root->node->right = nullptr;
		setNULLRecur(root->left, node);
		setNULLRecur(root->right, node);
	}
}

void ConcatStringTree::ParentsTree::setNULL(ConcatStringTree::BSTNode* node) {
	setNULLRecur(root, node);
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
	root = nullptr;
}

//Class implementation for LitStringHash
LitStringHash::LitStringHash(const HashConfig& hashConfig) {
	this->config = hashConfig;
	this->arr = new HashItem[this->config.initSize];
	this->size = 0;
	this->capacity = config.initSize;
	this->idx_last = -1;
	for (int i = 0; i < capacity; i++) {
		this->arr[i].node = nullptr;
		this->arr[i].isEmpty = true;
	}
}

int LitStringHash::hashFunc(string s, int size, int p) {
	int sum = 0;
	for (int i = s.length() - 1; i >= 0; i--)
		sum = (sum * p + (int)(s[i])) % size;
	return sum;
}

ConcatStringTree::BSTNode* LitStringHash::insertHash(string s) {
	if (this->size == 0) 
		this->arr = new HashItem[this->config.initSize];
	int idx = hashFunc(s, config.initSize, config.p);
	int i = 0;
	while (this->arr[idx].isEmpty == false) {
		if (this->arr[idx].isEmpty == false) {
			if (this->arr[idx].node->data == s) 
				this->arr[idx].node->numRef++;
			return this->arr[idx].node;
		}
		idx = idx + config.c1 * i + config.c2 * i * i;
		idx = idx % capacity;
		i++;
		if (i == capacity)
			throw std::runtime_error("No possible slot");
	}
	this->arr[idx].node = new ConcatStringTree::BSTNode(s, 0);
	ConcatStringTree::BSTNode* tmp_ptr = this->arr[idx].node;
	this->arr[idx].node->key = keyGenerator();
	this->arr[idx].node->numRef++;
	this->arr[idx].isEmpty = false;
	this->arr[idx].node->RCST = true;
	arr[idx].node->parent_node->insertNode(arr[idx].node);
	this->size++; this->idx_last = idx;
	if ((double)size / capacity > config.lambda)
		reHash();
	return tmp_ptr;
}

ConcatStringTree::BSTNode* LitStringHash::searchHash(string s) {
	int idx = hashFunc(s, config.initSize, config.p);
	int i = 1; int first_idx = idx;
	while (this->arr[idx].node->data != s && this->arr[idx].isEmpty == false) {
		idx = idx + config.c1 * i + config.c2 * i * i;
		idx = idx % capacity;
		i++;
		if (i == capacity)
			throw std::runtime_error("No possible slot");
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
	this->arr[idx].node->numRef--;
	if (this->arr[idx].node->numRef == 0) {
		//arr[idx].node->parent_node->setNULL(arr[idx].node);
		//this->arr[idx].node = nullptr;
		this->arr[idx].isEmpty = true;
		this->size--;
	}

	if (this->size == 0) {
		delete[] arr;
		this->arr = nullptr;
		this->idx_last = -1;
	}
}

void LitStringHash::reHash() {
	int newCapacity = int(config.alpha * capacity);
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
			res += "();";
		else {
			res += "(litS=\"" + this->arr[i].node->data + "\"";
			res += ");";
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
			//delete arr[i].node->parent_node;
			delete this->arr[i].node;
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
}

ReducedConcatStringTree ReducedConcatStringTree::concat(ReducedConcatStringTree& otherS) {
	BSTNode* newTree = new BSTNode();
	newTree->left = root;
	newTree->right = otherS.root;
	newTree->lengthStr = otherS.root->lengthStr + this->root->lengthStr;
	newTree->leftLength = this->root->lengthStr;
	newTree->key = keyGenerator();
	newTree->parent_node->insertNode(newTree);
	newTree->left->parent_node->insertNode(newTree);
	if(newTree->right != newTree->left)
		newTree->right->parent_node->insertNode(newTree);
	return ReducedConcatStringTree(newTree, this->size + otherS.size + 1, this->litStringHash);
}

ReducedConcatStringTree::~ReducedConcatStringTree() {
	if (root->data != "") {
		this->litStringHash->deleteHash(root);
		//root = nullptr;
	}

}
