#include <bits/stdc++.h>

#define F first
#define S second
#define pii pair<int, int>

using namespace std;

const int N = 100005;
const double INF = 1e9;

int n;
struct City{
    float x, y;
    int idx;
    int active;
};

vector<City> places, best;
double pr = 1e-6;
double ans, currans;

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

int orientation(City p, City q, City r) 
{ 
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y); 
    if (abs(val) < pr) return 0;  // colinear 
    return (val > pr)? 1: -1; // clockwise or counterclock wise 
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

City sort_origin; //must be set before calling comp anywhere
struct comp{
    bool operator()(const City &p1, const City &p2){
        // Find orientation 
        int o = orientation(sort_origin, p1, p2); 
        if (o == 0) 
            return (dist(sort_origin, p1) < dist(sort_origin, p2)); 
        return (o == 1); 
    }
};

void polar_sort(vector<City> &v){
    int mini = 0;
    int l = v.size();
    for(int i = 1; i < l; i++){
        if(v[i].y < v[mini].y - pr){
            mini = i;
        }
        else if(v[i].y  < v[mini].y + pr){
            if(v[i].x < v[mini].x) mini = i;
        }
    }
    swap(v[0], v[mini]);
    sort_origin = v[0];
    sort(v.begin()+1, v.end(), comp());
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

double compswap(int i, int j, vector<City> &v){
    int il = (i-1+n)%n, ir = (i+1)%n;
    int jl = (j-1+n)%n, jr = (j+1)%n;
    
    if(ir==j || il==j){
        double curr, next;
        if(ir==j){
            curr = dist(v[il], v[i]) + dist(v[j], v[jr]);
            next = dist(v[il], v[j]) + dist(v[i], v[jr]);
        }
        else{
            curr = dist(v[ir], v[i]) + dist(v[j], v[jl]);
            next = dist(v[ir], v[j]) + dist(v[i], v[jl]); 
        }
        return next - curr;
    }

    double curr = dist(v[i], v[il]) + dist(v[i], v[ir]);
    curr += dist(v[j], v[jl]) + dist(v[j], v[jr]);
    double next = dist(v[i], v[jl]) + dist(v[i], v[jr]);
    next += dist(v[j], v[il]) + dist(v[j], v[ir]);
    return next - curr;
}

double comp2opt(int i, int j, vector<City> &v){
    if(j-i==0 || j-i==n-1) return 0;
    int il = (i-1+n)%n, jr = (j+1)%n;
    double curr = dist(v[il], v[i]) + dist(v[j], v[jr]);
    double next = dist(v[il], v[j]) + dist(v[i], v[jr]);
    return next - curr;
}

void local_search(vector<City> &v){
    double mind = 0;
    int minj = -1, mink = -1, flag=0;

    for(int j = 0; j < n-1; j++){
        for(int nidx = j+1; nidx < n; nidx++){ //replace j+2 with n for O(N^2) swap space.
            //int nidx = (j+1)%n;
            double curr = comp2opt(j, nidx, v);
            if(curr < mind){
                mind = curr;
                minj = j;
                mink = nidx;
                flag=1; //This jumps on every lowering move
            }
            if(flag && n>10000) break;
        }
        if(flag && n>10000) break;
    }

    //cout << minj << " " << mink << endl;
    if(minj!=-1){
        //swap(v[minj], v[mink]); //This is when swapping cities
        reverse(v.begin() + minj, v.begin() + mink + 1);
        currans = cost(v);
    }
}

void restactive(vector<City> &v){
    for(int i = 0; i < v.size(); i++){
        v[i].active=0;
    }
}

bool choosenext(int &minj, int &mink, double &mind, int j, int nidx, vector<City> &v){
    double curr = comp2opt(j, nidx, v);
    if(curr < mind){
        mind = curr;
        minj = j;
        mink = nidx;
        return true;
    }
    return false;
}

void anneal_search(vector<City> &v, double t, int M, int &ntime, bool rand=false){
    double mind = INF;
    int minj = -1, mink = -1;

    if(rand){
        for(int i = 0; i < M; i++){  
            int j = rng(-1, n-2); int nidx = (j, n-1);
            choosenext(minj, mink, mind, j, nidx, v);
        }
    }
    else{
        int cntchng = 0;
        for(int j = 0; j < n-1; j++){
            ntime++;
            if(v[j].active > ntime) continue;
            v[j].active = ntime+n;
            bool flag = 0;
            for(int nidx = j+1; nidx < n; nidx++){ //replace j+2 with n for O(N^2) swap space.
                if(choosenext(minj, mink, mind, j, nidx, v)){
                    v[j].active=ntime; v[nidx].active=ntime;
                    if(!flag) cntchng++;
                    flag = 1;
                }
            }
            if(cntchng>M) break;
        }
    }

    //cerr << minj << " " << mink << endl;
    if(minj!=-1){
        double gen = drng(0, 1);
        double thresh = exp((-mind)/t);
        if(gen < thresh){
            reverse(v.begin() + minj, v.begin() + mink + 1);
            currans = cost(v);
        }
    }
}

void update_temp(double &t, double &a){
    t*=a;
}

void reheat_temp(double &t, double &st){
    t = st;
}

void test_in(){
    freopen("../data/tsp_100_1", "r", stdin);
}

signed main(){
    //freopen("tmp.in", "r", stdin);
    test_in();
    freopen("tmp100sa.out", "w", stdout);
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

    double st = 0.1*ans, a = 0.996;
    int rpdur=10, pdur = 1e4, tdur = 10, rdur = 5e5;
    int Titer = 1e3, ITER = 2e4, M = 1, rpt=500;
    bool rand = false;
    if(n==200){
        st = 0.01*ans; a = 0.99;
        pdur = 5e3; rdur = 5; tdur = 10;
        Titer = 1e3; ITER = 1e4; M = 1, rpt=20;
    }
    if(n==574){
        st = 0.02*ans; a = 0.992;
        pdur = 5000; rdur = 50000; tdur = 10;
        Titer = 1e3; ITER = 10000; M = 1, rpt=100;
    }
    if(n==1889){
        st = 0.01*ans; a = 0.98;
        pdur = 1000; rdur = 50000; tdur = 5;
        Titer = 1e3; ITER = 2000; M = 1, rpt=100;
    }
    if(n==33810){
        st = 0.001*ans; a = 0.99;
        pdur = 5000; rdur = 50000; tdur = 1;
        Titer = 1e3; ITER = 1000; M = 1000, rpt=50;
        rand = true;
    }
    /*for(int i = 0; i < Titer; i++){
        anneal_search(places, 0.0001, M); //improve initialization even more
        if(currans < ans){
            ans = currans;
            setvec(best, places);
        }
        else{
            break;
        }
    }
    cerr << ans << endl;*/

    vector<City> cp(n);
    double t;
    for(int shot = 0; shot < rpt; shot++){
        reheat_temp(t, st);
        int bestin = INF;

        if(shot%rpdur==0) cerr << shot/rpdur << " " << currans << " - " << ans << endl;
        setvec(cp, places);
        restactive(cp);
        int ntime = 0;

        for(int i = 0; i <= ITER; i++){
            if(i%pdur==0) cerr << i << " " << currans << " " << t << " - " << ans << endl;
            if(i%tdur==0){
                update_temp(t, a);
            }
            if(i%rdur==0){
                reheat_temp(t, st);
            }

            anneal_search(cp, t, M, ntime, rand);
            //cerr << currans << endl;
            if(currans < bestin){
                bestin = currans;
            }
            if(currans < ans){
                ans = currans;
                setvec(best, cp);
            }
        }
        
        cerr << shot << " | | " << bestin << endl;
        greedy_init(places, true);
        currans = cost(places);
        /*double tmp = currans;
        for(int i = 0; i < Titer; i++){
            anneal_search(places, 0.0001, M); //improve initialization even more
            if(currans < ans){
                ans = currans;
                setvec(best, places);
            }
            else{
                break;
            }
        }
        cerr << shot << " - " << tmp << " " << currans << endl;*/
    }

    cout << ans << " " << 0 << endl;
    for(int i = 0; i < n; i++){
        cout << best[i].idx << " ";
    }
}