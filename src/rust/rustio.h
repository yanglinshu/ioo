#ifndef __RUST_IO_H_
#define __RUST_IO_H_

#include "rustdef.h"

template<typename... T>
fn print(T... args) -> void;

template<typename... T>
fn println(T... args) -> void;

template<typename T>
fn _print(T arg) -> void;

#endif