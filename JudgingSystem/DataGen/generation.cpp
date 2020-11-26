#include <bits/stdc++.h>

#define pii pair<int, int>
#define F first
#define S second
#define pb push_back

using namespace std;

const int N = 2005, T = 100, V = 15, C = 5, WL = 3, WU = 10;

typedef unsigned long long ull;
int rng(const int before, const int after) { // Generate random base in [before, after] 
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count(); 
    std::mt19937 mt_rand(seed);
    int base = std::uniform_int_distribution<int>(before, after)(mt_rand);
    return base; //use steady_clock() for speed, for 64 bit mt19937_64 and lint
}

struct project{
    int idx;
    int score[C];
    int tot = 0;
} prj[N];

struct comp{
    bool operator()(const project &a, const project &b){
        return a.tot > b.tot;
    }
};

struct judge{
    int weight[C];
} juj[N];

int n, numj, jpp, mat[N][N], chance = 5, inp[N][N];

signed main(){
    ofstream data("data/fullinfo100.txt");
    ofstream input("data/inpfull100.txt");
    ofstream fedin("data/input100.in");
    //Input
    cout << "Enter number of projects, number of judges and judgements per project\n";
    cin >> n >> numj >> jpp;
    data << n << " " << numj << " " << jpp << endl;
    input << n << endl;
    fedin << n << endl;
    //project score assignment
    for(int i = 1; i <= n; i++){
        int base = rng(V, T);
        prj[i].idx = i;
        for(int j = 0; j < C; j++){
            prj[i].score[j] = base + rng(-V, V);
            prj[i].tot += prj[i].score[j];
        }
    }
    
    //Judge preference assignment
    for(int i = 1; i <= numj; i++){
        for(int j = 0; j < C; j++){
            juj[i].weight[j] = rng(WL, WU);
        }
    }

    //Perform comparisons for each judge
    for(int i = 1; i <= n; i++){
        for(int j = i+1; j <= n; j++){
            for(int k = 1; k <= jpp; k++){
                int nj = rng(1, numj);
                
                int si = 0;
                for(int it = 0; it < C; it++){
                    si += prj[i].score[it] * juj[nj].weight[it];
                }
                int sj = 0;
                for(int it = 0; it < C; it++){
                    sj += prj[j].score[it] * juj[nj].weight[it];
                }
                
                int nonce = rng(1, 100);
                if(si>sj){
                    mat[i][j] += 1; mat[j][i] -= 1;
                    if(nonce <= chance){
                        inp[i][j] += 1; inp[j][i] -= 1;
                    }
                }
                else if(sj>=si){
                    mat[i][j] -= 1; mat[j][i] += 1;
                    if(nonce <= chance){
                        inp[i][j] -= 1; inp[j][i] += 1;
                    }
                }
            }
        }  
    }

    //sort projects by total score (desc) and print
    sort(prj+1, prj+n+1, comp());
    for(int i = 1; i <= n; i++){
        data << prj[i].idx << " " << prj[i].tot << endl;
    }

    //print matrix
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            data << mat[i][j] << " ";
            if(mat[i][j]>0){
                input << mat[i][j] << " ";
            }
            else{
                input << 0 << " ";
            }
            if(inp[i][j] > 0){
                fedin << inp[i][j] << " ";
            }
            else{
                fedin << 0 << " ";
            }
        }
        data << endl;
        input << endl;
        fedin << endl;
    }
}