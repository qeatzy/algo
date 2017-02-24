#include "utility.h"
// data structure, ADT(Abstract data type).
// Design https://leetcode.com/tag/design/
// Stack https://leetcode.com/tag/stack/

// 155. Min Stack https://leetcode.com/problems/min-stack/?tab=Description
// 1. two stack solution. https://discuss.leetcode.com/topic/18556/c-using-two-stacks-quite-short-and-easy-to-understand https://discuss.leetcode.com/topic/6339/java-solution-accepted/4 https://discuss.leetcode.com/topic/8540/share-my-clean-ac-c-solution-with-explanation
// https://discuss.leetcode.com/topic/5071/shortest-and-fastest-1-stack-and-2-stack-solutions
// 2. push/pop current min if value <= min.   TODO https://discuss.leetcode.com/topic/7020/java-accepted-solution-using-one-stack
// 3. store gap with min instead. TODO https://discuss.leetcode.com/topic/4953/share-my-java-solution-with-only-one-stack
// 4. non O(1) solution.
// 5. using list?? TODO https://discuss.leetcode.com/topic/26866/6ms-java-solution-using-linked-list-clean-self-explanatory-and-efficient
class MinStack_extra_stack_for_min { // do not update stack for min if x == getMin() when do push.
public:
    typedef int T;
    /** initialize your data structure here. */
    MinStack_extra_stack_for_min() : stack{}, pmin_stack{} {
    }
    void push(int x) {
        bool need_to_update_pmin = (stack.empty() || getMin() > x);
        if (need_to_update_pmin) { pmin_stack.push_back(stack.size()); }
        stack.push_back(x);
    }
    
    void pop() {
        stack.pop_back();
        bool need_to_update_pmin = (stack.size() == pmin_stack.back());
        if (need_to_update_pmin) { pmin_stack.pop_back(); }
    }
    
    int top() {
        return stack.back();
    }
    
    int getMin() {
        return stack[pmin_stack.back()];
    }
// private:
protected:
    std::vector<T> stack;
    vector<size_t> pmin_stack;
};
class MinStack_O_n_pop {
public:
    typedef int T;
    /** initialize your data structure here. */
    // MinStack() : stack{}, pmin{stack.end()} {
    MinStack_O_n_pop() : stack{}, pmin{0} {
    }
    // MinStack() : stack{} {
    //     pmin = stack.end();
    // }
    
    void push(int x) { // 1. empty no need to update pmin. 2.
        bool need_to_update_pmin = false;
        // wait("before test");
        if (stack.empty() || getMin() > x) { need_to_update_pmin = true; }
        // if (pmin != stack.end() && getMin() > x) { need_to_update_pmin = true; }
        // wait("before push");
        stack.push_back(x);
        // wait("before update");
        // if (need_to_update_pmin) { pmin = stack.end() - 1; }
        // cout << need_to_update_pmin << endl;
        if (need_to_update_pmin) {
            // pmin = stack.end() - (!stack.empty());
            // pmin = stack.end() - 1;
            pmin = stack.size() - 1;
        }
    }
    
    void pop() { // if there are more than two value equal to *pmin, pmin will point to the first one.
        bool need_to_update_pmin = (pmin + 1 == stack.size());
        stack.pop_back();
        if (need_to_update_pmin) { pmin = std::min_element(stack.begin(), stack.end()) - stack.begin(); }
    }
    
    int top() {
        return stack.back();
    }
    
    int getMin() {
        return stack[pmin];
    }
private:
// protected:
        std::vector<T> stack;
        size_t pmin;
};
class MinStack : public MinStack_extra_stack_for_min {
// class MinStack : public MinStack_O_n_pop {
// private:
public:
    void print() {
        // cout << "sz = " << stack.size() << endl;
        // wait("before print");
        ::print(stack,"",-1);
        // ::print(stack);
        // cout << "pmin - stack.begin() = " << pmin - stack.begin() << endl;
        // wait("before print min");
        cout << "\t min = " << getMin() << endl;
    }
};

namespace test {
    void MinStack() {
        auto stack = ::MinStack();
        stack.push(-2);
        stack.print();
        stack.push(0);
        stack.print();
        stack.push(-3);
        stack.print();
        stack.pop();
        stack.print();
        stack.top();
    }
}

int main() {
    test::init();
    // test::isValidSudoku();
    test::MinStack();
}

