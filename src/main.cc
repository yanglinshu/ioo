#include "ioo.hpp"
#include "rustio.hpp"

int main() {
    ioo x = ioo_(10000000000002);
    
    ioo y = ioo(LLONG_MIN);
    ioo z = ioo(-1111111111113);
    std::cout << x + z << std::endl;    
    return 0;
}