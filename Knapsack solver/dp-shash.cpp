#include<bits/stdc++.h>
#define ld long double
#define pb push_back

using namespace std;

const int N = 52, M=4e5+5, INF=1E9;
const ld pr = 1e-10;

int n, k, dp[N][M], best[N][M];

struct item{
    int v, w, idx;
    ld g;
} obj[N];

void setINF(){
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= k; j++){
            dp[i][j] = -INF;
        }
    }
}

signed main(){
    cin >> n >> k;
    for(int i = 1; i <= n; i++){
        cin >> obj[i].v >> obj[i].w;
        obj[i].idx = i;
        obj[i].g = ((double) obj[i].v) / obj[i].w;
    }
    setINF();
    dp[0][0] = 0;
    for(int i = 1; i <= n; i++){
        for(int j = 0; j <= k; j++){
            dp[i][j] = dp[i-1][j];
            if(j<obj[i].w) continue;
            if(dp[i-1][j-obj[i].w] + obj[i].v > dp[i][j]){
                dp[i][j] = dp[i-1][j-obj[i].w] + obj[i].v;
                best[i][j] = 1;
            }
        }
    }
    vector<bool> ans(n+1);
    int mx = 0, st = 0;
    for(int i = 0; i <= k; i++){
        if(dp[n][i] > mx){
            mx = dp[n][i];
            st = i;
        }
    }
    for(int i = n; i >= 1; i--){
        ans[i] = best[i][st];
        st = st - obj[i].w * best[i][st];
    }
    cout << mx << " " << 1 << endl;
    for(int i = 1; i <= n; i++){
        cout << ans[i] << " ";
    }
}

