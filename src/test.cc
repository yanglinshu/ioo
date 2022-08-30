#include <vector>
#include <string>
#include <limits.h>
#include <math.h>
#include <iostream>
#include <assert.h>
#include <tuple>
#include <complex>

#define Vec       std::vector
#define vec_(...) std::vector({__VA_ARGS__})

#define String std::string

#define Tuple(...)  std::tuple<__VA_ARGS__>
#define tuple_(...) std::make_tuple(__VA_ARGS__)
#define let auto

#define mut
#define ref  &
#define fn   auto
#define pub  public:
#define priv private:
#define self this
#define unsafe

#define panic_(x) throw(x)

#define i8  signed char
#define i16 short
#define i32 int
#define i64 long long

#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

#define isize ptrdiff_t
#define usize size_t

#define f32 float
#define f64 double

#define bool bool

#define char unsigned char


template<typename T>
struct FFT {

    static_assert(std::is_integral<T>::value, "T must be integral");

    static const f64 PI;
    using Item = std::complex<f64>;
    using Poly = Vec<T>;

    static fn init_rev(usize n) -> Vec<usize> {
        usize limit = 1;
        usize bits = 0;
        while (limit < n) {
            limit <<= 1;
            bits += 1;
        }

        mut Vec<usize> rev = Vec<usize>();
        rev.assign(limit, 0);
        for (usize i = 0; i < limit; i += 1) {
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bits - 1));
        }
        return rev;
    }

    static fn to_complex(Poly& src) -> Vec<Item> {
        mut Vec<Item> result = Vec<Item>();
        for (usize i = 0; i < src.size(); i += 1) {
            result.push_back(Item(src[i], 0));
        }
        return result;
    }

    static fn _fft(Vec<Item>& src, Vec<usize> rev, usize limit, i32 opt) -> Vec<Item> {
        mut Vec<Item> result = src;
        result.resize(limit + 1, Item(0, 0));
        for (usize i = 0; i < limit; i += 1) {
            if (i < rev[i]) {
                std::swap(result[i], result[rev[i]]);
            }
        }

        
        for (usize mid = 1; mid < limit; mid <<= 1) {
            Item w_mid = Item(cos(PI / mid), opt * sin(PI / mid));
            for (usize j = 0; j < limit; j += mid << 1) {
                Item w = Item(1, 0);
                for (usize k = 0; k < mid; k += 1) {
                    Item u = result[j + k];
                    Item v = result[j + k + mid] * w;
                    result[j + k] = u + v;
                    result[j + k + mid] = u - v;
                    w = w * w_mid;
                }
            }
        }
        return result;
    }

    static fn _fft(Vec<Item>&& src, Vec<usize> rev, usize limit, i32 opt) -> Vec<Item> {
        return _fft(src, rev, limit, opt);
    }

    static fn fft(Poly& src) -> Vec<Item> {
        let rev = init_rev(src.size());
        return _fft(to_complex(src), rev, rev.size(), 1);
    }

    static fn ifft(Poly& src) -> Vec<Item> {
        let rev = init_rev(src.size());     
        return _fft(to_complex(src), rev, rev.size(), -1);
    }

    static fn mul(Poly& lhs, Poly& rhs) -> Poly {
        let rev = init_rev(lhs.size() + rhs.size() - 1);
        let limit = rev.size();
        mut Vec<Item> lhs_fft = _fft(to_complex(lhs), rev, limit, 1);
        mut Vec<Item> rhs_fft = _fft(to_complex(rhs), rev, limit, 1);
        for (usize i = 0; i < limit + 1; i += 1) {
            lhs_fft[i] = lhs_fft[i] * rhs_fft[i];
        }

        Vec<Item> lhs_ifft = _fft(lhs_fft, rev, limit, -1);        
        mut Poly result;
        result.assign(lhs_ifft.size(), 0);
        for (usize i = 0; i < lhs.size() + rhs.size() - 1; i += 1) {
            result[i] = lhs_ifft[i].real() / limit + 0.5;
        }

        return result;
    }

};

template<typename T>
const f64 FFT<T>::PI = acos(-1);

