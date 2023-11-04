#pragma once
// #define __cplusplus 202002L
#include <type_traits>
namespace My{

template<class A, class B>
struct is_same : std::false_type{};

/*this is template specialization for equal Types*/
template<class A>
struct is_same<A,A> : std::true_type{};

/*tests for function*/
static_assert(is_same<int,int>::value);
static_assert(!is_same<int,double>::value);
}