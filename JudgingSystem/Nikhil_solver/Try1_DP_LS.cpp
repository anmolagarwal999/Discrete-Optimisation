#include <bits/stdc++.h>
using namespace std ;

#define reMin(a, b) a = min(a, b)
#define reMax(a, b) a = max(a, b)

#define lint long long
#define pb push_back
#define F first 
#define S second 
#define sz(x) (int)x.size()
#define all(x) begin(x), end(x)
#define SET(x, val) memset(x, val, sizeof(x))
#define fastio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)

typedef vector < int > vi ;
typedef pair < int, int > pii ;

const int N = 20 ;
const lint INF = 1e18 ;
int submaskCost[N][1 << N] ;
int dp[1 << N] ; 

class WFAS {
	static const int K = N ; int MAX_ITER = 10000 ; 
	// int submaskCost[K][1 << K] ;
	// int dp[1 << K] ;
	vector < vi > cost ; int totalCost ;
	int n = -1, k = 1; vi globalPerm ; 

	vi greedyOrdering() {
		vector < pii > costs ;
		for(int i = 0; i < n; i++) {
			int val = 0 ;
			for(int j = 0; j < n; j++)
				val += cost[i][j] ;
			costs.pb({val, i});
		}
		sort(all(costs)); reverse(all(costs));
		vi order ;
		for(auto element : costs) order.pb(element.S);
		return order ; 
	}

	void setSubmaskCosts(vi& perm) {
		int mx = -1 ; 

		for(int j = 0; j < k; j++) {
			int prev = 0 ; submaskCost[j][prev] = 0 ;
			for(int i = 1; i < (1 << k); i++) {
				int mask = i ^ (i >> 1); // graycode 
				int dif = mask ^ prev ; 
				int pos = __builtin_ctz(dif);
				reMax(mx, pos);
				int on = mask & (1 << pos);
				if(on) submaskCost[j][mask] = submaskCost[j][prev] + cost[perm[j]][perm[pos]] ;
				else   submaskCost[j][mask] = submaskCost[j][prev] - cost[perm[j]][perm[pos]] ;
				prev = mask ; 

				// fprintf(stderr, "%d %d -- %d %d : %d\n", j, mask, pos, on, submaskCost[j][mask]);
			}
		}
		// fprintf(stderr, "%d\n", mx);
	}

	int solveDP(vi& perm, int mask = 0)
	{
	    if(__builtin_popcount(mask) == k) return dp[mask] = 0;
	    if(dp[mask] != -1) return dp[mask] ;
	    int ans = 1e9 ;

	    for(int i = 0; i < k; i++)
	        if(!(mask & (1 << i)))
	            reMin(ans, submaskCost[i][mask] + solveDP(perm, mask | (1 << i)));
	        
	    return dp[mask] = ans ;
	}

	vi getDPOrdering(vi& perm, int mask = 0) {
	    vi emp ; 
	    if(__builtin_popcount(mask) == k) return emp ; 
	    int ans = 1e9 ;
	    for(int i = 0; i < k; i++)
	    {
	        if(!(mask & (1 << i)))
	        {
	            int new_mask = mask | (1 << i) ;
	            if(dp[mask] == submaskCost[i][mask] + dp[new_mask]) {
	                vi nxt = getDPOrdering(perm, new_mask);
	                nxt.insert(nxt.begin(), perm[i]);
	                return nxt ; 
	            }
	        }
	    }
	    return emp ; // is this needed???
	}

	vi permuteDP(vi& perm, int& permCost) {
		SET(dp, -1);
		SET(submaskCost, 0);
		setSubmaskCosts(perm);
		int oldCost = getOrderingCost(perm);
		int newCost = solveDP(perm);
		vi newOrder = getDPOrdering(perm);
		permCost += newCost - oldCost ; 
		// fprintf(stderr, "Cost improved by %d\n", oldCost - newCost);
		return newOrder ; 
	}

	void improveUsingDP(vi& perm, int& permCost) {
		vi nxt, improved ; 

		int pos = rand() % (n - k + 1) ; 
		for(int j = 0; j < k; j++)
			nxt.pb(perm[pos + j]);

		improved = permuteDP(nxt, permCost);
		for(int j = 0; j < k; j++)
			perm[pos + j] = improved[j] ;
	}

	void evaluateSwap(int i, int j, vi& perm, int& currentCost) {
		int delta = 0 ; assert(i <= j);
		for(int k = i+1; k < j; k++) delta += cost[perm[i]][perm[k]], delta -= cost[perm[k]][perm[i]] ;
		for(int k = j-1; k >= i; k--) delta -= cost[perm[j]][perm[k]], delta += cost[perm[k]][perm[j]] ;

		if(delta < 0) {
			currentCost += delta ;

			// for(auto x: perm) cerr << x << " " ;
			// cerr << endl ;

			// cerr << "swapping " << i << " and " << j << " with delta = " << delta << endl ;
			// for(int k = i+1; k <= j; ++k)
			// 	fprintf(stderr, "%d to %d : %d\n", perm[i], perm[k], cost[perm[i]][perm[k]]);
			// for(int k = j-1; k >= i; k--)
			// 	fprintf(stderr, "%d to %d : %d\n", perm[j], perm[k], cost[perm[j]][perm[k]]);
			// cerr << endl ; 

			swap(perm[i], perm[j]);
		}
	}

