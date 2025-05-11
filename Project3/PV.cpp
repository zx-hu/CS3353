#include <vector>
#include "MyObj.h"


int PV(vector<MyObj>& v, int s, int t)
{ 
  //base case
  if(s >= t){
    return s;
  }

  bool sDecr = v[s] > v[s+1]; //true = decr, false = incr
  bool tDecr = v[t-1] > v[t]; //true = decr, false = incr

  if((sDecr == true) == (tDecr == true)){ //xor operation
    //sDecr and tDecr are the same
    //return s if decr, t if incr
    return sDecr ? s : t;
  }

  //middle of s and t
  int m = (s+t)/2; 
  //cout << s << " " << m << " " << t << endl;

  if(t-s == 2){ //if not the same and only 3 elements, return the middle
    return PV(v, s+1, t-1); //sending it to the base case, but it's just going to return the middle
  }
  //else, search again
  bool mDecr = v[m] > v[m+1];
  if(sDecr == mDecr){ //if mid matches start
    return PV(v, m, t);
  }
  //else, mid matches end
  return PV(v, s, m+1); 
}


