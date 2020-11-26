

/*Anmol Agarwal
CF-tonybanner
*/

/*2-factor approximation algorithm without iteration*/
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
    int dest_v = -1;
    double cost = INF;
};

double tot = 0;
int n;
vector<struct Point_data> p;
vector<int> v;
vector<bool> visited;
vector<vector<LL>> adj;
LL sz = 0;

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

void dfs(LL s)
{
    if (!visited[s])
    {
        visited[s] = true;
        v.pb(s);
        sz++;
        if (sz >1)
        {
            tot += dist(s, v[sz - 2]);
        }

        for (auto &x : adj[s])
        {
            dfs(x);
        }
    }
}

int main()
{
    // fastinput;
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    // cout<<"Messi\n"<<endl;;
    ifstream infile;
	infile.open("input_tsp.txt");
    LL i, j, k, t, temp, tc;
    infile >> n;
    // debug(n);
    tot = 0;
    p.resize(n);
    visited.assign(n, false);
    adj.assign(n, vector<LL>());
    for (i = 0; i < n; i++)
    {
        infile >> p[i].x >> p[i].y;
        p[i].done = false;
    }
    infile.close();

    for (i = 0; i < n; i++)
    {
        int curr;
        if (i == 0)
        {
            curr = 0;
            goto update;
        }
        curr = -1;
        for (j = 0; j < n; j++)
        {
            if (!p[j].done && (curr == -1 || p[j].cost < p[curr].cost))
            {
                curr = j;
            }
        }

        if (p[curr].dest_v != -1)
        {
            adj[curr].pb(p[curr].dest_v);
            adj[p[curr].dest_v].pb(curr);
            //tot += p[curr].cost;
            // cout << "edge :" << curr << " " << p[curr].dest_v << endl;
        }
    update:
        p[curr].done = true;
        // debug(curr);
        for (j = 0; j < n; j++)
        {
            if (j != curr)
            {
                double check = dist(curr, j);
                if (check < p[j].cost)
                {
                    p[j].cost = check;
                    p[j].dest_v = curr;
                }
            }
        }
        // part;
    }
    dfs(0);
    // part;

    tot += dist(v[0], v[n - 1]);
    ofstream outfile;
	outfile.open("output_tsp.txt",ofstream::out);
    outfile << tot << " 0" << endl;
    for (auto x : v)
    {
        outfile << x << " ";
    }
    outfile << endl;

    return 0;
}
