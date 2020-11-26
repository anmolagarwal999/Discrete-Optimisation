#include<bits/stdc++.h>
using namespace std;

int R, C, F, N, B, T;
//Stores data for each ride
struct Ride{
    int done, a, b, x, y, earliest_start, lastest_end, length;
}; vector<Ride> rides;

//Stores data for each car
struct Car{
    int x, y, occupied_till;
    vector<int> served;
}; vector<Car> cars;

struct pqsrt{ //sort in priority queue by till when car is occupied
    bool operator()(const int &u, const int &v){
        return cars[u].occupied_till > cars[v].occupied_till;
    }
};
priority_queue<int, vector<int>, pqsrt> pq; // stores index of car

int distFunc(int x1, int y1, int x2, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
}

void assign(int car_id, int ride_id, int t){
    int transit_dist = distFunc(cars[car_id].x, cars[car_id].y, rides[ride_id].a, rides[ride_id].b);
    int start_time = max(rides[ride_id].earliest_start, t + transit_dist - 1); //max of earliest start time or car reaching the ride

    cars[car_id].occupied_till = start_time + rides[ride_id].length; //car is now occupied till end of this ride
    cars[car_id].x = rides[ride_id].x; cars[car_id].y = rides[ride_id].y;
    cars[car_id].served.push_back(ride_id);

    rides[ride_id].done = 1;
}

void init(){
    cin >> R >> C >> F >> N >> B >> T; rides.resize(N); cars.resize(F);
    for(int i = 0; i < N; i++){
        cin >>rides[i].a >> rides[i].b >> rides[i].x >> rides[i].y >> rides[i].earliest_start >> rides[i].lastest_end;
        rides[i].done = 0; rides[i].length = distFunc(rides[i].a, rides[i].b, rides[i].x, rides[i].y);
    }
    for(int i = 0; i < F; i++){
        cars[i].x = cars[i].y = cars[i].occupied_till = 0;
        pq.push(i);
    }
}

int choose(int car_id, int t){
    int bidx = -1 ; double bscore = INT_MIN;
    //iterate over rides and pick one with best picking score
    for(int i = 0; i < N; i++){
        if(rides[i].done==1) continue;

        int trans_dist = distFunc(cars[car_id].x, cars[car_id].y, rides[i].a, rides[i].b);
        int start_time = max(rides[i].earliest_start, t + trans_dist - 1);
        if(start_time + rides[i].length > rides[i].lastest_end) continue; // will finish too late so just ignore

        int wasted_time = start_time - t; //time in which car does not earn any score
        
        int bonus = 0;
        if(start_time == rides[i].earliest_start)
            bonus = B;
        double delta = (rides[i].lastest_end - t);

        double score = bonus + 0.0*rides[i].length - wasted_time ; 
        // double score = bonus + 0.0065*rides[i].length - wasted_time ; //Test C 
        // double score = bonus + 0.0*rides[i].length - wasted_time - 0.018*delta; // Test D
        if(score > bscore){
            bidx = i;
            bscore = score;
        }
    } 
    return bidx;
}

signed main(int argc, char* argv[]){
    freopen(argv[1], "r", stdin); freopen("output_file.out", "w", stdout); 
    init();
    for(int i = 0; i < T; i++){ //simulate each time step
        while(pq.size() && cars[pq.top()].occupied_till <= i){ //pick a car that is now free/available
            int car_id = pq.top(); pq.pop();
            int ride_id = choose(car_id, i); //choose a ride for this car
            if(ride_id != -1){ //if such a ride exists
                assign(car_id, ride_id, i); //assign this car the ride
                pq.push(car_id); //push car back into the busy queue
            }
        }
    }
    for(int i = 0; i < F; i++){ //output stuff
        int m = cars[i].served.size();
        cout << m << " ";
        for(int j = 0; j < m; j++){
            cout << cars[i].served[j];
            if(j < m-1) cout << " ";
        }
        if(i < F-1) cout << endl;
    }
}