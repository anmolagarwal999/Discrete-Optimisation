/* 2-OPT on best approx intitialization*/

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

vector<LL> node_minima;
LL tot_minima = INF, min_pos = -1;
LL last_node = -1;

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

double POW(double x, int y)
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
double dist(const int i, const int j)
{
    return (sqrt(POW(p[i].x - p[j].x, 2) + POW(p[i].y - p[j].y, 2)));
}

void dfs(int s)
{
    if (!visited[s])
    {
        visited[s] = true;
        v.pb(s);
        sz++;
        if (sz > 1)
        {
            // cout << "edge " << s << " " << v[sz - 2] << endl;
            tot += dist(s, v[sz - 2]);
        }

        for (auto x : adj[s])
        {
            dfs(x);
        }
    }
}
void modify_vector(int pos1, int pos2)
{
    while (pos1 < pos2)
    {
        swap(v[pos1], v[pos2]);
        pos1++;
        pos2--;
    }
}
bool try_2_opt()
{
    int i, j, t1, t2, t3, t4;
    int pos1 = -1, pos2 = -1;
    double now, poss;
    double diff = -1;
    double max_diff = -1;
    for (t2 = 0; t2 < n; t2++)
    {
        for (t4 = t2 + 1; t4 < n - (t2 == 0); t4++)
        {
            t1 = ((t2 - 1) + n) % n;
            t3 = (t4 + 1) % n;
            now = dist(v[t1], v[t2]) + dist(v[t3], v[t4]);
            poss = dist(v[t1], v[t4]) + dist(v[t2], v[t3]);
            diff = now - poss;
            if (diff > 0)
            {
                if (diff > max_diff)
                {
                    max_diff = diff;
                    pos1 = t2;
                    pos2 = t4;
                }
            }
        }
    }

    if (pos1 == -1 || pos2 == -1)
    {
        return false;
    }
    else
    {
        // debug(pos1);
        // debug(pos2);
        tot -= max_diff;
        modify_vector(pos1, pos2);
        return true;
    }
}
int main()
{
    fastinput;
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    // cout<<"Messi\n"<<endl;;
    int i, j, k, t, temp, tc;
    ifstream infile;
	infile.open("input_tsp.txt");
    infile >> n;
    // debug(n);
    tot = 0;
    p.resize(n);
    node_minima.resize(n);
    visited.assign(n, false);
    adj.assign(n, vector<LL>());
    for (i = 0; i < n; i++)
    {
        infile >> p[i].x >> p[i].y;
        p[i].done = false;
        node_minima[i] = INF;
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
    // return 0;

    for (i = 0; i < n; i++)
    {

        // debug(i);
        tot = 0;
        last_node = -1;
        visited.assign(n, false);
        sz = 0;
        v.clear();
        dfs(i);
        tot += dist(i, last_node);
        // debug(tot);
        if (tot < tot_minima)
        {

            tot_minima = tot;
            min_pos = i;
        }
        // debug(i);
        // debug(tot);
        // part;
        if (n > 33809)
        {
            break;
        }
    }
    sz = 0;
    v.clear();
    visited.assign(n, false);

    tot = 0;
    dfs(min_pos);
    tot += dist(v[0], v[n - 1]);

    // debug(tot_minima);
    // debug(tot);
    // cout << tot_minima << " 0" << endl;
    // for (auto x : v)
    // {
    //     cout << x << " ";
    // }
    // cout << endl;

    // part;
    bool improved = false;
    int iterations = 0;
    LL limit = INF;
    if (n > 33809)
    {
        limit = 0;
    }
    while (iterations < limit)
    {
        // debug(iterations);
        iterations++;
        improved = false;
        improved = try_2_opt();
        // debug(iterations);
        if (!improved)
        {
            break;
        }

        // part;
        // debug(tot);
        // for (auto x : v)
        // {
        //     cout << x << " ";
        // }
        // cout << endl;

        // part;
    }
    ofstream outfile;
	outfile.open("output_tsp.txt",ofstream::out);
    outfile << tot << " 0" << endl;
    for (auto x : v)
    {
        outfile << x << " ";
    }
    outfile << endl;
    // debug(iterations);

    return 0;
}