template<usize length = 8>
class ioo {

    static_assert(length > 0, "length must be greater than 0");
    static_assert(length < 9, "length must be less than 64");

    using Item = u64;
    using Self = ioo;    
    
    static const usize     ITEM_LENGTH = length;    
    static const Vec<Item> ITEM_BASES;
    static const Item      ITEM_MAX;



    Vec<Item> buf;
    bool      sign;

    fn from_string(String& src) {
        mut usize len = src.size();

        assert(len != 0);
        assert(src[0] == '-' || ('0' <= src[0] && src[0] <= '9'));

        mut usize start = 0;
        if (src[0] == '-') {
            self->sign = true;
            start = 1;
        } else if ('0' <= src[0] && src[0] <= '9') {
            self->sign = false;
        }

        bool marked = false;
        for (mut usize i = start; i < len; i += 1) {
            assert(('0' <= src[i] && src[i] <= '9'));

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






    fn _eq(Self& other) -> bool {
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

    fn _ne(Self& other) -> bool {
        return !self->_eq(other);
    }

    fn _lt(Self& other) -> bool {
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

    fn _le(Self& other) -> bool {
        return *self == other || *self < other;
    }

    fn _gt(Self& other) -> bool {
        return !self->_le(other);
    }

    fn _ge(Self& other) -> bool {
        return !self->_lt(other);
    }

    fn _neg() -> Self {
        mut Self result = *self;
        result.sign = !result.sign;
        return result;
    }

    fn _not() -> bool {
        return *self == 0;
    }

    fn _add(Self& other) -> Self {
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

    fn _sub(Self& other) -> Self {
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

    fn _mul(Self& other) -> Self {
        mut Self result = Self();
        result.sign = self->sign ^ other.sign;
        result.buf.pop_back();
        result.buf.assign(self->buf.size() + other.buf.size(), 0);
        for (mut usize i = 0; i < self->buf.size(); i += 1) {
            for (mut usize j = 0; j < other.buf.size(); j += 1) {                
                u64 product = (u64)self->buf[i] * (u64)other.buf[j];
                result.buf[i + j] += product % ITEM_MAX;                       
                result.buf[i + j + 1] += product / ITEM_MAX;
                for (mut usize k = i + j; k < result.buf.size() - 1; k += 1) {
                    if (result.buf[k] >= ITEM_MAX) {
                        result.buf[k + 1] += result.buf[k] / ITEM_MAX;
                        result.buf[k] %= ITEM_MAX;
                    } else {
                        break;
                    }
                }
            }
        }
        result.trim();
        return result;
    }

    fn _div(Self& other) -> Tuple(Self, Self) {

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
        mut String result = "";
        if (self->sign) {
            result += "-";
        }
        for (mut usize i = 0; i < self->buf.size(); i += 1) {
            mut String item = std::to_string(self->buf[self->buf.size() - 1 - i]);
            if (i != 0) {
                item = String(ITEM_LENGTH - item.size(), '0') + item;
            }
            result += item;
        }
        return result;
    }

    pub fn to_i64() -> i64 {
        assert(*self <= LLONG_MAX && *self >= LLONG_MIN);

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

    pub fn qmul(Self& other) -> Self {
        // ![Unstable] This may cause overflow when the number is too big.

        mut Self result = Self();
        result.sign = self->sign ^ other.sign;
        result.buf.pop_back();
        result.buf = FFT<Item>::mul(self->buf, other.buf);
        mut u64 carry = 0;
        for (mut usize i = 0; i < result.buf.size(); i += 1) {
            result.buf[i] += carry;
            carry = result.buf[i] / ITEM_MAX;
            result.buf[i] %= ITEM_MAX;
        }
        while (carry != 0) {
            result.buf.push_back(carry % ITEM_MAX);
            carry /= ITEM_MAX;
        }
        result.trim();
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
        return self->_eq(other);
    }

    fn operator==(Self&& other) -> bool {
        return *self == other;
    }

    fn operator==(mut i64 other) -> bool {
        return *self == ioo(other);
    }

    fn operator!=(Self& other) -> bool {
        return self->_ne(other);
    }

    fn operator!=(Self&& other) -> bool {
        return *self != other;
    }

    fn operator!=(mut i64 other) -> bool {
        return *self != ioo(other);
    }

    fn operator<(Self& other) -> bool {
        return self->_lt(other);
    }

    fn operator<(Self&& other) -> bool {
        return *self < other;
    }

    fn operator<(mut i64 other) -> bool {
        return *self < ioo(other);
    }

    fn operator<=(Self& other) -> bool {
        return self->_le(other);
    }

    fn operator<=(Self&& other) -> bool {
        return *self <= other;
    }

    fn operator<=(mut i64 other) -> bool {
        return *self <= ioo(other);
    }

    fn operator>(Self& other) -> bool {
        return self->_gt(other);
    }

    fn operator>(Self&& other) -> bool {
        return *self > other;
    }

    fn operator>(mut i64 other) -> bool {
        return *self > ioo(other);
    }

    fn operator>=(Self& other) -> bool {
        return self->_ge(other);
    }

    fn operator>=(Self&& other) -> bool {
        return *self >= other;
    }

    fn operator>=(mut i64 other) -> bool {
        return *self >= ioo(other);
    }







    fn operator-() -> Self {
        return self->_neg();
    }

    fn operator!() -> bool {
        return self->_not();
    }






    fn operator+(Self& other) -> Self {
        return self->_add(other);
    }

    fn operator+(Self&& other) -> Self {
        return *self + other;
    }

    fn operator+(mut i64 other) -> Self {
        return *self + ioo(other);
    }

    fn operator+=(Self& other) -> Self {
        *self = *self + other;
        return *self;
    }

    fn operator+=(Self&& other) -> Self {
        *self += other;
        return *self;
    }

    fn operator+=(mut i64 other) -> Self {
        *self += ioo(other);
        return *self;
    }

    fn operator-(Self& other) -> Self {
        return self->_sub(other);
    }

    fn operator-(Self&& other) -> Self {
        return *self - other;
    }

    fn operator-(mut i64 other) -> Self {
        return *self - ioo(other);
    }

    fn operator-=(Self& other) -> Self {
        *self = *self - other;
        return *self;
    }

    fn operator-=(Self&& other) -> Self {
        *self -= other;
        return *self;
    }

    fn operator-=(mut i64 other) -> Self {
        *self -= ioo(other);
        return *self;
    }

    fn operator*(Self& other) -> Self {
        return self->_mul(other);
    }





    friend fn operator<<(std::ostream& os, ioo &src) -> std::ostream& {
        os << src.to_string();
        return os;
    }

    friend fn operator<<(std::ostream& os, ioo &&src) -> std::ostream& {
        return os << src;
    }

    friend fn operator>>(std::istream& is, ioo &src) -> std::istream& {
        mut String buf = "";
        src.buf.clear();
        is >> buf;
        src = ioo(buf);
        return is;
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
const typename ioo<length>::Item ioo<length>::ITEM_MAX = ([]() {
    usize n = length;
    Item base = 10;
    Item result = 1;
    while (n > 0) {
        if (n & 1) {
            result *= base;
        }
        base *= base;
        n >>= 1;
    }
    return result;
})();


#define ioo_(x) ioo(#x)

#define ioo1 ioo<1>
#define ioo2 ioo<2>
#define ioo3 ioo<3>
#define ioo4 ioo<4>
#define ioo5 ioo<5>
#define ioo6 ioo<6>
#define ioo7 ioo<7>
#define ioo8 ioo<8>

#define ioo1_(x) ioo1(#x)
#define ioo2_(x) ioo2(#x)
#define ioo3_(x) ioo3(#x)
#define ioo4_(x) ioo4(#x)
#define ioo5_(x) ioo5(#x)
#define ioo6_(x) ioo6(#x)
#define ioo7_(x) ioo7(#x)
#define ioo8_(x) ioo8(#x)



int main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    // ioo1 x = ioo1_(1234512345123456);
    // ioo1 y = ioo1_(6543265432654321);
    ioo2 x, y;
    std::cin >> x >> y;
    std::cout << x.qmul(y) << std::endl;
}