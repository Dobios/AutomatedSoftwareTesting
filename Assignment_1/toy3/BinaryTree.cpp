#include "BinaryTree.h"

BinaryTree::Node::Node(int x) : value(x) {}

void BinaryTree::add(int value) {
    Node *new_node = new Node(value);
    if (m_root == nullptr) {
        m_root = new_node;
        return;
    }

    Node *node = m_root;

    while (true) {
        if (value <= node->value) {
            if (node->left == nullptr) {
                node->left = new_node;
                return;
            }
            node = node->left;
        } else {
            if (node->right == nullptr) {
                node->right = new_node;
                return;
            }
            node = node->right;
        }
    }
}

BinaryTree::Node *BinaryTree::remove_smallest(Node **root_ptr) {
    Node **parent_slot = root_ptr;
    Node *node = *root_ptr;

    while (node->left != nullptr) {
        parent_slot = &node->left;
        node = *parent_slot;
    }

    *parent_slot = nullptr;
    return node;
}

BinaryTree::Node *BinaryTree::remove_largest(Node **root_ptr) {
    Node **parent_slot = root_ptr;
    Node *node = *root_ptr;

    while (node->right != nullptr) {
        parent_slot = &node->right;
        node = *parent_slot;
    }

    *parent_slot = nullptr;
    return node;
}

bool BinaryTree::remove(int value) {
    Node **parent_slot = &m_root;
    Node *node = m_root;

    while (node != nullptr) {
        if (value == node->value) {
            Node *replacement = nullptr;

            if (node->right != nullptr) {
                replacement = remove_smallest(&node->right);
            } else if (node->left != nullptr) {
                replacement = remove_largest(&node->left);
            }

            if (replacement != nullptr) {
                replacement->left = node->left;
                replacement->right = node->right;
            }
            *parent_slot = replacement;
            delete node;
            return true;
        } else if (value <= node->value) {
            parent_slot = &node->left;
        } else {
            parent_slot = &node->right;
        }
        node = *parent_slot;
    }

    return false;
}
