#include "parallel_radix_sort.h"

#include <cstdio>

int main() {
  // Sorting keys
  {
    int data[5] = {-1, 2, 0, -2, 1};

    parallel_radix_sort::SortKeys(data, 5);

    for (int i = 0; i < 5; ++i) printf("%d ", data[i]);
    puts("\n");
  }

  // Sorting pairs
  {
    double keys[5] = {-0.1, 0.2, 0.0, -0.2, 0.1};
    int vals[5] = {1, 2, 3, 4, 5};

    parallel_radix_sort::SortPairs(keys, vals, 5);

    for (int i = 0; i < 5; ++i) printf("%+.1f ", keys[i]);
    puts("");
    for (int i = 0; i < 5; ++i) printf("%4d ", vals[i]);
    puts("\n");
  }

  // When you perform sorting more than once, you can avoid
  // the cost of initialization using classes |KeySort| or |PairSort|
  {
    int data[5] = {-1, 2, 0, -2, 1};

    parallel_radix_sort::KeySort<int> key_sort;
    key_sort.Init(5);
    int *sorted = key_sort.Sort(data, 5);

    for (int i = 0; i < 5; ++i) printf("%d ", sorted[i]);
    puts("\n");
  }
  {
    double keys[5] = {-0.1, 0.2, 0.0, -0.2, 0.1};
    int vals[5] = {1, 2, 3, 4, 5};

    parallel_radix_sort::PairSort<double, int> pair_sort;
    pair_sort.Init(5);
    std::pair<double*, int*> sorted = pair_sort.Sort(keys, vals, 5);

    for (int i = 0; i < 5; ++i) printf("%+.1f ", sorted.first[i]);
    puts("");
    for (int i = 0; i < 5; ++i) printf("%4d ", sorted.second[i]);
    puts("\n");
  }

  // You can specify the number of threads.
  // (Otherwise default value given by OpenMP would be used.)
  {
    int data[5] = {-1, 2, 0, -2, 1};

    parallel_radix_sort::SortKeys(data, 5, 4);  // 4 thread

    for (int i = 0; i < 5; ++i) printf("%d ", data[i]);
    puts("\n");
  }
  {
    int data[5] = {-1, 2, 0, -2, 1};

    parallel_radix_sort::KeySort<int> key_sort;
    key_sort.Init(5, 4);
    int *sorted = key_sort.Sort(data, 5, 4);

    for (int i = 0; i < 5; ++i) printf("%d ", sorted[i]);
    puts("\n");
  }

  return 0;
}
