#include "utility.h"

// reverse a C style string, that is, a char array with extra trailing '\0'.
char* reverse(char * str) {
    char *head = str;
    for (; *head; ++head) {}
    char *res = head;
    // for (; str < head; ++str, --head) std::iter_swap(str, head);
    std::reverse(str, head + 1);
    return res;
}

namespace test {
    void reverse() {
        char str[] = "hello world!";
        cout << "str = " << str << endl;
        auto p = ::reverse(str);
        cout << "reversed str = ";
        for (; p != str; ) cout << *p--;
        cout << endl;
    }
}

int main() {
    test::init();
    test::reverse();
}
