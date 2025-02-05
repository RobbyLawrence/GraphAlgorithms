#include <iostream>
#include <vector>
using namespace std;
int main () {
  vector<int> test;
  for (int i=0;i < 10;i++) {

       for(int j=0;j <= i;j++) {

         for(int k=0; k <= j ;k++) {

           test.push_back(i); }}}
  cout << test.size();
}
