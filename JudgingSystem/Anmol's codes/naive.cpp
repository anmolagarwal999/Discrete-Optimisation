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
vector<int> v, v_best;
LL violations_now = 0;
unordered_map<string, int> id;
vector<string> id_to_team;

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
    //shifting the elements of the ordering based on whether the insertion is in the beginning or in the end
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
    LL best_improvement = -1;
    for (i = 0; i < n; i++)
    {
        LL node_now = v[i];
        LL personal_best_pos = -1;
        LL personal_best_improvement = -1;
        LL curr_change = 0;
        for (j = i - 1; j >= 0; j--)
        {
            LL ver = v[j];
            //maintaining prefix sum array
            curr_change += cost[ver][node_now] - cost[node_now][ver];
            if (curr_change < 0)
            {
                if (fabs(curr_change) > personal_best_improvement)
                {
                    personal_best_improvement = fabs(curr_change);
                    personal_best_pos = j;
                }
            }
        }
        curr_change = 0;
        for (j = i + 1; j < n; j++)
        {
            LL ver = v[j];
            //maintaining prefix sum array
            curr_change += cost[node_now][ver] - cost[ver][node_now];
            if (curr_change < 0)
            {
                if (fabs(curr_change) > personal_best_improvement)
                {
                    personal_best_improvement = fabs(curr_change);
                    personal_best_pos = j;
                }
            }
        }

        if (personal_best_improvement > best_improvement)
        {
            best_improvement = personal_best_improvement;
            idx_of_best_node = i;
            seeked_pos_best = personal_best_pos;
        }
    }
    //returning false if no pissble improvement found
    if (idx_of_best_node == -1)
    {
        return false;
    }
    else
    {     
        //improvement found by changing vertex at position 'idx_of_best_node' to position 'seeked_pos_best'
        //performing the change to reflect the insertion
        perform_change(idx_of_best_node, seeked_pos_best);
        violations_now -= best_improvement;
        return true;
    }
}
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

//cost(i,j)-> no of judgements where i better than j
int main()
{
    fastinput;
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    LL i, j, k, t, temp;
    LL tot_cost_initial = 0;
    cin >> n;
    cost.assign(n, vector<int>(n, 0));
    id_to_team.resize(n);
    for (i = 0; i < n; i++)
    {
        // cin >> j;
        v.pb(i);
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
    // cout << "initial number of violations is " << violations_now << endl;
    // debug(tot_cost_initial);
    // debug(violations_now);
    // cout << "sub is " << tot_cost_initial - violations_now << endl;
    // return 0;
    // for (auto x : v)
    // {
    //     cout << id_to_team[x] << endl;
    // }
    // part;
    while (true)
    {
        // debug(violations_now);
        if (find_best_local_move() == false)
        {
            break;
        }
    }

//     part;
//     part;
//     cout << "RESULT-------------\n";
//  //   cout << "initial number of violations is " << violations_now << endl;
//     debug(tot_cost_initial);
//     debug(violations_now);
    cout << tot_cost_initial - violations_now << endl;
    for (auto &x : v)
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
