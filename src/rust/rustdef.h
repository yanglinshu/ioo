#ifndef __RUST_DEF_H_
#define __RUST_DEF_H_

#include <vector>
#include <string>
#include <tuple>

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

#endif
