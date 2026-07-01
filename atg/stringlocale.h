#pragma once

#define assert_enum(T,N) static_assert(sizeof(T)/sizeof(T[0])==(N+1), "Invalid count " #T);
#define BSENUM(T) template<> const char* bsenum<T>::names[]
#define BSINFO(T) template<> const char* bsenum<T>::info[]
