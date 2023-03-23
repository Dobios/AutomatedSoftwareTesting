#include "prime.h"
#include <iostream>
#include <functional>

bool test_prime(int in, bool out) {
    return is_prime(in) == out;
}

// TODO add test cases to achieve 100% branch coverage (taken at least once) in prime.cpp
std::function<bool()> tests[] = {
        [] () { return test_prime(2, true); }
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

