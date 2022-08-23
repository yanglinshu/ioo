#include "ioo.h"
#include "utils.h"



fn ioo::from_string(String& src) {
    mut usize len = src.size();
    
    if (len == 0) {
        panic_("Unable to convert empty string to ioo");
    }

    mut usize start = 0;
    if (src[0] == '-') {
        self->sign = true;
        start = 1;
    } else if ('0' <= src[0] && src[0] <= '9') {
        self->sign = false;
    } else {
        panic_("Unable to convert string to ioo");
    }

    bool marked = false;
    for (mut usize i = start; i < len; i += 1) {
        if (src[i] < '0' || src[i] > '9') {
            panic_("Unable to convert string to ioo");
        }
        if ('0' < src[i] && (!marked)) {
            start = i;
            marked = true;
        }
    }

    mut i32 item = 0;
    for (mut usize i = 0; i < len - start; i += 1) {
        item += (src[len - 1 - i] - '0') * TEN_BASES[i % ITEM_LENGTH];
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

fn ioo::from_i64(mut i64 src) {
    if (src < 0) {
        self->sign = true;
    } 
    while (src != 0) {
        if (src < 0) {
            self->buf.push_back(-(src % (ITEM_MAX * 10)));
        } else {
            self->buf.push_back(src % (ITEM_MAX * 10));
        }
        src /= ITEM_MAX * 10;
    }
    if (self->buf.empty()) {
        self->buf.push_back(0);
    }
}

fn ioo::trim() {
    while (self->buf.size() > 1 && self->buf[self->buf.size() - 1] == 0) {
        self->buf.pop_back();
    }
    if (*self == 0 && self->sign) {
        self->sign = false;
    }
}





ioo::ioo(): buf(), sign(false) {
    self->buf.push_back(0);
}

ioo::ioo(Self &other): buf(other.buf), sign(other.sign) {}

ioo::ioo(mut Self &&other): buf(std::move(other.buf)), sign(other.sign) {}

ioo::ioo(String&& src): buf(), sign(false) {
    self->from_string(src);
}

ioo::ioo(String& src): buf(), sign(false) {
    self->from_string(src);
}

ioo::ioo(mut i64 src): buf(), sign(false) {
    self->from_i64(src);
}


fn ioo::to_string() -> String {
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

fn ioo::to_i64() -> i64 {
    if (*self > LLONG_MAX || *self < LLONG_MIN) {
        panic_("Unable to convert ioo to i64");
    }
    mut i64 result = 0;
    mut i64 base = 1;
    if (self->sign) {
        base = -1;
    } else {
        base = 1;
    }
    for (mut usize i = 0; i < self->buf.size(); i += 1) {
        result += self->buf[i] * base;
        base *= ITEM_MAX * 10;        
    }
    return result;
}




fn ioo::from(String& src) -> Self {
    return ioo(src);
}

fn ioo::from(String&& src) -> Self {
    return ioo::from(src);
}

fn ioo::from(mut i64 src) -> Self {
    return ioo(src);
}




fn ioo::operator==(Self& other) -> bool {
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

fn ioo::operator==(Self&& other) -> bool {
    return *self == other;
}

fn ioo::operator==(mut i64 other) -> bool {
    return *self == ioo(other);
}

fn ioo::operator!=(Self& other) -> bool {
    return !(*self == other);
}

fn ioo::operator!=(Self&& other) -> bool {
    return *self != other;
}

fn ioo::operator!=(mut i64 other) -> bool {
    return *self != ioo(other);
}

fn ioo::operator<(Self& other) -> bool {
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
        if (self->buf[i] != other.buf[i]) {
            if (self->sign) {
                return self->buf[i] > other.buf[i];
            } else {
                return self->buf[i] < other.buf[i];
            }            
        }
    }
    return false;
}

fn ioo::operator<(Self&& other) -> bool {
    return *self < other;
}

fn ioo::operator<(mut i64 other) -> bool {
    return *self < ioo(other);
}

fn ioo::operator<=(Self& other) -> bool {
    return *self == other || *self < other;
}

fn ioo::operator<=(Self&& other) -> bool {
    return *self <= other;
}

fn ioo::operator<=(mut i64 other) -> bool {
    return *self <= ioo(other);
}

fn ioo::operator>(Self& other) -> bool {
    return !(*self <= other);
}

fn ioo::operator>(Self&& other) -> bool {
    return *self > other;
}

fn ioo::operator>(mut i64 other) -> bool {
    return *self > ioo(other);
}

fn ioo::operator>=(Self& other) -> bool {
    return !(*self < other);
}

fn ioo::operator>=(Self&& other) -> bool {
    return *self >= other;
}

fn ioo::operator>=(mut i64 other) -> bool {
    return *self >= ioo(other);
}

fn ioo::operator-() -> Self {
    mut Self result = *self;
    result.sign = !result.sign;
    return result;
}

fn ioo::operator!() -> bool {
    return *self == 0;
}




fn operator<<(mut std::ostream& os, ioo& src) -> std::ostream& {
    os << src.to_string();
    return os;
}

fn operator<<(mut std::ostream& os, ioo&& src) -> std::ostream& {
    return os << src;
}