#ifndef __RUST_DEF_H_
#define __RUST_DEF_H_

#include <stdint.h>

#include <string>
#include <tuple>
#include <vector>

#define Vec std::vector
#define vec_(...) std::vector({__VA_ARGS__})

#define String std::string

#define Tuple(...) std::tuple<__VA_ARGS__>
#define tuple_(...) std::make_tuple(__VA_ARGS__)

#define Option std::optional
#define Some std::make_optional
#define None std::nullopt

#define move std::move
#define mut
#define let auto
#define ref &
#define fn auto
#define pub public:
#define priv private:
#define self this
#define unsafe

#define panic_(x) throw(x)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef size_t usize;
typedef ptrdiff_t isize;

#endif
