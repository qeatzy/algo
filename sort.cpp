#include "utility.h"
#include <typeinfo>

/*
 * comparator must satisfy strict weak ordering. 1. cmp(x,x) == false, 2. cmp(x,y) == !cmp(y,x), 3. cmp(x,y) and cmp(y,z) ==> cmp(x,z).
insertion sort + merge sort[friendly to expensive comparison] [both are stable]  -- timsort
quick sort + heap sort[in place]   [both are unstable]  -- introsort
sorting by exchange (bubble sort), in each pass repeatedly swapping the adjacent elements, stop if no swap was perform in some pass.
sorting by insertion (insertion sort)
Shellsort, https://en.wikipedia.org/wiki/Shellsort#Applications
Comb sort improves on bubble sort.  https://en.wikipedia.org/wiki/Comb_sort
memory.
@user672009, O(1) space means "in place," and quicksort is not quite in place because it requires O(lgn) extra space.  http://cs.stackexchange.com/questions/24446/why-does-introsort-use-heapsort-rather-than-mergesort
stability.  https://en.wikipedia.org/wiki/Sorting_algorithm#Comparison_of_algorithms   https://www.infopulse.com/blog/timsort-sorting-algorithm/
http://stackoverflow.com/questions/13498213/quicksort-alogorithm-stability
http://stackoverflow.com/questions/1278243/stability-of-quicksort-partitioning-approach
Shellsort is unstable.
Bubble sort is a stable sort algorithm, like insertion sort.
non-comparative sort.
Radix sort, https://en.wikipedia.org/wiki/Radix_sort
bucket sort
counting sort
parallel.
Bitonic mergesort, https://en.wikipedia.org/wiki/Bitonic_sorter
Batcher odd-even mergesort, https://en.wikipedia.org/wiki/Batcher_odd–even_mergesort
Sorting network, https://en.wikipedia.org/wiki/Sorting_network
performance.
Have you tried types with much more complex comparison? Mergesort tends to optimize the number of comparisons, so if you have benchmarked against a vector of primitive values quicksort would be almost unanimously faster (which optimizes the number of movements).    timsort was "the best" in Python, where we can loosely characterize the situation when sorting a python list as: 1) Element comparisons are expensive (python function call), 2) Element swaps/moves are very cheap (always pointer sized). Neither of (1) nor (2) need to be the case in Rust.   https://users.rust-lang.org/t/why-does-rust-use-mergesort-as-slice-sort-in-standard-library/6223/9
timsort was "the best" in Python, where we can loosely characterize the situation when sorting a python list as: 1) Element comparisons are expensive (python function call), 2) Element swaps/moves are very cheap (always pointer sized). Neither of (1) nor (2) need to be the case in Rust.
why is bubble sort so slow? even compared to insertion sort and selection sort. Experiments by Astrachan sorting strings in Java show bubble sort to be roughly one-fifth as fast as an insertion sort and 70% as fast as a selection sort.[2]  https://en.wikipedia.org/wiki/Bubble_sort#In_practice
https://en.wikipedia.org/wiki/Adaptive_sort
Smoothsort, Better asymptotic doesn't imply better performance. http://stackoverflow.com/questions/14001278/why-isnt-smoothsort-more-common
Timsort, https://www.infopulse.com/blog/timsort-sorting-algorithm/
Timsort, https://news.ycombinator.com/item?id=3214527
简单来说，Timsort是一种结合了归并排序和插入排序的混合算法，它基于一个简单的事实，实际中大部分数据都是部分有序 (升序或降序) 的。 Timsort排序算法中定义数组中的有序片段为run，每个run都要求单调递增或严格单调递减 (保证算法的稳定性) 。 http://www.freebuf.com/vuls/62129.html
运用广泛的还要加一个Timsort，这是Python御用排序。而且关于这个排序有个趣闻如何找出Timsort算法和玉兔月球车中的Bug？ https://www.zhihu.com/question/31387715/answer/52555631
居然没有人提著名的 煎饼排序 (Wikipedia: Pancake sorting), 每次只允许反转数组前面的 k 个元素，最少多少次能够使得数组有序。 比尔盖茨同学唯一的学术论文就是证明了该排序问题的边界 (论文原文：http://www.cs.berkeley.edu/~christos/papers/Bounds%20For%20Sorting%20By%20Prefix%20Reversal.pdf) ，这个问题在《编程之美》中也有讲到。 作者：高兴不起来 链接：https://www.zhihu.com/question/31387715/answer/62376666 来源：知乎 著作权归作者所有，转载请联系作者获得授权。
6分钟演示15种排序算法   http://www.bilibili.com/video/av685670/
Introsort, https://en.wikipedia.org/wiki/Introsort
insightful comparison of some non-comparison sort algorithm. http://functionspace.com/topic/3506/opinion/6539
insightful comparison of hybrid sort algorithm (eg, introsort/Timsort), below is some excerpt. http://functionspace.com/topic/3506/opinion/6409
Introsort gives the speed of quicksort while solving the O(n2) worst case by going to heapsort if the recursion level is too high. It's an elegant solution using two classic algorithms. Low memory, simple, in many cases faster than other methods, but not stable and will do more comparisons than merge sort. Note that a practical implementation may add insertion sort for small partitions and hence have three different algorithms.
Timsort uses merge sort and insertion sort. Both are stable and merge sort has worst case O(nlogn). It does particularly well on ordered or partially ordered data, which isn't uncommon. It does fewer comparisons than most sorts, which is a huge advantage in some cases (with a complicated comparison function, which isn't unusual in Python or Perl, for example). Like most merge-sort methods, it uses more temporary memory though there are ways to mitigate this at the expense of speed and/or complexity.
TODO  timsort(merge_sort)    inplace merge(eg, in std::stable_sort, as fallback.)
*/

