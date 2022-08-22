#include <iostream>

#include "rustio.h"

template<typename... T>
fn print(T... args) -> void {
    (_print(args), ...);
    _print(' ');
}

template<typename... T>
fn println(T... args) -> void {
    print(args...);
    _print('\n');
}

template<typename T>
fn _print(T arg) -> void {
    std::cout << arg;
}
