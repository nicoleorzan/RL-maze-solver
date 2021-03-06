#include "Environment.h"
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

struct compare {
    int key;
    compare(int const &i): key(i) {}
 
    bool operator()(int const &i){ 
        return (i == key);  
    }
};

Environment::Environment(int n, int fs, std::vector<int> w){
    N = n;
    final_state = fs; // if here the simulation terminates
    maze = new int[N*N];
    walls = w;
    fill_maze();
};

Environment::~Environment(){
    delete[] maze;
};

int Environment::random_start(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    int randval = dis(gen);

    while ( maze[randval] == 1 ){
        randval =  dis(gen);
    }
    return randval;
};

int Environment::get_final_state(){
    return final_state;
};

void Environment::set_final_state(int fs){
    final_state = fs;
};

void Environment::display_maze() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if ((i*N + j) == final_state) {
				printf("◎ ");
			}
			else if (maze[i*N + j] == 1) {
				printf("■ ");
			}
			else {
				printf("□ ");
                //std::cout<<i*N+j<<" ";
			}
		}
		printf("\n");
	}
    printf("\n");
};

std::vector<int> Environment::allowed_actions(int state){

    std::vector<int> allow_act;
    if ( state/N-1>=0 && maze[state-N] == 0 ){
        allow_act.push_back(0); // up
    }
    if ( state/N+1<N && maze[state+N] == 0 ){
        allow_act.push_back(1); // down
    }
    if ( (state)%N-1>=0 && maze[state-1] == 0 ){
        allow_act.push_back(2); // left
    }
    if ( (state)%N+1<N && maze[state+1] == 0 ){
        allow_act.push_back(3); // right
    }

    return allow_act;
};

void Environment::fill_maze(){

    for (int i=0; i<N*N; i++){
        if (std::find(walls.begin(), walls.end(), i) != walls.end()) {
            maze[i] = 1;
        } else { maze[i] = 0;}
    }

};

int Environment::next_state(int state, int action){
    int next_state = state;

    if (action == 0){
        if (state - N >= 0){ next_state = state - N; }
    }  
    else if (action == 1){
        if (state + N < N*N){ next_state = state + N; }
    } 
    else if (action == 2){
        if (state%N > 0){ next_state = state - 1; }
    } 
    else if (action == 3){
        if (state+1%N != 0 && state+1%N < N*N){ next_state = state + 1; }
    }

    if (maze[next_state] == 1){ next_state = state;}

    return next_state;
};

double Environment::sample_reward(int s, int reward_strategy){

    double reward = 0;
    if (reward_strategy == 0){
        reward = reward_func1(s);
    } else if (reward_strategy == 1){
        reward = reward_func2(s);
    }
    
    return reward;
};

double Environment::reward_func1(int state){

    double reward = 0;
    if ( state == final_state ){ reward = 1; }
    return reward;
};

double Environment::reward_func2(int state){

    double reward = -0.1;
    if ( state == final_state ){ reward = 100; }
    else if ( maze[state] == 1 ){ reward = -2; }
    
    return reward;
};

void Environment::print_policy(double *Q){

    double max_val;
    double max_idx;

    for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
            if ((i*N + j) == final_state) {
				printf("◎ ");
			}
            else if (maze[i*N + j] == 1) {
				printf("■ ");
			}
            else{
                max_val = -9999;
                max_idx = 4;
                for (int act=0; act<4; act++){
                    if ( Q[(i*N+j)*4+act] > max_val ){
                        max_val = Q[(i*N+j)*4+act];
                        max_idx = act;
                    }
                }
                if (max_idx==0){ printf("^ "); }
                else if (max_idx==1){ printf("V "); }
                else if (max_idx==2){ printf("< "); }
                else if (max_idx==3){ printf("> "); }
                else if (max_idx==4){ printf("□ "); }
            }
        }
        printf("\n");
    }
    printf("\n");
};