auto stack_level = 0;
enum { S_threshold = 4 };

template <typename RandomIt>
inline void shift_left(RandomIt first, RandomIt next) {
    assert(first < next);
    auto val = std::move(*next);
    while (next != first) {
        --next;
        next[1] = next[0];
    }
    *first = val;
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
inline void unguarded_shift_left(RandomIt next, Compare comp) {
    auto val = std::move(*next);
    while (comp(val, *--next)) {
        next[1] = next[0];
    }
    next[1] = val;
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    assert(first < last);
    // insert sort. http://codereview.stackexchange.com/a/47189/81817
    auto next = first;
    while (++next < last) {
        if (comp(*next, *first)) {
            shift_left(first, next);
        } else {
            unguarded_shift_left(next, comp);
        }
    }
    // print(first, last, ' ', " post insert");
}

// The worst-case running time of Shellsort using Hibbard's increments is Theta(N3/2).
template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void shell_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    constexpr int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1}; // https://en.wikipedia.org/wiki/Shellsort#Gap_sequences
    auto p_gaps = gaps;
    auto gapped_shift_left = [](RandomIt first, RandomIt next, int gap) {
        assert(first < next && (next - first) % gap == 0);
        auto val = std::move(*next);
        while (first < next) {
            next -= gap;
            next[gap] = next[0];
        }
        *first = val;
    };
    auto gapped_unguarded_shift_left = [](RandomIt next, int gap, Compare comp) {
        auto val = std::move(*next);
        while (comp(val, *(next -= gap))) {
            next[gap] = next[0];
        }
        next[gap] = val;
    };
    auto gapped_insertion_sort = [gapped_shift_left, gapped_unguarded_shift_left](RandomIt first, RandomIt last, int gap, Compare comp) {
        auto next = first;
        while ((next += gap) < last) {
            if (comp(*next, *first)) {
                gapped_shift_left(first, next, gap);
            } else {
                gapped_unguarded_shift_left(next, gap, comp);
            }
        }
    };
    while (true) {
        auto step = *p_gaps++;
        if (step >= last - first) continue;
        for (auto p = first, p_end = p + step; p < p_end; ++p) {
            gapped_insertion_sort(p, last, step, comp);
        }
        if (step == 1) break;
    }
}

namespace test {
    auto test_case = []() {
        std::vector<std::vector<int>> res;
        res.push_back({13,14,16,19,21,19,68,65,26,32,31});
        res.push_back({81,94,11,96,12,35,17,95,28,58,41,75,15});
        res.push_back({81,94,11});
        res.push_back({13,19,9,5,12,8,7,4,11,2,6,21});
        res.push_back({13,21,16,24,31,19,68,65,26,32});
        res.push_back({2,2,2,1,1,1,1,3,3,5,5,5,4,4,});
        res.push_back({2,2,2,2,2,2,2,2,2,2,2,2});
        res.push_back({1,9,2,10,3,11,4,12,5,13,6,14,7,15,8,16});
        return res;
    };
    template <typename Container>
    bool is_sorted(const Container& v) {
        typedef typename std::iterator_traits<decltype(std::begin(v))>::value_type T;
        std::vector<T> original {v};
        std::vector<T> sorted {original};
        std::sort(sorted.begin(), sorted.end());
        return original == sorted;
    }
    template <typename Container,
             // typename RandomIt = typename std::remove_const<decltype(std::begin(Container()))>::type,  // const_iterator?
             typename RandomIt = typename std::remove_const<decltype(Container().begin())>::type,
             typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>,
             typename SortFunc = void(RandomIt, RandomIt, Compare) >
    void test_sort_func(SortFunc f, Container& v) {
        f(std::begin(v), std::end(v), Compare());
        std::string description = is_sorted(v)? "sorted" : "not sorted";
        print(v,' ', description);
    }
    template <typename Container = std::vector<int>,    // a workaround
             // typename RandomIt = typename std::remove_const<decltype(std::begin(Container()))>::type,  // const_iterator?
             typename RandomIt = typename std::remove_const<decltype(Container().begin())>::type,
             typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>,
             typename SortFunc = void(RandomIt, RandomIt, Compare) >
    void test_sort_func_for_all_test_case(SortFunc f, std::string description = "sort") {
        cout << "\n==== " << description << " ====\n";
        for (auto v: test_case()) {
            // auto lv = stack_level;
            test_sort_func(f, v);
            // cout << stack_level - lv << " call of " << description << endl;
        }
    }
    void shell_sort() {
        test_sort_func_for_all_test_case(::shell_sort, "shell_sort");
    }
}

