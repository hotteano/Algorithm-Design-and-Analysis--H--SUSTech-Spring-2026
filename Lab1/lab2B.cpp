#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, k;
    cin >> n >> k;
    if(n == 1){
        printf("1");
    }
    else{
        printf("%d", 2 * min(k, n - k + 1));
    }
}