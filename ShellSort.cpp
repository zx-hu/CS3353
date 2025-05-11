#include "MyObj.h"
#include <vector>

vector<int> ShellSort(vector<MyObj>& arr, int code)
{
  vector<int> res = {1};
  int len = arr.size();
  if (len <= 1){return res;}

  if(code == 0){   //hlist should be [1] (i.e. essentially insertion sort)
    //hlist is already 1, do nothing
  }else if(code == 1){ //hlist should be [2^k-1, 2^(k-1)-1, 2^(k-2)-1 ... 3, 1] where k is the maximum number such that 2^k – 1 is still (strictly) smaller than the number of objects to be sorted
    int count = 2;
    int num = static_cast<int>(pow(2, count)) - 1;
    while(num < len){
      res.push_back(num);
      count++;
      num = pow(2, count) - 1;
    }
  }else if(code == 2){ //hlist should be [(3^k-1)/2, (3^(k-1)-1)/2 , ... 4, 1], where k is k is the maximum number such that (3k-1)/2 is still (strictly) smaller than the number of objects to be sorted.
    int count = 2;
    int num = static_cast<int>((pow(3, count)) - 1)/2;
    while(num < len){
      res.push_back(num);
      count++;
      num = (pow(3, count) - 1)/2;
    }
  }else if(code == 3){
    //The first number of the list is [1]
    //Every iteration we add three numbers to the end of the list: [2* the last number, 3 * the last number, 6 * the last number]
    //Repeat the previous step until the number is larger than n (the size of the array to be sorted)
    //Remove all numbers that is larger or equal to n
    //Hlist is the reverse of the list created above.
    int count = 1;
    while(count < len){
      res.push_back(count*2);
      res.push_back(count*3);
      res.push_back(count*6);
      count *=6;
    }
    
    //find first element greater than len
    auto it = upper_bound(res.begin(), res.end(), len);
    //erase numbers too large
    res.erase(it, res.end());
  }else{    //not 0, 1, 2, 3  
    cerr << "Error: invalid method parameter " << code;        //print an error message and exit the program immediately
    exit(1);
  }

  reverse(res.begin(), res.end());

  //shell sort code
  for(int k = 0; k < res.size(); k++){
    int h = res[k];
    for(int j = 1; j < len; j++){
      int i = j;
      while(i >= h && arr[i] < arr[i-h]){ //while prev element is larger than curr element
        MyObj temp = arr[i];
        arr[i] = arr[i-h];
        arr[i-h] = temp;
        
        i-=h;
      }
    }
  }



  /*//test code
  cout << "hlist: ";
  for(auto i : res){
    cout << i << " ";
  }
  cout << endl;
  */

  return res;
}
