#include <bits/stdc++.h>

using namespace std;

#define pb push_back

const int N=1e3+5, M=1e6+6; 
struct edge
{
	int u, v, w;
} edges[M], cedges[M]; //edges of normal and condensation graph
int num, m, cmpcnt, cmpof[N], condejcnt, vis[N]; //cmpcnt = no. of components, condejcnt = no. of edges in DAG
vector< int > adj[N], dfsord, radj[N], cadj[N]; //radj is reversed adjlist, cadj is condensation graph adjlist
vector < vector < int > > comps; //reverse toposorted list of components
void dfs1(int u) {
	vis[u]=true;
	for(auto it: adj[u])
		if(!vis[edges[it].v])
			dfs1(edges[it].v);
	dfsord.pb(u);
}
void dfs2(int v) {
	vis[v]=1; cmpof[v]=cmpcnt;	comps[cmpcnt].pb(v);
	for(auto it: radj[v]){
		if(!vis[edges[it].u]){
			dfs2(edges[it].u);
        }
    }
}

void resetscc() { //clears the contents
	cmpcnt=condejcnt=0;
	for(int i = 1; i <= num; i++) {
		vis[i]=cmpof[i]=0;	radj[i].clear(); cadj[i].clear();
	}
	for(auto cc: comps) cc.clear();
	comps.clear();	dfsord.clear();
}
void SCC() {
	resetscc();
	for(int i = 1; i <= num; i++)
		for(auto it: adj[i])
			radj[edges[it].v].pb(it);
	for(int i = 1; i <= num; i++)
		if(!vis[i]){
			dfs1(i);
        }
	for(int i = 1; i <= num; i++) vis[i]=0;
	vector<int> tmp; comps.pb(tmp);
	for(auto v: dfsord) {
		if(!vis[v]) {
            //cerr << v << endl;
			cmpcnt++; comps.pb(tmp); dfs2(v);
		}
	}
}
void Condense() {
	for(int i = 0; i < m; i++) {
		if(cmpof[edges[i].u]!=cmpof[edges[i].v]) {
			cadj[cmpof[edges[i].u]].pb(condejcnt);
			cedges[condejcnt]={cmpof[edges[i].u], cmpof[edges[i].v], edges[i].w};
			condejcnt++;
		}
	}
}

int mat[N][N];
int main(){
    freopen("data/inp250_2.txt", "r", stdin);
    freopen("data/sccinp250_2.txt", "w", stdout);
    m = 0;
    cin >> num;
    for(int i = 1; i <= num; i++){
        for(int j = 1; j <= num; j++){
            cin >> mat[i][j];
            if(mat[i][j]!=0){
                edges[m] = {i, j, mat[i][j]};
                //if(m%1000==0) cerr << edges[m].u << " " << edges[m].v << endl;
                adj[edges[m].u].pb(m);
                m++;
            }
        }
    }
    SCC();
    Condense();
    cout << m << " " << cmpcnt << endl;
    for(int i = 1; i <= cmpcnt; i++){
        cout << comps[i].size() << " - ";
        for(auto v: comps[i]){
            cout << v << " ";
        }
        cout << endl;
    }
}