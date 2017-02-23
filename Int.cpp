#include "utility.h"
// Boost.Multiprecision  http://www.boost.org/doc/libs/1_63_0/libs/multiprecision/doc/html/index.html
// Brent and Zimmermann's Modern Computer Arithmetic is a good book on the theory and implementation of big-number arithmetic. Probably worth a read if you want to know what's known.  http://stackoverflow.com/a/17324388/3625404
// http://stackoverflow.com/questions/9257612/division-of-large-numbers
// Now that I have revisited this project, I have finally implemented big integer division using two different algorithms.  http://stackoverflow.com/questions/33018923/how-does-one-divide-a-big-integer-by-another-big-integer
// Use a library for huge integers, e.g. GNU MP, NTL or FLINT. There probably are others. Weaving your own is a waste of time.  http://stackoverflow.com/a/33071234/3625404
// FLINT: Fast Library for Number Theory http://www.flintlib.org/
// NTL: A Library for doing Number Theory http://shoup.net/ntl/
// not maintained  C++ Big Integer Library   https://mattmccutchen.net/bigint/   https://mattmccutchen.net/bigint/bigint.git/
// Division of a big number of 100 digits stored as string http://stackoverflow.com/a/24582287/3625404
// file:///E:/bks/mf/nan/Modern_Computer_Arithmetic[2010][Richard_Brent,_Paul_Zimmermann](Book4You).pdf
// Modern Computer Arithmetic 2010 by Richard P. Brent, Paul Zimmermann https://www.amazon.com/dp/0521194695/?tag=stackoverfl08-20

