#include "fizzbuzz.h"
#include <iostream>
#include <functional>

bool test_fizzbuzz(unsigned in, std::string out) {
    std::string res = fizzbuzz(in);
    if (res != out) {
        std::cout << in << ": expected=" << out << ", actual=" << res << std::endl;
        return false;
    }
    return true;
}

// TODO add two (!) more test cases to achieve 100% line coverage in fizzbuzz.cpp
// Q: Which output scenario are we missing in our tests despite 100% line coverage?
// A: The scenario where in % 3 == 0 && in % 5 == 0 isn't covered
// Q: Name a coverage metric that would be better suited for this, i.e. will not produce
//    full coverage unless we hit all scenarios (all numeric strings are the same scenario).
// A: Path coverage 
std::function<bool()> tests[] = {
    [] () { return test_fizzbuzz(1, "1"); },
    [] () { return test_fizzbuzz(3, "Fizz"); },
    [] () { return test_fizzbuzz(5, "Buzz"); }
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
