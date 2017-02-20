#include "utility.h"
#include <unordered_map>
#include <deque>
// Definition for singly-linked list.
template <typename T = int>
class ListNode_template {
    public:    T val;
               ListNode_template *next;
               ListNode_template(T x) { val = x; next = T(); }
};
using ListNode = ListNode_template<int>;

// 328. Odd Even Linked List  https://leetcode.com/problems/odd-even-linked-list/
    ListNode* oddEvenList(ListNode* head) {
        if (head == nullptr) return head;
        auto odd = head, even = head->next, even_head = even;
        for (; even && even->next;) {
            odd->next = odd->next->next;
            even->next = even->next->next;
            odd = odd->next;
            even = even->next;
        }
        odd->next = even_head;
        return head;
    }

// 133. Clone Graph  https://leetcode.com/problems/clone-graph/
// Definition for undirected graph.
struct UndirectedGraphNode {
    int label;
    vector<UndirectedGraphNode *> neighbors;
    UndirectedGraphNode(int x) : label(x) {};
};
class Solution {
private:
    std::unordered_map<UndirectedGraphNode*, UndirectedGraphNode*> hash;
public:
    UndirectedGraphNode *cloneGraph_bfs_defaultdict(UndirectedGraphNode *node) {
        if (node == nullptr) return nullptr;
        std::deque<UndirectedGraphNode*> deq {node};
        hash[node] = new UndirectedGraphNode(0);
        for (; !deq.empty();) {
            auto old = deq.front();
            deq.pop_front();
            hash[old]->label = old->label;
            for (auto nb: old->neighbors) {
                if (hash.find(nb) == hash.end()) {
                    deq.push_back(nb);
                    hash[nb] = new UndirectedGraphNode(0);
                }
                hash[old]->neighbors.push_back(hash[nb]);
            }
        }
        auto res = hash[node];
        hash.clear();
        return res;
    }
    UndirectedGraphNode *cloneGraph_dfs_recurse(UndirectedGraphNode *node) {  // memory leak?
        if (hash.find(node) == hash.end()) {
            auto copy = new UndirectedGraphNode(node->label);
            hash[node] = copy;
            for (auto nb: node->neighbors) {
                nb = cloneGraph_dfs_recurse(nb);
                copy->neighbors.push_back(nb);
            }
        }
        return hash[node];
    }
    UndirectedGraphNode *cloneGraph(UndirectedGraphNode *node) {
        // return cloneGraph_bfs_defaultdict(node);
        return node? cloneGraph_dfs_recurse(node) : nullptr;
    }
};

// 138. Copy List with Random Pointer  https://leetcode.com/problems/copy-list-with-random-pointer/
// put the nodes in the original linked list to keep track of the order, smart!  https://discuss.leetcode.com/post/53745
// https://discuss.leetcode.com/topic/7594/a-solution-with-constant-space-complexity-o-1-and-linear-time-complexity-o-n
// Definition for singly-linked list with a random pointer.
struct RandomListNode {
    int label;
    RandomListNode *next, *random;
    RandomListNode(int x) : label(x), next(NULL), random(NULL) {}
};
    RandomListNode *append(RandomListNode *tail, int val) {
        tail->next = new RandomListNode(val);
        return tail->next;
    }
    RandomListNode *copyRandomList_insert_fix_separate(RandomListNode *head) { // insert copy node just after original in input list, fix random, then separate.
    // PaladinHL  https://discuss.leetcode.com/post/53745
        if (head == nullptr) return nullptr;
        auto p = head;
        for (; p;) {
            auto old_next = p->next;
            p->next = new RandomListNode(p->label);
            p->next->random = nullptr;
            p->next->next = old_next;
            p = old_next;
        }
        p = head;
        for (; p;) {
            p->next->random = (p->random == nullptr)? nullptr : p->random->next;
            p = p->next->next;
        }
        p = head;
        auto res = head->next;
        for (auto copy = res; ;) {
            p->next = copy->next;
            if (copy->next == nullptr) break;
            p = p->next;
            copy->next = p->next;
            copy = copy->next;
        }
        return res;
    }
    RandomListNode *copyRandomList_mix_and_fix(RandomListNode *head) {
    // TODO https://discuss.leetcode.com/topic/5831/2-clean-c-algorithms-without-using-extra-array-hash-table-algorithms-are-explained-step-by-step
        if (head == nullptr) return nullptr;
        auto p = head;
        for (; p;) {
            auto copy = new RandomListNode(p->label);
            copy->next = p->random;
            p->random = copy;
            p = p->next;
        }
        auto res = head->random;
        p = head;
        for (; p;) {
            auto copy = p->random;
            copy->random = (copy->next == nullptr)? nullptr : copy->next->random;
            p = p->next;
        }
        p = head;
        for (auto copy = res; ;) {
            p->random = copy->next;
            if (p->next == nullptr) {
                copy->next = nullptr;
                break;
            }
            copy->next = p->next->random;
            copy = copy->next;
            p = p->next;
        }
        return res;;
    }
    std::unordered_map<RandomListNode*, RandomListNode*> hash; // class global hash.
    RandomListNode *copyRandomList_hash_recurse(RandomListNode *head) {
    // https://discuss.leetcode.com/post/13710
        if (head == nullptr) return nullptr;
        if (hash.find(head) != hash.end()) return hash.find(head)->second;
        RandomListNode *copy = new RandomListNode(head->label);
        hash[head] = copy;
        copy->next = copyRandomList_hash_recurse(head->next);
        copy->random = copyRandomList_hash_recurse(head->random);
        return copy;
    }
    RandomListNode *copyRandomList_hash(RandomListNode *head) {
        std::unordered_map<RandomListNode*, RandomListNode*> hash;
        // RandomListNode *dummy = new RandomListNode(-1);
        for (auto p = head; p;) {
            hash[p] = new RandomListNode(p->label);
            hash[p]->next = p->next;
            hash[p]->random = p->random;
            p = p->next;
        }
        for (auto p = head; p;) {
            auto copy = hash[p];
            copy->next = hash[copy->next];
            copy->random = hash[copy->random];
            p = p->next;
        }
        return hash[head];
    }
    RandomListNode *copyRandomList_hash_defaultdict(RandomListNode *head) {
    // second version in  https://discuss.leetcode.com/topic/9557/clear-and-short-python-o-2n-and-o-n-solution
    // TODO strange not accepted.
        if (head == nullptr) return nullptr;
        std::unordered_map<RandomListNode*, RandomListNode*> hash;
        hash[nullptr] = nullptr;
        auto p = head;
        for (; p;) {
            if (hash.find(p) == hash.end()) { hash[p] = new RandomListNode(0); }
            hash[p]->label = p->label;
            if (hash.find(p->next) == hash.end()) { hash[p->next] = new RandomListNode(0); }
            hash[p]->next = hash[p->next];
            if (hash.find(p->random) == hash.end()) { hash[p]->random = new RandomListNode(0); }
            hash[p]->random = hash[p->random];
            p = p->next;
        }
        return hash[head];
    }
