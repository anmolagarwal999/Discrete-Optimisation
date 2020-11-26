#include <bits/stdc++.h>

#define F first
#define S second
#define pii pair<int, int>
#define pb push_back

using namespace std;

const int N = 505;
const double INF = 1e9;

int n, mat[N][N], matsum;
struct Project{
    int idx;
    int points;
};

vector<Project> places, best;
int ans, currans;

void swap(Project &p1, Project &p2) 
{ 
    Project temp = p1; 
    p1 = p2; 
    p2 = temp; 
} 

//returns cost
int cost(vector<Project> &v){
    int ret = 0;
    for(int i = 1; i <= n; i++){
        for(int j = i+1; j <= n; j++){
            ret += mat[v[j].idx][v[i].idx];
        }
    }
    return ret;
}

void setvec(vector<Project> &dest, vector<Project> &src){
    for(int  i = 0; i < src.size(); i++){
        dest[i] = src[i];
    }
}

void setpos(int i, int j, vector<Project> &v){
    vector<Project> temp(v.size());
    int idx = 0;
    for(int it = 1; it <= n; it++){
        //cerr << it << endl;
        idx++;
        if(idx==i) idx++; //skip ith element
        if(it==j){
            temp[it] = v[i]; //ith element on jth position
            idx--; //as not using idxth element here
            continue;
        }
        temp[it] = v[idx];
        //cerr << it << " " << idx << " - " << i << " " << j << endl;
    }
    setvec(v, temp);
}

struct pointsort{
    bool operator()(const Project &a, const Project &b){
        return a.points > b.points;
    }
};

void init(vector<Project> &v){

    /*points greedy*/
    for(int i = 1; i <= n; i++){
        v[i].points = 0;
        for(int j = 1; j <= n; j++){
            v[i].points += mat[v[i].idx][j] - mat[j][v[i].idx];
        }
        //cerr << v[i].idx << " - " << v[i].points << endl;
    }
    sort(v.begin()+1, v.end(), pointsort());
}

void local_search(vector<Project> &v){
    int mind = INF;
    int mini = 0, minj = 0;

    for(int i = 1; i <= n; i++){
        int moveval = 0;
        for(int j = i-1; j >= 1; j--){
            moveval += mat[v[j].idx][v[i].idx] - mat[v[i].idx][v[j].idx];
            if(moveval < mind){
                mini = i; minj = j; 
                mind = moveval;
            }
        }
        moveval = 0;
        for(int j = i+1; j <= n; j++){
            moveval += mat[v[i].idx][v[j].idx] - mat[v[j].idx][v[i].idx];
            if(moveval < mind){
                mini = i; minj = j; 
                mind = moveval;
            }
        }
    }
    //cerr << mini << " " << minj << " - " << mind << endl;
    if(mind < 0){
        setpos(mini, minj, v);
        //cerr << "here\n";
        currans += mind;
    }
}

void setup(int i){
    string fin = "lolib_2010/RandA1/N-t1d500." + to_string(i);
    string fout = "IOout/RandA1/N-t1d500." + to_string(i) + ".out";
    //string fin = "IO/N-tiw56n72";
    //string fout = "IOout/N-tiw56n72";
    freopen(fin.c_str(), "r", stdin);
    freopen(fout.c_str(), "w", stdout);
    //freopen("../data/tsp_51_1", "r", stdin);

    matsum = 0;
    cin >> n;
    places.resize(n + 1); best.resize(n + 1);
    for(int i = 1; i <= n; i++){
        //cerr << i << endl;
        places[i].idx = i;
        for(int j = 1; j <= n; j++){
            cin >> mat[i][j];
            if(i==j){
                mat[i][j] = 0;
            }
            matsum += mat[i][j];
        }
    }
    currans = ans = cost(places);
//    cerr << ans << endl;
    init(places);
    currans = ans = cost(places);
  //  cerr << ans << endl;
    setvec(best, places);
}

void solve(int x){
    setup(x);
    int rpt=5000;

    vector<Project> cp(n + 1);
    setvec(cp, places);
    int bestin = currans;
    for(int shot = 0; shot < rpt; shot++){
        //cerr << shot << endl;
        local_search(cp);
        if(currans >= ans) break; //not improving
        setvec(best, cp);
        ans = currans;
    //    cerr << shot << " " << ans << endl;
    }

    ans = cost(best);
    cout << ans << " " << matsum-ans << endl;
    cerr << x << " - " << matsum-ans << endl;
    for(int i = 1; i <= n; i++){
        cout << best[i].idx << " ";
    }
}
signed main(){
    for(int i = 1; i <= 25; i++){
        solve(i);
    }   
}