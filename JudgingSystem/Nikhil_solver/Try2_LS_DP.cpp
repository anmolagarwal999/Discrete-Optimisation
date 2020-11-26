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
int koptDP[1 << N] ;

class WFAS {
	static const int K = N ; int MAX_ITER = 10000 ; 
	// int submaskCost[K][1 << K] ;
	// int dp[1 << K] ;
	vector < vi > cost ; int totalCost ;
	int n = -1, k = 1; vi globalPerm ; 
	int K_OPT = 15 ; set < int > indices ; 
	int OFFSET = 2e9 ;

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
		for(int j = 0; j < k; j++) {
			int prev = 0 ; submaskCost[j][prev] = 0 ;
			for(int i = 1; i < (1 << k); i++) {
				int mask = i ^ (i >> 1); // graycode 
				int dif = mask ^ prev ; 
				int pos = __builtin_ctz(dif);

				int on = mask & (1 << pos);
				if(on) submaskCost[j][mask] = submaskCost[j][prev] + cost[perm[j]][perm[pos]] ;
				else   submaskCost[j][mask] = submaskCost[j][prev] - cost[perm[j]][perm[pos]] ;
				prev = mask ; 
			}
		}
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
	        if(!(mask & (1 << i)))
	        {
	            int new_mask = mask | (1 << i) ;
	            if(dp[mask] == submaskCost[i][mask] + dp[new_mask]) {
	                vi nxt = getDPOrdering(perm, new_mask);
	                nxt.insert(nxt.begin(), perm[i]);
	                return nxt ; 
	            }
	        }
	    return emp ; // is this needed???
	}

	vi permuteDP(vi& perm, int& permCost) {
		for(int i = 0; i < (1 << k); i++) 
			dp[i] = -1 ;
		setSubmaskCosts(perm);
		int oldCost = getOrderingCost(perm);
		int newCost = solveDP(perm);
		vi newOrder = getDPOrdering(perm);
		permCost += newCost - oldCost ; 
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
			swap(perm[i], perm[j]);
		}
	}


	int dpKopt(int mask, vi& idx, vi& sol, int& _cost) {
		if(__builtin_popcount(mask) == sz(idx)) return koptDP[mask] = 0 ;
		if(koptDP[mask] != OFFSET) return koptDP[mask] ;

		int taken = __builtin_popcount(mask);
		int best = 1e9 ; 
		for(int i = 0; i < K_OPT; i++) {
			if(mask & (1 << i)) continue ; 

			int old_pos = idx[i] ;
			int new_pos = idx[taken] ; 
			int delta = 0 ;

			if(old_pos < new_pos) {
				for(int j = old_pos + 1; j <= new_pos; j++) {
					// if(indices.find(j) != indices.end()) continue ;
					delta += cost[sol[idx[i]]][sol[j]] ;
					delta -= cost[sol[j]][sol[idx[i]]] ;
				}
			}
			else {
				for(int j = old_pos - 1; j >= new_pos; j--) {
					if(indices.find(j) != indices.end()) continue ;
					delta -= cost[sol[idx[i]]][sol[j]] ;
					delta += cost[sol[j]][sol[idx[i]]] ;
				}
			}

			for(int j = 0; j < n; j++) {
				if(i == j) continue ;
				if(!(mask & (1 << j))) continue ; 

			}

			int new_mask = mask | (1 << i) ;
			reMin(best, delta + dpKopt(new_mask, idx, sol, _cost));
		}
		return koptDP[mask] = best ;
	}

	vi getKOPTordering(int mask, vi& idx, vi& sol, int& _cost) {
		vi emp ; 
		if(__builtin_popcount(mask) == sz(idx)) return emp ;

		int taken = __builtin_popcount(mask);
		int best = 1e9 ; 
		for(int i = 0; i < K_OPT; i++) {
			if(mask & (1 << i)) continue ; 

			int old_pos = idx[i] ;
			int new_pos = idx[taken] ; 
			int delta = 0 ;

			if(old_pos < new_pos) {
				for(int j = old_pos + 1; j <= new_pos; j++) {
					// if(indices.find(j) != indices.end()) continue ;
					delta += cost[sol[idx[i]]][sol[j]] ;
					delta -= cost[sol[j]][sol[idx[i]]] ;
				}
			}
			else {
				for(int j = old_pos - 1; j >= new_pos; j--) {
					if(indices.find(j) != indices.end()) continue ;
					delta -= cost[sol[idx[i]]][sol[j]] ;
					delta += cost[sol[j]][sol[idx[i]]] ;
				}
			}

			int new_mask = mask | (1 << i) ;
			if(koptDP[mask] == delta + koptDP[new_mask]) {
                vi nxt = getKOPTordering(new_mask, idx, sol, _cost);
                nxt.insert(nxt.begin(), idx[i]);
                return nxt ; 
			}
		}
	}


	void doKopt(vi& sol, int& currentCost) {
		indices.clear();
		while(sz(indices) < K_OPT) {
			int idx = rand() % n ;	
			indices.insert(idx);
		}
		vi indexs ;
		for(int x : indices) indexs.pb(x);

		for(int i = 0; i < (1 << K_OPT); i++)
			koptDP[i] = OFFSET ;

		fprintf(stderr, "Time to do K-OPT\n");
		int delta = dpKopt(0, indexs, sol, currentCost);
		cerr << "yoohoo " ; 
		vi order = getKOPTordering(0, indexs, sol, currentCost);

		vi better = sol;
		for(int i = 0; i < K_OPT; i++) {
			better[indexs[i]] = sol[order[i]] ;
		}
		sol = better ; 
		fprintf(stderr, "%d\n", delta);
		currentCost += delta ; 

		int expec = getOrderingCost(sol);
		cerr << expec << " " << currentCost << endl ; 
		assert(expec == currentCost);
	}

	vi localSearch() {
		vi current = greedyOrdering(); 
		for(int i = 0; i < n; i++) current[i] = i ;
		random_shuffle(all(current));

		int currentCost = getOrderingCost(current);
		int iter = 0 ;

		vi best = current ;
		int bestCost = currentCost ;

		vector < pii > pairs ;
		for(int i = 0; i < n; i++) for(int j = i+1; j < n; j++) pairs.pb({i, j});
		random_shuffle(all(pairs));

		int MAX_INNER_ITER = min(sz(pairs), 3*k*(1 << k)/n) ;
		cerr << MAX_INNER_ITER << endl ; 
		int idx = 0 ; int megaDP = 0; int stuckIter = 0 ;  

		fprintf(stderr, "Iterations: %d\tCost: %d\n", iter++, totalCost - currentCost);
		for(; iter < MAX_ITER; iter++) {

			int bef = currentCost ;

			int prev ;
			do {
				prev = currentCost ;
				improveUsingDP(current, currentCost);
			} while(currentCost < prev); 


			for(int cnt = 0; cnt < MAX_INNER_ITER; cnt++) {
				prev = currentCost ;
				evaluateSwap(pairs[idx].F, pairs[idx].S, current, currentCost);
				idx++; idx %= sz(pairs); 
			}

			// if(rand() % 100 == 1) {
			// 	int i = rand() % n ;
			// 	int j = rand() % n ;
			// 	swap(current[i], current[j]);
			// 	currentCost = getOrderingCost(current); 
			// }

			if(stuckIter >= 10) {
				doKopt(current, currentCost); 
			}

			if(bef == currentCost) stuckIter++ ;
			else stuckIter = 0 ;

			// stuck at local minima!!! no point trying further 
			if(stuckIter > n/2) break ; 

			if(iter % 10 == 1)
				fprintf(stderr, "Iterations: %d\tCost: %d\n", iter, totalCost - currentCost);
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
		// MAX_ITER = 1000 ;

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
			int _cost = getOrderingCost(now);
			if(_cost < minCost) {
				minCost = _cost ;
				best = now ; 
			}
			printf("Global Iterations: %d Best Cost: %d\n", iter, totalCost - minCost);
		}

		printf("Optimal Value: %d\nTotal Cost: %d\n", totalCost - minCost, totalCost);
		return best ; 
	}
};

int main() {
	srand(time(0)); 
	// freopen("Lolib/IO/N-usa79", "r", stdin);
	freopen("Lolib/IO/N-stabu70", "r", stdin); 
	// freopen("input.in","r", stdin);
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