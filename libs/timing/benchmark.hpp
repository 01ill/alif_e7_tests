#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <cstdint>

void setupTests();
void stopTests();

template <typename Func, typename Result, typename A, typename B, typename C>
uint32_t benchmark(const Func f, const uint32_t iterations, Result result, A a, B b, C c, uint32_t len);

template <typename T>
bool compare(T resultA, T resultB, uint32_t size);


#endif // BENCHMARK_H