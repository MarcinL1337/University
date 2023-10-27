#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(){
    int n, a, b, c;
    cin >> n >> a >> b >> c;
    int dp[n+1];
    for(int i = 0; i < n+1; i++)
        dp[i] = -1;

    dp[0] = 0;

    for(int i = 0; i <= n; i++){
        if(dp[i] == -1) continue;

        if(a + i <= n) dp[a + i] = max(dp[a + i], dp[i] + 1);
        if(b + i <= n) dp[b + i] = max(dp[b + i], dp[i] + 1);
        if(c + i <= n) dp[c + i] = max(dp[c + i], dp[i] + 1);
    }

    int res = max(dp[n], 0);
    cout << res;
}