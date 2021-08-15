#include <iostream> 
#include <vector>
using namespace std;

// Method 1
//class Solution{
//public:
//    int conCompleteCircuit(vector<int>& gas, vector<int>& cost) {
//       for (int i = 0; i < cost.size(); i++) {
//           int reset = gas[i] - cost[i]; // least gas
//           int index = (i + 1) % cost.size(); // cur path
//           while (reset > 0 && index != i) {
//               // run around
//               reset += gas[i] - cost[i];
//               index = (index + 1) % cost.size();
//           }
//           if (reset >= 0 && index == i) {
//               return i;
//           }
//       }
//       return -1;
//    }
//};

// Method 2
class Solution {
public:
    int conCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int curSum = 0;
        int min = INT_MAX;
        for (int i = 0; i < gas.size(); i++) {
            int reset = gas[i] - cost[i];
            curSum += reset;
            if (curSum < min) {
                min = curSum;
            }
        }
        if (curSum < 0) return -1;
        if (min >= 0) return 0;

        for (int i = gas.size() - 1; i >= 0; i--) {
            int reset = gas[i] - cost[i];
            min += reset;
            if (min >= 0) {
                return i;
            }
        }
        return -1;
    }
}

int main() {
    int a[5] = {1, 2, 3, 4, 5};
    int b[5] = {3, 4, 5, 1, 2};
    vector<int> gas(a, a + 5);
    vector<int> cost(b, b + 5);
    Solution sol;
    int result = sol.conCompleteCircuit(gas, cost);
    cout << result  << endl;
    return 0;
}
