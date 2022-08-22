#include "ioo.h"

int main() {
    ioo x = ioo_(12345678901234567890);
    ioo y = ioo(LLONG_MIN);
    std::cout << y.to_i64() << std::endl;
    return 0;
}