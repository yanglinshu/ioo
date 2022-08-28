#ifndef __IOO_HPP_
#define __IOO_HPP_

#include <iostream>
#include <assert.h>
#include "rustdef.h"
#include "utils.h"

template<usize length = 8>
class ioo {

    static_assert(length > 0, "length must be greater than 0");
    static_assert(length < 10, "length must be less than 64");

    using Item = u32;
    using Self = ioo;    
    
    static const usize     ITEM_LENGTH = length;    
    static const Vec<Item> ITEM_BASES;
    static const Item      ITEM_MAX;



    Vec<Item> buf;
    bool      sign;

    fn from_string(String& src) {
        mut usize len = src.size();

        assert(len != 0);
        // if (len == 0) {
        //     panic_("Unable to convert empty string to ioo");
        // }

        assert(src[0] == '-' || ('0' <= src[0] && src[0] <= '9'));

        mut usize start = 0;
        if (src[0] == '-') {
            self->sign = true;
            start = 1;
        } else if ('0' <= src[0] && src[0] <= '9') {
            self->sign = false;
        } // else {
        //     panic_("Unable to convert string to ioo");
        // }

        bool marked = false;
        for (mut usize i = start; i < len; i += 1) {
            assert(('0' <= src[i] && src[i] <= '9'));
            // if (src[i] < '0' || src[i] > '9') {
            //     panic_("Unable to convert string to ioo");
            // }
            if ('0' < src[i] && (!marked)) {
                start = i;
                marked = true;
            }
        }
        
        mut i32 item = 0;
        for (mut usize i = 0; i < len - start; i += 1) {
            item += (src[len - 1 - i] - '0') * ITEM_BASES[i % ITEM_LENGTH];
            if ((i + 1) % ITEM_LENGTH == 0) {
                self->buf.push_back(item);
                item = 0;
            }
        }
        if (item != 0) {
            self->buf.push_back(item);
        }
        if (self->buf.empty()) {
            self->buf.push_back(0);
        }
    }

    fn from_i64(mut i64 src) {
        if (src < 0) {
            self->sign = true;
        } 
        while (src != 0) {
            if (src < 0) {
                self->buf.push_back(-(src % ITEM_MAX));
            } else {
                self->buf.push_back(src % ITEM_MAX);
            }
            src /= ITEM_MAX;
        }
        if (self->buf.empty()) {
            self->buf.push_back(0);
        }
    }

    fn trim() {
        while (self->buf.size() > 1 && self->buf[self->buf.size() - 1] == 0) {
            self->buf.pop_back();
        }
        if (*self == 0 && self->sign) {
            self->sign = false;
        }
    }





    pub ioo(): buf(), sign(false) {
        self->buf.push_back(0);
    }

    pub ioo(Self &other): buf(other.buf), sign(other.sign) {}

    pub ioo(mut Self &&other): buf(std::move(other.buf)), sign(other.sign) {}

    pub ioo(String& src): buf(), sign(false) {
        self->from_string(src);
    }

    pub ioo(String&& src): buf(), sign(false) {
        self->from_string(src);
    }

    pub ioo(mut i64 src): buf(), sign(false) {
        self->from_i64(src);
    }





    
    pub fn to_string() -> String {
        mut String ret = "";
        if (self->sign) {
            ret += "-";
        }
        for (mut usize i = 0; i < self->buf.size(); i += 1) {
            mut String item = std::to_string(self->buf[self->buf.size() - 1 - i]);
            if (i != 0) {
                item = String(ITEM_LENGTH - item.size(), '0') + item;
            }
            ret += item;
        }
        return ret;
    }

    pub fn to_i64() -> i64 {
        assert(*self <= LLONG_MAX && *self >= LLONG_MIN);
        // if (*self > LLONG_MAX || *self < LLONG_MIN) {
        //     panic_("Unable to convert ioo to i64");
        // }
        mut i64 result = 0;
        mut i64 base = 1;
        if (self->sign) {
            base = -1;
        } else {
            base = 1;
        }
        for (mut usize i = 0; i < self->buf.size(); i += 1) {
            result += self->buf[i] * base;
            base *= ITEM_MAX;        
        }
        return result;
    }






    static fn from(Self &other) -> Self {
        return ioo(other);
    }

    static fn from(mut Self &&other) -> Self {
        return ioo(std::move(other));
    }

    static fn from(String& src) -> Self {
        return ioo(src);
    }

    static fn from(String&& src) -> Self {
        return ioo::from(src);
    }

    static fn from(mut i64 src) -> Self {
        return ioo(src);
    }






    fn operator=(Self& other) -> Self {
        self->buf = other.buf;
        self->sign = other.sign;
        return *self;
    }
    
    fn operator=(mut Self&& other) -> Self {
        *self = other;
        return *self;
    }





    fn operator==(Self& other) -> bool {
        if (self->sign != other.sign || self->buf.size() != other.buf.size()) {
            return false;
        }
        for (mut usize i = 0; i < self->buf.size(); i += 1) {
            if (self->buf[i] != other.buf[i]) {
                return false;
            }
        }
        return true;
    }

