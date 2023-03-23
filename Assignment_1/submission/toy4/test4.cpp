#include "prime.h"
#include <iostream>
#include <functional>

bool test_prime(int in, bool out) {
    return is_prime(in) == out;
}

// TODO add test cases to achieve 100% branch coverage (taken at least once) in prime.cpp
std::function<bool()> tests[] = {
        [] () { return test_prime(2, true); },
        [] () { return test_prime(3, true); },
        [] () { return test_prime(0, false); },
        [] () { return test_prime(4, false); },
        [] () { return test_prime(9, false); },
        [] () { return test_prime(6, false); },
        [] () { return test_prime(5, true); },
        [] () { return test_prime(25, false); },
        [] () { return test_prime(11, true); },
        [] () { return test_prime(17, true); },
        [] () { return test_prime(37, true); },
        [] () { return test_prime(49, false); }
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

