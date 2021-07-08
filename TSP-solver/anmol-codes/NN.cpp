
/*Anmol Agarwal
CF-tonybanner
*/

/*Nearest neighbour greedy technique for TSP*/
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const LL MOD = 1000000007;
const LL INF = 1e17;
#define lcm(x, y) ((x) * (y) / __gcd((x), (y)))
#define pb push_back
#define all(c) (c).begin(), (c).end()
#define debug(x) cout << #x << " : " << x << endl
#define part cout << "----------------------------------" << endl;
#include <iostream>
#define ff first
#define ss second
struct Point_data
{
    double x, y;
    bool done;
};
vector<struct Point_data> p;
long double tot = 0, n;
vector<int> v;

int dx[] = {1, 1, 0, -1, -1, -1, 0, 1}; // trick to explore an implicit 2D grid
int dy[] = {0, 1, 1, 1, 0, -1, -1, -1}; // S,SE,E,NE,N,NW,W,SW neighbors  //GO FOR EVEN FOR 4 moves

#define fastinput                     \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL);

double POW(double x, LL y)
{
    double ans = 1;
    double base = x;
    while (y)
    {
        if (y & 1)
        {
            ans *= base;
        }
        base = base * base;
        y = y >> 1;
    }
    return ans;
}

double dist(const LL i, const LL j)
{
    return (sqrt(POW(p[i].x - p[j].x, 2) + POW(p[i].y - p[j].y, 2)));
}

int find_nearest_point(int idx)
{
    int min_idx = -1;
    double min_dist = INF;
    for (int i = 0; i < n; i++)
    {
        if (p[i].done)
        {
            continue;
        }
        // cout << "in loop\n";

        double check = dist(idx, i);
        // debug(i);
        // debug(check);
        if (check < min_dist || min_idx == -1)
        {
            min_idx = i;
            check = min_dist;
        }
    }
    return min_idx;
}

int main()
{
    fastinput;
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    LL i, j, k, t, temp, tc;
    cin >> n;
    // debug(n);
    tot = 0;
    p.resize(n);
    for (i = 0; i < n; i++)
    {
        cin >> p[i].x >> p[i].y;
        p[i].done = false;
    }

    v.pb(0);
    p[0].done = true;

    for (i = 1; i < n; i++)
    {
        int now = v[i - 1];
        int next_v = find_nearest_point(now);
        double add = dist(now, next_v);
        // debug(now);
        // debug(next_v);
        // debug(add);

        // part;
        tot += add;
        v.pb(next_v);
        p[next_v].done = true;
    }

    //for n=1, gives 0 anyway
    tot += dist(v[0], v[n - 1]);
    // part;
    // part;
    cout << tot << " 0" << endl;
    for (auto x : v)
    {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}
