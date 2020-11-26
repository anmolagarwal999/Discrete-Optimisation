// Solving TSP problem using Simulated Annealing method
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

//////////////////////////////
int metrop(double de, double t);
void perform_annealing();
double get_cost(int t2, int t4);
double best_cost = INF;
///////////////////////////////

struct Point_data
{
    double x, y;
    bool done;
    int dest_v = -1;
    double cost = INF;
};

//////////////////////////////////////

//RANDOM NUMBER GEENRATOR
#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0 / MBIG)

float ran3(long *idum)
{
    static int inext, inextp;
    static long ma[56];
    static int iff = 0;
    long mj, mk;
    int i, ii, k;

    if (*idum < 0 || iff == 0)
    {
        iff = 1;
        mj = MSEED - (*idum < 0 ? -*idum : *idum);
        mj %= MBIG;
        ma[55] = mj;
        mk = 1;
        for (i = 1; i <= 54; i++)
        {
            ii = (21 * i) % 55;
            ma[ii] = mk;
            mk = mj - mk;
            if (mk < MZ)
                mk += MBIG;
            mj = ma[ii];
        }
        for (k = 1; k <= 4; k++)
            for (i = 1; i <= 55; i++)
            {
                ma[i] -= ma[1 + (i + 30) % 55];
                if (ma[i] < MZ)
                    ma[i] += MBIG;
            }
        inext = 0;
        inextp = 31;
        *idum = 1;
    }
    if (++inext == 56)
        inext = 1;
    if (++inextp == 56)
        inextp = 1;
    mj = ma[inext] - ma[inextp];
    if (mj < MZ)
        mj += MBIG;
    ma[inext] = mj;
    // debug(mj);
    return mj * FAC;
}

#undef MBIG
#undef MSEED
#undef MZ
#undef FAC

//////////////////////////////////////

double tot = 0;
int n;
vector<struct Point_data> p;
vector<int> v;
bool improved = false;
vector<bool> visited;
vector<vector<int>> adj;
int sz = 0, num_cities;
int pos = 0, neg = 0;

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
    // cout<<"--------modifying vextor------"<<endl;
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
                debug(diff);
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
    num_cities = n;
    // debug(n);
    tot = 0;
    p.resize(n);
    visited.assign(n, false);
    adj.assign(n, vector<int>());
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

#define TFACTR 0.9 //Annealing schedule: reduce t by this factor on each step.
    best_cost = min(best_cost, tot);

    if (num_cities > 10000)
    {
        tot = best_cost;
        goto get_output;
    }

    perform_annealing();
