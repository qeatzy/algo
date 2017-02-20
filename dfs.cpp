#include "utility.h"
#include <deque>
// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 110. Balanced Binary Tree  https://leetcode.com/problems/balanced-binary-tree/
// TODO iterative solution. 
// https://discuss.leetcode.com/topic/9346/a-iterative-postorder-traversal-java-solution
// https://discuss.leetcode.com/topic/31515/4ms-accepted-java-solution-using-bfs
// https://discuss.leetcode.com/topic/42953/very-simple-python-solutions-iterative-and-recursive-both-beat-90
    bool isBalanced_dfs_stack(TreeNode* root) {
    }
    int balancedDepth(TreeNode* root) {
        if (!root) return 0;
        int ld = balancedDepth(root->left);
        if (ld >= 0) {
            int rd = balancedDepth(root->right);
            if (abs(ld - rd) <= 1) return 1 + std::max(ld, rd);
        }
        return -2;
    }
    bool isBalanced(TreeNode* root) {
        return 0 <= balancedDepth(root);
    }

// 104. Maximum Depth of Binary Tree  https://leetcode.com/problems/maximum-depth-of-binary-tree/
// TODO analysis complexity of below code. from https://discuss.leetcode.com/topic/936/time-limited-when-i-change-to-simplified-code
// return maxDepth(root->left)>maxDepth(root->right) ? maxDepth(root->left)+1 : maxDepth(root->right)+1;
    int maxDepth_bfs(TreeNode* root) {
        if (!root) return 0;
        int cnt = 0;
        vector<TreeNode*> queue {root}, tmp;
        while (!queue.empty()) {
            ++cnt;
            for (auto p: queue) {
                if (p->left) tmp.push_back(p->left);
                if (p->right) tmp.push_back(p->right);
            }
            queue.swap(tmp);
            tmp.clear();
        }
        return cnt;
    }
    // https://discuss.leetcode.com/topic/19207/java-solution-both-recursion-and-iteration
    // https://discuss.leetcode.com/topic/33826/two-java-iterative-solution-dfs-and-bfs
    int maxDepth_dfs_stack(TreeNode* root) {
        if (!root) return 0;
        int cnt = 0;
        vector<TreeNode*> stack {root};
        vector<int> values {1};
        while(!stack.empty()) {
            root = stack.back();
            stack.pop_back();
            auto val = values.back();
            values.pop_back();
            cnt = std::max(cnt, val);
            if (root->left) {
                stack.push_back(root->left);
                values.push_back(1 + val);
            }
            if (root->right) {
                stack.push_back(root->right);
                values.push_back(1 + val);
            }
        }
        return cnt;
    }
    int maxDepth_dfs(TreeNode* root) {
        return (!root)? 0 : 1 + std::max(maxDepth_dfs(root->left), maxDepth_dfs(root->right));
    }
    int maxDepth(TreeNode* root) {
        return maxDepth_dfs_stack(root);
        return maxDepth_bfs(root);
    }

// 101. Symmetric Tree  https://leetcode.com/problems/symmetric-tree/
    bool isSymmetric_bfs(TreeNode* root) {
        if (!root) return true;
        std::deque<TreeNode*> queue {root->left, root->right};
        while (!queue.empty()) {
            auto p = queue.front();
            queue.pop_front();
            auto q = queue.front();
            queue.pop_front();
            if (p == q) continue;
            if (!p || !q || p->val != q->val) return false;
            queue.push_back(p->left);
            queue.push_back(q->right);
            queue.push_back(p->right);
            queue.push_back(q->left);
        }
        return true;
    }
    bool isMirror_tail_recursion_optimized_away(TreeNode* p, TreeNode* q) {
        while (p && q) {
            if (p->val != q->val || isMirror_tail_recursion_optimized_away(p->left, q->right) == false) return false;
            p = p->right, q = q->left;
        }
        return p == q;
    }
    bool isMirror(TreeNode* p, TreeNode* q) {
        return p == q || (p && q && p->val == q->val && isMirror(p->left, q->right) && isMirror(p->right, q->left));
    }
    bool isSymmetric(TreeNode* root) {
        // return isSymmetric_bfs(root);
        return root == nullptr || isMirror_tail_recursion_optimized_away(root->left, root->right);
        return root == nullptr || isMirror(root->left, root->right);
    }

// 100. Same Tree  https://leetcode.com/problems/same-tree/
    // bool isSameTree(TreeNode* p, TreeNode* q) {
    //     const int tmp = (p != nullptr) + (q != nullptr);
    //     if (tmp == 0) return true;
    //     if (tmp == 1 || p->val != q->val || !isSameTree(p->left, q->left)) return false;
    //     return isSameTree(p->right, q->right);
    // }
    // bool isSameTree(TreeNode* p, TreeNode* q) {
    //     return (p == q) || ((p && q) && (p->val == q->val) && isSameTree(p->left, q->left) && isSameTree(p->right, q->right));
    // }
    bool isSameTree_dfs(TreeNode* p, TreeNode* q) {
        vector<TreeNode*> stack {p,q};
        while (!stack.empty()) {
            q = stack.back();
            stack.pop_back();
            p = stack.back();
            stack.pop_back();
            if (p == q) continue;
            if (!p || !q || p->val != q->val) return false;
            stack.push_back(p->left);
            stack.push_back(q->left);
            stack.push_back(p->right);
            stack.push_back(q->right);
        }
        return true;
    }
    bool isSameTree_bfs(TreeNode* p, TreeNode* q) {
        std::deque<TreeNode*> queue {p,q};
        while (!queue.empty()) {
            p = queue.front();
            queue.pop_front();
            q = queue.front();
            queue.pop_front();
            if (p == q) continue;
            if (!p || !q || p->val != q->val) return false;
            queue.push_back(p->left);
            queue.push_back(q->left);
            queue.push_back(p->right);
            queue.push_back(q->right);
        }
        return true;
    }
    bool isSameTree(TreeNode* p, TreeNode* q) {
        return isSameTree_bfs(p,q);
    }

int main() {
    test::init();
}
