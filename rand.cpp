#include "utility.h"

// Select a random number from stream, with O(1) space.  http://www.geeksforgeeks.org/select-a-random-number-from-stream-with-o1-space/
// How does this work? We need to prove that every element is picked with 1/n probability where n is the number of items seen so far. For every new stream item x, we pick a random number from 0 to 'count -1', if the picked number is 'count-1', we replace the previous result with x.

// TODO Reservoir Sampling http://www.geeksforgeeks.org/reservoir-sampling/

// You may use Fisher-Yates shuffle and then avoid to construct the huge array a.  http://stackoverflow.com/a/25865255/3625404

// Select N random elements from a List<T> in C#
// A very good summary of select k from n.  http://stackoverflow.com/a/48114/3625404
// Fisher Yates shuffle  http://en.wikipedia.org/wiki/Fisher-Yates_shuffle   http://stackoverflow.com/a/8876593/3625404
// It is a lot harder than one would think. See the great Article "Shuffling" from Jeff.  http://stackoverflow.com/a/1042863/3625404

// mt19937, random_device, uniform_int_distribution,  http://stackoverflow.com/a/4196775/3625404
// a sample implementation of select_randomly,  http://stackoverflow.com/a/16421677/3625404
// https://gist.github.com/cbsmith/5538174
// E:\bks\ndal\cpp\rand  rand() Considered Harmful
// refer to Julienne's excellent article on using rand.  http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx   http://stackoverflow.com/a/4196060/3625404
// template<typename Iter, typename RandomGenerator>
// Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
//     std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
//     std::advance(start, dis(g));
//     return start;
// }
// template<typename Iter>
// Iter select_randomly(Iter start, Iter end) {
//     static std::random_device rd;
//     static std::mt19937 gen(rd());
//     return select_randomly(start, end, gen);
// }

int main() {
    test::init();
}