get_output:
    ofstream outfile;
	outfile.open("output_tsp.txt",ofstream::out);
    outfile << tot << " 0" << endl;
    for (auto x : v)
    {
        outfile << x << " ";
    }
    outfile << endl;

    // debug(pos);
    // debug(neg);
    // debug(best_cost);
    return 0;
}
void perform_annealing()
{
    // cout<<"****************************"<<endl;
    int ans, nover, nlimit, i1, i2;
    int i, j, k, nsucc, nn, idec;
    static int n[7];
    long idum;
    unsigned long iseed;
    double path, de, t;
    int ncity = num_cities;

    ////////////////////////////////////////////////////////////
    nover = 1000 * ncity; //    Maximum number of paths tried at any temperature.
    nlimit = 10 * ncity;  //    Maximum number of successful path changes before continuing.
    t = 10000;

    //300,25000->30548
    //10000,100->30111.2
    //nover=210,8000,100
    //
    //
    //
    path = tot;

    if (ncity < 100)
    {
        nover = 100 * ncity; //    Maximum number of paths tried at any temperature.
        nlimit = 10 * ncity; //    Maximum number of successful path changes before continuing.
        t = 30;
    }
    else if (ncity > 10000)
    {
        // nover = 1 * ncity; //    Maximum number of paths tried at any temperature.
        // nlimit = 1 * ncity; //    Maximum number of successful path changes before continuing.
        // t = 10;
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    idum = -1;
    iseed = 911;

    //  Try up to 100 temperature steps.
    int tries = 1;
    for (j = 1; j <= 100; j++)
    {
        nsucc = 0;
        //nover=100*city_num
        for (k = 1; k <= nover; k++)
        {
            do
            {

                n[1] = (int)(ncity * ran3(&idum));       //   Choose beginning of segment
                n[2] = (int)((ncity - 1) * ran3(&idum)); //..and end of segment...
                if (n[2] >= n[1])
                {
                    ++n[2];
                }
                nn = 1 + ((n[1] - n[2] + ncity - 1) % ncity); //   nn is the number of cities  not on the segment;
            } while (nn < 3);

            // idec = irbit1(&iseed);
            int idec = 1;
            //Decide whether to do a segment reversal or transport.
            if (idec == 0)
            {
                // // Do a transport when idec==0
                // n[3] = n[2] + (int)(abs(nn - 2) * ran3(&idum)) + 1;
                // n[3] = 1 + ((n[3] - 1) % ncity);
                // // Transport to a location not on the path.
                // de = trncst(x, y, iorder, ncity, n);
                // // Calculate cost.
                // ans = metrop(de, t);
                // //Consult the oracle.
                // if (ans)
                // {
                //     ++nsucc;
                //     path += de;
                //     //Carry out the transport.
                //     trnspt(iorder, ncity, n);
                // }
            }
            else
            {
                // Do a path reversal.
                // Calculate cost.
                // debug(n[1]);
                // debug(n[2]);
                de = get_cost(n[1], n[2]);
                ans = metrop(de, t);
                // debug(de);
                // Consult the oracle.

                if (ans)
                {
                    ++nsucc;
                    path += de;
                    tot += de;
                    best_cost = min(best_cost, tot);

                    //Carry out the reversal.
                    modify_vector(min(n[1], n[2]), max(n[1], n[2]));

                    // if (tot < 3.9)
                    // {
                    //     debug(tot);
                    //     cout << "WOAH\n";
                    //     for (auto x : v)
                    //     {
                    //         cout << x << " ";
                    //     }
                    //     cout << endl;
                    //     // exit(0);
                    // }
                }
                // part;
            }
            if (nsucc >= nlimit)
            {
                break;
            }
            //Finish early if we have enough successful changes.
        }
        // printf("\n %s %10.6f %s %12.6f \n", "T =", t, " Path Length =", path);
        // printf("Successful Moves: %6d\n", nsucc);
        //Annealing schedule
        t *= TFACTR;
        // If no success, we are done.
        if (nsucc == 0)
        {
            return;
        }
    }
}

int metrop(double de, double t)
{
    static long gljdum = 1;
    // debug(de);
    if (de < 0)
    {
        neg++;
    }
    else
    {
        pos++;
    }
    return de < 0.0 || ran3(&gljdum) < exp(-de / t);
}

double get_cost(int t2, int t4)
{
    int minima = min(t2, t4);
    t4 = t2 + t4 - minima;
    t2 = minima;
    int t1 = ((t2 - 1) + n) % n;
    int t3 = (t4 + 1) % n;
    if (t1 == t4)
    {
        return INF;
    }
    // debug(t2);
    // debug(t4);
    double now = dist(v[t1], v[t2]) + dist(v[t3], v[t4]);
    double poss = dist(v[t1], v[t4]) + dist(v[t2], v[t3]);
    double diff = poss - now;
    // if (diff > 0)
    // {
    //     debug(diff);
    //     if (diff > max_diff)
    //     {
    //         max_diff = diff;
    //         pos1 = t2;
    //         pos2 = t4;
    //     }
    // }
    // debug(diff);
    // if(diff<0)
    // {
    //     // cout<<"WOAH COOL DOWN\n";
    //     for(auto x:v)
    //     {
    //         cout<<x<<" ";
    //     }
    //     cout<<endl;
    //     // exit(0);
    // }
    return diff;
}

//400,100-> 20958
//500,100-> 20889
//301,100-> 20866
//110000,100->20813
//303,100-> 20855