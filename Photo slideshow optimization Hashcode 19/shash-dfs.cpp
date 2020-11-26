#include <bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>

using namespace std ;
using namespace __gnu_pbds;

template <typename T> // *s.find_by_order(0), s.order_of_key(2) ;
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

#define reMin(a, b) a = min(a, b)
#define reMax(a, b) a = max(a, b)

#define lint long long
#define pb push_back
#define F first 
#define S second 
#define sz(x) (int)x.size()
#define all(x) begin(x), end(x)
#define SET(x, val) memset(x, val, sizeof(x))
#define fastio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)

typedef vector < int > vi ;
typedef pair < int, int > pii ;
typedef pair < double, double > point ;

const int N = 1e5 + 2 ;
const int M = 3000  + 2 ;
const lint INF = 1e18 ;
lint MAX_OPS = 1e8 ; 

int n ;

// TYPE:
// 0 for vertical
// 1 for horizontal 
// tags and tags2 are same (only difference is one is set and another is vector)

struct photo {
    set < int > tags ;
    int type ;
    int index ; 
};
vector < photo > photos ; 

struct slide {
    set < int > tags ; 
    vi photos ;  // contains index of photos in this slide 
};
vector < slide > slides ; 

int transScore(slide &s1, slide &s2){
    int s1only=0, s2only=0, both=0;
    for(auto tag: s1.tags){
        if(s2.tags.find(tag) == s2.tags.end()){
            s1only++;
        }
        else{
            both++;
        }
    }
    for(auto tag: s2.tags){
        if(s1.tags.find(tag) == s1.tags.end()){
            s2only++;
        }
    }
    int lol = min(s1only, s2only);
    lol = min(lol, both);
    return lol;
}

void H2slide(photo &a, slide &sl){
    sl.tags.clear(); sl.photos.clear();
    for(auto tag: a.tags){
        sl.tags.insert(tag);
    }
    sl.photos.pb(a.index);
}

void V2slide(photo &a, photo &b, slide &sl){
    sl.tags.clear(); sl.photos.clear();
    for(auto tag: a.tags){
        sl.tags.insert(tag);
    }
    for(auto tag: b.tags){
        sl.tags.insert(tag);
    }
    sl.photos.pb(a.index); sl.photos.pb(b.index);
}

void outputSol(vector < slide >& ans) {
    int cnt = sz(ans) ;
    cout << cnt << endl ; 

    for(int i = 0; i < cnt; i++) {
        for(int j = 0; j < sz(ans[i].photos); j++) {
            cout << ans[i].photos[j] ;
            if(j < (sz(ans[i].photos) - 1)) cout << " " ;
        }
        if(i < (cnt - 1)) cout << endl ; 
    }
}

map<int, vector<int>> tagin;
void analyze_tags(){
    freopen("tagsE.out", "w", stdout);
    map<int, int> cnt;
    for(auto it: tagin){
        int num = it.S.size();
        if(num > 1000){
      //      num /= 1000;
        }
        cnt[num]++;
    }
    for(auto it: cnt){
        cout << it.F << " " << it.S << endl;
    }
}

void analyze_edges(){
    freopen("edgesD.out", "w", stdout);
    
    int bothV=0, VH=0, bothH=0;
    for(int i = 0; i < n; i++){
        if(i%10==0){
            cerr << i << " looping" << endl;
        }
        for(int j = i+1; j < n; j++){
            slide tmp1, tmp2;
            H2slide(photos[i], tmp1);
            H2slide(photos[j], tmp2);
            int scr = transScore(tmp1, tmp2);
            if(scr>0){
                if(photos[i].type == 0 && photos[j].type == 0){
                    bothV++;
                }
                else if(photos[i].type == 1 && photos[j].type == 1){
                    bothH++;
                }
                else{
                    VH++;
                }
            }
        }
    }
    cerr << bothV << " " << bothH << " " << VH << endl;
}

struct adjsrt{
    bool operator()(const pii &a, const pii &b){
        return a.S < b.S;
    }
};
vector< pii > adj[N];
void process_tags(){

    slide tmp1, tmp2;
    for(auto it: tagin){
        int num = it.S.size();
        if(num==2){
            H2slide(photos[it.S[0]], tmp1);
            H2slide(photos[it.S[1]], tmp2);
            int w = transScore(tmp1, tmp2);
            adj[it.S[0]].pb({it.S[1], w});
            adj[it.S[1]].pb({it.S[0], w});
        }
    }
    for(int i = 0; i < n; i++){
        sort(adj[i].begin(), adj[i].end(), adjsrt());
    }
}

vector<slide> dfsorder;
bool vis[N];
int cnt = 0;
slide tmpu;
void dfs(int u){
    //cerr << "u - " << u << endl;
    cnt++;
    vis[u]=1;
    H2slide(photos[u], tmpu);
    dfsorder.pb(tmpu);

    for(auto v: adj[u]){
        //cerr << "v - " << v << endl;
        if(!vis[v.F]){
            dfs(v.F);
        }
    }
}

int main(int argc, char* argv[])
{
    srand(time(0)); 
    freopen(argv[1], "r", stdin); 

    cin >> n; int tagCnt ;   
    photos.resize(n); char ch ; 

    map < string, int> tagIndex ; 
    int tagidx = 0;

    for(int i = 0; i < n; i++) {
        cin >> ch ; photos[i].type = (ch == 'H');
        photos[i].index = i ; 

        cin >> tagCnt ;

        for(int j = 0; j < tagCnt; j++) {
            string tmptag;
            cin >> tmptag;
            if(tagIndex.find(tmptag)==tagIndex.end()){
                tagIndex[tmptag] = tagidx++;
            }
            int idx = tagIndex[tmptag];
            photos[i].tags.insert(idx);
            tagin[idx].pb(i);
        }
    }


    cerr << sz(photos) << endl ; 
    /*slide tmp1, tmp2;
    H2slide(photos[3], tmp1);
    V2slide(photos[1], photos[2], tmp2);
    cerr << transScore(tmp1, tmp2) << endl;*/
    //analyze_tags();
    //analyze_edges();

    process_tags();
    for(int i = 0; i < n; i++){
        if(!vis[i]){
            dfs(i);
            //cerr << "done" << endl;
        }
        //cerr << dfsorder.size() << endl;
    }
    
    freopen("b.out", "w", stdout);
    outputSol(dfsorder);
    return 0;   
}