// Int was represented as array of 4-byte signed integer in base 1e9, least significant digit first.
// maintain a class/thread level buffer to avoid allocation and improve cache performance for small input.
// TODO maintain a class/thread level memory pool as optimization for small number, possibly in the form of operator new/delete member function.
// or placement new? how does std::vector manage memory arena?   http://eli.thegreenplace.net/2011/02/17/the-many-faces-of-operator-new-in-c
class Int {
public:
    typedef int ValType;    // or use int32_t in <cstdint> or boost/cstdint.hpp
    typedef int SumType;
    typedef long long ProdType;
    Int(ValType x) { // TODO or being explicit?
        m_sign = 1;
        value = std::vector<ValType> {x % BASE};
        const bool negative = x < 0;
        x %= BASE;
        if (negative) {
            m_sign = -1;
            value[0] *= -1;
            x *= -1;
        }
        if (x) value.push_back(x);
    }
    Int(): Int(0) {}
    Int(const Int &obj): m_sign{obj.m_sign}, value{obj.value} {}
    Int (string x) {
        value = rebase(x);
    }
    void negate() { this->m_sign *= -1; }
    Int& operator+=(const Int& obj);
    Int& operator-=(const Int& obj) { *this += -obj; return *this; }  // TODO, not efficient, create temporary then discard.
    const Int operator-() const { auto res = Int(*this); res.negate(); return res; }
    size_t dlen() const { return this->value.size(); }
    friend std::ostream& operator<<(std::ostream &out, const Int &obj);
    int sign() const { return this->m_sign; }
private:
    static constexpr ValType BASE = 1e9;     // BASE + BASE do not overflow ValType.
    static constexpr ValType kdigits = 9;
    static constexpr ValType VAL_MAX = INT_MAX;
    static constexpr ValType kb_cutoff = 16;
    int m_sign;
    vector<ValType> value;
    // ValType sign;
    static ValType digits_to_int (const char* p, const char* pend, int base) {
        // caution, **NO** error handling. 
        // assert(p < pend) // TODO, fix same place in 13.cpp, currently misspelled as 11.cpp.
        ValType res = 0;
        for(; p != pend; ++p) {
            res = res * base + (*p - '0');
        }
        return res;
    }
    vector<ValType> rebase(string num) {
        vector<ValType> res;
        auto pbegin = num.c_str();
        auto pend = pbegin + num.size();
        for (; (pbegin < pend) && isspace(*pbegin); ++pbegin) {}
        if (*pbegin == '-') {
            this->m_sign = -1;
            ++pbegin;
        } else if (*pbegin == '+') {
            this->m_sign = 1;
            ++pbegin;
        } else {
            this->m_sign = 1;
        }
        auto pleft = pend - kdigits;
        for (; pleft >= pbegin; pleft -= kdigits, pend -= kdigits) {
            res.push_back(digits_to_int(pleft, pend, 10));
        }
        if (pbegin < pend) { // TODO, fix same place in 13.cpp, currently misspelled as 11.cpp.
            res.push_back(digits_to_int(pbegin, pend, 10));
        }
        return res;
    }
    static void rebase(std::vector<Int::ValType> & value) {
        Int::ValType carry = 0;
        for (auto &x: value) {
            x += carry;
            carry = x / Int::BASE;
            x %= Int::BASE;
        }
        if (carry > 0) {
            value.push_back(carry);
        }
    }
    static vector<Int::ValType> rebase(std::vector<Int::ProdType> & value_prod) {
        // printf("value_prod: "); print(value_prod);
        vector<Int::ValType> res;
        res.reserve(value_prod.size());
        Int::ProdType carry = 0;
        for (auto &x: value_prod) {
            x += carry;
            carry = x / Int::BASE;
            x %= Int::BASE;
            res.push_back(x);
        }
        if (res.back() == 0) {
            res.pop_back();
        }
        // printf("res: "); print(res);
        // wait();
        return res;
    }
    void print(std::ostream& out) const {
        // auto old_DEBUG = DEBUG;
        // DEBUG = 0;
        char *output = new char[Int::kdigits * this->value.size() + 1];   // allocate
        auto p = output;
        if (this->sign() < 0) {
            *p++ = '-';
        }
        auto it = this->value.rbegin();
        if (DEBUG)
            p +=  sprintf(p, "%d ", *it);
        else
            p +=  sprintf(p, "%d", *it);
        for (++it; it != this->value.rend(); ++it) {
            if (DEBUG)
                p += sprintf(p, "%09d ", *it);   // caution, Int::ValType is not guaranteed to be int and may not be 1e9, though unlikely.
            else
                p += sprintf(p, "%09d", *it);   // caution, Int::ValType is not guaranteed to be int and may not be 1e9, though unlikely.
        }
        // std::ofstream out("/home/Dell/code/qeatzy/project_euler/tmpc");
        // out << "y = " << output;
        if (DEBUG) out << "dlen() = " << this->dlen() << ": ";
        out << output;
        if (DEBUG) out << '\n';
        delete[] output;
        // DEBUG = old_DEBUG;
    }
    static int computeAddition(std::vector<ValType> &dest, const std::vector<ValType> &addendum) {
        // 923 + 99
        assert(dest.size() >= addendum.size());
        SumType carry = 0;
        size_t i = 0;
        for (; i < addendum.size(); ++i) {
            carry = carry + (dest[i] + addendum[i]);
            dest[i] = carry % BASE;
            carry = carry / BASE;
        }
        for (; carry != 0 && i < dest.size(); ++i) {
            carry += dest[i];
            dest[i] = carry % BASE;
            carry = carry / BASE;
        }
        return carry;
    }
    static void computeSubtraction(std::vector<ValType> &dest, std::vector<ValType>::const_iterator first, std::vector<ValType>::const_iterator last) {
        assert(dest.size() >= size_t(std::distance(first, last)));
        SumType carry = 0;
        auto p = dest.begin();
        for (; first != last; ++p, ++first) {
            carry = carry + (*p - *first);
            decltype(carry) tmp = carry;
            if (carry < 0) {
                tmp += BASE;
                carry = -1;
            } else {
                carry = 0;
            }
            *p = tmp % BASE;
        }
    }
public:
    Int addition(const Int &rhs) const {
        // TODO use method, int add(Iterator first, last, first2, last2, dest), and if return non-zero then push_back new carry.
        // -- and cacel equal prefix when of same digits and sign differ.
        // -- a bonus, you only need to trim one digit.
        // 123 - 45 | 123 - 126 | 45 - 123 | 123 + 45 | 45 + 123 -- if BASE == 10 instead of 1e9.
        if (this->sign() == rhs.sign()) {
            if (this->dlen() < rhs.dlen()) {
                auto res = rhs;
                auto carry = Int::computeAddition(res.value, this->value);
                if (carry) { res.value.push_back(carry); }
                return res;
            } else {
                auto res = *this;
                auto carry = Int::computeAddition(res.value, rhs.value);
                if (carry) { res.value.push_back(carry); }
                return res;
            }
        } else {
            std::vector<ValType>::const_iterator first1, last1, first2, last2;
            first1 = value.cbegin(), last1 = value.cend();
            first2 = rhs.value.cbegin(), last2 = rhs.value.cend();
            auto sign = this->sign();
            if (this->dlen() == rhs.dlen()) {
                for (; last1 != first1 && last1[-1] == last2[-1]; --last1, --last2) {}
                if (last1[-1] < last2[-1]) {
                    std::swap(first1, first2);
                    std::swap(last1, last2);
                    sign = -sign;
                }
            } else if (this->dlen() < rhs.dlen()) {
                std::swap(first1, first2);
                std::swap(last1, last2);
                sign = -sign;
            }
            std::vector<ValType> dest(first1, last1);
            Int::computeSubtraction(dest, first2, last2);
            // TODO trim leading zero, eg, 1001 - 998
            return Int(sign, std::move(dest));
        }
    }
    // Int addition(int x) {
    //     // TODO 
    //     auto res = *this;
    //     return res;
    // }
    Int subtraction(const Int &rhs) const {
        // TODO 
        return *this;
    }
    static void computeMultiplication(std::vector<ValType>::iterator dest, std::vector<ValType>::const_iterator first1, std::vector<ValType>::const_iterator last1,
            std::vector<ValType>::const_iterator first2, std::vector<ValType>::const_iterator last2) {
        ProdType carry = 0, other;
        for (; first1 != last1; ++dest, ++first1) {
            other = *first1;
            auto p = dest;
            for (auto q = first2; q != last2; ++p, ++q) {
                carry = carry + *p + other * *q;
                *p = carry % BASE;
                carry = carry / BASE;
            }
            *p += carry;
            carry = 0;
        }
    }
    Int multiply(const Int &rhs) const {
        // TODO 3456 * 789
        auto dest = decltype(value)(value.size() + rhs.value.size());
        computeMultiplication(dest.begin(), value.cbegin(), value.cend(), rhs.value.cbegin(), rhs.value.cend());
        return Int(this->sign() * rhs.sign(), std::move(dest));
    }
    Int multiply(int val) const {
        auto res = *this;
        ProdType carry = 0, other = val;
        if (val < 0) {
            res.negate();
            other = -other;
        }
        for (auto &x: res.value) {
            carry = carry + other * x;
            x = carry % BASE;
            carry = carry / BASE;
        }
        // if (DEBUG) cout << "carry - BASE = " << double(carry - BASE) << endl;
        while (carry) {
            res.value.push_back(carry % BASE);
            carry = carry / BASE;
        }
        return res;
    }
private:
    Int(int sign, std::vector<ValType> &&value): m_sign{sign}, value{value} {}
    // Int(int sign, std::vector<ValType>::iterator first, std::vector<ValType>::iterator last): m_sign{sign}, value(first, last) {}
};

