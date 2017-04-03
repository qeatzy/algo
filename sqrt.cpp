// sqrt.cpp, sqrt partition, see also " exp.cpp
// sqrt partition achieve fast complexity, or better compromise, could be complicated.
// examples are
// Doing this, we can do a lot of things in. Usually we use them in the problems with modify and ask queries. Problems: Holes, DZY Loves Colors, RMQ (range minimum query) problem   http://codeforces.com/blog/entry/15729
// 1. fast prime test, first partion to number less than sqrt(n). TODO 
// 2. range query, use sqrt(n) bucket to achieve N^0.5 query and O(1) update.  http://www.geeksforgeeks.org/segment-tree-set-1-sum-of-given-range/
// 2.1 range minimum query, An <O(N), O(sqrt(N))> solution An interesting idea is to split the vector in sqrt(N) pieces.   It's easy to see that this algorithm doesn't make more than 3 * sqrt(N) operations per query.  The main advantages of this approach are that is to quick to code (a plus for TopCoder-style competitions) and that you can adapt it to the dynamic version of the problem (where you can change the elements of the array between queries).   https://www.topcoder.com/community/data-science/data-science-tutorials/range-minimum-query-and-lowest-common-ancestor/#Segment_Trees
