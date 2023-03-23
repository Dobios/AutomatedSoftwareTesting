#include "fizzbuzz.h"

std::string fizzbuzz(unsigned n) {
    std::string out = "";
    if ((n % 3) == 0) {
        out += "Fizz";
    }
    if ((n % 5) == 0) {
        out += "Buzz";
    }
    if (out.empty()) {
        return std::to_string(n);
    }
    return out;
}