// TODO? Nice and simple, just treating it as an interconnected graph and do a BFS.  https://discuss.leetcode.com/post/90770
    RandomListNode *copyRandomList(RandomListNode *head) {
        // return copyRandomList_hash_recurse(head);
        // return copyRandomList_hash(head);
        return copyRandomList_hash_defaultdict(head);
        return copyRandomList_mix_and_fix(head);
        return copyRandomList_insert_fix_separate(head);
    }

// "ls
    // template <typename T = int, typename ListNode = ListNode_template<T>>
    size_t length(ListNode* head, ListNode* tail = nullptr) {
        size_t cnt = 0;
        for (; head != tail; head = head->next) { ++cnt; }
        return cnt;
    }
    inline ListNode* moveForward(ListNode* p, int n) {
        if (p == nullptr) return p;
        if (n < 0) {
            for (; p->next; p = p->next) {}
        } else for (; n > 0; --n) { p = p->next; }
        return p;
    }
    ListNode* append(ListNode* head, int val) {
        assert(head && head->next == nullptr);
        head->next = new ListNode(val);
        return head->next;
    }
    ListNode* addToFront(ListNode* head, int val) {
        ListNode *res = new ListNode(val);
        res->next = head;
        return res;
    }
    void removeNext(ListNode* prev) {
        auto next = prev->next;
        prev->next = next->next;
        delete next;
    }
    bool compareList(ListNode* lhs, ListNode* rhs) {
        for (; lhs && rhs && lhs->val == rhs->val;) {
            lhs = lhs->next, rhs = rhs->next;
        }
        return lhs == rhs; // both equal to nullptr.
    }

    ListNode* makeList(ListNode* head) {
        if (head == nullptr) return nullptr;
        ListNode *res = new ListNode(head->val);
        head = head->next;
        for (auto p = res; head;) {
            p->next = new ListNode(head->val);
            p = p->next;
            head = head->next;
        }
        return res;
    }
