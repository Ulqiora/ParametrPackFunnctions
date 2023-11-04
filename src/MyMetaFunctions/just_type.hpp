#pragma once
// #define __cplusplus 202002L
#include <type_traits>
#include "is_same.hpp"
namespace My{
template<class T>
struct just_type{
    using Type = T; 
};

constexpr auto type = just_type<int>();
using type_t = decltype(type)::Type;
static_assert(is_same<type_t,int>::value);
}