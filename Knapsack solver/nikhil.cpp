#include <bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
 
using namespace std ;
using namespace __gnu_pbds;
 
template <typename T> // *s.find_by_order(0), s.order_of_key(2) ;
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
 
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
 
const int N = 1e8 + 2 ;
const int MOD = 998244353;
const lint INF = 1e10 + 3 ;
const int M = 1e4 + 2 ;
const int MAX_LEN = 1e6 ;
 
int n, K ;
vector < pii > items ; 
vector < vi > dp ;
int best = -1 ; 
bitset < M > chosen ;
vi order ;
int OPS = 0 ;
bool itemsTaken[M] ; 
vector < double > densities ; 
 
bool cmp(int i, int j) {
    return densities[i] < densities[j] ; 
}
 
void notFeasible() {
    cout << "0 0" << endl ;
    for(int i = 0; i < n; i++) cout << 0 << " " ;
    cout << endl ;
}
 
double getEstimate(int p, int wt) {
	/* Gets best estimate of remaining items using fractional relaxation*/ 
    double cur = wt ; double value = 0 ;
    for(int i = 0; i < n; i++) {
        if(i <= p) continue ; 
        int j = order[i] ; 
        double take = 1 ;
        if( (cur + items[j].S) > K)
            take = (K - cur)/items[j].S ; 
        value += take * items[j].F ; 
        cur += items[j].S ; 
        if(take != 1) break ; 
    }
    return value ; 
}
 
struct state {
    int index = 0, weight = 0, value = 0;
    bitset < M > mask ; 
    state(int idx, int wt, int val, bitset < M > bitmask) : index(idx), weight(wt), value(val), mask(bitmask) {}
};
 
void bfs() {
    queue < state > q ; bitset < M > dummy ; 
    state init(0, 0, 0, dummy), cur(0, 0, 0, dummy) ; 
    q.push(init); int length ; 
    while(!q.empty()) {
        cur = q.front(); q.pop(); OPS++ ; length = q.size();  
 
        // Ensures code doesn't overwhelm the memory/time 
        assert(OPS < N); assert(length < MAX_LEN);
 
        // Update best
        if(cur.value > best and cur.weight <= K) {
            best = cur.value ;
            chosen = cur.mask ; 
        }
        // State exhausted
        if(cur.index >= n or cur.weight > K) continue ; 
 
        int originalIdx = order[cur.index] ;
        int new_weight = cur.weight + items[originalIdx].S ;
 
        // Estimate of best value achievable with and without the current (index) item 
        double without = getEstimate(cur.index, cur.weight) + cur.value ; 
        double with = getEstimate(cur.index, new_weight) + cur.value + items[originalIdx].F ; 
 
        if(without < with) {
            cur.mask.reset(cur.index);
            if(without > best) q.push(state(cur.index + 1, cur.weight, cur.value, cur.mask));
            cur.mask.set(cur.index);
            if(new_weight <= K and with > best) q.push(state(cur.index + 1, new_weight, cur.value + items[originalIdx].F, cur.mask));
        }
        else {
            cur.mask.set(cur.index);
            if(new_weight <= K and with > best) q.push(state(cur.index + 1, new_weight, cur.value + items[originalIdx].F, cur.mask));
            cur.mask.reset(cur.index);
            if(without > best) q.push(state(cur.index + 1, cur.weight, cur.value, cur.mask));
        }
    }
}
 
void greedyEstimate() {
    /* Get the value achieve by sorting by density and picking height ratio item */
    int wt = 0, val = 0 ; bitset < M > taken ;
    for(int i : order)
        if( (wt + items[i].S) <= K)
            val += items[i].F, wt += items[i].S, taken.set(i);
 
    if(val > best) {
        best = val ;
        chosen = taken ;
    }
}
 
void run() {
    greedyEstimate(); 
    bitset < M > dummy ; OPS = 0 ;
    bfs();
 
    cout << OPS << endl ; 
    cout << best << " " << "1" << endl ;
 
    for(int i = 0; i < n; i++) itemsTaken[order[i]] = chosen.test(i);
 
    int exp = 0 ;
    for(int i = 0; i < n; i++) {
        bool yes = itemsTaken[i] ;
        exp += yes * items[i].F ; 
        cout << yes << " " ;
    }
    cout << endl ;  
    assert(exp == best); 
}
 
int main()
{
    fastio; 
    freopen("tmp.in", "r", stdin); // freopen("tmp.out", "w", stdout);
 
    cin >> n >> K ;
    lint totalWeight = 0, totalValue = 0 ; items.resize(n);
 
    for(int i = 0; i < n; i++) {
        cin >> items[i].F >> items[i].S ;
        assert(items[i].S > 0); 
 
        totalWeight += items[i].S ;
        totalValue += items[i].F ;
 
        order.pb(i); 
        double density = double(items[i].F)/items[i].S ;
        densities.pb(density); 
    }
    sort(all(order), cmp);
    reverse(all(order)); 
    run();
    return 0;   
}