template <typename T = int, typename ListNode = ListNode_template<T>>
ListNode* makeList(std::initializer_list<T> values) {
    auto first = values.begin(), last = values.end();
    // if (DEBUG) print(first, last,"makeList");
    if (first == last) return nullptr;
    auto head = new ListNode(*first);
    auto tail = head;
    for (; ++first != last;) {
        tail->next = new ListNode(*first);
        tail = tail->next;
    }
    // if (DEBUG) {
    //     cout << "after makeList:";
    //     for (auto p = head; p; p = p->next) { cout << p->val << ' '; } cout << endl;
    // }
    return head;
}
void printCycleList(ListNode* head, ListNode* last = nullptr, std::string description="", signed char sep=' ', std::string trailing_desc = "") {
    std::vector<int> values;
    std::set<ListNode*> s;
    for (;;) {
        if (head == last || s.find(head) != s.end()) { break; }
        s.insert(head);
        values.push_back(head->val);
        head = head->next;
    }
    if (head != last) {
        // cout << "cycle node: " << head->val << endl;
        char buf[100] = {};
        // trailing_desc += (trailing_desc.empty()? "has cycle" : ", has cycle");
        if (0 < snprintf(buf, sizeof(buf) / sizeof(buf[0]), "\tcycle node: %d\n", head->val))
            trailing_desc += buf;
    }
    print(values, description, sep, trailing_desc);
}
void print(ListNode* head, ListNode* last, std::string description="", signed char sep=' ', std::string trailing_desc = "") {
    printCycleList(head, last, description, sep, trailing_desc);
}
void print(ListNode* head, std::string description="", signed char sep=' ', std::string trailing_desc = "") {
    // vector<int> values;
    // for (; head;) {
    //     values.push_back(head->val);
    //     // cout << head->val << ' ';
    //     head = head->next;
    // }
    printCycleList(head, nullptr, description, sep, trailing_desc);
    // cout << endl;
}
ListNode* getOneCycleList() {
    // auto res = makeList({0,1,2,3,5,8});
    auto res = makeList({3,2,0,-4});
    auto last = ::moveForward(res, ::length(res) - 1);
    last->next = res->next;
    // last->next = res->next->next;
    return res;
}
template <typename T = int, typename ListNode = ListNode_template<T>>
    ListNode* reverse(ListNode* head, ListNode* tail = nullptr) {
        // if (head == tail || head->next == tail) return head;
        ListNode* rhead = tail;
        for (; head != tail;) {
            auto next = head->next;
            head->next = rhead;
            rhead = head;
            head = next;
        }
        return rhead;
    }
    ListNode* reverse(ListNode* head, size_t cnt) {
        assert(length(head) >= cnt);
        auto rhead = head;
        auto old_head = head;
        head = head->next;
        for (; cnt > 1; --cnt) { // no need to reverse when cnt == 1.
            auto next = head->next;
            head->next = rhead;
            rhead = head;
            head = next;
        }
        old_head->next = head;
        return rhead;
    }
    void reverse_n_plus_one_next(ListNode* prev, size_t cnt) { // extracted from reverseBetween_dummy_head.
        // add tail->next after prev, update tail->next. similar to reverse(), which successively add to front of head.
        assert(length(prev) > cnt + 1);
        auto tail = prev->next;
        for (; cnt > 0; --cnt) {
            auto move = tail->next;
            tail->next = move->next;
            move->next = prev->next;
            prev->next = move;
        }
    }

// 21. Merge Two Sorted Lists  https://leetcode.com/problems/merge-two-sorted-lists/
    ListNode* mergeTwoLists_copy(ListNode* l1, ListNode* l2) {
        assert(l1 != l2);
        ListNode prehead(-1);
        ListNode *tail = &prehead;
        for (; l1 && l2;) {
            if (l2->val < l1->val) {
                tail->next = new ListNode(l2->val);
                l2 = l2->next;
            } else {
                tail->next = new ListNode(l1->val);
                l1 = l1->next;
            }
            tail = tail->next;
        }
        tail->next = makeList(l1? l1 : l2);
        return prehead.next;
    }
    ListNode* mergeTwoLists_inplace(ListNode* l1, ListNode* l2) {
        assert(l1 != l2);
        ListNode prehead(-1);
        ListNode *tail = &prehead;
        for (; l1 && l2;) {
            if (l2->val < l1->val) {
                tail->next = l2;
                l2 = l2->next;
            } else {
                tail->next = l1;
                l1 = l1->next;
            }
            tail = tail->next;
        }
        tail->next = l1? l1 : l2;
        return prehead.next;
    }
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        return mergeTwoLists_inplace(l1, l2);
    }

// 148. Sort List  https://leetcode.com/problems/sort-list/
// http://stackoverflow.com/questions/1525117/whats-the-fastest-algorithm-for-sorting-a-linked-list/1525419#1525419
// http://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.html
// TODO quick sort for list. https://discuss.leetcode.com/topic/15029/56ms-c-solutions-using-quicksort-with-explanations
    ListNode* mergeAdjacentList(ListNode* preleft,ListNode* leftlast,  ListNode* rightlast) {
        // cout << preleft->val << ", " << leftlast->val << ", " << rightlast->val << endl;
        // auto a = preleft->next, b = leftlast->next, c = rightlast->next,  aa = preleft;
        // wait("");
        // print(a,b,"start merge",-1); print(b,c);
        assert(preleft && leftlast && rightlast);
        // assert(length(preleft, leftlast) >= length(leftlast, rightlast));
        // assert(isSorted(preleft, leftlast) && isSorted(leftlast, rightlast));
        if (preleft == leftlast || leftlast == rightlast) return rightlast;
        if (leftlast->val <= leftlast->next->val) return rightlast;
        if (preleft->next->val > rightlast->val) { // change '>' to '>=' will lose stability  // short-circuit if largest of right part less than smallest of left part
        // wait("trivial merge. return.");
            auto right = leftlast->next;
            leftlast->next = rightlast->next;
            rightlast->next = preleft->next;
            preleft->next = right;
            // print(aa->next, leftlast->next,"after trival merge");
            return leftlast;
        }
        auto end = rightlast->next;
        for (; preleft != leftlast && leftlast->next != end;) { // merge
        // wait("merge loop");
            auto left = preleft->next, right = leftlast->next;
            if (left->val > right->val) {
        // wait("left large");
                leftlast->next = right->next; // remove head of right part
                right->next = left;
                preleft = preleft->next = right; // add one to front and forward head of left part
            } else {
        // wait("right large");
                preleft = preleft->next;
            }
        }
            // print(aa->next, rightlast->next,"after normal merge");
            // print(aa->next, leftlast->next,"after normal merge");
            // print(aa->next, end,"after normal merge");
        return (rightlast->next == end)? rightlast : leftlast;
        // return leftlast;
    }
    ListNode* sortList(ListNode* head) {
        int K = 1;
        ListNode prehead(-1);
        prehead.next = head;
        for (;; K *= 2) {
            ListNode *prev = &prehead, *mid, *last;
            for(;;) {
                mid = prev;
                for (int i = 0; i < K && mid; ++i)  mid = mid->next; 
                if (mid == nullptr || mid->next == nullptr) break;
                last = mid;
                for (int i = 0; i < K && last->next; ++i)  last = last->next; 
                prev = mergeAdjacentList(prev, mid, last);
            }
            if (prev == &prehead) return prehead.next;
        }
    }

