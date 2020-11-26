#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

#define lint long long
#define pb push_back
#define F first
#define S second
#define sz(x) (int)x.size()
#define all(x) begin(x), end(x)
#define SET(x, val) memset(x, val, sizeof(x))
#define fastio                    \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);                   \
    cout.tie(0)

typedef vector<int> vi;
typedef pair<int, int> pii;
typedef pair<double, double> point;
#define part cout << "----------------------------------\n";
#define debug(x) cout << #x << " : " << x << endl

const LL INF = 1e15;

LL score_now = 0;

int n;

// TYPE:
// 0 for vertical
// 1 for horizontal
// t_set and tags2 are same (only difference is one is set and another is vector)

struct photo
{
    unordered_set<string> t_set;
    vector<string> tags2;
    int type;
    int index;
};
vector<photo> photos;
vector<LL> v;
struct slide
{
    set<string> t_set;
    vector<string> tags2;
    vi photos_there; // contains index of photos in this slide
    int id1;
};
vector<slide> slides_v;
int num_pics;

void outputSol(vector<slide> &ans)
{
    int cnt = sz(ans);
    cout << cnt << endl;

    for (int i = 0; i < cnt; i++)
    {
        for (int j = 0; j < sz(ans[i].photos_there); j++)
        {
            cout << ans[i].photos_there[j];
            if (j < (sz(ans[i].photos_there) - 1))
                cout << " ";
        }
        if (i < (cnt - 1))
            cout << endl;
    }
}

int find_edge_score(photo &a, photo &b)
{
    int sz1 = a.t_set.size();
    int sz2 = b.t_set.size();
    int common = 0;
    // debug(sz1);
    // debug(sz2);
    for (auto &x : a.t_set)
    {

        if (b.t_set.find(x) != b.t_set.end())
        {
            common++;
        }
    }
    // debug(common);
    return min({sz1 - common, sz2 - common, common});
}

void cal_initial_score()
{
    int i, j, k;
    score_now = 0;
    debug(num_pics);
    for (i = 0; i < num_pics - 1; i++)
    {
        score_now += find_edge_score(photos[v[i]], photos[v[i + 1]]);
    }
}
LL get_score(int id1, int id2)
{
    if (id1 == -1 || id2 == -1)
    {
        return 0;
    }
    else
    {
        return find_edge_score(photos[id1], photos[id2]);
    }
}

bool comparator(photo a, photo b)
{
    return a.t_set.size() > b.t_set.size();
}

bool perform_swap()
{
    int i, j, k;

    int id1_swap = -1, id2_swap = -1;
    LL curr_max = -INF;

    for (i = 0; i < num_pics; i++)
    {
        for (j = i + 1; j < min(i + 10, num_pics); j++)
        {
            int id1 = v[i];
            int id1_left = -1;
            int id1_right = -1;

            int id2 = v[j];
            int id2_left = -1;
            int id2_right = -1;
            // debug(i);
            // debug(j);

            if (i != 0)
            {
                id1_left = v[i - 1];
            }
            if (j != 0)
            {
                id2_left = v[j - 1];
            }
            if (i != num_pics - 1)
            {
                id1_right = v[i + 1];
            }
            if (j != num_pics - 1)
            {
                id2_right = v[j + 1];
            }

            int curr_score = get_score(id1, id1_left) + get_score(id1, id1_right) + get_score(id2, id2_left) + get_score(id2, id2_right);
            int projected_score = get_score(id2, id1_left) + get_score(id2, id1_right) + get_score(id1, id2_left) + get_score(id1, id2_right);
            int inc = projected_score - curr_score;
            if (inc > curr_max && inc > 0)
            {
                curr_max = inc;
                id1_swap = i;
                id2_swap = j;
                break;
            }
        }
    }

    if (id1_swap == -1)
    {
        return false;
    }
    else
    {
        score_now += curr_max;
        swap(v[id1_swap], v[id2_swap]);
        return true;
    }
}

int main()
{
    // srand(time(0));
    // freopen(argv[1], "r", stdin);

    LL i, j, k, t;
    cin >> n;
    num_pics = n;
    debug(num_pics);
    debug(n);
    int tagCnt;
    photos.resize(n);
    v.resize(n);
    char ch;
    set<string> total;
    // return 0;

    for (int i = 0; i < n; i++)
    {
        cin >> ch;

        //1 for h, 0 for v
        photos[i].type = (ch == 'H');
        photos[i].index = i;

        cin >> tagCnt;
        photos[i].tags2.resize(tagCnt);

        for (int j = 0; j < tagCnt; j++)
        {
            cin >> photos[i].tags2[j];
            photos[i].t_set.insert(photos[i].tags2[j]);

            total.insert(photos[i].tags2[j]);
        }

        // cerr << photos[i].t_set.size() << endl;
        // v[i] = i;
    }

    //sort(all(photos), comparator);
    cerr << sz(photos) << endl;
    cerr << sz(total) << endl;

    // freopen("b.out", "w", stdout);
    // outputSol();
    for (i = 0; i < num_pics; i++)
    {
        cin >> v[i];
    }

    LL limit = 1000;
    cal_initial_score();
    cout << "initial score is " << score_now << endl;

    // return 0;
    part;
    part;
    //return 0;
    while (true)
    {
        if (perform_swap() == false)
        {
            break;
        }
        debug(score_now);
    }
    part;
    cout << "Max score is " << score_now << endl;

    for (auto x : v)
    {
        cout << x << " ";
    }
    cout << endl;
    part;
    debug(score_now);
    return 0;
}
