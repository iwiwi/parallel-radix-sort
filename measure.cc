#include "parallel_radix_sort.h"
#include "measure.h"

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <algorithm>

const int DEFAULT_N = 200000000;
const int TRIAL = 3;

void InitRandom(int *a, int n) {
  for (int i = 0; i < n; ++i) {
    a[i] = rand();
  }
}

int main(int argc, char **argv) {
  if (argc >= 3) {
    fprintf(stderr, "usage: measure [size]\n");
    exit(EXIT_FAILURE);
  }

  int N = DEFAULT_N;
  if (argc == 2) N = 1 << atoi(argv[1]);
  printf("N = %d\n", N);

  int *buf;
  buf = new int[N];
  assert(buf);

  parallel_radix_sort::KeySort<int> key_sort;
  key_sort.Init(N);

  for (int t = 0; t < TRIAL; ++t) {
    InitRandom(buf, N);
    benchmark("parallel_radix_sort::KeySort(%d)", t) {
      key_sort.Sort(buf, N);
    }
  }

  for (int t = 0; t < TRIAL; ++t) {
    InitRandom(buf, N);
    benchmark("std::sort(%d)", t) {
      std::sort(buf, buf + N);
    }
  }

  exit(EXIT_SUCCESS);
}
