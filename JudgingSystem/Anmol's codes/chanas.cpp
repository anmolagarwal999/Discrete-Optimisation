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
LL uneven = 0;

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
       // cout << "WEIRD STUFF IN GEORGIA" << endl;
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
            //shift elements and update violations now
            perform_change(i, best_pos);
            violations_now += best_diff;
        }
    }
    // debug(violations_at_beginning);
    // debug(violations_now);
    return {violations_at_beginning, violations_now};
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
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (cost[i][j] != 0 && cost[j][i] != 0)
            {
                uneven++;
            }
        }
    }

    /////////////////////////////
    // part;
    // part;
    // cout << "RESULT-------------\n";
    // //   cout << "initial number of violations is " << violations_now << endl;
    // debug(violations_now);
    cout << tot_cost_initial - violations_now << endl;
    for (auto x : v)
    {
        cout << x << " ";
    }
    cout << endl;
    // debug(uneven);
    // for (auto x : v)
    // {
    //     cout << id_to_team[x] << endl;
    // }

    return 0;
}
