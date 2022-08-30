#include "ioo.hpp"


int main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ioo x = ioo_(65432654326543210000000000000000);
    ioo y = ioo_(1234512345123456);
    std::cout << x / y << std::endl;
    std::cout << x % y << std::endl;
}