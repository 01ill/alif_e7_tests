#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <cstdint>
#include "arm_math.h"

void setupTests();
void stopTests();

template <typename Func, typename Result, typename A, typename B, typename C>
uint32_t benchmark(const Func f, const uint32_t iterations, Result result, A a, B b, C c, uint32_t len);

template <typename Func>
uint32_t benchmarkArm(const Func f, const uint32_t iterations, arm_status* status, arm_matrix_instance_f32 * a, arm_matrix_instance_f32 * b, arm_matrix_instance_f32 * c);


template <typename T>
bool compare(T resultA, T resultB, uint32_t size);


#endif // BENCHMARK_H