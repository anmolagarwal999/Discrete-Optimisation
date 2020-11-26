// Solves Knapsack Problem
#include<bits/stdc++.h>
#define ld long double
#define pb push_back

using namespace std;

const int N = 1e4+5;
const ld pr = 1e-14;
int n, k;

struct item{
    int v, w, idx;
    ld g;
} obj[N];

//sort, currently used for relaxation + items
struct gsrt{
    bool operator()(const item &a, const item &b){
        if(abs(a.g-b.g)<pr){
            return a.w < b.w; //if same density prioritize less weight
        }
        return a.g > b.g; //prioritze more density
    }
};

//decision tree node
struct Node{
    ld rlx = 0;
    vector<bool> decvars;
    int cv = 0, cw = 0, idx = 0;
    int numright = 0; //for LDS
} best;

//convert vector to string
string vtostr(vector<bool> &v){
    string ret="";
    for(int i = 1; i < v.size(); i++){
        if(v[i]) ret+="1";
        else ret+="0";
    }
    return ret;
}

//Types of searching possible
struct LDSBestHybrid{
    bool operator()(const Node &u, Node &v){
        if(u.numright == v.numright){
            if(abs(u.rlx - v.rlx) < pr){
                return u.idx < v.idx; //favor lower nodes if equal relaxation
            }
            return u.rlx < v.rlx;  //Best first search
        }
        return u.numright > v.numright; //favor less numrights first (LDS)
    }
};
struct BestFirst{
    bool operator()(const Node &u, Node &v){
        if(abs(u.rlx - v.rlx) < pr){
            if(u.numright == v.numright){
                return u.idx < v.idx;
            }
            return u.numright > v.numright;
        }
        return u.rlx < v.rlx;
    }
};
struct DepthFirst{
    bool operator()(const Node &u, Node &v){
        return u.decvars > v.decvars;
    }
};
struct BreadthFirst{
    bool operator()(const Node &u, Node &v){
        return u.decvars.size() > v.decvars.size();
    }
};

//Whether the current node is valid or not
bool eligible(Node &curr){
    if(curr.cw > k){
        return false;
    }
    if(curr.rlx < (ld) best.cv){
        return false;
    }
    return true;
}

//Compute Relaxation score for the current node
void computerlx(Node &curr){
    ld leftwt = k - curr.cw;
    curr.rlx = curr.cv;
    for(int idx = curr.decvars.size(); idx <= n; idx++){
        if(obj[idx].w < leftwt + pr){
            leftwt -= obj[idx].w;
            curr.rlx += obj[idx].v;
        }
        else{
            curr.rlx += leftwt*obj[idx].g;
            leftwt = 0;
        }
    }
}

void Solve(){
    priority_queue<Node, vector<Node>, DepthFirst> searchQ;
    Node init;
    init.decvars.pb(0); computerlx(init);
    // cerr << init.rlx << endl;
    searchQ.push(init);

    while(searchQ.size()){
        Node curr = searchQ.top();
        searchQ.pop();
        if(!eligible(curr)){
            continue;
        }
        if(curr.cv > best.cv){
            best = curr;
        }
        if(curr.idx>=n){
            // cerr << curr.numright << " " << curr.cv << " - " << best.cv << " " << searchQ.size() << endl;
            //see progress of algorithm
            continue;
        }
        //cerr << vtostr(curr.decvars) << endl;
        Node lc = curr; lc.idx++; lc.decvars.pb(1);
        lc.cv += obj[lc.idx].v; lc.cw += obj[lc.idx].w; 
        computerlx(lc);
        Node rc = curr; rc.idx++; rc.decvars.pb(0);
        rc.numright++;
        computerlx(rc);
        if(eligible(lc)) searchQ.push(lc);
        if(eligible(rc)) searchQ.push(rc);
    }
}

signed main(){
    ifstream infile;
    infile.open("input_sack.txt");
    infile >> n >> k;
    for(int i = 1; i <= n; i++){
        infile >> obj[i].v >> obj[i].w;
        obj[i].idx = i;
        obj[i].g = ((double) obj[i].v) / obj[i].w;
    }
    infile.close();
    sort(obj+1, obj+n+1, gsrt());
    for(int i = 1; i <= n; i++){
      //  cerr << obj[i].v << " " << obj[i].w << " " << obj[i].g << endl;
    }
    Solve();
    vector<bool> ans(n+1);
    for(int i = 1; i <= n; i++){
        int idx = obj[i].idx;
        ans[idx] = best.decvars[i];
    }
    ofstream outfile;
    outfile.open("output_sack.txt",ofstream::out);
    outfile << best.cv << " " << 1 << endl;
    for(int i = 1; i <= n; i++){
        outfile << ans[i] << " ";
    }
}


