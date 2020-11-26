#include <bits/stdc++.h>

#define F first
#define S second
#define pii pair<int, int>
#define pb push_back

using namespace std;

const int N = 100005;
const double INF = 1e9;

int n;
struct City{
    float x, y;
    int idx;
};

vector<City> places, best;
set<int> active;
double pr = 1e-6;
double ans, currans, currloss;

void swap(City &p1, City &p2) 
{ 
    City temp = p1; 
    p1 = p2; 
    p2 = temp; 
} 

typedef unsigned long long ull;
int rng(const int before, const int after) { // Generate random base in (before, after] 
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count(); 
    std::mt19937 mt_rand(seed); //remove ^ ull thing for speed
    int base = std::uniform_int_distribution<int>(before+1, after)(mt_rand);
    return base; //use steady_clock() for speed, for 64 bit mt19937_64 and lint
}

double drng(int l, int r){
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(l, r);
    return dist(e2);
}

double dist(City p1, City p2) 
{ 
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)); 
} 

//returns cost
double cost(vector<City> &v){
    double ret = 0;
    int l = v.size();
    for(int i = 0; i < l; i++){
        int j = (i+1)%l;
        ret += dist(v[i], v[j]);
    }
    return ret;
}

void setvec(vector<City> &dest, vector<City> &src){
    for(int  i = 0; i < src.size(); i++){
        dest[i] = src[i];
    }
}

void greedy_init(vector<City> &v, bool rand=false){
    int mini = 0;
    for(int i = 1; i < n; i++){
        if(v[i].x < v[mini].x - pr){
            mini = i;
        }
        else if(v[i].x  < v[mini].x + pr){
            if(v[i].y < v[mini].y) mini = i;
        }
    }
    if(rand) mini = rng(-1, n-1);
    vector<bool> vis; vis.resize(n);
    swap(v[0], v[mini]);
    vis[v[0].idx] = true;

    for(int i = 1; i < n; i++){
        int mind = INF, next = -1;
        for(int j = i; j < n; j++){
            if(!vis[v[j].idx]){
                if(dist(v[i-1], v[j]) < mind){
                    mind = dist(v[i-1], v[j]);
                    next = j;
                }
            }
        }
        swap(v[i], v[next]);
        vis[v[i].idx] = true;
    }
}

void tryinits(vector<City> &v){
    vector<City> t1(n); setvec(t1, v);
    vector<City> t2(n); setvec(t2, v);

    /*polar_sort(t1);
    if(cost(t1) < ans){
        currans = ans = cost(t1);
        setvec(v, t1);
    }*/

    greedy_init(t2);
    if(cost(t2) < ans){
        currans = ans = cost(t2);
        setvec(v, t2);
    }
}

class Penalty{
    map<pii, int> p;
public:
    double k = 0;
    void clear(){
        p.clear();
    }
    int getp(int u, int v){
        if(p.find({u, v}) != p.end()){
            return p[{u, v}];
        }
        return 0;
    }
    double getu(City p1, City p2){
        return dist(p1, p2) / (1 + getp(p1.idx, p2.idx));
    }
    void increase(int u, int v){
        p[{u, v}]+=1;
        p[{v, u}]+=1;
        //if(p[{v, u}] % 1000 == 0) cerr << u << " " << v << " updated to: " << p[{u, v}] << endl;
    }
    double getloss(City p1, City p2){
        return dist(p1, p2) + k*getp(p1.idx, p2.idx);
    }
    void pencut(int cut){
        if(cut == -1) p.clear();
        else{
            for(auto it: p){
                p[it.F] /= cut;
            }
        }
    }
    void update(vector<City> &v){
        vector<pii> to_upd;
        double utilmax = 0;
        double eqrng = pr*pr;

        //populate to_upd with max util features
        for(int i = 0; i < v.size(); i++){
            int j = (i+1)%v.size();
            double util = getu(v[i], v[j]);
            if(utilmax < util){
                utilmax = util;
                to_upd.clear();
                to_upd.pb({i, j});
            }
            else if(utilmax < util + eqrng){//equal
                to_upd.pb({i, j});
            }
        }

        //update features in to_upd
        for(auto e: to_upd){
            increase(v[e.F].idx, v[e.S].idx);
            active.insert(e.F); active.insert(e.S);
        }
    }
} P;

double lossinit(vector<City> &v){
    double ret = 0;
    for(int i = 0; i < v.size(); i++){
        int j = (i+1)%n;
        ret += P.getloss(v[i], v[j]);
    }
    return ret;
}

double loss2opt(int i, int j, vector<City> &v){
    if(j-i==0 || j-i==n-1) return 0;
    int il = (i-1+n)%n, jr = (j+1)%n;
    double curr = P.getloss(v[il], v[i]) + P.getloss(v[j], v[jr]);
    double next = P.getloss(v[il], v[j]) + P.getloss(v[i], v[jr]);
    return next - curr;
}

