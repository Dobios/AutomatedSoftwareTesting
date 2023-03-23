#include "LinkedList.h"
#include <iostream>
#include <functional>

bool testcase0() {
    LinkedList list;
    list.push_back(5);

    return list.size() == 1;
}

bool testcase1() {
    LinkedList list;

    list.push_front(-1);
    list.erase(0); //trigger the (node->prev == nullptr) case
    list.erase(0); //trigger the (node == nullptr) case

    list.push_back(1);
    list.push_back(2);
    list.erase(1); //trigger the (node->next == nullptr) case

    list.push_front(0);
    list.push_front(3);
    list.push_back(4);

    list.erase(1); //trigger the general case

    //trigger the [] general case
    std::cout << list[1] << std::endl;

    // Trigger the [] error case
    try {
        std::cout << list[5] << std::endl;
    } catch(std::out_of_range e) {
        std::cout << e.what() << std::endl;
    }

    return true;
}

// TODO add test cases to achieve 100% line coverage in LinkedList.cpp
std::function<bool()> tests[] = {
        testcase0,
        testcase1
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

