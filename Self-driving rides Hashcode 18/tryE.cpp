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
typedef pair < double, double > point ;

const int N = 2000 + 2 ;
const int M = 3000  + 2 ;
const lint INF = 1e18 ;
lint MAX_OPS = 1e8 ; 

int n, m, R, C, B, T ;

struct ride {
    pii start, stop ;
    int earliest, latest ;
    int index ;
    int done = 0 ; 
};
vector < ride > rides ;

struct vehicle {
    pii pos = {0, 0};
    int index ; 
    int timePassed = 0 ; 
    vi served ; 
};
vector < vehicle > vehicles ;

bool cmp(ride& x, ride& y) {
    return x.latest < y.latest ;
}

int getDistance(pii x, pii y) {
    return abs(x.F - y.F) + abs(x.S - y.S);
}

void outputSol() {
    int cnt =0 ;
    for(int i = 0; i < m; i++) {
        vehicle& v = vehicles[i] ;
        cout << sz(v.served) ;
        for(auto x : v.served) cout << " " << x ;
        cout << endl ; 
        if(!sz(v.served)) cnt++ ;
    }
    cerr << cnt << endl ; 
}

int main(int argc, char* argv[])
{
    srand(time(0)); 
    freopen(argv[1], "r", stdin); 

    cin >> R >> C >> m >> n >> B >> T ;

    rides.resize(n); vehicles.resize(m);
    for(int i = 0; i < n; i++) {
        cin >> rides[i].start.F >> rides[i].start.S >> rides[i].stop.F >> rides[i].stop.S >> rides[i].earliest >> rides[i].latest ;
        rides[i].index = i ; 
    }

    sort(all(rides), cmp);
    vector < ride > remaining ; 
    for(int j = 0; j < m; j++) {

        pii last = {0, 0} ;
        vehicle& v = vehicles[j] ;

        for(int i = 0; i < n; i++) {
            ride& cur = rides[i] ;
            if(cur.done) continue ;

            if( (v.timePassed + getDistance(cur.start, v.pos) ) > cur.earliest )
                continue ;

            v.timePassed += getDistance(cur.start, v.pos);
            v.timePassed += max(0, cur.earliest - v.timePassed);
            v.timePassed += getDistance(cur.start, cur.stop);

            cur.done = 1 ;
            v.served.pb(cur.index);
            v.pos = cur.stop ; 
        }
    }

    // outputSol();
    for(int j = 0; j < n; j++) {
        ride& cur = rides[j] ;
        if(cur.done) continue ;
        int bestIndex = -1 ;
        int best = 0 ;

        // cout << cur.start.F << " " << cur.start.S << " " << cur.stop.F << " " << cur.stop.S << " " << cur.earliest << " " << cur.latest << stopl ;

        for(int i = 0; i < m; i++) {
            vehicle& v = vehicles[i] ;

            if( (v.timePassed + getDistance(cur.start, v.pos) + getDistance(cur.start, cur.stop) ) > cur.latest )
                continue ;

            v.timePassed += getDistance(cur.start, v.pos);
            v.timePassed += max(0, cur.earliest - v.timePassed);
            v.timePassed += getDistance(cur.start, cur.stop);

            cur.done = 1 ;
            v.served.pb(cur.index);
            v.pos = cur.stop ; 
            break ; 
        }
    }


    // for(int i = 0; i < m; i++) {
    //     vehicle& v = vehicles[i] ;
    //     v.timePassed = 0 ;

    //     vi served ;
    //     for(int j : v.served) {
    //         ride& cur = rides[j] ;
    //         if(cur.done == 1) {
    //             cerr << cur.index << " " << i << endl ;

    //         }
    //         if( (v.timePassed + getDistance(cur.start, v.pos) + getDistance(cur.start, cur.stop) ) > cur.latest )
    //         {
    //             cerr << "Error!\n" ; 
    //         }

    //         v.timePassed += getDistance(cur.start, v.pos);
    //         int waitingTime =  max(0, cur.earliest - v.timePassed);

    //         bool found = 1;
    //         vi between ;
    //         while(found) {
    //             found = 0 ; 
    //             for(int k = 0; k < n; k++) {
    //                 ride& now = rides[k] ;
    //                 if(now.done) continue ;

    //                 if( (getDistance(cur.start, now.start) + getDistance(now.start, now.stop) ) > now.latest )
    //                     continue ;
    //                 if( (getDistance(cur.start, now.start) + getDistance(now.start, now.stop) + getDistance(now.stop, cur.start)) > waitingTime )
    //                     continue ;

    //                 waitingTime -= getDistance(cur.start, now.start) + getDistance(now.start, now.stop) + getDistance(now.stop, cur.start) ;
    //                 now.done = 1 ;
    //                 served.pb(now.index);
    //                 found = 1 ; 
    //             }
    //         }
    //         v.timePassed += max(0, cur.earliest - v.timePassed);
    //         v.timePassed += getDistance(cur.start, cur.stop);

    //         v.pos = cur.stop ; 
    //         served.pb(cur.index);
    //     }
    //     v.served = served ; 
    // }

    int tot = 0 ;
    // cerr << "Rides left:" << endl ; 
    for(int j = 0; j < n; j++) {
        ride& cur = rides[j] ;
        if(cur.done) continue ;
        int bestIndex = -1 ;
        int best = 0 ;

        // cout << cur.start.F << " " << cur.start.S << " " << cur.stop.F << " " << cur.stop.S << " " << cur.earliest << " " << cur.latest << endl ;
        // cout << getDistance(cur.start, cur.stop) << endl << endl ;
        tot += getDistance(cur.start, cur.stop) ;
    }
    cerr << tot << endl ;

    freopen("e_high_bonus.out", "w", stdout);
    outputSol();
    return 0;   
}
