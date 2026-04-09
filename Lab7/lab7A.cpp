#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

using int64 = long long;
const int BASE = 10000;
const int BASE_DIGS = 4;
const int KARATSUBA_CUTOFF = 64;

string trimZero(const string& s) {
    int i = 0;
    while (i + 1 < (int)s.size() && s[i] == '0') {
        i++;
    }
    return s.substr(i);
}

void trimPoly(vector<int64>& a) {
    while (!a.empty() && a.back() == 0) {
        a.pop_back();
    }
}

vector<int64> addPoly(const vector<int64>& a, const vector<int64>& b) {
    vector<int64> c(max(a.size(), b.size()), 0);
    for (size_t i = 0; i < a.size(); i++) {
        c[i] += a[i];
    }
    for (size_t i = 0; i < b.size(); i++) {
        c[i] += b[i];
    }
    trimPoly(c);
    return c;
}

void subPolyInPlace(vector<int64>& a, const vector<int64>& b) {
    for (size_t i = 0; i < b.size(); i++) {
        a[i] -= b[i];
    }
    trimPoly(a);
}

void addShifted(vector<int64>& dst, const vector<int64>& src, int shift) {
    if (src.empty()) {
        return;
    }
    if ((int)dst.size() < shift + (int)src.size()) {
        dst.resize(shift + (int)src.size(), 0);
    }
    for (size_t i = 0; i < src.size(); i++) {
        dst[shift + (int)i] += src[i];
    }
}

vector<int64> multiplySchool(const vector<int64>& a, const vector<int64>& b) {
    if (a.empty() || b.empty()) {
        return {};
    }
    vector<int64> c(a.size() + b.size(), 0);
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < b.size(); j++) {
            c[i + j] += a[i] * b[j];
        }
    }
    trimPoly(c);
    return c;
}

vector<int64> karatsuba(const vector<int64>& a, const vector<int64>& b) {
    if (a.empty() || b.empty()) {
        return {};
    }
    if ((int)min(a.size(), b.size()) <= KARATSUBA_CUTOFF) {
        return multiplySchool(a, b);
    }
    int n = max((int)a.size(), (int)b.size());

    vector<int64> aa = a, bb = b;
    aa.resize(n, 0);
    bb.resize(n, 0);

    int k = n / 2;
    vector<int64> a0(aa.begin(), aa.begin() + k);
    vector<int64> a1(aa.begin() + k, aa.end());
    vector<int64> b0(bb.begin(), bb.begin() + k);
    vector<int64> b1(bb.begin() + k, bb.end());

    vector<int64> z0 = karatsuba(a0, b0);
    vector<int64> z2 = karatsuba(a1, b1);
    vector<int64> a01 = addPoly(a0, a1);
    vector<int64> b01 = addPoly(b0, b1);
    vector<int64> z1 = karatsuba(a01, b01);

    subPolyInPlace(z1, z0);
    subPolyInPlace(z1, z2);

    vector<int64> res(2 * n, 0);
    addShifted(res, z0, 0);
    addShifted(res, z1, k);
    addShifted(res, z2, 2 * k);
    trimPoly(res);
    return res;
}

vector<int64> parseBigInt(const string& s) {
    vector<int64> a;
    a.reserve((s.size() + BASE_DIGS - 1) / BASE_DIGS);
    for (int i = (int)s.size(); i > 0; i -= BASE_DIGS) {
        int l = max(0, i - BASE_DIGS);
        int len = i - l;
        a.push_back(stoi(s.substr(l, len)));
    }
    trimPoly(a);
    return a;
}

string polyToString(vector<int64> a) {
    if (a.empty()) {
        return "0";
    }
    int64 carry = 0;
    for (size_t i = 0; i < a.size(); i++) {
        int64 cur = a[i] + carry;
        if (cur >= 0) {
            a[i] = cur % BASE;
            carry = cur / BASE;
        } else {
            int64 borrow = (-cur + BASE - 1) / BASE;
            cur += borrow * BASE;
            a[i] = cur;
            carry = -borrow;
        }
    }
    while (carry > 0) {
        a.push_back(carry % BASE);
        carry /= BASE;
    }
    trimPoly(a);
    if (a.empty()) {
        return "0";
    }

    string s = to_string(a.back());
    for (int i = (int)a.size() - 2; i >= 0; i--) {
        string block = to_string(a[i]);
        s += string(BASE_DIGS - (int)block.size(), '0') + block;
    }
    return s;
}

string multiplyBigInt(string x, string y) {
    bool neg = false;
    if (!x.empty() && x[0] == '-') {
        neg = !neg;
        x = x.substr(1);
    }
    if (!y.empty() && y[0] == '-') {
        neg = !neg;
        y = y.substr(1);
    }

    x = trimZero(x);
    y = trimZero(y);
    if (x == "0" || y == "0") {
        return "0";
    }

    vector<int64> a = parseBigInt(x);
    vector<int64> b = parseBigInt(y);
    string p = polyToString(karatsuba(a, b));
    if (neg && p != "0") {
        p = "-" + p;
    }
    return p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string x, y;
    cin >> x >> y;
    cout << multiplyBigInt(x, y) << "\n";
    return 0;
}