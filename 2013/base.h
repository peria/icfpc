#ifndef BASE_H_
#define BASE_H_

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef long long int64;

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

#endif  // BASE_H_
