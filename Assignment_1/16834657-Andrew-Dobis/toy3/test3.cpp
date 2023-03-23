#include "BinaryTree.h"
#include <iostream>
#include <functional>

bool testcase0() {
    BinaryTree tree;

    tree.add(0);

    return true;
}

bool testcase1() {
    BinaryTree tree;
    
    tree.add(0);

    //Add something smaller 
    tree.add(-1);
    tree.add(-2);
    tree.add(-3);

    //Add something bigger
    tree.add(1);
    tree.add(2);

    tree.add(10);
    tree.add(7);
    tree.add(8);
    tree.add(9);
    tree.add(7);
    tree.add(6);
    tree.add(3);
    tree.add(5);

    //Remove the root
    tree.remove(0);

    //Remove a random node
    tree.remove(10);

    //Remove a leaf
    tree.remove(5);

    //Remove the small values
    tree.remove(-1);
    tree.remove(-3);
    tree.remove(-2);

    //Remove something that's not there
    tree.remove(12315);
    tree.remove(-1231245);
    return true;
}

bool testcase2() {
    BinaryTree tree;
    tree.add(0);

    tree.add(1000);
    tree.add(-1000);
    tree.add(1500);
    tree.add(500);
    tree.add(-1500);
    tree.add(-500);
    tree.add(250);
    tree.add(750);
    tree.add(1250);
    tree.add(1750);
    tree.add(-250);
    tree.add(-750);
    tree.add(-1250);
    tree.add(-1750);

    //Remove root
    tree.remove(0);

    return true;
    
}

bool testcase3() {
    BinaryTree tree;
    tree.add(0);

    tree.add(-1000);
    tree.add(-1500);
    tree.add(-500);
    tree.add(-250);
    tree.add(-750);
    tree.add(-1250);
    tree.add(-1750);

    //Remove root
    tree.remove(0);

    return true;
    
}
// TODO add test cases to achieve 100% line coverage in BinaryTree.cpp
std::function<bool()> tests[] = {
        testcase0,
        testcase1,
        testcase2,
        testcase3
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

