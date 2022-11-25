#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

class ReducedConcatStringTree; //forward declaration
class LitStringHash;

class ConcatStringTree {
public: 
    class BSTNode; //forward declaration
    class ParentsTree; //forward declaration
protected:
    BSTNode* root;
    int size; //Capacity

    //Method for BST Tree
    void preOrderString(BSTNode* root, string& ) const;
    void inOrdertoString(BSTNode* root, string& ) const;
    BSTNode* inOrderReverse(BSTNode* root) const;
    BSTNode* preOrderSubString(BSTNode* root, int from, int to) const;
    void updateLeftL(BSTNode* root) const;
    void clearParents(BSTNode* root);
    bool checkLeaf(BSTNode* root);

public:
    ConcatStringTree(const char* s);
    ConcatStringTree(BSTNode* root, int size) {
        this->root = root;
        this->size = size;
    }
    ConcatStringTree(){}
    int length() const;
    char get(int index);
    int indexOf(char c);
    string toStringPreOrder() const;
    string toString() const;
    ConcatStringTree concat(const ConcatStringTree & otherS) const;
    ConcatStringTree subString(int from, int to) const;
    ConcatStringTree reverse() const;
    ~ConcatStringTree();

    int getParTreeSize(const string & query) const;
    string getParTreeStringPreOrder(const string & query) const;

    class ParentsTree {
    public:
        class AVLNode; //forward declaration
    private:
        AVLNode* root;
        int count;
        //Implementation for AVL Tree
        int height(AVLNode* node);
        int getBalance(AVLNode* node);
        AVLNode* rightRotate(AVLNode* node);
        AVLNode* leftRotate(AVLNode* node);
        AVLNode* maxValue(AVLNode* node);
        void PreOrderTraversal(AVLNode* root, string&) const;
        void clear(AVLNode* root);
        AVLNode* insert(AVLNode* root, ConcatStringTree::BSTNode* node);
        AVLNode* remove(AVLNode* root, ConcatStringTree::BSTNode* node, bool &canDelete);
        void setNULLRecur(AVLNode* root, ConcatStringTree::BSTNode* node);

    public:
        ParentsTree() {
            this->root = nullptr;
            this->count = 0;
        }
        int size() const;
        string toStringPreOrder() const;
        void insertNode(ConcatStringTree::BSTNode* node);
        void deleteNode(ConcatStringTree::BSTNode* node);
        void setNULL(ConcatStringTree::BSTNode* node);
        ~ParentsTree();

        class AVLNode {
        private:
            ConcatStringTree::BSTNode* node;
            int key; //Left length string
            AVLNode* left;
            AVLNode* right;
            int height;
            friend class ParentsTree;

        public:
            AVLNode(ConcatStringTree::BSTNode* node) {
                this->node = node;
                this->key = node->key;
                this->height = 1;
                this->left = nullptr;
                this->right = nullptr;
            }
        };
    };

    class BSTNode {
    protected:
        int key;
        string data;
        int leftLength; //Left length string
        int lengthStr;
        int numRef;
        bool RCST;
        BSTNode* left;
        BSTNode* right;
        ParentsTree *parent_node;
        friend class ConcatStringTree;
        friend class ParentsTree;
        friend class LitStringHash;
        friend class ReducedConcatStringTree;
    public:
        BSTNode() {
            this->data = "";
            this->left = nullptr;
            this->right = nullptr;
            this->parent_node = new ParentsTree();
            this->numRef = 0;
            this->RCST = false;
        }
        BSTNode(string data, int leftLength) {
            this->leftLength = leftLength;
            this->key = 0;
            this->left = nullptr;
            this->right = nullptr;
            this->lengthStr = (int)data.length();
            this->parent_node = new ParentsTree();
            this->RCST = false;
            this->data = data;
            this->numRef = 0;
        }
        ~BSTNode() {
            delete this->parent_node;
            this->parent_node = nullptr;
        }
    };
};

class ReducedConcatStringTree; // forward declaration

class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;

    friend class LitStringHash;

public:
    HashConfig() {}
    HashConfig(int p, double c1, double c2, double lambda, double alpha, int initSize) {
        this->p = p;
        this->c1 = c1;
        this->c2 = c2;
        this->lambda = lambda;
        this->alpha = alpha;
        this->initSize = initSize;
    }
};

class LitStringHash {
private:
    class HashItem; //forward declaration
    HashItem* arr;
    HashConfig config;
    int size;
    int capacity;
    int idx_last;

    int hashFunc(string s, int, int);
    void reHash();
    ConcatStringTree::BSTNode* insertHash(string s);
    ConcatStringTree::BSTNode* searchHash(string s);
    void deleteHash(ConcatStringTree::BSTNode* node);
    void updateNumRef(ConcatStringTree::BSTNode* node);
    friend class ReducedConcatStringTree;

public:
    LitStringHash(const HashConfig& hashConfig);
    int getLastInsertedIndex() const;
    string toString() const;
    ~LitStringHash();

private:
    class HashItem {
    private:
        ConcatStringTree::BSTNode* node;
        bool isEmpty;
        friend class LitStringHash;
    public:
        HashItem() {
            this->node = nullptr;
            this->isEmpty = true;
        }
    };
};

class ReducedConcatStringTree : public ConcatStringTree {
private:
    BSTNode* inOrderReverse(BSTNode* root) const;
    BSTNode* preOrderSubString(BSTNode* root, int from, int to) const;
public:
    LitStringHash* litStringHash;
    ReducedConcatStringTree(BSTNode* root, int size, LitStringHash*& litStringHash) {
        this->root = root;
        this->size = size;
        this->litStringHash = litStringHash;
    }
    ReducedConcatStringTree(const char* s, LitStringHash*& litStringHash);
    ReducedConcatStringTree concat(ReducedConcatStringTree& otherS);
    ~ReducedConcatStringTree();
};

#endif // __CONCAT_STRING_TREE_H_
