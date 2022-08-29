#include "ioo.hpp"


int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    ioo<1> x, y;
    std::cin >> x >> y;
    std::cout << x.qmul(y) << std::endl;
}