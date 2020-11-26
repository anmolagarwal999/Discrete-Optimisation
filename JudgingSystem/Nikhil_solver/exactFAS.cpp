#include <bits/stdc++.h>
using namespace std ;

#define vi vector < int > 

const int N = 21 ;
vi adj[N] ;
int dp[1 << N] ;
int n, m ;

vi getOrdering(int mask = 0) {
    vi emp ; 
    if(__builtin_popcount(mask) == n) return emp ; 
    int ans = 1e8, cnt = 0 ;
    for(int i = 0; i < n; i++)
    {
        cnt = 0 ;
        if(!(mask & (1 << i)))
        {
            int new_mask = mask | (1 << i) ;
            for(int u : adj[i])
                if(mask & (1 << u)) 
                    cnt++ ;

            if(dp[mask] == cnt + dp[new_mask]) {
                vi nxt = getOrdering(new_mask);
                nxt.insert(nxt.begin(), i);
                return nxt ; 
            }
        }
    }
    return emp ; // is this needed???
}

int solve(int mask = 0)
{
    if(__builtin_popcount(mask) == n) return dp[mask] = 0;
    if(dp[mask] != -1) return dp[mask] ;
    int ans = 1e8, cnt = 0 ;
    for(int i = 0; i < n; i++)
    {
        cnt = 0 ;
        if(!(mask & (1 << i)))
        {
            int new_mask = mask | (1 << i) ;
            for(int u : adj[i])
                if(mask & (1 << u)) cnt++ ;
            ans = min(ans, cnt + solve(new_mask));
        }
    }
    return dp[mask] = ans ;
}

int main()
{
    cin >> n >> m ; int u, v ;
    for(int i = 0; i < m; i++)
    {
        cin >> u >> v ; u-- ; v-- ;
        adj[u].push_back(v);
    }
    memset(dp, -1, sizeof(dp));
    cout << solve() << endl ;
    vi order = getOrdering();
    for(auto x : order) cout << x+1 << " " ;
    cout << endl ; 
}

/*
5 7 
1 2 
2 3 
3 1 
3 5 
5 4 
4 1 
1 4
*/
