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

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((Y) < (X)) ? (X) : (Y))

#define MINSIZE 2

class vEB
{
    private:
        unsigned int u; // Size of the universe of the current recursive vEB
        unsigned int subU; //The sizes of the sub universes directly connected to the current vEB (Size of clusters)
        unsigned int* min;
        unsigned int* max; //Min Max help to reduce the amount of potential recursive steps
        vEB *summary;
        vEB **cluster;

        unsigned int high(unsigned int x) {
            return x/subU;
        }

        unsigned int low(unsigned int x) {
            return x%subU;
        }

    public:
        vEB(unsigned int u) {
            this->u = u;
            min = max = NULL;
            if (u == MINSIZE) {
                this->summary = NULL;
                this->cluster = NULL;
            } else {
                subU = ceil(sqrt(u));
                u = subU * subU;
                //Allocate the necessary memory for the clusters if they have yet been accessed yet.
                //Memory allocation only needs to be done once per cluster and summary.
                this->summary = new vEB(subU);
                this->cluster = new vEB*[subU];

                for (unsigned int i=0; i<subU; i++) {
                    this->cluster[i] = NULL;
                }
            }
        }
        

        void insert(unsigned int x) {
            if (min == NULL) {
                max = new unsigned int(x);
                min = new unsigned int(x);
            } else{
                if (x < *min) {
                    swap(x, *min);
                }

                if (u > 2) {
                    unsigned int highIndex = high(x);
                    unsigned int lowIndex  = low(x);
                    if (cluster[highIndex] == NULL) {
                        cluster[highIndex] = new vEB(subU);
                        summary->insert(highIndex);
                        cluster[highIndex]->insert(lowIndex);
                    } else {
                        cluster[highIndex]->insert(lowIndex);
                    }
                }

                if (x > *max) *max = x;
            }
        }
};

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
    return true;
}


int main()
{
    if(!test_performance()) {
        cout << "Error testing tree's performance" << endl;
    }
    return 0;
}
