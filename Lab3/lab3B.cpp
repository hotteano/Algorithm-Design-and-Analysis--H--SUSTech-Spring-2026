#include<bits/stdc++.h>
using namespace std; 

long long N, M;
vector<pair<long long, string>> v;

void solve(){
    long long countA = 0, countB = 0;
    for(const auto& p : v){
        long long tmpA = 0, tmpB = 0;
        for(char c : p.second){
            if(c == 'A') tmpA++;
            else if(c == 'B') tmpB++;
        }
        countA += tmpA * p.first;
        countB += tmpB * p.first;
    }

    if(countA < countB){
        cout << -1 << "\n";
        return;
    }

    long long K = countA - N;
    long long ans_val = -2e18; 
    long long CurP = 0;
    long long CurK = 0;

    for(const auto& p : v){
        long long x = p.first;
        string s = p.second;
        if(x == 0 || s.empty()) continue;

        long long bc = 0;
        long long max_local = -2e18;
        for(int j = 0; j < s.length(); j++){
            if(s[j] == 'B'){
                bc++;
                long long local_v = (j + 1) - 2 * bc;
                max_local = max(max_local, local_v);
            }
        }

        if(bc == 0){
            CurP += x * s.length();
            continue;
        }

        long long slope = (long long)s.length() - 2 * bc;

        long long val0 = CurP - 2 * CurK + max_local;
        long long val_end = CurP - 2 * CurK + (x - 1) * slope + max_local;

        ans_val = max({ans_val, val0, val_end});

        CurP += x * s.length();
        CurK += x * bc;
    }

    long long delay = 0;
    if(ans_val != -2e18){
        delay = max(0LL, ans_val - 2 * K);
    }
    cout << delay << "\n";
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if(cin >> N >> M){
        for(int i = 0; i < M; i++){
            string s; long long x;
            cin >> s >> x;
            v.push_back({x, s});
        }
        solve();
    }
    return 0;
}