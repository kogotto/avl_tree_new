#include <stdlib.h>
#include <iostream>
using namespace std;


template<class T, class U>
class TAvlTree {
public:
    typedef T key_t;
    typedef U data_t;
    typedef unsigned char height_t;
    typedef int delta_t;

    TAvlTree():
        root(0)
    {}

    ~TAvlTree() {
        free(root);
    }

    void insert(const key_t & key, const data_t & data = data_t()) {
        node_t * parrent = 0;
        node_t *& newNode = baseFind(root, key, parrent);
        newNode = new node_t(parrent, key, data);

        fixParrentsHeights(newNode);
    }

private:
    struct node_t {
        node_t(node_t * parrent, const key_t & key, const data_t data):
            parrent(parrent),
            key(key),
            data(data),
            height(1),
            left(0),
            right(0)
        {}


        node_t * parrent;
        key_t key;
        data_t data;
        height_t height;
        node_t * left;
        node_t * right;
    };

    static bool isRoot(node_t * node) {
        return node->parrent == 0;
    }

    static bool isLeftChild(node_t * node) {
        return !isRoot(node) && node->parrent->left == node;
    }

    static bool isRightChild(node_t * node) {
        return !isRoot(node) && node->parrent->right == node;
    }

    static height_t height(node_t * node) {
        return (node == 0) ? 0 : node->height;
    }

    static delta_t dh(node_t * node) {
        return (node == 0) ?
                    0 :
                    (static_cast<delta_t>(height(node->left)) - static_cast<delta_t>(height(node->right)));
    }

    static void fixHeight(node_t * node) {
        node->height = max(height(node->left), height(node->right)) + 1;
    }

    static node_t *& baseFind(node_t *& root, const key_t & key, node_t *& parrent) {
        if (root == 0) {
            parrent = 0;
            return root;
        }

        parrent = root;
        while(1) {
            node_t *& current = (key < parrent->key) ?
                        parrent->left :
                        parrent->right;
            if (current == 0) {
                return current;
            }

            parrent = current;
        }
    }

    static void fixParrentsHeights(node_t *& node) {
        node_t * parrent = node->parrent;
        while (parrent != 0) {
            fixHeight(parrent);

            if (abs(dh(parrent)) > 1) {
                parrent = balance(parrent);
            }

            parrent = parrent->parrent;
        }
    }

    static node_t * balance(node_t *& root) {
        if (dh(root) == 2) {
            if (dh(root->left) == -1) {
                leftRotate(root->left);
            }
            return rightRotate(root);
        }

        if (dh(root) == -2) {
            if (dh(root->right) == 1) {
                rightRotate(root->right);
            }
            return leftRotate(root);
        }

        return root;
    }

    static node_t * leftRotate(node_t *& root) {
        node_t * right = root->right;
        node_t * rightLeft = right->left;

        root->right = rightLeft;
        if (rightLeft != 0) {
            rightLeft->parrent = root;
        }
        right->left = root;
        right->parrent = root->parrent;
        root->parrent = right;
        root = right;

        fixHeight(root->left);
        fixHeight(root);

        return root;
    }

    static node_t * rightRotate(node_t *& root) {
        node_t * left = root->left;
        node_t * leftRight = left->right;

        root->left = leftRight;
        if (leftRight != 0) {
            leftRight->parrent = root;
        }
        left->right = root;
        left->parrent = root->parrent;
        root->parrent = left;
        root = left;

        fixHeight(root->left);
        fixHeight(root->right);

        return root;
    }

    void free(node_t * root) {
        if (root == 0) {
            return;
        }

        free(root->left);
        free(root->right);

        delete root;
    }

    node_t * root;
};

typedef int key_t;
typedef struct {} data_t;

int main()
{
    TAvlTree<key_t, data_t> tree;
    size_t size = 0;
    cin >> size;
    for (size_t i = 0; i < size; ++i) {
        key_t key = 0;
        cin >> key;
        tree.insert(key);
    }

    return 0;
}

