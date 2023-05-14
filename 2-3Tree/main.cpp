#include "two_three_tree.h"
#include <bits/stdc++.h>
#include "windows.h"
#include "psapi.h"

using namespace std;
using namespace std::chrono;

bool test_performance(void);

int main()
{
    if(!test_performance()) {
        cout << "Error testing tree's performance" << endl;
    }
    return 0;
}


bool test_performance()
{
    srand(clock());
    tree<unsigned int> test_tree;

    std::ifstream fin;
    fin.open("C:\\Users\\TaoY\\Desktop\\VSCode\\EE Code\\data.txt");
    string line;

    std::vector<unsigned int> arr;
    int cnt = 0;
    if (fin.is_open()) {
        while (getline(fin, line)) {
            arr.push_back({(unsigned int)(stoll(line))});
            cnt++;
        }
        fin.close();
    }
    auto begin = high_resolution_clock::now();
    for (auto a : arr) {
        if (!test_tree.insert(a)) {
            cout << "Insert failed" << endl;
            return false;
        }
    }
    auto end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>(end-begin).count() << endl;
    
    vector<unsigned int>().swap(arr);
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
    
    cout << physMemUsedByMe << endl;
    cout << cnt << endl;

    test_tree.ret_val();
    
    return true;
}
