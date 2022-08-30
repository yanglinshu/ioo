#ifndef __RUST_IO_HPP_
#define __RUST_IO_HPP_

#include "rustdef.h"

// ![Unfinished] Not available yet.
template<typename... T>
fn print(T... args) -> void {
    (_print(args), ...);
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

#endif