#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int MOD = 998244353; 
const int G = 3;

string s;

ll power(ll a, ll b) {
    ll res = 1;
    a %= MOD;
    while(b > 0){
        if (b & 1) res = res * a % MOD; 
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

void NTT(vector<ll>& a, bool invert){
    int n = a.size(); 
    for(int i = 1, j = 0; i < n; i++){
        int bit = n >> 1; 
        for(; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if(i < j) swap(a[i], a[j]);
    }

    for(int len = 2; len <= n; len <<= 1){
        ll wlen = power(G, (MOD - 1) / len);
        if(invert) wlen = power(wlen, MOD - 2);
        for(int i = 0; i < n; i += len){
            ll w = 1;
            for(int j = 0; j < len / 2; j++){
                ll u = a[i + j], v = a[i + j + len / 2] * w % MOD;
                a[i + j] = (u + v) % MOD;
                a[i + j + len / 2] = (u - v + MOD) % MOD;
                w = w * wlen % MOD;
            }
        }
    }

    if(invert){
        ll n_inv = power(n, MOD - 2);
        for(ll & x : a) x = (x * n_inv) % MOD;
    }

}

vector<ll> multiply(vector<ll> a, vector<ll> b){
    int n = 1;
    while(n < a.size() + b.size()) n <<= 1;

    a.resize(n);
    b.resize(n);

    NTT(a, false);
    NTT(b, false);

    for(int i = 0; i < n; i++) a[i] = a[i] * b[i] % MOD;

    NTT(a, true);
    return a;
}


void solve(){
    int n = s.length();
    vector<ll> A0(n), A1(n);
    for(int i = 0; i < n; i++){
        if(s[i] == '0') A0[i] = 1;
        else if(s[i] == '1') A1[i] = 1;
    }
    vector<ll> B0(n), B1(n);
    for(int i = 0; i < n; i++){
        if(s[n - 1 - i] == '0') B0[i] = 1;
        else if(s[n - 1 - i] == '1') B1[i] = 1;
    }

    vector<ll> C01 = multiply(A0, B1);
    vector<ll> C10 = multiply(A1, B0);

    ll ans = 0;

    for(int len = 1; len <= n/2; len++){
        if(C01[len-1] == 0 && C10[len-1] == 0){
            ans ^= 1LL * len * len;
        }
    }

    cout << ans << endl;
}

int main() {
    cin >> s;
    solve();
    return 0;
}