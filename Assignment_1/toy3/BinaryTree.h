class BinaryTree {
    private:
    class Node {
        public:
        const int value;
        Node* left = nullptr;
        Node* right = nullptr;

        Node(int value);
    };

    Node *remove_smallest(Node **root_ptr);
    Node *remove_largest(Node **root_ptr);

    public:
    Node *m_root = nullptr;
    void add(int value);
    bool remove(int value);
};
