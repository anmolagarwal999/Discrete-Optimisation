// Guided local search method of solving TSP
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
double best_tot = -1;
vector<int> v_best;
double tot = 0, master_tot = 0;
double lambda = 0;
double alpha = 30;
int n;
vector<struct Point_data> p;
vector<int> v;
bool improved = false;
vector<bool> visited;
vector<vector<int>> adj;
vector<vector<int>> penalty;
int sz = 0;
double local_val = 0;
LL n_features;

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
vector<double> util;

void penalize_features(double g_val)
{
    int src, dest;
    int max_pos = -1;
    double curr_max = -1;
    for (int i = 0; i < n; i++)
    {
        src = v[i];
        dest = v[(i + 1) % n];
        util[i] = (dist(src, dest)) / (1 + penalty[src][dest]);
        if (util[i] > curr_max)
        {
            max_pos = i;
            curr_max = util[i];
        }
    }

    if (max_pos == -1)
    {
        cout << "WEIRD STUFF\n";
        exit(0);
    }
    src = v[max_pos];
    dest = v[(max_pos + 1) % n];
    penalty[src][dest]++;
    penalty[dest][src]++;
    lambda = alpha * (local_val) / n_features;
    // debug(lambda);
    // debug(tot);
}
bool try_2_opt()
{
    int i, j, t1, t2, t3, t4;
    int pos1 = -1, pos2 = -1;
    double now, poss;
    double diff = -1;
    double a1, a2;
    double max_diff = -1, diff_store = 0, use_diff_store;
    for (t2 = 0; t2 < n; t2++)
    {
        for (t4 = t2 + 1; t4 < n - (t2 == 0); t4++)
        {
            t1 = ((t2 - 1) + n) % n;
            t3 = (t4 + 1) % n;
            // debug(penalty[t1][t2]);
            // debug(penalty[t3][t4]);

            double d_now = dist(v[t1], v[t2]) + dist(v[t3], v[t4]);
            double d_poss = dist(v[t1], v[t4]) + dist(v[t2], v[t3]);
            double pen_now = lambda * (penalty[v[t1]][v[t2]] + penalty[v[t3]][v[t4]]);
            double pen_poss = lambda * (penalty[v[t1]][v[t4]] + penalty[v[t2]][v[t3]]);
            now = d_now + pen_now;
            poss = d_poss + pen_poss;
            diff_store = d_now - d_poss;
            diff = now - poss;
            if (diff > 0)
            {
                //debug(diff);
                if (diff > max_diff)
                {
                    max_diff = diff;
                    pos1 = t2;
                    pos2 = t4;
                    // a1 = penalty[t1][t2];
                    // a2 = penalty[t3][t4];
                    use_diff_store = diff_store;
                }
            }
        }
    }

    if (pos1 == -1 || pos2 == -1)
    {
        local_val = tot;
        return false;
    }
    else
    {
        // debug(pos1);
        // debug(pos2);
        // debug(a1);
        // debug(a2);
        // debug(use_diff_store);
        tot -= use_diff_store;
        // debug(diff);

        // debug(tot);
        modify_vector(pos1, pos2);
        // for (auto x : v)
        // {
        //     cout << x << " ";
        // }
        // cout << endl;
        // part;
        if (tot < best_tot)
        {
            best_tot = tot;
            v_best = v;
        }
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
    visited.assign(n, false);
    util.assign(n, 0.0);
    adj.assign(n, vector<int>());
    penalty.assign(n, vector<int>(n, 0));
    n_features = (n * (n - 1)) / 2;

    /////////////////////////////////
    lambda = 0;
    ////////////////////////////////
    for (i = 0; i < n; i++)
    {
        infile >> p[i].x >> p[i].y;
        p[i].done = false;
    }

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
    int iterations = 0;
    // debug(tot);
    best_tot = tot;
    v_best = v;
    // part;
    // cout<<"INITIAL STUFF\n";
    // debug(tot);
    // for (auto x : v)
    // {
    //     cout << x << " ";
    // }
    // cout << endl;

    // part;
    LL limit = 20000;
    if (n > 1000)
    {
        limit =10000;
    }
    if (n > 10000)
    {
        limit = 5;
    }
    while (iterations < limit)
    {
        iterations++;
        improved = false;
        improved = try_2_opt();
        if (!improved)
        {
            penalize_features(local_val);
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

    // cout << tot << " 0" << endl;
    // for (auto x : v)
    // {
    //     cout << x << " ";
    // }
    // cout << endl;
    // debug(best_tot);
    // debug(iterations);
    ofstream outfile;
	outfile.open("output_tsp.txt",ofstream::out);
    outfile << best_tot << " 0" << endl;
    for (auto x : v_best)
    {
        outfile << x << " ";
    }
    outfile << endl;

    return 0;
}
