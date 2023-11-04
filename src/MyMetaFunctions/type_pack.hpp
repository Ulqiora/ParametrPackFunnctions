#pragma once
#include <cstddef>
#include <algorithm>
#include <array>
#include <type_traits>
#include "just_type.hpp"
// __cpl
template <class... Type>
struct type_pack{};

using empty_pack = type_pack<>;

template<class... Type>
constexpr size_t size(type_pack<Type...>){
    return sizeof...(Type);
}
// проверка на пустоту пака
template<class... Type>
constexpr bool empty(type_pack<Type...> tp){
    return size(tp) == 0;
}
// Получение первого параметра списка типов
template <class T, class... args>
constexpr My::just_type<T> head(type_pack<T,args...>){
    return {};
}
// получение оставшейся части
template <class T, class... args>
constexpr type_pack<args...> tail(type_pack<T,args...>){
    return {};
}
// операторы сравнения для параметр-паков
template<class... T1, class... T2>
constexpr bool operator==(type_pack<T1...>, type_pack<T2...>){return false;}

template<class... T1>
constexpr bool operator==(type_pack<T1...>, type_pack<T1...>){return true;}

template<class... T1, class... T2>
constexpr bool operator!=(type_pack<T1...>, type_pack<T2...>){return true;}

template<class... T1>
constexpr bool operator!=(type_pack<T1...>, type_pack<T1...>){return false;}

// операторы сравнения для простых типов

template<class T1, class T2>
constexpr bool operator==(My::just_type<T1>, My::just_type<T2>){return false;}

template<class T1>
constexpr bool operator==(My::just_type<T1>, My::just_type<T1>){return true;}

template<class T1, class T2>
constexpr bool operator!=(My::just_type<T1>, My::just_type<T2>){return true;}

template<class T1>
constexpr bool operator!=(My::just_type<T1>, My::just_type<T1>){return false;}

//Push and pop
//type-based
template<class T, class... Ts>
constexpr type_pack<T,Ts...> push_front(type_pack<Ts...>){return{};}
template<class T, class... Ts>
constexpr type_pack<Ts...,T> push_back(type_pack<Ts...>){return{};}
//value-based
template<class... Ts, class T>
constexpr type_pack<T,Ts...> push_front(type_pack<Ts...>,My::just_type<T>){return{};}
template<class... Ts, class T>
constexpr type_pack<Ts...,T> push_back(type_pack<Ts...>,My::just_type<T>){return{};}
//tests
static_assert(push_front<int>(type_pack<double,char>{}) == type_pack<int,double,char>{});
static_assert(push_front(type_pack<double,char>{},My::just_type<int>{}) == type_pack<int,double,char>{});

template<class... Ts, class T>
constexpr type_pack<Ts...> pop_back(type_pack<Ts...,T>){return{};}
template<class T, class... Ts>
constexpr type_pack<Ts...> pop_front(type_pack<T,Ts...>){return{};}

// type is founded
template<class T, class... Ts>
constexpr bool contains(type_pack<Ts...>){
    return ( ... || std::is_same_v<T,Ts>);
}
static_assert(contains<int>(type_pack<int,double,char>{}));

template<class T, class... Ts>
constexpr size_t find(type_pack<Ts...> ts){
    std::array bs = {std::is_same_v<Ts,T>...};
    return std::distance(std::find(bs.begin() ,bs.end(),true),bs.begin());
}

static_assert(find<int>(type_pack<int,double*,char>{}) == 0);

template< template<class...> class F, class... Ts>
constexpr size_t find_if(type_pack<Ts...> tp){
    bool bs[] = {F<Ts>::value...};
    return std::find(bs,bs+2, true)-bs;
}

static_assert(find_if<std::is_pointer>(type_pack<int,double*,char>{}) == 1);

//Metafunction wrapper for value-returning metafunctions
template<template<class...> class F>
struct value_fn{
    template<class... Ts>
    constexpr auto operator()(My::just_type<Ts>...){
        return F<Ts...>::value;
    }
};
template<template<class...> class F>
constexpr value_fn<F> value_fn_v;
//Metafunction wrapper for value-returning metafunctions
template<template<class...> class F>
struct type_fn{
    template<class... Ts>
    constexpr auto operator()(My::just_type<Ts>...){
        return My::just_type<typename F<Ts...>::type>{};
    }
};

template<template<class...> class F>
constexpr type_fn<F> type_fn_v;

template <class F, class... Ts>
constexpr std::size_t find_if(F f,type_pack<Ts...> tp){
    std::array bs={f(My::just_type<Ts>{})...};
    for(std::size_t i = 0;i<size(tp);++i){
        if constexpr (bs[i]) return i;
    }
    return size(tp);
}

template<class F,class... Ts>
constexpr bool all_of(F f, type_pack<Ts...> tp){
    return (... && f(My::just_type<Ts>{}));
}
template<class F,class... Ts>
constexpr bool any_of(F f, type_pack<Ts...> tp){
    return (... || f(My::just_type<Ts>{}));
}

template<class F,class... Ts>
constexpr bool none_of(F f, type_pack<Ts...> tp){
    return !any_of(f,tp);
}

template<template<class...> class F, class... Ts>
constexpr auto transform(type_pack<Ts...> tp){
    return type_pack<typename F<Ts>::type...>{};
}
// generate N elements of Type T
template<class... Ts>
constexpr type_pack<Ts...> generate_helper(Ts*...){return {};}
template<class T, size_t... Is>
constexpr auto generate_impl(std::index_sequence<Is...>){
    return generate_helper(((void)Is,(T*)0)...);
}
template<std::size_t I, class T>
constexpr auto generate(){
    return generate_impl<T>(std::make_index_sequence<I>{});
}

template<template<class ...> class F, class T>
constexpr auto filter_one(){
    if constexpr(F<T>::value){
        return type_pack<T>{};
    } else {
        return empty_pack{};
    }
}
template<class... Ts, class... Os>
constexpr auto operator+(type_pack<Ts...>,type_pack<Os...>){
    return type_pack<Ts...,Os...>{};
}
template<template<class...> class F, class... Ts>
constexpr auto filter(type_pack<Ts...>){
    return (empty_pack{} + ... + filter_one<F,Ts>());
}
