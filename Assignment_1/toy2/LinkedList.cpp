#include "LinkedList.h"

LinkedList::Node::Node(int x) : value (x) {}

LinkedList::Node *LinkedList::get_node(size_t idx) {
    if (idx >= m_size) {
        return nullptr;
    }

    LinkedList::Node *node;
    if (idx < m_size/2) {
        node = m_head;
        for (size_t i = 0; i < idx; i++) {
            node = node->next;
        }
    } else {
        node = m_tail;
        for (size_t i = m_size-1; i > idx; i--) {
            node = node->prev;
        }
    }
    return node;
}

size_t LinkedList::size() {
    return m_size;
}

void LinkedList::push_front(int value) {
    LinkedList::Node *node = new Node(value);
    if (m_size++ == 0) {
        m_tail = node;
    } else {
        m_head->prev = node;
        node->next = m_head;
    }
    m_head = node;
}

void LinkedList::push_back(int value) {
    LinkedList::Node *node = new Node(value);
    if (m_size++ == 0) {
        m_head = node;
    } else {
        m_tail->next = node;
        node->prev = m_tail;
    }
    m_tail = node;
}

int LinkedList::operator[](size_t idx) {
    LinkedList::Node *node = get_node(idx);
    if (node == nullptr) {
        throw std::out_of_range("invalid list index");
    }
    return node->value;
}

bool LinkedList::erase(size_t idx) {
    LinkedList::Node *node = get_node(idx);
    if (node == nullptr) {
        return false;
    }

    if (node->prev == nullptr) {
        m_head = node->next;
    } else {
        node->prev->next = node->next;
    }

    if (node->next == nullptr) {
        m_tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }

    m_size--;
    delete node;

    return true;
}