namespace test {
    void sortList() {
        // auto prehead = makeList({-1,1,100,2});
        // auto prehead = makeList({-1,2,1});
        // auto prehead = makeList({-1,3,2,1});
        // auto prehead = makeList({-1,29,81,-39,112});
        auto prehead = makeList({-1,-84,142,41,-17,-71,170,186,183,-21,-76,76,10,29,81,112,-39,-6,-43,58,41,111,33,69,97,-38,82,-44,-7,99,135,42,150,149,-21,-30,164,153,92,180,-61,99,-81,147,109,34,98,14,178,105,5,43,46,40,-37,23,16,123,-53,34,192,-73,94,39,96,115,88,-31,-96,106,131,64,189,-91,-34,-56,-22,105,104,22,-31,-43,90,96,65,-85,184,85,90,118,152,-31,161,22,104,-85,160,120,-31,144,115});
        auto li = prehead->next;
        print(li," pre sort");
        li = ::sortList(li);
        // auto mid = moveForward(prehead, 2);
        // auto last = moveForward(li, length(li) - 1);
        // ::mergeAdjacentList(prehead, mid, last);
        // last->next = prehead;
        print(li,"post sort");
    }
}

// 23. Merge k Sorted Lists  https://leetcode.com/problems/merge-k-sorted-lists/
// TODO priority queue version. use heap utility.
    ListNode* mergeKLists_merge(vector<ListNode*>& lists) {
        auto first = lists.begin(), last = lists.end(), dest = last;
        if (first == dest) return nullptr;
        for (;;) {
            if (first + 1 == dest) return *first;
            last = dest, dest = first;
            auto p = first;
            for (; p + 1 < last; p += 2) {
                *dest = mergeTwoLists_inplace(p[0], p[1]);
                ++dest;
            }
            if (p != last) { *dest++ = *p; }
        }
    }
    ListNode* mergeKLists_concate_then_sort(vector<ListNode*>& lists) {
        ListNode prehead(-1);
        auto tail = &prehead;
        for (auto li: lists) {
            if (li) {
                tail->next = li;
                for (; tail->next; tail = tail->next) {}
            }
        }
        return sortList(prehead.next);
    }
    ListNode* mergeKLists_simple_selection(vector<ListNode*>& lists) {
        auto first = lists.begin(), last = lists.end(), min = last;
        for (auto p = first; p != last;) {
            if (*p == nullptr) {
                std::iter_swap(p, --last);
            } else {
                if (min >= last || (*p)->val < (*min)->val) { min = p; }
                ++p;
            }
        }
        if (first == last) return nullptr;
        // cout << "min - first = " << min - first << endl;
        if (first != min) {
            // wait("swap min");
            auto tmp = *min;
            *min = (*min)->next;
            tmp->next = *first;
            *first = tmp;
                if (*min == nullptr) {
                    // wait("min branch exhausted");
                    std::iter_swap(min, --last);
                }
        }
        auto head = *first, tail = head;
        // cout << "begin merge: K = " << last - first << endl;
        // cout << "head = " << head->val << endl;
        std::reverse(first + 1, last);
        // for (auto p = first; p != last; ++p) { print(*p); }
        // cout << "start" << endl;
        for (; first + 1 != last;) {
            if (tail->next == nullptr) {
                --last;
                tail->next = *last;
                continue;
            }
            auto curr = tail->next;
            auto p = last - 1, min = p;
            for (; --p != first;) {
                if ((*p)->val < (*min)->val) { min = p; }
            }
            // cout << "curr->val = " << curr->val << ", (*min)->val = " << (*min)->val << endl;
            if (curr->val >= (*min)->val) {
                auto tmp = *min;
                *min = (*min)->next;
                tail->next = tmp;
                tmp->next = curr;
                if (*min == nullptr) {
                    // wait("min branch exhausted");
                    std::iter_swap(min, --last);
                }
                tail = tmp;
            } else {
                tail = curr;
            }
        }
        return head;
    }
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        return mergeKLists_merge(lists);
        return mergeKLists_concate_then_sort(lists);
        return mergeKLists_simple_selection(lists);
    }

namespace test {
    void mergeKLists() {
        vector<ListNode*> lists;
        // lists.push_back(makeList({1}));
        // lists.push_back(makeList({0}));
        //
        // lists.push_back(makeList({1,2,2}));
        // lists.push_back(makeList({1,1,2}));
        //
        // lists.push_back(makeList({}));
        // lists.push_back(makeList({-1,5,11}));
        // lists.push_back(makeList({}));
        // lists.push_back(makeList({6,10}));
        //
        lists.push_back(makeList({-1,1}));
        lists.push_back(makeList({-3,1,4}));
        lists.push_back(makeList({-2,-1,0,2}));
        auto li = ::mergeKLists(lists);
        print(li);
    }
}

