#include <bits/stdc++.h>
using namespace std; 

const int N = 505;
int n;
int ddl[N], punish[N];

void solve(){
    vector<pair<int, int>> tasks;
    for(int i = 0; i < n; i++) tasks.push_back({ddl[i], punish[i]});
    sort(tasks.begin(), tasks.end());

    priority_queue<int, vector<int>, greater<int>> completed;  
    
    for(auto &[d, p] : tasks){
        if((int)completed.size() < d){
            completed.push(p); 
        } else if(!completed.empty() && completed.top() < p){
            completed.pop();
            completed.push(p);
        }
    }
    
    long long ans = 0;
    long long total = 0;
    for(int i = 0; i < n; i++) total += punish[i];
    while(!completed.empty()){
        ans += completed.top();
        completed.pop();
    }
    cout << total - ans << endl;  
}

int main(){
    cin >> n; 

    for(int i = 0; i < n; i++){
        cin >> ddl[i];
    }

    for(int i = 0; i < n; i++){
        cin >> punish[i];
    }

    solve();

    return 0;
}