#include <random>
#include "utility.h"

std::vector<int> select_k_from_0_to_n(size_t n, size_t k) {
    assert(n >= k);
    auto v = range(n);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);
    return std::vector<int>(v.begin(), v.begin() + k);
}

namespace test {
    void select_k_from_0_to_n() {
        int n = 3e6, k = 20;
        auto res = ::select_k_from_0_to_n(n,k);
        cout << "n = " << n << ", k = " << k << endl;
        print(res);
    }
}

// 1. data, n * k, nested array, or a flat array with strides, eg, std::valarray.
// 2. centers, eg, mean of clusters, or median, medoid, etc.
// 2.1 seeding, set initial centers, eg, kmeans++.
// 3. stop condition, which value, threshold, max_iteration, absolute or relative difference.
// 4. which norm/distance should be used.
// 5. other concerns, eg, dithering, floating point number errors (loss of precision, numerical stability).
class Kmeans {
    public:
    private:
};

int main() {
    test::init();
    auto t = Timer();
    // test::readCSV();
    test::select_k_from_0_to_n();
    t.show();
}