// 147. Insertion Sort List  https://leetcode.com/problems/insertion-sort-list/
    ListNode* insertionSortList(ListNode* head) {
        if (head == nullptr) return head;
        ListNode prehead(-1);
        prehead.next = head;
        for (auto last = head; last->next;) {
            auto curr = last->next;
            auto val = curr->val;
            if (last->val <= val) {
                last = curr;
                continue;
            }
            auto p = &prehead;
            for (; p->next->val <= val; p = p->next) {}
            last->next = curr->next;
            curr->next = p->next;
            p->next = curr;
        }
        return prehead.next;
    }

namespace test {
    void insertionSortList() {
        auto li = makeList({1,1});
        print(li);
        li = ::insertionSortList(li);
        printCycleList(li);
    }
}

// 25. Reverse Nodes in k-Group  https://leetcode.com/problems/reverse-nodes-in-k-group/
// Given a linked list, reverse the nodes of a linked list k at a time and return its modified list. 
// k is a positive integer and is less than or equal to the length of the linked list.
// If the number of nodes is not a multiple of k then left-out nodes in the end should remain as it is.
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode prehead(-1);
        prehead.next = head;
        ListNode *prev = &prehead, *tail;
        for (;;) {
            head = prev->next, tail = head;
            int i = 0;
            for (; i < k && tail; ++i) { tail = tail->next; }
            if (i < k) break;
            prev->next = reverse(head, tail);
            prev = head;
        }
        return prehead.next;
    }

// 143. Reorder List  https://leetcode.com/problems/reorder-list/
// TODO fix below accumulate recursive solution.
    void reorderList_accumulate_recurse(ListNode* &head, ListNode* tail) {
        assert(head);
        if (tail == nullptr) {
            auto tmp = head->next;
            head->next = nullptr;
            head = tmp;
        } else if (tail->next == nullptr) {
            head = head->next;
            auto tmp = head->next;
            head->next = nullptr;
            head = tmp;
        } else {
            auto curr = head;
            head = head->next;
            reorderList_accumulate_recurse(head, tail->next->next);
            auto tmp = head->next;
            head->next = curr->next;
            curr->next = head;
            head = tmp;
        }
    }
    void reorderList_reverse(ListNode* head) {
        if (head == nullptr) return;
        auto mid = head, fast = head->next;
        for (; fast && fast->next;) {
            mid = mid->next;
            fast = fast->next->next;
        }
        auto right = reverse(mid->next);
        mid->next = nullptr;
        for (; right;) {
            auto next = head->next;
            head->next = right;
            right = right->next;
            head->next->next = next;
            head = next;
        }
    }
    void reorderList(ListNode* head) {
        if (head == nullptr) return;
        // reorderList_accumulate_recurse(head, head->next);
        reorderList_reverse(head);
    }

namespace test {
    void reorderList() {
        // auto li = makeList({1,2,3,4,5});
        auto li = makeList({1,2,3,4});
        print(li);
        ::reorderList(li);
        // print(li);
        printCycleList(li);
    }
}

// 61. Rotate List  https://leetcode.com/problems/rotate-list/
    ListNode* rotateRight(ListNode* head, int k) { // rotate the list to the right by k places, where k is non-negative.
        int len = length(head);
        if (len > 1 && (k %= len) != 0) {
            auto new_tail = moveForward(head, len - k - 1);
            auto tail = moveForward(new_tail, -1);
            tail->next = head;
            head = new_tail->next;
            new_tail->next = nullptr;
        }
        return head;
    }

// 24. Swap Nodes in Pairs  https://leetcode.com/problems/swap-nodes-in-pairs/
    ListNode* swapPairs(ListNode* head) {
        ListNode prehead = ListNode(-1);
        prehead.next = head;
        auto prev = &prehead;
        for (; prev->next && prev->next->next;) {
            auto n = prev->next, nn = n->next;
            prev->next = nn;
            n->next = nn->next;
            nn->next = n;
            prev = n;
        }
        return prehead.next;
    }

// 19. Remove Nth Node From End of List  https://leetcode.com/problems/remove-nth-node-from-end-of-list/
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if (head == nullptr) return nullptr;
        auto len = length(head);
        if (size_t(n) < len) {
            auto p = moveForward(head, len - n - 1);
            p->next = p->next->next;    // memory leak?
        } else {
            // auto p = head->next;
            head = head->next;
            // delete p; // what if not created by new?
        }
        return head;
    }

// 142. Linked List Cycle II  https://leetcode.com/problems/linked-list-cycle-ii/
// Given a linked list, return the node where the cycle begins. If there is no cycle, return null. 
// Note: Do not modify the linked list.
    ListNode *detectCycle(ListNode *head) {
        // let length(head) == n, n == h + C, C is cycle length. d is distance between cycle point to where two pointer meet.
        // 2 * (h + d) == (h + d) + C  -- fast walk one more cycle than slow, slow walk no more than one cycle.
        // h + d == C
        // h = C - d
        // so reset fast to head, and let fast and slow move one step, then they will meet at cycle point.
        if (head && head->next) {
            auto slow = head, fast = head;
            int m = 0;
            for (; fast && fast->next;) {
                ++m;
                slow = slow->next;
                fast = fast->next->next;
                if (slow == fast) break;
            }
            if (slow == fast) {
                fast = head;
                while (slow != fast) {
                    slow = slow->next;
                    fast = fast->next;
                }
                return slow;
            }
        }
        return nullptr;
    }

