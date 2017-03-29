#include "utility.h"
// BST, binary search tree, and similar tree related.

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

void printInorder(TreeNode *root) {
    // wait("print");
    if (root) {
        printInorder(root->left);
        cout << root->val << ' ';
        printInorder(root->right);
    }
}
void print(TreeNode *root, string description="") {
    if (!description.empty()) cout << description << ": ";
    printInorder(root);
    cout << endl;
}
TreeNode* parseTree(std::string s) {
    auto first = s.begin(), last = s.end();
    for (; first != last && !isdigit(*first) && *first != 'n' && *first != 'N'; ++first) {} // integer or null
    for (; last != first && !isdigit(last[-1]) && last[-1] != 'L' && last[-1] != 'l'; --last) {}
    auto tokens = split(string(first, last), ',');
    // wait("after split");
    int sz = tokens.size();
    cout << "sz = " << sz << endl;
    if (sz < 1 || (sz == 1 && (tokens.back().empty() || !isdigit(tokens.back()[0])))) return NULL;
    // wait("after exclude empty tree");
    TreeNode *root = new TreeNode(std::stoi(tokens[0]));
    std::queue<TreeNode*> que, swp;
    que.push(root);
    // wait("before loop");
    for (int i = 1; i < sz;) {
        int j = i, j_end = std::min(sz, j + 2 * int(que.size()));
        for (; j < j_end;) {
            auto p = que.front(); que.pop();
            if (isdigit(tokens[j][0])) {
                int x = std::stoi(tokens[j]);
                p->left = new TreeNode(x);
                que.push(p->left);
            }
            if (++j < j_end && isdigit(tokens[j][0])) {
                int x = std::stoi(tokens[j]);
                p->right = new TreeNode(x);
                que.push(p->right);
            }
            ++j;
        }
        i = j_end;
    }
    return root;
}
namespace test {
    using ::parseTree;
    void parseTree() {
        std::vector<std::string> vec;
        vec.push_back("[3,9,20,null,null,15,7],");
        vec.push_back("[1,null,3,2]");
        for (auto &s: vec) {
            auto root = ::parseTree(s);
            print(root);
        }
    }
}

// 530. Minimum Absolute Difference in BST https://leetcode.com/problems/minimum-absolute-difference-in-bst/?tab=Description
// Given a binary search tree with non-negative values, find the minimum absolute difference between values of any two nodes.
// Note: There are at least two nodes in this BST.
    int getMinimumDifference(TreeNode* root) {
        // cout << "root->val = " << root->val << endl;
        assert(root && (root->left || root->right));
        int prev = root->val, res = INT_MAX;
        if (!root->left) { // root is first of inorder traversal
            root = root->right;
        }
        std::function<void(TreeNode*)> inorder;
        inorder = [&res,&prev,&inorder](TreeNode* p) {
            if (p) {
                inorder(p->left);
        // cout << "val = " << p->val << ", prev = " << prev << endl;
                // res = std::min(res, p->val - prev);
                res = std::min(res, abs(p->val - prev));
                prev = p->val;
                inorder(p->right);
            }
        };
        inorder(root);
        return res;
    }
namespace test {
    void getMinimumDifference() {
        std::vector<std::string> vec;
        // vec.push_back("");
        // vec.push_back("[1,null,3,2]");
        vec.push_back("[5,4,7]");
        for (auto &s: vec) {
            auto root = parseTree(s);
            auto res = ::getMinimumDifference(root);
            print(root,"root");
            cout << "min abs diff = " << res << endl;
        }
    }
}

// 352. Data Stream as Disjoint Intervals  https://leetcode.com/problems/data-stream-as-disjoint-intervals/#/description
// Given a data stream input of non-negative integers a1, a2, ..., an, ..., summarize the numbers seen so far as a list of disjoint intervals.
// For example, suppose the integers from the data stream are 1, 3, 7, 2, 6, ..., then the summary will be:
// [1, 1]
// [1, 1], [3, 3]
// [1, 1], [3, 3], [7, 7]
// [1, 3], [7, 7]
// [1, 3], [6, 7]
// Follow up: What if there are lots of merges and the number of disjoint intervals are small compared to the data stream's size?
// 1. array log(n) search, O(n) insert, 2. list O(n) search, O(1) insert, 3. binary search tree  -- here n <= len(set(stream))
// TODO 

int main() {
    test::init();
    // test::findMaxLength();
    // test::parseTree();
    test::getMinimumDifference();
}

