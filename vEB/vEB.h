#ifndef VEB
#define VEB

typedef unsigned long long llu;
typedef unsigned long long DataType;

class vEB {

protected:
    llu size; // size of van emde boas tree
    llu subSize;
    DataType *minValue, *maxValue; // store min and max value
    vEB *summary;
    vEB **cluster;

    llu high(DataType x);
    llu low(DataType x);

public:
    vEB(llu u);
    virtual ~vEB();

    bool find(DataType x);
    DataType successor(DataType x);

    DataType predecessor(DataType x);

    void insert(DataType x);

    void deleteData(DataType x);

    DataType getMin();

    DataType getMax();

    DataType extractMin();

    void decreaseKey(DataType x, DataType y);
};

#include "vEB.cpp"

#endif