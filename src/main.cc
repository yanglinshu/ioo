#include "ioo.h"

int main() {
    ioo x = ioo_(123456);
    ioo y = ioo(LLONG_MIN);
    std::cout << x << std::endl;
    return 0;
}