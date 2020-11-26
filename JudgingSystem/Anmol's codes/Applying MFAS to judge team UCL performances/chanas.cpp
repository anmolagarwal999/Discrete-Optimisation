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
LL curr_set = 0;
unordered_map<string, int> id;
unordered_map<int, string> id_to_team;
LL uneven = 0;

int threshold = 20;
vector<string> t1, t2;
vector<int> wt;
map<int, int> occur;

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
        exit(-2);
        for (int i = idx; i < seeked_idx; i++)
        {
            v[i] = v[i + 1];
        }
        v[seeked_idx] = node;
    }
}
pair<LL, LL> sort_star()
{
    LL violations_at_beginning = violations_now;
    for (int i = 0; i < n; i++)
    {
        int idx_to_be_moved = i;
        LL lb = 0, ub = i - 1;
        LL best_diff = 0, curr_diff = 0;
        int best_pos = i;
        int n1 = v[i];
        for (int j = ub; j >= 0; j--)
        {
            int n2 = v[j];
            curr_diff += cost[n2][n1] - cost[n1][n2];
            if (curr_diff < best_diff)
            {
                best_diff = curr_diff;
                best_pos = j;
            }
        }

        if (best_pos != i)
        {
            perform_change(i, best_pos);
            violations_now += best_diff;
        }
    }

    return {violations_at_beginning, violations_now};
}
void print_ids()
{
    part;
    cout << "id mapping is \n";
    for (auto &x : id)
    {
        if (occur[x.second] >= threshold)
            cout << x.first << " " << x.second << "  occur:" << occur[x.second] << endl;
    }
    part;
}
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
            if(occur[i]<threshold || occur[j]<threshold)
            {
                continue;
            }
            if(cost[i][j]==0)
            {
                continue;
            }
            cout<<id_to_team[i]<<" "<<id_to_team[j]<<" "<<cost[i][j]<<endl;
        }
    }
}
int main()
{
    fastinput;

    LL i, j, k, t, temp;
    LL tot_cost_initial = 0;

    LL num_lines = 1050;
    // cin>>num_lines;
    string s1, s2;
    int sc;
    for (i = 0; i < num_lines; i++)
    {
        cin >> t >> s1 >> s2 >> temp >> sc;
        t1.pb(s1);
        t2.pb(s2);
        wt.pb(sc);
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
    // debug(n);
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
    // debug(tot_cost_initial);
    // cout << "initial number of violations is " << violations_now << endl;
    // debug(violations_now);
    // cout << "SUM OF NON MFAS COSTS " << tot_cost_initial - violations_now << endl;
    // part;
    //take initial and set it as global best
    //sort_star();
    //set this as global best
    pair<LL, LL> check;
    check = sort_star();
    LL v_have = violations_now;
    cal_initial_cost();
    LL v_there = violations_now;
    // debug(v_have);
    // debug(v_there);

    // part;
    // cout << "AFTER INTIAL SORT_STAR" << endl;
    // debug(violations_now);
    // for (auto x : v)
    // {
    //     cout << x << " ";
    // }
    // cout << endl;

    // part;
    // goto here;
    while (true)
    {
        if (check.first == check.second)
        {
            //cout << "FOUND SAME\n";
            break;
        }
        else if (check.first < check.second)
        {
            cout << "WEIRD STUFF ALASKA" << endl;
            exit(-1);
        }
        else
        {
            // here:
            LL before_reversal = violations_now;
            reverse(all(v));
            LL easy_cal = tot_cost_initial - violations_now;
            cal_initial_cost();
            if (violations_now != easy_cal)
            {
                // debug(tot_cost_initial);
                // debug(easy_cal);
                // debug(violations_now);
                // cout << "WEIRD STUFF MIAMI" << endl;
                // exit(-1);
            }
            check = sort_star();
            if (violations_now == before_reversal)
            {
                break;
            }
        }
    }

    ///////////////////////////////////////
    // for (i = 0; i < n; i++)
    // {
    //     for (j = 0; j < n; j++)
    //     {
    //         if (cost[i][j] != 0 && cost[j][i] != 0)
    //         {
    //             uneven++;
    //         }
    //     }
    // }

    /////////////////////////////
    // part;
    // part;
    // cout << "RESULT-------------\n";
    // //   cout << "initial number of violations is " << violations_now << endl;
    // debug(violations_now);
    // part;
    // part;
    // debug(tot_cost_initial);
    // debug(violations_now);
    cout << tot_cost_initial - violations_now << endl;
    for (auto x : v)
    {
        cout << id_to_team[x] << endl;
    }
    cout << endl;
    // print_edges();

    return 0;
}
