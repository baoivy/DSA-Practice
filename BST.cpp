#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define MAX 100
#define PI 3.14
using namespace std;

struct Node {
    int data;
    Node* left, * right;
    int height;
};

void printInOrder(Node* root) {
    if (root != nullptr) {
        printInOrder(root->left);
        cout << root->data << " ";
        printInOrder(root->right);
    }
}

Node* createNode(int x) {
    Node* node = new Node;
    node->data = x;
    node->left = nullptr; node->right = nullptr;
    node->height = 0;
    return node;
}
Node* insertNode(Node* root, int x) {
    if (root == nullptr)
        return createNode(x);

    if (root->data < x) {
        root->right = insertNode(root->right, x);
    }
    else if (root->data > x) {
        root->left = insertNode(root->left, x);
    }
    return root;
}

Node* findMin(Node* root) {
    Node* tmp = root;
    while (tmp != nullptr && tmp->left != nullptr) {
        tmp = tmp->left;
    }
    return tmp;
}


Node* findMax(Node* root) {
    Node* tmp = root;
    while (tmp != nullptr && tmp->right != nullptr) {
        tmp = tmp->right;
    }
    return tmp;
}

Node* deleteNode(Node* root, int key)
{
    // base case
    if (root == nullptr)
        return root;
    if (key < root->data)
        root->left = deleteNode(root->left, key);

    else if (key > root->data)
        root->right = deleteNode(root->right, key);

    else {
 
        if (root->left == NULL and root->right == NULL)
            return NULL;

        else if (root->left == NULL) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        else {
            Node* temp = findMin(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data);
        }
    }
    return root;
}

int main()
{
    Node* root = nullptr;
    root = insertNode(root, 50);
    root = insertNode(root, 30);
    root = insertNode(root, 20);
    root = insertNode(root, 40);
    root = insertNode(root, 70);
    root = insertNode(root, 60);
    root = insertNode(root, 80);
    root = insertNode(root, 100);
    printInOrder(root);

    cout << endl;
    root = deleteNode(root, 70);
    root = deleteNode(root, 40);
    root = deleteNode(root, 50);
    printInOrder(root);
    return 0;
}