Int operator*(const Int &obj, int val) {
    return obj.multiply(val);
}
Int operator*(int val, const Int &obj) {
    return obj.multiply(val);
}

Int operator*(const Int &lhs, const Int &rhs) {
    return lhs.multiply(rhs);
}

// TODO
// Int operator+(const Int &obj, int val) {
//     return obj.addition(val);
// }
// Int operator+(int val, const Int &obj) {
//     return obj.addition(val);
// }

Int operator+(const Int &lhs, const Int &rhs) {
    return lhs.addition(rhs);
}

std::ostream& operator<<(std::ostream &out, const Int &obj) {
    obj.print(out);
    return out;
}

namespace test {
    void Int() { // or use a Int class, and use like this, `test::Int::division`?
    using ::Int;
        // auto c = Int();
        auto a = Int(), b = Int(-2123456789), c = Int("-999999999549123491234912349123491234912349123491234912349123499994912349"), d=c;
        // auto c = ::Int("-999999999549123491234912349123491234912349123491234912349123499994912349");
        cout << "c = " << c;
        // cout << a << b << c << d << c+d << (c*d) * (c*d);
        // cout << a << b << c << c*c;
        // cout << a-b << b-a;
        // auto tmp = Int::pow(2,42*29);
        // auto tmp = Int::pow(2,21*29);
        // auto tmp = Int("2124551971267068394758352826209874509318372470908127692797776552801614239443408970956650009060917142675557317944986004061386317350610828957638079915066349407775325083341572876126912512"); // 2**(21*29)
        // cout << " tmp: " << tmp;
        // tmp *= tmp;
        // cout << " tmp: " << tmp;
        // auto f42 = Int("4513721078614786210022654658754022076207575792470901370842837631998555767764876163651334186529206088049982418198714538469250807179723842787735400581098036879598420878073788073478633526793353330401336902135069154012389883318957655774958078857324256712388820015707438180916154489315708470767711885060380524427604580826158560154020090008127121505888111464555809702150144"); // 2**(42*29)
        // cout << " f42: " << f42;
        // cout << "\ntmp.cmp_abs(f42) = " << tmp.cmp_abs(f42) << '\n';
        // auto diff = f42 - f42;
        // cout << diff;
        // cout << (a == diff) << (a < diff) << (a > diff);
        // cout << Int::pow(2,10000);
        // tmp = 2**(29*21) is right, whereas tmp * tmp get wrong
        // auto f = [](int base, int exponent) { cout << std::pow(base, exponent) << '\n'; };
        // f(2,10);
        // f(2,100000);
        // f(INT_MAX, INT_MAX);
        auto start = std::chrono::high_resolution_clock::now();
        auto tmp = c;
        // auto tmp = Int::pow(11,1000000);
        // auto tmp = Int("2423285551989543969259886147306320615721694717012975552426444448158985017722789267546553034738712987127346362442309271495645764807314487385596126924659433020959638410571315406303196994043324038030803068668509323897700215957022383545283810557899591580902307500436706661372076856211818662627186819885605667216486349283517459646495626188985295134800963771933733229691796170211328");
        // cout << " tmp: " << tmp;
        // tmp *= tmp;
        cout << " tmp: " << tmp;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << diff.count() << " s\n";
    }
    namespace bigint {
        using ::Int;
        void addition() {
            auto a = Int(), b = Int(-2123456789), c = Int("-999999999549123491234912349123491234912349123491234912349123499994912349"), d=c;
            cout << "b = " << b << "\n, c = " << c << "\n, b + c = " << b + c << "\n, c + b = " << c + b << endl;
            b.negate();
            cout << "b = " << b << "\n, c = " << c << "\n, b + c = " << b + c << "\n, c + b = " << c + b << endl;
        }
        void multiply(int caseNo = 0) {
            auto a = Int(), b = Int(-2123456789), c = Int("-999999999549123491234912349123491234912349123491234912349123499994912349"), d=c;
            if (caseNo == 1) { // Int * int, or, int * Int
                // int x = 3456789;
                int x = INT_MAX;
                cout << "x = " << x << "\n, c = " << c << "\n, x * c = " << x * c << "\n, c * x = " << c * x << endl;
                x = -x;
                cout << "x = " << x << "\n, c = " << c << "\n, x * c = " << x * c << "\n, c * x = " << c * x << endl;
            } else { // Int * Int
                cout << "c = " << c << "\n, c * c = " << c * c << endl;
            }
        }
    }
}

int main() {
    test::init();
    // test::Int();
    // test::bigint::addition();
    test::bigint::multiply();
    // test::divmod(1,12,10);
    // test::divmod(1,10,10);
    // test::divmod(1,9,10);
}
