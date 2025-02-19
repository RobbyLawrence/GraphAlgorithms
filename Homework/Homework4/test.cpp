#include <iostream>
#include <vector>
#include <random>
using namespace std;

int main() {
  random_device rd;
  mt19937 gen{rd()};
  uniform_int_distribution<int> uid(1,1000);
  for (int i = 0;i<50;i++) {
    if (i % 5 == 0) {
      cout << '\n';
    }
    cout << uid(gen) << " ";
  }
  return 0;
}
