#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;



int main()
{

  vector< int > list( 5, 10 );
    list[ 2 ] = 99;
  for (int i=0; i < list.size(); i++){
    cout << list[i] << " ";
  }
  cout <<endl;
}
