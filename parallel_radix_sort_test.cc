// Copyright 2010, Takuya Akiba
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Takuya Akiba nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Usage:
//   % g++ -O3 parallel_radix_sort_test.cc -lgtest -lgtest_main -fopenmp
//   % ./a.out

#include "parallel_radix_sort.h"
#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <gtest/gtest.h>

using testing::Types;

const int kMaxNumElems = 100000;
const int kMaxNumThreads = 32;
const int kNumTrials = 100;

namespace {
uint8_t Random8bit() {
  return rand() & 0xFF;
}

template<typename T> T Random() {
  T r(0);
  for (size_t i = 0; i < sizeof(T); ++i) {
    r |= static_cast<T>(Random8bit()) << (i * 8);
  }
  return r;
}

template<> float Random() {
  for (;;) {
    union {
      uint32_t u;
      float f;
    } uf;
    uf.u = Random<uint32_t>();
    if (isnanf(uf.f)) continue;
    return uf.f;
  }
}

template<> double Random() {
  for (;;) {
    union {
      uint64_t u;
      double f;
    } uf;
    uf.u = Random<uint64_t>();
    if (isnan(uf.f)) continue;
    return uf.f;
  }
}

template<typename T> void FillRandom(T *a, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    a[i] = Random<T>();
  }
}
}  // namespace

typedef Types<unsigned char, char, signed char,
              unsigned short, short,
              unsigned int, int,
              unsigned long, long,
              unsigned long long, long long,
              float, double> SortingTypes;

template<typename T>
class MapSortTest : public testing::Test {};
TYPED_TEST_CASE(MapSortTest, SortingTypes);

TYPED_TEST(MapSortTest, KeySort) {
  TypeParam *dat = new TypeParam[kMaxNumElems];
  TypeParam *ans = new TypeParam[kMaxNumElems];
  ASSERT_NE(reinterpret_cast<TypeParam*>(NULL), dat);
  ASSERT_NE(reinterpret_cast<TypeParam*>(NULL), ans);

  parallel_radix_sort::KeySort<TypeParam> key_sort;
  key_sort.Init(kMaxNumElems, kMaxNumThreads);

  for (int t = 0; t < kNumTrials; ++t) {
    int num_elems = 1 + rand() % kMaxNumElems;
    int num_threads = 1 + rand() % kMaxNumThreads;

    FillRandom(dat, num_elems);

    std::partial_sort_copy(dat, dat + num_elems, ans, ans + num_elems);
    TypeParam *res = key_sort.Sort(dat, num_elems, num_threads);

    for (int i = 0; i < num_elems; ++i) {
      // std::cout << ans[i] << " " << res[i] << std::endl;
      ASSERT_EQ(ans[i], res[i]);
    }
    // puts("");
  }
}

