#include "polyval2d.hpp"

#include <iostream>
#include <span>

int main() {
    std::vector<double> c(9, 1.0);
    std::vector<double> x{1.0, 2.0, 3.0, 4.0};
    std::vector<double> y{2.0, 3.0, 4.0, 5.0};

    std::vector<double> res(4);

    polyval2d(std::span{c}, {3, 3}, std::span{x}, std::span{y}, 30, std::span{res});

    for(const auto val: res) {
        std::cout << val << ", ";
    }

    return 0;
}
