#include <iostream>
#include <random>
#include <fstream>
#include <limits>

using namespace std;

int main()
{
    ofstream myfile;
    myfile.open("data.txt");
    
    long n;
    cin >> n;
    random_device rd;   
    mt19937_64 eng(rd());
    uniform_int_distribution<unsigned int> distr;

    
    for (int i=0; i<n; i++) {
        myfile << distr(eng) << endl;
    }
}
