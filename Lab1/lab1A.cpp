#include <bits/stdc++.h>
using namespace std;

int main(){
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        if(n < 6){
            printf("Alice\n");
        }
        else if(n % 6 == 0){
            printf("Bob\n");
        }
        else{
            printf("Alice\n");
        }
    }
    return 0;
}