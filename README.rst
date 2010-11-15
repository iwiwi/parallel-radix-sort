概要
----
OpenMP を用いて並列化した Radix Sort です．
また，参考文献の論文で提案されている高速化手法である Buffer based scheme を採用しています．

キーのみのソートと，キー・値のペアのソートができます．
キーとして，以下の型がとれます．

* 符号付き整数 (char, short, int, long, long long)
* 符号なし整数 (上のに unsigned がついたもの)
* 浮動小数点数 (float, double)

使い方
------
sample.cc や measure.cc を見ると大体分かると思います．

コンパイル時に -fopenmp を付けないと並列化されないので注意してください．

性能
----
measure.cc で 2 億要素の int 配列のソートの時間を測ります．

実行例::

  % g++ -O3 measure.cc -fopenmp
  % ./a.out
  N = 200000000
  parallel_radix_sort::KeySort(0): 1.159468 sec
  parallel_radix_sort::KeySort(1): 0.972533 sec
  parallel_radix_sort::KeySort(2): 1.013231 sec
  std::sort(0): 19.788240 sec
  std::sort(1): 19.786527 sec
  std::sort(2): 19.858960 sec

参考文献
--------
* Nadathur Satish, Changkyu Kim, Jatin Chhugani, Anthony D. Nguyen, Victor W. Lee, Daehyun Kim, and Pradeep Dubey. 2010. Fast sort on CPUs and GPUs: a case for bandwidth oblivious SIMD sort. In Proceedings of the 2010 international conference on Management of data (SIGMOD '10). ACM, New York, NY, USA, 351-362. DOI=10.1145/1807167.1807207 http://doi.acm.org/10.1145/1807167.1807207
