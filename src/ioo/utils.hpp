#ifndef __UTILS_H_
#define __UTILS_H_

#include <complex>
#include <math.h>
#include <iostream>

#include "rustdef.h"

// !FIX: remake this fft.
struct FFT {

    static const f64 PI;
    using Item = std::complex<f64>;
    using Poly = Vec<u64>;

    static fn init_rev(usize n) -> Tuple(usize, Vec<usize>) {
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
        return tuple_(limit, rev);
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
        result.resize(limit, Item(0, 0));
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
        let [_, rev] = init_rev(src.size());
        return _fft(to_complex(src), rev, rev.size(), 1);
    }

    static fn ifft(Poly& src) -> Vec<Item> {
        let [_, rev] = init_rev(src.size());     
        return _fft(to_complex(src), rev, rev.size(), -1);
    }

    static fn mul(Poly& lhs, Poly& rhs) -> Poly {
        let [_, rev] = init_rev(lhs.size() + rhs.size());
        mut Vec<Item> lhs_fft = _fft(to_complex(lhs), rev, rev.size(), 1);
        mut Vec<Item> rhs_fft = _fft(to_complex(rhs), rev, rev.size(), 1);
        for (usize i = 0; i < rev.size(); i += 1) {
            lhs_fft[i] = lhs_fft[i] * rhs_fft[i];
        }

        Vec<Item> lhs_ifft = _fft(lhs_fft, rev, rev.size(), -1);        
        mut Poly result;
        result.assign(lhs_ifft.size(), 0);
        for (usize i = 0; i < rev.size(); i += 1) {
            result[i] = lhs_ifft[i].real() / rev.size() + 0.5;
        }

        return result;
    }

};

const f64 FFT::PI = acos(-1);

#endif