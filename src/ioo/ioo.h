#ifndef __IOO_H_
#define __IOO_H_

#include <iostream>

#include "rustdef.h"

class ioo {

    using Item = u32;
    using Self = ioo;    

    static const Item  ITEM_MAX    = 100000000;
    static const usize ITEM_LENGTH = 9;



    Vec<Item> buf;
    bool      sign;

    fn from_string(String& src);
    fn from_i64(i64 src);
    fn trim();

    pub ioo();
    pub ioo(Self &other);
    pub ioo(mut Self &&other);
    pub ioo(String& src);
    pub ioo(String&& src);
    pub ioo(mut i64 src);

    
    pub fn to_string() -> String;


    static fn from(String& src) -> Self;
    static fn from(String&& src) -> Self;
    static fn from(i64 src) -> Self;

    fn operator==(Self& other) -> bool;
    fn operator==(Self&& other) -> bool;
    fn operator==(mut i64 other) -> bool;
    fn operator!=(Self& other) -> bool;
    fn operator!=(Self&& other) -> bool;
    fn operator!=(mut i64 other) -> bool;
    fn operator<(Self& other) -> bool;
    fn operator<(Self&& other) -> bool;
    fn operator<(mut i64 other) -> bool;
    fn operator<=(Self& other) -> bool;
    fn operator<=(Self&& other) -> bool;
    fn operator<=(mut i64 other) -> bool;
    fn operator>(Self& other) -> bool;
    fn operator>(Self&& other) -> bool;
    fn operator>(mut i64 other) -> bool;
    fn operator>=(Self& other) -> bool;
    fn operator>=(Self&& other) -> bool;
    fn operator>=(mut i64 other) -> bool;

    fn operator-() -> Self;
    fn operator!() -> bool;

    friend fn operator<<(std::ostream& os, ioo &dst) -> std::ostream&;
    friend fn operator<<(std::ostream& os, ioo &&dst) -> std::ostream&;

};

#define ioo_(x) ioo(#x)

#endif