    fn operator==(Self&& other) -> bool {
        return *self == other;
    }

    fn operator==(mut i64 other) -> bool {
        return *self == ioo(other);
    }

    fn operator!=(Self& other) -> bool {
        return !(*self == other);
    }

    fn operator!=(Self&& other) -> bool {
        return *self != other;
    }

    fn operator!=(mut i64 other) -> bool {
        return *self != ioo(other);
    }

    fn operator<(Self& other) -> bool {
        if (self->sign != other.sign) {
            return self->sign;
        }
        if (self->buf.size() != other.buf.size()) {
            if (self->sign) {
                return self->buf.size() > other.buf.size();
            } else {
                return self->buf.size() < other.buf.size();
            }
        }
        for (mut usize i = 0; i < self->buf.size(); i += 1) {
            usize index = self->buf.size() - 1 - i;
            if (self->buf[index] != other.buf[index]) {
                if (self->sign) {
                    return self->buf[index] > other.buf[index];
                } else {
                    return self->buf[index] < other.buf[index];
                }
            }
        }
        return false;
    }

    fn operator<(Self&& other) -> bool {
        return *self < other;
    }

    fn operator<(mut i64 other) -> bool {
        return *self < ioo(other);
    }

    fn operator<=(Self& other) -> bool {
        return *self == other || *self < other;
    }

    fn operator<=(Self&& other) -> bool {
        return *self <= other;
    }

    fn operator<=(mut i64 other) -> bool {
        return *self <= ioo(other);
    }

    fn operator>(Self& other) -> bool {
        return !(*self <= other);
    }

    fn operator>(Self&& other) -> bool {
        return *self > other;
    }

    fn operator>(mut i64 other) -> bool {
        return *self > ioo(other);
    }

    fn operator>=(Self& other) -> bool {
        return !(*self < other);
    }

    fn operator>=(Self&& other) -> bool {
        return *self >= other;
    }

    fn operator>=(mut i64 other) -> bool {
        return *self >= ioo(other);
    }







    fn operator-() -> Self {
        mut Self result = *self;
        result.sign = !result.sign;
        return result;
    }

    fn operator!() -> bool {
        return *self == 0;
    }






    fn operator+(Self& other) -> Self {
        if (self->sign == other.sign) {
            mut Self result = Self();
            result.sign = self->sign;
            result.buf.pop_back();
            mut i64 carry = 0;
            for (mut usize i = 0; i < std::max(self->buf.size(), other.buf.size()); i += 1) {
                mut i64 sum = carry;
                if (i < self->buf.size()) {
                    sum += self->buf[i];
                }
                if (i < other.buf.size()) {
                    sum += other.buf[i];
                }
                result.buf.push_back(sum % ITEM_MAX);
                carry = sum / ITEM_MAX;
            }
            if (carry != 0) {
                result.buf.push_back(carry);
            }
            result.trim();
            return result;
        } else {
            if (self->sign) {
                return other - (-*self);
            } else {
                return *self - (-other);
            }
        }
    }

    fn operator+(Self&& other) -> Self {
        return *self + other;
    }

    fn operator+(mut i64 other) -> Self {
        return *self + ioo(other);
    }

    fn operator-(Self& other) -> Self {
        if (self->sign == other.sign) {
            if (self->sign) {
                return -((-*self) - (-other));
            } else if (*self < other) {
                return -(other - *self);            
            } else {
                mut Self result = Self();
                result.sign = self->sign;
                result.buf.pop_back();
                mut i64 borrow = 0;
                for (mut usize i = 0; i < std::max(self->buf.size(), other.buf.size()); i += 1) {
                    mut i64 diff = borrow;
                    if (i < self->buf.size()) {
                        diff += self->buf[i];
                    }
                    if (i < other.buf.size()) {
                        diff -= other.buf[i];
                    }
                    if (diff < 0) {
                        borrow = -ceil((-diff) / (ITEM_MAX * 1.0));
                        diff += ITEM_MAX * (-borrow);
                    } else {
                        borrow = 0;
                    }
                    result.buf.push_back(diff);
                }
                result.trim();
                return result;
            }
        } else {
            if (self->sign) {
                return -((-*self) + other);
            } else {
                return *self + (-other);
            }
        }
    }

    fn operator-(Self&& other) -> Self {
        return *self - other;
    }

    fn operator-(mut i64 other) -> Self {
        return *self - ioo(other);
    }





    friend fn operator<<(std::ostream& os, ioo &src) -> std::ostream& {
        os << src.to_string();
        return os;
    }

    friend fn operator<<(std::ostream& os, ioo &&src) -> std::ostream& {
        return os << src;
    }

};

template<usize length>
const Vec<typename ioo<length>::Item> ioo<length>::ITEM_BASES = ([]() {
    Vec<Item> bases;
    Item base = 1;
    for (usize i = 0; i <= length; i++) {
        bases.push_back(base);
        base *= 10;
    }
    return bases;
})();

template<usize length>
const typename ioo<length>::Item ioo<length>::ITEM_MAX = ioo<length>::ITEM_BASES[length];

#define ioo_(x) ioo(#x)

#endif