namespace test {
    void detectCycle() {
        auto li = getOneCycleList();
        // print(li);
        printCycleList(li);
        cout << ::detectCycle(li)->val << endl;
    }
}

// 141. Linked List Cycle  https://leetcode.com/problems/linked-list-cycle/
// ways to detect cycle in linked list. 1. floyd cycle detection, 2. use external mapping, eg hash map, 3. set internal flag on original input.
    bool hasCycle_floyd(ListNode *head) { // floyd cycle detection, relative distance incremented, slow catch up fast eventually if cycle exist.
        auto fast = head;
        for (; fast && fast->next;) {
            head = head->next;
            fast = fast->next->next;
            if (head == fast) return true;
        }
        return false;
    }
    bool hasCycle_by_reverse(ListNode *head) {
    // how does this work? Just reverse the list. https://discuss.leetcode.com/topic/20604/just-reverse-the-list/2
    // if has cycle, then last will point to some node previous, name it as k.
    // when reversing it, at some time, rhead == last, p == k, then you going to reverse
    // [k, head), which you have just reversed, thus they will be reversed back, and the
    // final return value rhead == head;
        if (head && head->next) {
            auto rhead = reverse(head);
            reverse(head);
            return rhead == head;
        }
        return false;
    }
    bool hasCycle(ListNode *head) {
        return hasCycle_by_reverse(head);
        return hasCycle_floyd(head);
    }
    // bool hasCycle_except(ListNode *head) {
    // Except-ionally fast Python  https://discuss.leetcode.com/topic/16098/except-ionally-fast-python/2

// 86. Partition List  https://leetcode.com/problems/partition-list/
    ListNode* partition(ListNode* head, int x) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode left_head(-1), right_head(-1);
        auto left = &left_head, right = &right_head;
        for (;; ) {
            if (head->val < x) {
                left->next = head;
                left = left->next;
            } else {
                right->next = head;
                right = right->next;
            }
            if (head->next == nullptr) break;
            head = head->next;
        }
        right->next = nullptr;
        left->next = right_head.next;
        return left_head.next;
    }

namespace test {
    void partition() {
        auto li = makeList({2,1});
        print(li);
        li = partition(li, 1);
        print(li);
        printCycleList(li);
    }
}
// 82. Remove Duplicates from Sorted List II  https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
// Given a sorted linked list, delete all nodes that have duplicate numbers, leaving only distinct numbers from the original list. 
    ListNode* deleteDuplicates(ListNode* head) {
        if (head == nullptr) return head;
        ListNode prehead = ListNode(head->val - 1);
        prehead.next = head;
        auto prev = &prehead;
        for (; prev && prev->next && prev->next->next;) {
            auto curr = prev->next;
            auto p = curr->next;
            if (p->val != curr->val) { prev = curr; }
            else {
                for (; p && p->val == curr->val; p = p->next) {}
                prev->next = p;
            }
        }
        return prehead.next;
    }

// // 83. Remove Duplicates from Sorted List  https://leetcode.com/problems/remove-duplicates-from-sorted-list/
//     ListNode* deleteDuplicates_two_loop(ListNode* head) {
//         for (auto curr = head; curr;) {
//             auto p = curr->next;
//             for (; p && p->val == curr->val;) {
//                 delete p;
//                 p = p->next;
//             }
//             curr->next = p;
//             curr = curr->next;
//         }
//         return head;
//     }
//     ListNode* deleteDuplicates_one_loop(ListNode* head) {
//         for (auto curr = head; curr && curr->next;) {
//             if (curr->next->val == curr->val) {
//                 curr->next = curr->next->next;
//             } else {
//                 curr = curr->next;
//             }
//         }
//         return head;
//     }
//     ListNode* deleteDuplicates(ListNode* head) {
//         return deleteDuplicates_one_loop(head);
//         return deleteDuplicates_two_loop(head);
//     }