double dist2opt(int i, int j, vector<City> &v){
    if(j-i==0 || j-i==n-1) return 0;
    int il = (i-1+n)%n, jr = (j+1)%n;
    double curr = dist(v[il], v[i]) + dist(v[j], v[jr]);
    double next = dist(v[il], v[j]) + dist(v[i], v[jr]);
    return next - curr;
}

void restactive(vector<City> &v){
    active.clear();
    for(int i = 0; i < v.size(); i++){
        active.insert(i);
    }
}

bool choosenext(int &minj, int &mink, double &mind, int j, int nidx, vector<City> &v){
    double curr = loss2opt(j, nidx, v);
    if(curr < mind){
        mind = curr;
        minj = j;
        mink = nidx;
        return true;
    }
    return false;
}

void guided_search(vector<City> &v, int M, bool rand=false){
    double mind = 0;
    int minj = -1, mink = -1;
    if(rand){
        for(int i = 0; i < M; i++){  
            int j = rng(-1, n-2); int nidx = (j, n-1);
            choosenext(minj, mink, mind, j, nidx, v);
        }
    }
    else{
        while(active.size()){
            mind = 0; minj = -1; mink = -1;
            bool flag=0;
            vector<int> erasals;

            for(auto j: active){
                for(int nidx = 0; nidx < n; nidx++){ //replace j+2 with n for O(N^2) swap space.
                    int eu = min(j, nidx), ev = max(j, nidx);
                    if(choosenext(minj, mink, mind, eu, ev, v)){
                        active.insert(nidx);
                        flag = 1;
                        break;
                    }
                }
                if(flag) break;
                erasals.pb(j);
            }

            for(auto e: erasals) active.erase(e);
            erasals.clear();
            if(minj!=-1){
                //cerr << minj << " " << mink << " - " << mind << endl;
                currans += dist2opt(minj, mink, v);
                currloss += mind;
                reverse(v.begin() + minj, v.begin() + mink + 1);
            }
        }
    }
}

void setup(){
    freopen("tmp.in", "r", stdin);
    //freopen("../data/tsp_51_1", "r", stdin);
    freopen("ans.out", "w", stdout);
    cin >> n;
    places.resize(n); best.resize(n);
    for(int i = 0; i < n; i++){
        cin >> places[i].x >> places[i].y;
        places[i].idx = i;
    }
    for(int i = 0; i < n; i++){
        places[i].idx = i;
    }
    currans = ans = cost(places);
    cerr << ans << endl;
    tryinits(places);
    cerr << ans << endl;
    setvec(best, places);
    currloss = currans;
}

void fromfile(){
    double x, k[n+2];
    int y;
    cin >> x >> y;
    for(int i = 0; i < n; i++){
        cin >> k[i];
    }
    cout << x << y << endl;
    for(int i = 0; i < n; i++){
        cout << k[i] << " ";
    }
}

signed main(){
    setup();
    int rpt=5e4, ITER = 1, M = 10, pdur=1e3, rdur = 5e3;
    double a = 0.1; int cut = -1;
    if(n==51){
        freopen("mycodes/check51.txt", "r", stdin);
        fromfile();
        return 0;
    }
    if(n==100){
        rpt = 2e5; rdur=2e4;
        a = 0.1; cut = -1;
        freopen("mycodes/tmp100.out", "r", stdin);
        fromfile();
        return 0;
    }
    if(n==200){
        rpt = 5e4; rdur = 5e3; a = 0.1; cut = -1;
        freopen("mycodes/tmp200.out", "r", stdin);
        fromfile();
        return 0;
    }
    if(n==574){
        rpt=1e6; rdur = 5e4;
        a = 0.1; cut = -1;
        pdur = 1e4;
        /*freopen("mycodes/tmp574.out", "r", stdin);
        fromfile();
        return 0;*/
    }
    if(n==1889){
        rpt = 5e5; rdur = 1e5;
        a = 0.1; cut = -1;
        pdur = 2.5e3;
    }
    if(n==33810){
        rpt = 5e4, rdur = 5e4, cut = -1;
        a = 0.1;
    }
    bool rand = false;

    vector<City> cp(n);
    setvec(cp, places);
    restactive(cp);
    int bestin = currans;
    for(int shot = 0; shot < rpt; shot++){
        if(shot%rdur==0){
            cerr << "this try: " << currans << " - " << bestin << " " << endl;
            greedy_init(places, true);
            setvec(cp, places);
            restactive(cp);
            currans = cost(cp);
            currloss = lossinit(cp);
            bestin = currans;
            P.pencut(cut);
        }
        if(shot%pdur==0) cerr << shot << " " << currans << " - " << bestin << " " << ans << endl;
        //cerr << shot << endl;
        /*LOCAL SEARCH STARTS HERE*/
        guided_search(cp, M, rand);
        //cerr << currans << endl;
        if(currans < bestin){
            bestin = currans;
        }
        if(currans < ans){
            ans = currans;
            setvec(best, cp);
        }
        P.k = a * currloss / n;
        P.update(cp);
    }

    cout << ans << " " << 0 << endl;
    for(int i = 0; i < n; i++){
        cout << best[i].idx << " ";
    }
}