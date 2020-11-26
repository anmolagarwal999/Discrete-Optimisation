// Solve TSP using greedy method
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
ll n;
ld dist(pair< ld, ld > vertex[], ll i, ll j)
{
	return sqrt(((vertex[i].first - vertex[j].first)*(vertex[i].first - vertex[j].first)) + ((vertex[i].second - vertex[j].second)*(vertex[i].second - vertex[j].second)));
}


int main()
{
	srand(time(NULL));
	time_t s_time;
	s_time = time(NULL);
	
	ifstream infile;
	infile.open("input_tsp.txt");
	infile>>n;

	pair < ld, ld > vertex[n];
	ll route[n], used[n];
	ld route_sum = 0;

	for(ll i = 0; i < n; i++)
	{	
		used[i] = 0;
		infile>>vertex[i].first>>vertex[i].second;
	}
	infile.close();

	
	route[0] = 0;
	used[0] = 1;
	for(ll i = 0; i < n - 1; i++)
	{
		ld min = -1;
		ll ind = -1;
		for(ll j = 0; j < n; j++)
		{
			if(!used[j])
			{
				if(min == -1)
				{
					min = dist(vertex, route[i], j);
					ind = j;
					continue;
				}
				if(dist(vertex, route[i], j) < min)
				{
					min = dist(vertex, route[i], j);
					ind = j;
				}
			}
		}
		route[i + 1] = ind;
		used[ind] = 1;
		route_sum += min;
	}
	route_sum += dist(vertex, 0, route[n-1]);
	ofstream outfile;
	outfile.open("output_tsp.txt",ofstream::out);
	outfile<<route_sum<<" 0\n";
	for(ll i = 0; i < n; i++)
	{
		outfile<<route[i]<<" ";
	}

	outfile<<endl;
	
}