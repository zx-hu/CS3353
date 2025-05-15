#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

pair< vector<int>, float >  assign(int N, int E, int M, float **prob)
{
    vector <int> res0;
    int choice[N][E+1];     //the choice we made for each step
    float dp[N][E+1];       //array for probability of success

    //assorted edge cases
    //if less energy than number of vehiles
    if (E < N) {
        return make_pair(vector<int>(N,0), 0.0f);  // Not enough energy for min requirements
    }


    //just want to fill all the slots with default values, avoid errors
    for(int i=0; i < N; i++){
        for(int j=0; j<E+1; j++){
            choice[i][j] = 0;       //all choices 0
            dp[i][j] = -INFINITY;   //all probs the min
        }
    }


    //use log prob so we can add them, instead of needing to mult
    //base case first vehicle
    for(int c=1; c<=min(M, E); c++){
        dp[0][c] = log(prob[0][c-1]); //because prob[i][j] maps to energy j+1
        choice[0][c] = c;   //the only choice rn lol
    }

    

    for (int r=1; r<N; r++){        //N = number of vehicles
        for (int c=r+1; c<E+1; c++){    //E = total unit available
            for (int i=1; i<=min(M, c); i++){//energy given to this vehile
                float prev = dp[r-1][c-i]; //look at dp of prev vehicle. total energy used so far-energy allocated to this vehicle
                if(prev != -INFINITY){  //skip if prev has no meaningful value
                    float curr = log(prob[r][i-1]) + prev;
                    if(curr > dp[r][c]){    //if current is better than prev recorded
                        choice[r][c] = i;   //record in choice
                        dp[r][c] = curr;    //record in dp array
                    }
                }
            } 
        }
    }

    //trace through the best choices
    int remainingEnergy = min(E, N*M); //covers if E > N*M and the last slot is unfilled
    float bestProb = exp(dp[N-1][remainingEnergy]);
    for (int i=N-1; i >=0; i--){
        res0.push_back(choice[i][remainingEnergy]);	
        remainingEnergy-=choice[i][remainingEnergy];
    }
    //we worked backwards so the vector is flipped. unflip it
    reverse(res0.begin(), res0.end());

    return make_pair(res0, bestProb);

}




pair< vector<int>, float >  assign2(int N, int E, int M, float **prob)
{
    vector <int> res0;
    float p = prob[N-1][M-1];

    //array of overloaded probabilities
    float overload_prob[N][M];
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            overload_prob[i][j] = min(float(prob[i][j] + 0.2), max(float(0.999), prob[i][j]));
        }
    }

    //base prob from q1
    pair<vector<int>, float> best = assign(N, E, M, prob);
    
    //1 overload
    if(E-1 >= N){   //check that we have enough energy to overload
        for(int i=0; i<N; i++){
            //new prob table we will pass to the function
            float** temp_prob = new float*[N];
            for (int j = 0; j < N; j++) {
                temp_prob[j] = new float[M];
            }

            for(int r=0; r<N; r++){             
                if(r == i){
                    for(int c=0; c<M; c++){      //copy overloaded prob for current vehicle
                        temp_prob[r][c] = overload_prob[r][c];
                    }
                }
                else{
                    for(int c=0; c<M; c++){     //copy prob for all NOT current vehicle
                        temp_prob[r][c] = prob[r][c];
                    }
                }
            }
            pair<vector<int>, float> curr = (assign(N, E-1, M, temp_prob));    //one less energy because 1 overload
            if(curr.second > best.second){
                curr.first[i] = -curr.first[i]; //flip the value to indicate overloaded
                best = curr;
            }

            //clean up memory
            for (int j = 0; j < N; j++) {
                delete[] temp_prob[j];
            }
            delete[] temp_prob;
        }
    }

    //2 overload
    if(E-2 >= N){   //check that we have enough energy to overload
        for(int i=0; i<N; i++){         //overloaded vehicle 1
            for(int j=i+1; j<N; j++){   //overloaded vehicle 2
                //new prob table we will pass to the function
                float** temp_prob = new float*[N];
                for (int k = 0; k < N; k++) {
                    temp_prob[k] = new float[M];
                }


                for(int r=0; r<N; r++){             
                    if(r == i || r == j){
                        for(int c=0; c<M; c++){      //copy overloaded prob for current vehicle
                            temp_prob[r][c] = overload_prob[r][c];
                        }
                    }
                    else{                            //copy prob for all NOT current vehicle
                        for(int c=0; c<M; c++){ 
                            temp_prob[r][c] = prob[r][c];
                        }
                    }
                }

                pair<vector<int>, float> curr = (assign(N, E-2, M, temp_prob));    //two less energy because 2 overload
                if(curr.second > best.second){
                    curr.first[i] = -curr.first[i]; //flip the value to indicate overloaded
                    curr.first[j] = -curr.first[j]; //flip the value to indicate overloaded
                    best = curr;
                }
                //clean up memory
                for (int k = 0; k < N; k++) {
                    delete[] temp_prob[k];
                }
                delete[] temp_prob;
            }
        } 
    }
    
    return best;

}