template <typename RandomIt,
         typename DistanceType, 
         typename ValueType, 
         typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
inline void adjust_max_heap(RandomIt first, DistanceType hole, DistanceType length, ValueType value, Compare comp) {
    auto secondChild = 2 * (hole + 1);
    while (secondChild < length) {
        if (comp(first[secondChild], first[secondChild - 1]))
            --secondChild;
        if (comp(value, first[secondChild])) {
            // cout << "value = " << value << ", ";
            // cout << "hole = " << first[hole] << ", secondChild = " << first[secondChild] << endl;
            first[hole] = first[secondChild];
            hole = secondChild;
            secondChild = 2 * (hole + 1);
        } else {
            break;
        }
    }
    if (secondChild == length && comp(value, first[--secondChild])) {
        first[hole] = first[secondChild];
        hole = secondChild;
    }
    first[hole] = std::move(value);
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void make_max_heap(RandomIt first, RandomIt last, Compare comp) {
    auto sz = last - first;
    for (auto parent = sz / 2 - 1; parent >= 0; --parent) {
        adjust_max_heap(first, parent, sz,
                std::move(first[parent]), comp);
    }
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void pop_push_max_heap(RandomIt first, RandomIt last, RandomIt result, Compare comp) {
    auto value = std::move(*result);
    *result = std::move(first[0]);
    adjust_max_heap(first, 0, last - first, value, comp);
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void heap_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    // print(first, last, "before make heap");
    // make_max_heap(first, last, comp);
    make_heap(first, last, comp);
    // print(first, last, "after make heap");
    while (last - first > 1) {
        --last;
        pop_push_max_heap(first, last, last, comp);
    }
}

namespace test {
    void heap_sort() {
        test_sort_func_for_all_test_case(::heap_sort, "heap_sort");
    }
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
inline void move_median_of_three_to_first(RandomIt first, RandomIt last, Compare comp) {
    assert(last - first >= 2);
    auto mid = first + (last - first) / 2;
    --last;
    // cout << "pre median:" << " a = " << *first << ", b = " << *mid << ", c = " << *last << endl;
    if (comp(*first, *mid)) {
        if (comp(*first, *last)) {  // a is smallest
            if (comp(*mid, *last)) {
                std::iter_swap(first, mid);
            } else {
                std::iter_swap(first, last);
            }
        } else {
            // c <= a < b, do nothing
        }
    } else {
        if (comp(*first, *last)) {
            // b <= a < c, do nothing
        } else {  // a is largest
            if (comp(*mid, *last)) {
                std::iter_swap(first, last);
            } else {
                std::iter_swap(first, mid);
            }
        }
    }
    // cout << "after median pivoting:" << " a = " << *first << ", b = " << *mid << ", c = " << *last << endl;
    // cout << ((comp(*mid, *first) ^ comp(*last, *first))? "is median" : "not median") << endl;
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
RandomIt unguarded_partition_with_first_as_pivot(RandomIt first, RandomIt last, Compare comp) {
    // unguarded partition; Hoare-Partition. could be extended to 'Bentley-McIlroy 3-way partitioning'.
    // cout << " pre partition: "; print(first, last);
    auto left = first, right = last;
    while (true) {
        do ++left; while (comp(*left, *first));
        do --right; while (comp(*first, *right));
        if (left < right) { std::iter_swap(left, right); }
        else {
            std::iter_swap(first, right);
            // cout << "post partition: "; print(first, last);
            return right;  // right == left, or, right + 1 == left, when return.
        }
    }
}
template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
inline RandomIt unguarded_partition_pivot(RandomIt first, RandomIt last, Compare comp) {
    move_median_of_three_to_first(first, last, comp);
    return unguarded_partition_with_first_as_pivot(first, last, comp);
}

template <typename RandomIt, typename Size, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void heap_select(RandomIt first, RandomIt middle, RandomIt last, Compare comp) {
    make_max_heap(first, middle);
    for (auto p = middle; p < last; ++p) {
        if (comp(*p, *first)) {
            pop_push_max_heap(first, middle, p, comp);
        }
    }
}

template <typename RandomIt, typename Size, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void intro_select(RandomIt first, RandomIt nth, RandomIt last, Size depth_limit, Compare comp) {
    while (last - first > 3) {
        if (depth_limit == 0) {
            heap_select(first, nth + 1, last, comp);
            std::swap(first, nth);
            return;
        }
        --depth_limit;
        auto pivot = unguarded_partition_pivot(first, last, comp);
        if (pivot <= nth) {
            first = pivot;
        } else {
            last = pivot;
        }
    }
    insertion_sort(first, last, comp);
}

template <typename RandomIt, typename Size, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void intro_select(RandomIt first, RandomIt nth, RandomIt last, Compare comp) {
    intro_select(first, last,
            Size(log2(last - first)), comp);
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quick_insertion_sort_non_loop(RandomIt first, RandomIt last, Compare comp) {
    ++stack_level;
    if (last - first < 2) return;
    if (last - first < S_threshold) {
        insertion_sort(first, last, comp);
        return;
    }
    auto pivot = unguarded_partition_pivot(first, last, comp);
    quick_insertion_sort_non_loop(first, pivot, comp);
    quick_insertion_sort_non_loop(pivot + 1, last, comp);
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quick_insertion_sort_loop(RandomIt first, RandomIt last, Compare comp) {
    ++stack_level;
    while (first < last) {
        if (last - first < 2) return;
        if (last - first < S_threshold) {
            insertion_sort(first, last, comp);
            return;
        }
        auto pivot = unguarded_partition_pivot(first, last, comp);
        quick_insertion_sort_loop(first, pivot, comp);
        first = pivot + 1;
    }
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quick_insertion_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    quick_insertion_sort_loop(first, last, comp);
    // quick_insertion_sort_non_loop(first, last, comp);
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quick_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    ++stack_level;
    if (last - first < 2) return;
    move_median_of_three_to_first(first, last, comp);
    auto pivot = unguarded_partition_with_first_as_pivot(first, last, comp);
    quick_sort(first, pivot, comp);
    quick_sort(pivot + 1, last, comp);
}

namespace test {
    void quick_insertion_sort() {
        test_sort_func_for_all_test_case(::quick_insertion_sort, "quick_insertion_sort");
    }
    void quick_sort() {
        test_sort_func_for_all_test_case(::quick_sort, "quick_sort");
    }
}

template <typename RandomIt, typename BufferIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
inline void left_merge_with_buffer(RandomIt first, RandomIt mid, RandomIt last, BufferIt buf, Compare comp) {
    // cout << "   left half: "; print(first, mid);
    // cout << "  right half: "; print(mid, last);
    std::move(first, mid, buf);
    auto buf_end = buf + (mid - first);
    const bool right_half_exhausted = comp(last[-1], mid[-1]);
    while (true) {
        if (comp(*mid, *buf)) {
            *first = std::move(*mid);
            ++mid;
        } else {
            *first= std::move(*buf);
            ++buf;
        }
        // cout << "current value = " << *first << endl;
        ++first;
        if (right_half_exhausted) {
            if (mid == last) {
                std::move(buf, buf_end, first);
                break;
            }
        } else {
            if (first == mid)
                break;
        }
    }
    // cout << " after merge: "; print(first, last);
    // wait();
}

template <typename RandomIt, typename BufferIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
inline void right_merge_with_buffer(RandomIt first, RandomIt mid, RandomIt last, BufferIt buf, Compare comp) {
}

template <typename RandomIt, typename BufferIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
inline void merge_sort_with_buffer(RandomIt first, RandomIt last, BufferIt buf, Compare comp) {
    ++stack_level;
    if (last - first < 2) return;
    if (last - first < S_threshold) {
        insertion_sort(first, last, comp);
        return;
    }
    auto mid = first + (last - first) / 2;
    merge_sort_with_buffer(first, mid, buf, comp);
    merge_sort_with_buffer(mid, last, buf, comp);
    left_merge_with_buffer(first, mid, last, buf, comp);
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void merge_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    auto buf = new T[(last - first) / 2 + 1];
    merge_sort_with_buffer(first, last, buf, comp);
    delete[] buf;
}

namespace test {
    void merge_sort() {
        test_sort_func_for_all_test_case(::merge_sort, "merge_sort");
    }
}

int main() {
    test::quick_insertion_sort();
    test::quick_sort();
    test::heap_sort();
    test::shell_sort();
    test::merge_sort();
}
