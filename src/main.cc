#include "ioo.hpp"


int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    // ioo1 x = ioo1_(1234512345123456);
    // ioo1 y = ioo1_(6543265432654321);
    ioo<2, true> x, y;
    std::cin >> x >> y;
    std::cout << x * y << std::endl;
}