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

LL n, num_edges;
vector<vector<int>> cost;
vector<int> v;
LL violations_now = 0;
unordered_map<string, int> id;
vector<string> id_to_team;
LL iteration_cnt = -1;
LL g_max_pos = -1;
LL tot_cost_initial = 0;

vector<int> v_best_yet;
vector<LL> cum_g;
int cum_g_best_val;
int cum_g_best_pos;
int tot_violations_best = 0;

#define fastinput                     \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL);

void cal_initial_cost()
{
    violations_now = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            violations_now += cost[v[j]][v[i]];
        }
    }
}

void perform_change(int idx, int seeked_idx)
{
    // debug(idx);
    // debug(seeked_idx);
    int node = v[idx];
    if (seeked_idx < idx)
    {
        for (int i = idx; i > seeked_idx; i--)
        {
            v[i] = v[i - 1];
        }
        v[seeked_idx] = node;
    }
    else
    {
        for (int i = idx; i < seeked_idx; i++)
        {
            v[i] = v[i + 1];
        }
        v[seeked_idx] = node;
    }
}

bool find_best_local_move()
{
    LL i, j, k, t, temp;
    LL idx_of_best_node = -1;
    LL seeked_pos_best = -1;
    LL violations_change = INF;
    for (i = 0; i < n; i++)
    {
        LL node_now = v[i];
        LL personal_best_pos = -1;
        LL personal_best_improvement = INF;
        LL curr_change = 0;
        for (j = i - 1; j >= 0; j--)
        {
            LL ver = v[j];
            //i want this to be negative
            curr_change += cost[ver][node_now] - cost[node_now][ver];

            if (curr_change < personal_best_improvement)
            {
                personal_best_improvement = curr_change;
                personal_best_pos = j;
            }
        }
        curr_change = 0;
        for (j = i + 1; j < n; j++)
        {
            LL ver = v[j];
            //i want this to be negative
            curr_change += cost[node_now][ver] - cost[ver][node_now];
            if (curr_change < personal_best_improvement)
            {
                personal_best_improvement = curr_change;
                personal_best_pos = j;
            }
        }

        if (personal_best_improvement < violations_change)
        {
            violations_change = personal_best_improvement;
            idx_of_best_node = i;
            seeked_pos_best = personal_best_pos;
        }
    }

    if (idx_of_best_node == -1)
    {
        cout << "WEIRD STUFF" << endl;
        exit(-1);
        return false;
    }
    else
    {
        // debug(idx_of_best_node);
        // debug(v[idx_of_best_node]);
        // debug(seeked_pos_best);
        // debug(iteration_cnt);
        // debug(violations_change);
        perform_change(idx_of_best_node, seeked_pos_best);
        violations_now += violations_change;
        // debug(violations_now);
        if (iteration_cnt == 0)
        {
            tot_violations_best = violations_now;
            cum_g.clear();
            cum_g.pb(-violations_change);
            // cout << "Pushing " << violations_change << endl;
            v_best_yet = v;
            cum_g_best_pos = 0;
            cum_g_best_val = cum_g[0];
            // debug(cum_g.size());
            // debug(cum_g_best_val);
        }
        else
        {
            LL to_add = cum_g[cum_g.size() - 1] - violations_change;
            // cout << "Pushing " << to_add << endl;
            cum_g.pb(to_add);
            if (to_add > cum_g_best_val)
            {
                cum_g_best_val = to_add;
                cum_g_best_pos = iteration_cnt;
                v_best_yet = v;
                tot_violations_best = violations_now;
            }
            // debug(cum_g.size());

            // debug(cum_g_best_val);
        }

        // for (auto x : v)
        // {
        //     cout << x << " ";
        // }
        // cout << endl;
        // debug(violations_now);
        // part;
        return true;
    }
}

//cost(i,j)-> no of judgements where i better than j
int main()
{
    fastinput;
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    LL i, j, k, t, temp;
    cin >> n;
    cost.assign(n, vector<int>(n, 0));
    id_to_team.resize(n);
    for (i = 0; i < n; i++)
    {
        // cin >> j;
        v.pb(n-1-i);
    }

    // cout << "Suggested ordering is" << endl;
    // for (auto x : v)
    // {
    //     cout << x << " ";
    // }
    // cout << endl;

    LL cnt = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cin >> cost[i][j];
            if (i == j)
            {
                cost[i][j] = 0;
            }
            tot_cost_initial += cost[i][j];
        }
    }

    //outputting edges
    // part;
    // for (i = 0; i < n; i++)
    // {
    //     for (j = 0; j < n; j++)
    //     {
    //         if (i != j)
    //         {
    //             if (cost[i][j] != 0)
    //             {
    //                 tot_cost_initial += cost[i][j];
    //                 cout << i << " " << j << " " << cost[i][j] << endl;
    //             }
    //         }
    //     }
    // }
    // part;

    cal_initial_cost();
    // cout << "NOW number of violations is " << violations_now << endl;
    // debug(tot_cost_initial);
    // debug(violations_now);
    // cout << "sub is " << tot_cost_initial - violations_now << endl;
    // part;
    // return 0;
    // for (auto x : v)
    // {
    //     cout << id_to_team[x] << endl;
    // }
    // part;
    while (true)
    {
        // debug(violations_now);
        iteration_cnt = (iteration_cnt + 1) % n;

        if (iteration_cnt == n - 1)
        {
            //restore bets of previous stint
            //reinit others
            // debug(cum_g_best_val);
            if (cum_g_best_val <= 0)
            {
                break;
            }

            violations_now = tot_violations_best;
            // cout << "BRAKE\n";
            // debug(iteration_cnt);
            // debug(violations_now);
            v = v_best_yet;
            v_best_yet.clear();
        }
        else
        {
            find_best_local_move();
        }
        // part;
    }

    // part;
    // part;
    // cout << "RESULT-------------\n";
    //   cout << "initial number of violations is " << violations_now << endl;
    // debug(tot_cost_initial);
    // debug(violations_now);
    cout << tot_cost_initial - violations_now << endl;
    for (auto x : v)
    {
        cout << x << " ";
    }
    cout << endl;
    // for (auto x : v)
    // {
    //     cout << id_to_team[x] << endl;
    // }

    return 0;
}
