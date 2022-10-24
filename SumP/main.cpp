#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>

using namespace std;


bool isSubsetSum(vector<int> set, int n, int sum)
{

    if (sum == 0)
        return true;
    if (n == 0)
        return false;


    if (set[n - 1] > sum)
        return isSubsetSum(set, n - 1, sum);


    return isSubsetSum(set, n - 1, sum)
           || isSubsetSum(set, n - 1, sum - set[n - 1]);
}

int main()
{

    ifstream inFile("input.txt");

    if (inFile.fail()) {
        cerr << "Could not find file" << endl;
    }
    vector<int> set;
    int S;
    while (inFile>>S) {
        set.push_back(S);
    }
    inFile.close();

    int sum = 5;
    int n = sizeof(set) / sizeof(set[0]);
    if (isSubsetSum(set, n, sum) == true)
        cout <<"Found a subset with given sum";
    else
        cout <<"No subset with given sum";
    return 0;
}
