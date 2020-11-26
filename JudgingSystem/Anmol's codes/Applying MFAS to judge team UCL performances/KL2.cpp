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
LL iteration_cnt = -1;
LL g_max_pos = -1;
LL tot_cost_initial = 0;
LL curr_set = 0;
unordered_map<string, int> id;
unordered_map<int, string> id_to_team;
LL uneven = 0;

int threshold = 20;
vector<string> t1, t2;
vector<int> wt;
map<int, int> occur;
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
void print_ids()
{
    part;
    cout << "id mapping is \n";
    LL cnt = 0;
    for (auto &x : id)
    {
        if (occur[x.second] >= threshold)
            cout << cnt++ << "->" << x.first << " " << x.second << "  occur:" << occur[x.second] << endl;
    }
    part;
}
LL num_lines = 1050;
void print_edges()
{
    // for (int i = 0; i < num_lines; i++)
    // {
    //     int id1 = id[t1[i]];
    //     int id2 = id[t2[i]];
    //     int sc = wt[i];
    //     if (occur[id1] < threshold || occur[id2] < threshold)
    //     {
    //         continue;
    //     }
    //     cout<<t1[i]<<" "<<t2[i]<<
    //     // cost[id1][id2] += sc;
    //     // tot_cost_initial += sc;
    // }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int v1=v[i];
            int v2=v[j];
            if (occur[v1] < threshold || occur[v2] < threshold)
            {
                continue;
            }
            if (cost[v1][v2] == 0)
            {
                continue;
            }
            cout << id_to_team[v1] << " " << id_to_team[v2] << " " << cost[v1][v2] << endl;
        }
    }
}
//cost(i,j)-> no of judgements where i better than j
int main()
{
    fastinput;
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    LL i, j, k, t, temp;
    LL tot_cost_initial = 0;

    // cin>>num_lines;
    string s1, s2;
    int sc;
    for (i = 0; i < num_lines; i++)
    {
        cin >> t >> s1 >> s2 >> temp >> sc;
        t1.pb(s1);
        t2.pb(s2);
        wt.pb(sc);
        // if (s1 == "Bayern_München_" && s2 == "FC_Barcelona_")
        // {
        //     cout << "---------------------sc is " << sc << endl;
        // }
        if (id.find(s1) == id.end())
        {
            id[s1] = curr_set++;
            id_to_team[curr_set - 1] = s1;
        }
        if (id.find(s2) == id.end())
        {
            id[s2] = curr_set++;
            id_to_team[curr_set - 1] = s2;
        }
        occur[id[s1]]++;
        occur[id[s2]]++;
    }
    n = id.size();
    // debug(n);
    // debug(curr_set);
    print_ids();
    //cin >> n;

    int fake_n = n;
    n = 0;
    for (i = 0; i < curr_set; i++)
    {
        if (occur[i] >= threshold)
        {
            n++;
        }
    }
    // part;
    // part;
    //debug(n);
    print_ids();

    // part;
    // part;
    cost.assign(fake_n, vector<int>(fake_n, 0));

    LL cnt = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cost[i][j] = 0;
        }
    }
    for (i = 0; i < num_lines; i++)
    {
        int id1 = id[t1[i]];
        int id2 = id[t2[i]];
        int sc = wt[i];
        if (occur[id1] < threshold || occur[id2] < threshold)
        {
            continue;
        }
        cost[id1][id2] += sc;
        tot_cost_initial += sc;
    }

    for (i = 0; i < fake_n; i++)
    {
        // cin >> j;
        if (occur[i] >= threshold)
        {
            v.pb(i);
        }
    }
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
    cout << "RESULT-------------\n";
    //   cout << "initial number of violations is " << violations_now << endl;
    // debug(tot_cost_initial);
    // debug(violations_now);
    cout << "sub is " << tot_cost_initial - violations_now << endl;
    cnt = 1;
    for (auto x : v)
    {
        cout << cnt++ << " " << id_to_team[x] << " " << endl;
    }
    cout << endl;
    // for (auto x : v)
    // {
    //     cout << id_to_team[x] << endl;
    // }
    // print_edges();

    // int id_bar = id["FC_Barcelona_"];
    // int id_bate = id["BATE_Borisov_"];
    // debug(id_bar);
    // debug(id_bate);
    // debug(cost[id_bar][id_bate]);

    return 0;
}
