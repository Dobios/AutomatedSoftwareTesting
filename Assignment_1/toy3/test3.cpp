#include "BinaryTree.h"
#include <iostream>
#include <functional>

bool testcase0() {
    BinaryTree tree;

    tree.add(0);

    return true;
}

// TODO add test cases to achieve 100% line coverage in BinaryTree.cpp
std::function<bool()> tests[] = {
        testcase0
};

int main() {
    unsigned ran = 0, passed = 0;
    for (auto test : tests) {
        ran++;
        passed += test();
    }
    std::cout << "Passed " << passed << "/" << ran << " test cases!" << std::endl;
    return 0;
}

