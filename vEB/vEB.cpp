/*
Credits to zhuziyu_edward, the author to the article of https://blog.csdn.net/zhuziyu_edward/article/details/104342403
The program is roughly based on this implementation with minor changes to suit the needs of this experiment.
The program is under this license https://creativecommons.org/licenses/by-sa/4.0/
*/

#include <bits/stdc++.h>
#include "windows.h"
#include "psapi.h"

using namespace std;
using namespace std::chrono;
#define ERR_NOT_FOUND -1000000000

typedef unsigned int llu;
typedef unsigned int DataType;
int cnt1 = 0;
class vEB {
protected:
    llu size; // size of van emde boas tree
    llu subSize;
    llu minValue, maxValue; // store min and max value
    vEB *summary;
    vEB **cluster;

    llu high(DataType x) {
        return x / subSize;
    }

    llu low(DataType x) {
        return x % subSize;
    }

public:
    vEB(llu u) : size(u), minValue(-1), maxValue(-1) {
        if (u == 2) {
            this->summary = NULL;
            this->cluster = NULL;
        }
        else {
            subSize = (llu) ceil(sqrt(u));
            size = subSize * subSize;
            this->summary = new vEB(subSize);
            this->cluster = new vEB*[subSize];
            for (int i = 0; i < subSize; i++)
                this->cluster[i] = NULL;
        }
    }

    virtual ~vEB() {
        delete summary;
        if (cluster != NULL) {
            for (int i = 0; i < subSize; i++)
                delete cluster[i];
        }
        // delete minValue;
        // delete maxValue;
    }

    bool find(DataType x) {
        if (size == 2) {
            if (minValue == -1)
                return false;
            else if (x == minValue || x == maxValue)
                return true;
            else
                return false;
        }
        else {
            if (minValue == -1)
                return false;
            if (x < minValue || x > maxValue)
                return false;
            if (x == minValue || x == maxValue)
                return true;
            if (cluster[high(x)] == NULL)
                return false;
            return cluster[high(x)]->find(low(x));
        }
    }

    

    void insert(DataType x) {
        if (size == 2) {
            if (minValue == -1)
                minValue = x;
            else
                minValue = min(minValue, x);
            if (maxValue == -1)
                maxValue = x;
            else
                maxValue = max(maxValue, x);
        }
        else {
            if (minValue == -1) {
                minValue = (x);
                maxValue = (x);
            }
            else {
                if (x < minValue)
                    swap(minValue, x);
                if (x > maxValue)
                    maxValue = x;
                if (cluster[high(x)] == NULL) {
                    cnt1++;
                    cluster[high(x)] = new vEB(subSize);
                    summary->insert(high(x));
                }
                cluster[high(x)]->insert(low(x));
            }
        }
    }


    DataType getMin() {
        if (minValue == -1)
            return ERR_NOT_FOUND;
        else
            return minValue;
    }

    DataType getMax() {
        if (maxValue == -1)
            return ERR_NOT_FOUND;
        else
            return maxValue;
    }
};

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
    
    vEB *tree = new vEB(4294967295);
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
        tree->insert(a);
    }
    auto end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>(end-begin).count() << endl;
    
    vector<unsigned int>().swap(arr);
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
    
    cout << physMemUsedByMe << endl;
    cout << cnt << endl;
    cout << cnt1 << endl;
    return true;
}