// 92. Reverse Linked List II  https://leetcode.com/problems/reverse-linked-list-ii/
    ListNode* reverseBetween_no_dummy_head(ListNode* head, int m, int n) { //  1 ≤ m ≤ n ≤ length of list. 
        if (m == n) return head;
        n = n - m + 1;
        ListNode *before = head;
        for (int i = m; i > 2; --i) { before = before->next; }
        ListNode *pm;
        if (m == 1) {
            before = nullptr;
            pm = head;
        } else {
            pm = before->next;
        }
        ListNode *rhead = nullptr;
        ListNode *pn = pm;
        for (; n > 0; --n) {
            auto next = pn->next;
            pn->next = rhead;
            rhead = pn;
            // if (DEBUG) print(rhead,"rhead");
            pn = next;
        }
        if (rhead != nullptr) { // have reversed something. <==> m < n.
            pm->next = pn;
            if (m > 1) {
                before->next = rhead;
            } else {
                head = rhead;
            }
        }
        return head;
    }
    ListNode* reverseBetween_dummy_head(ListNode* head, int m, int n) { //  1 ≤ m ≤ n ≤ length of list. 
        ListNode prehead(-1);
        prehead.next = head;
        ListNode *prev = moveForward(&prehead, m - 1);
        reverse_n_plus_one_next(prev, n - m);
        return prehead.next;
    }
    ListNode* reverseBetween_two_pass(ListNode* head, int m, int n) { //  1 ≤ m ≤ n ≤ length of list. 
        ListNode prehead = ListNode(-1);
        prehead.next = head;
        auto prev = moveForward(&prehead, m - 1);
        auto last = moveForward(prev->next, n - m + 1);
        auto rhead = reverse(prev->next, last);
        prev->next = rhead;
        return prehead.next;
    }
    ListNode* reverseBetween_cnt_reverse(ListNode* head, int m, int n) { //  1 ≤ m ≤ n ≤ length of list. 
        ListNode *prev = nullptr, *first;
        if (m > 1) {
            prev = moveForward(head, m - 2);
            first = prev->next;
        } else {
            first = head;
        }
        auto rhead = reverse(first, n - m + 1);
        if (m > 1) {
            prev->next = rhead;
            rhead = head;
        }
        return rhead;
    }
    ListNode* reverseBetween(ListNode* head, int m, int n) { //  1 ≤ m ≤ n ≤ length of list. 
        // return reverseBetween_two_pass(head, m, n);
        return reverseBetween_cnt_reverse(head, m, n);
        return reverseBetween_dummy_head(head, m, n);
        return reverseBetween_no_dummy_head(head, m, n);
    }

namespace test {
    void reverseBetween() {
        DEBUG = 1;
        auto li = makeList({3,5});
        print(li);
        // auto res = ::reverseBetween(li, 1,2);
        auto res = ::reverseBetween(li, 2,2);
        print(res);
    }
}

// https://leetcode.com/problems/add-two-numbers/
    ListNode* addTwoNumbers_least_significant_first(ListNode* l1, ListNode* l2) {
        assert (l1 != nullptr && l2 != nullptr);
        const int base = 10;
        int carry = l1->val + l2->val;
        ListNode *head = new ListNode(carry % base);
        carry /= base;
        l1 = l1->next;
        l2 = l2->next;
        ListNode *tail = head;
        for (; l1 && l2;) {
            carry += l1->val + l2->val;
            tail->next = new ListNode(carry % base);
            carry /= base;
            tail = tail->next;
            l1 = l1->next;
            l2 = l2->next;
        }
        if (l1 == nullptr) { l1 = l2; }
        for (; l1;) {
            carry += l1->val;
            tail->next = new ListNode(carry % base);
            carry /= base;
            tail = tail->next;
            l1 = l1->next;
        }
        if (carry) { tail->next = new ListNode(carry); }
        return head;
    }

// 160. Intersection of Two Linked Lists  https://leetcode.com/problems/intersection-of-two-linked-lists/
    ListNode *getIntersectionNode_two_pointer(ListNode *headA, ListNode *headB) {
        // if (headA == nullptr || headB == nullptr) return nullptr;
        auto a = headA, b = headB;
        for (; a != b;) {
            a = (a == nullptr)? headB : a->next;
            b = (b == nullptr)? headA : b->next;
        }
        return a;
    }
    ListNode *getIntersectionNode_get_length_then_align(ListNode *headA, ListNode *headB) {
        int lenA = length(headA), lenB = length(headB);
        for (; lenA > lenB;) {
            --lenA;
            headA = headA->next;
        }
        for (; lenB > lenA;) {
            --lenB;
            headB = headB->next;
        }
        for (; headA != headB;){
            headA = headA->next;
            headB = headB->next;
        }
        return headA;
    }
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // return getIntersectionNode_two_pointer(headA, headB);
        return getIntersectionNode_get_length_then_align(headA, headB);
    }

// 203. Remove Linked List Elements  https://leetcode.com/problems/remove-linked-list-elements/
    ListNode* removeElements_use_dummy_head(ListNode* head, int val) {
        if (head) {
            auto dummy = new ListNode(-1);
            dummy->next = head;
            for (auto p = dummy; p->next;) {
                if (p->next->val == val) { removeNext(p); }
                else { p = p->next; }
            }
            head = dummy->next;
            delete dummy;
        }
        return head;
    }
    // use pointer of pointer instead of dummy. 
    // https://discuss.leetcode.com/topic/19529/simple-and-elegant-solution-in-c
    // http://wordaligned.org/articles/two-star-programming
    ListNode* removeElements_pointer_of_pointer(ListNode* head, int val) { // memory leak?
        ListNode** pp = &head;
        for (; *pp;) {
            auto curr = *pp;
            if (curr->val == val) {
                *pp = curr->next;
            } else {
                pp = &(curr->next);
            }
        }
        return head;
    }
    ListNode* removeElements(ListNode* head, int val) {
        return removeElements_use_dummy_head(head, val);
        return removeElements_pointer_of_pointer(head, val);
    }

// 237. Delete Node in a Linked List  https://leetcode.com/problems/delete-node-in-a-linked-list/
// **catch**. you can't delete node literally since previous node not given. -- pretend you have deleted it. by overwritten with next value and delete next node. note that you can't 'delete' tail node in such way.
    void deleteNode(ListNode* node) {
        // assert(node == nullptr || node->next == nullptr); // not the tail.
        if (node == nullptr || node->next == nullptr) return;
        auto next = node->next;
        node->val = next->val;
        node->next = next->next;
        // *node = *(node->next); // same effect for above given definition of ListNode in C++.
        delete next;
    }

