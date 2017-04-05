#include "utility.h"
// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 109. Convert Sorted List to Binary Search Tree  https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree/
// links for O(n) bottom up solutions(similar to make heap). https://discuss.leetcode.com/topic/1245/   and   vivek_nalla https://discuss.leetcode.com/post/60569
    size_t length(ListNode* head) {
        size_t cnt = 0;
        for (; head; head = head->next) { ++cnt; }
        return cnt;
    }
    TreeNode* buildBST_bottom_up(ListNode* &head, size_t sz) { // damn smart to use reference of pointer here instead of global variable -- to feedback changed value of head.
        // 1. pass by reference to accumulate change to head. 2. construct tree by in-order traversal -- which is in sorted order, same as input.
        // 3. O(n) time complexity, similar to make-heap from bottom up.
        if (sz == 0) return nullptr;
        auto left = buildBST_bottom_up(head, sz / 2); // ceiling((sz-1)/2)
        TreeNode* root = new TreeNode(head->val);
        head = head->next;
        root->left = left;
        root->right = buildBST_bottom_up(head, (sz - 1) / 2); // floor((sz-1)/2)
        return root;
    }
    ListNode* midPointOfList(ListNode* head, ListNode* end) { // note *ceiling* mid instead of usual floor mide was returned.
        if (head == end) return head;
        for (auto fast = head->next; fast != end && fast->next != end;) {
            head = head->next;
            fast = fast->next->next;
        }
        return head;
    }
    TreeNode* buildBST_top_down(ListNode* head, ListNode* end) {  // O(n*log n) time complexity.
        if (head == end) return nullptr;
        auto mid = midPointOfList(head, end);
        auto left = buildBST_top_down(head, mid);
        auto root = new TreeNode(mid->val);
        root->left = left;
        root->right = buildBST_top_down(mid->next, end);
        return root;
    }
    TreeNode* sortedListToBST(ListNode* head) {
        // return buildBST_bottom_up(head, length(head));
        return buildBST_top_down(head, nullptr);
    }

// 108. Convert Sorted Array to Binary Search Tree  https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/
    template <typename RandomIt, typename T = typename std::iterator_traits<RandomIt>::value_type>
    TreeNode* sortedArrayToBST_bottom_up(RandomIt first, RandomIt last) {
        if (first == last) return nullptr;
        auto mid = first + std::distance(first, last) / 2;
        auto left = sortedArrayToBST_bottom_up(first, mid);
        auto root = new TreeNode(*mid);
        root->left = left;
        root->right = sortedArrayToBST_bottom_up(mid + 1, last);
        return root;
    }
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return sortedArrayToBST_bottom_up(nums.begin(), nums.end());
    }

int catalanNumber(int n) {
    long long res = 1;
    // if (n > 18) return (n == 19)? 1767263190 : INT_MAX;
    for (int i = 2; i <= n; ++i) { res = res * 2 * (2*i - 1) / (i + 1); }
    return res;
}
// 96. Unique Binary Search Trees    https://leetcode.com/problems/unique-binary-search-trees/?tab=Description
    int numTrees(int n) {
        return catalanNumber(n);
    }
// 95. Unique Binary Search Trees II    https://leetcode.com/problems/unique-binary-search-trees-ii/?tab=Description
// TODO
    vector<TreeNode*> generateTrees(int n) {
        vector<TreeNode*> res;
        return res;
    }
namespace test {
    void catalanNumber() {
        for (auto x: range(20)) {
            auto res = ::catalanNumber(x);
            cout << "catalanNumber(" << x << ") is\t" << res << endl;
        }
    }
}

// 226. Invert Binary Tree https://leetcode.com/problems/invert-binary-tree/#/description
// Invert a binary tree.
//      4
//    /   \
//   2     7
//  / \   / \
// 1   3 6   9
// to
//      4
//    /   \
//   7     2
//  / \   / \
// 9   6 3   1
// see also https://www.quora.com/What-is-the-algorithmic-approach-to-invert-a-given-binary-tree
// https://www.quora.com/Is-invert-a-binary-tree-a-good-question-for-Google-to-ask-in-a-technical-interview
// Google: 90% of our engineers use the software you wrote (Homebrew), but...  https://news.ycombinator.com/item?id=9695102
// is the problem clear? or my understanding unclear? flip left right?  -- equivalently -- to min-max a tree, ascending to descending?
    TreeNode* invertTree(TreeNode* root) {
        if (root) {
            std::swap(root->left, root->right);
            invertTree(root->left);
            invertTree(root->right);
        }
        return root;
    }

int main() {
    test::init();
    test::catalanNumber();
}