	vi localSearch() {
		vi current = greedyOrdering(); 
		int currentCost = getOrderingCost(current);
		int iter = 0 ;

		vi best = current ;
		int bestCost = currentCost ;

		vector < pii > pairs ;
		for(int i = 0; i < n; i++) for(int j = i+1; j < n; j++) pairs.pb({i, j});
		random_shuffle(all(pairs));
		// for(int i = 0; i < 50; i++)
		// 	cerr << pairs[i].F << " " << pairs[i].S << endl ; 

		int MAX_INNER_ITER = min(sz(pairs), 3*k*(1 << k)) ;
		cerr << MAX_INNER_ITER << endl ; 
		int idx = 0 ; int megaDP = 0; 

		fprintf(stderr, "Iterations: %d\tCost: %d\n", iter++, currentCost);
		for(; iter < MAX_ITER; iter++) {

			int bef = currentCost ;

			int prev ;
			do {
				prev = currentCost ;
				improveUsingDP(current, currentCost);
			} while(currentCost < prev); 
			

			// for(int i = 0; i < n; i++) {
			// 	for(int j = i+1; j < n; j++) {
			// 		evaluateSwap(i, j, current, currentCost);
			// 		// int expec = getOrderingCost(current);
			// 		// cerr << expec << " : " << currentCost << endl ; 
			// 		// assert(expec == currentCost);
			// 	}
			// }




			for(int cnt = 0; cnt < MAX_INNER_ITER; cnt++) {
			// do {
				prev = currentCost ;
				evaluateSwap(pairs[idx].F, pairs[idx].S, current, currentCost);
				idx++; idx %= sz(pairs); 
				// cerr << currentCost << endl ; 
			// } while(currentCost < prev); 
			}

			// if(rand() % 3 == 2) {
			// 	int expec = getOrderingCost(current);
			// 	assert(expec == currentCost);
			// }


			// if(rand() % 100 == 1) {
			// 	int i = rand() % n ;
			// 	int j = rand() % n ;
			// 	swap(current[i], current[j]);
			// 	currentCost = getOrderingCost(current); 
			// }





			if(iter % 10 == 1) {
				// printf("%d\n", ++megaDP);
				int oldK = k;
				k = K ;
				improveUsingDP(current, currentCost);
				k = oldK ; 
			}
			else megaDP= 0 ;

			// currentCost = getOrderingCost(current);
			if(iter % 10 == 1)
				fprintf(stderr, "Iterations: %d\tCost: %d\n", iter, totalCost -currentCost);
			// for(auto x : current) cerr << x << " " ;
			// cerr << endl ;
		}
		return current ; 
	}

public:
	WFAS(vector < vi > matrix) {
		cost = matrix ;
		n = sz(matrix);
		for(int i = 0; i < n; i++) {
			assert(sz(matrix[i]) == n);
			cost[i][i] = 0 ; 
		}
		k = 0 ;
		while( (1 << k)*k < (n*n*n) )
			k++ ; 
		reMin(k, 10);

		MAX_ITER = 1e9/(1 << k) ;
		MAX_ITER /= 5*k ;
		MAX_ITER = 1000 ;

		fprintf(stderr, "For N = %d, K has been set to %d. Max Iterations = %d\n", n, k, MAX_ITER);
	}

	int getOrderingCost(vi perm) {
		int val = 0 ;
		for(int i = 0; i < sz(perm); i++) {
			for(int j = 0; j < i; j++) {
				val += cost[perm[i]][perm[j]] ;
			}
		}
		return val ; 
	}

	vi solve() {
		totalCost = 0 ; 
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				totalCost += cost[i][j] ; 

		int times = 30 ; vi best(n, -1); int minCost = 2e9 ; 
		for(int iter = 0; iter < times; iter++) {
			vi now = localSearch();
			int _cost = getOrderingCost(best);
			if(_cost < minCost) {
				minCost = _cost ;
				best = now ; 
			}
		}

		printf("Optimal Value: %d\nTotal Cost: %d\n", totalCost - minCost, totalCost);
		return best ; 
	}
};

int main() {
	srand(time(0)); 
	freopen("Lolib/IO/N-stabu75", "r", stdin);
	int n ; cin >> n ;

	vector < vi > matrix(n, vi(n, 0));
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			cin >> matrix[i][j] ; 

	WFAS obj(matrix);
	vi ordering = obj.solve();
	for(int x : ordering) cout << x+1 << " " ;
	cout << endl ;
}

/*
5
0 1 0 1 0
0 0 1 0 0
1 0 0 0 1
1 0 0 0 0
0 0 0 1 0

5 7 
1 2 
2 3 
3 1 
3 5 
5 4 
4 1 
1 4
*/