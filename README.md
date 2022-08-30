# IOO - A C++ Implementation of High Accuracy Algorithm

IOO, meaning i-infinity (like i32),  is a C++ implementation of high accuracy algorithm, providing support to basic arithmetic operations. Some rust style macros are added for convenience, which can be removed easily with the help of your compiler.

## Feature
- Customizable item size by using generic.
- Build in FFT support.
- Compatible with C++ build in types.
- Well designed and rust-like API.



## Get Started

You can use IOO simply by include "ioo.hpp". And the ioo_ macro allows you to create an ioo instance from literals.

```cpp
#include "ioo.hpp"

int main() {
    ioo x = ioo_(1145141919810), y;
    std::cin >> y;
    std::cout << x * y << std::endl;
}
```



## License

Licensed under the [MIT License](https://github.com/yanglinshu/ioo/blob/master/LICENSE)

All rights reserved by yanglinshu.