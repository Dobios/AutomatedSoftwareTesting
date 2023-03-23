#include <stdexcept>

class LinkedList {
    private:
    class Node {
        public:
        const int value;
        Node *prev = nullptr;
        Node *next = nullptr;

        Node(int x);
    };

    Node *m_head = nullptr;
    Node *m_tail = nullptr;
    size_t m_size = 0;

    Node *get_node(size_t idx);

    public:
    size_t size();
    int operator[](size_t idx);
    void push_front(int value);
    void push_back(int value);
    bool erase(size_t idx);
};