// 234. Palindrome Linked List  https://leetcode.com/problems/palindrome-linked-list/
// to do it in O(n) time and O(1) extra space. reverse first half and compare with rest, then restore.
    bool isPalindrome_by_reverse(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return true;
        ListNode *rtail = nullptr, *mid = head, *fast = head;
        bool res;
        for (; ;) {
            if (fast == nullptr) {
                res = compareList(rtail, mid);
                break;
            }
            if (fast->next == nullptr) {
                res = compareList(rtail, mid->next);
                break;
            }
            fast = fast->next->next;
            auto next = mid->next;
            mid->next = rtail;
            rtail = mid;
            mid = next;
        }
        reverse(rtail);
        rtail->next = mid;
        return res;
    }
    bool isPalindrome_by_accumulate(ListNode* head, ListNode* &conjugate) { // accumulate and recursion. similar to buildBST_bottom_up(ListNode* &head, size_t sz) in tree.cpp
        // an implementation of same idea, by via global variable.  https://discuss.leetcode.com/topic/27605/my-easy-understand-c-solution/2
        if (head == nullptr) return true;
        bool res = isPalindrome_by_accumulate(head->next, conjugate) & (head->val == conjugate->val);
        conjugate = conjugate->next;
        return res;
    }
    bool isPalindrome(ListNode* head) {
        return isPalindrome_by_reverse(head);
        auto conjugate = head;
        return isPalindrome_by_accumulate(head, conjugate);
    }

// https://leetcode.com/problems/add-two-numbers-ii/
// 2358 + 123
    ListNode* addInMiddleOrBack(ListNode* head, int val) {
        ListNode *tmp = new ListNode(val);
        tmp->next = head->next;
        head->next = tmp;
        return tmp;
    }
    ListNode* addTwoNumbers_most_significant_first(ListNode* l1, ListNode* l2) {
        assert (l1 != nullptr && l2 != nullptr);
        const int base = 10;
        // l1 = reverse(l1), l2 = reverse(l2);
        // auto res = addTwoNumbers_least_significant_first(l1, l2);
        // l1 = reverse(l1), l2 = reverse(l2);
        // return reverse(res);
        
        int sz1 = length(l1), sz2 = length(l2);
        ListNode *head = nullptr;
        while (sz1 > 0 && sz2 > 0) { // reverse output instead. https://discuss.leetcode.com/topic/67586/
            int sum = 0;
            // **excellent** way to handle unequal size!
            if (sz1 >= sz2) { sum += l1->val; l1 = l1->next; --sz1; }
            if (sz1 < sz2) { sum += l2->val; l2 = l2->next; --sz2; }
            head = addToFront(head, sum);
        }
        int carry = 0;
        ListNode* rhead = nullptr;
        for (; head;) { // same logic as reverse.
            carry += head->val;
            head->val = carry % base;
            carry /= base;
            auto next = head->next;
            head->next = rhead;
            rhead = head;
            head = next;
        }
        if (carry) { rhead = addToFront(rhead, carry); }
        return rhead;
    }
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        // return addTwoNumbers_least_significant_first(l1, l2);
        return addTwoNumbers_most_significant_first(l1, l2);
    }

namespace test {
    void addTwoNumbers() {
        auto test_case = []() {
            std::vector<std::vector<ListNode*>> res;
            res.push_back({makeList({2,4,3}), makeList({5,6,4})});
            res.push_back({makeList({7,2,4,3}), makeList({5,6,4})});
            // std::vector<ListNode*> res;
            // res.push_back(makeList({2,4,3}));
            return res;
        };
        for (auto li_pair: test_case()) {
            auto li = addTwoNumbers(li_pair[0],  li_pair[1]);
            print(li_pair[0],"",-1,"+");
            print(li_pair[1],"",-1,"=");
            print(li);
            // print(li);
        }
    }
}

namespace test {
    using ::printCycleList;
    void List() {
        DEBUG = 1;
        auto test_case = []() {
            vector<ListNode*> res;
            res.push_back(::makeList({2,3,5,8}));
            res.push_back(::makeList({2,3,5}));
            res.push_back(::makeList({2,3}));
            res.push_back(::makeList({2}));
            return res;
        };
        for (auto li: test_case()) {
            print(li);
            // li = reverse(li);
            li = reverse(li,length(li));
            print(li);
            // li = reverse(li);
            li = reverse(li,length(li));
            print(li);
            cout << endl;
        }
    }
    void cycleList() {
        auto res = getOneCycleList();
        // print(res);
        printCycleList(res);
        res = reverse(res);
        printCycleList(res);
        res = reverse(res);
        printCycleList(res);
    }
}


// // Definition for singly-linked list.
// struct ListNode {
//     int val;
//     ListNode *next;
//     ListNode(int x) : val(x), next(NULL) {}
// };

int main() {
    test::init();
    // test::addTwoNumbers();
    // test::reverseBetween();
    // test::cycleList();
    // test::detectCycle();
    test::reorderList();
    // test::insertionSortList();
    // test::sortList();
    // test::mergeKLists();
    // test::partition();
    // test::List();
    // char buf[100];
    // auto head = makeList({3,4});
    // string s = snprintf(buf, sizeof(buf) / sizeof(buf[0]), "cycle node: %d\n", head->val)
}
