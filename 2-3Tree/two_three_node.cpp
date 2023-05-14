#include "two_three_tree.h"
#include <bits/stdc++.h>
#include "windows.h"
#include "psapi.h"
using namespace std;
using namespace std::chrono;

//Wrapper for the insert
template<class DATA>
bool node<DATA>::insert(node *& root, const DATA& data)
{
    DATA temp_data;
    node<DATA> * right = NULL;
    node<DATA> * left = NULL;
    node<DATA> * temp_root = NULL;

    if( !root -> insert( data, root, temp_data, left, right) )
        return false;

    if( left || right )
    {
        cnt1++;
        temp_root = new node<DATA>(temp_data);
        temp_root -> connect(left, 0);
        temp_root -> connect(right, 2);
        temp_root -> connect(NULL, 1);
        root = temp_root;
    }

    return true;
}

//Recursively insert the data into the tree
unsigned int split_time = 0;
template<class DATA>
bool node<DATA>::insert(const DATA& data, node<DATA> * root, 
        DATA& middle_data, 
        node<DATA> *& new_left, 
        node<DATA> *& new_right)
{
    if( !root ) return false;

    //Recurisve call
    insert(data, root -> next(data), middle_data, new_left, new_right);

    //If the recursive call had a push up event, we need to either take that
    //data or split ourselves.
    if( new_left || new_right )
        return root -> absorb(middle_data, new_left, new_right );

    //If we're a leaf and we're NOT full we can insert.
    if( root -> is_leaf() && !root -> is_full() )
        return root -> insert_here(data);

    //If we're a leaf and we're full we need to split.
    if( root -> is_leaf() && root -> is_full() ) {
        auto begin = high_resolution_clock::now();
        bool result = root -> split(data, middle_data, new_left, new_right);
        auto end = high_resolution_clock::now();
        split_time += duration_cast<nanoseconds>(end-begin).count();
        return result;
    }

    return true;
}
template<class DATA>
void node<DATA>::ret_val()
{
    cout << split_time << endl;
    return;
}
template<class DATA>
bool node<DATA>::search(node<DATA> *& root, const DATA &obj)
{
    if (!root) return false;

    switch (data_count) {
        case 1:
            if (obj > data[0]) {
                return root->search(child[2], obj);
            } else if (obj < data[0]) {
                return root->search(child[0], obj);
            } else {
                return true;
            }
            break;

        case 2:
            if (obj > data[1]) {
                return root->search(child[2], obj);
            } else if (obj < data[0]) {
                return root->search(child[0], obj);
            } else if (obj == data[0] || obj == data[1]) {
                return true;
            } else {
                return root->search(child[1], obj);
            }
            break;
    }

    return false;
}


//Node absorb function.
template<class DATA>
bool node<DATA>::absorb(DATA& middle_data, node<DATA> *& new_left, node<DATA> *& new_right)
{
    int child_index = -1;
    if( !is_full() )
    {
        child_index = which_child(middle_data);
        insert_here(middle_data);

        if( child_index == 2 )
            connect(new_left, 1);

        if( child_index == 0 )
        {
            connect(new_left, 0);
            connect(new_right, 1);
        }

        new_left = new_right = NULL;
        return true;
    }

    else //this node is full.
    {
        node<DATA> * temp_right = new_right;
        node<DATA> * temp_left = new_left;
        child_index = which_child(middle_data);
        DATA temp_middle = middle_data;
        new_right = new_left = NULL;
        split(temp_middle, middle_data, new_left, new_right);

        if( child_index == 2 )
        {
            new_left -> connect(child[0],0);
            new_left -> connect(child[1],2);
            new_left -> connect(NULL, 1);

            new_right -> connect(temp_left,0);
            new_right -> connect(temp_right,2);
            new_right -> connect(NULL,1);
        }

        if( child_index == 1 )
        {
            new_left -> connect(child[0],0);
            new_left -> connect(temp_left,2);
            new_left -> connect(NULL, 1);

            new_right -> connect(temp_right,0);
            new_right -> connect(child[2],2);
            new_right -> connect(NULL,1);
        }

        if( child_index == 0 )
        {
            new_left -> connect(temp_left, 0);
            new_left -> connect(temp_right, 2);
            new_left -> connect(NULL, 1);

            new_right -> connect(child[1],0);
            new_right -> connect(child[2],2);
            new_right -> connect(NULL,1);
        }
    }
    return true;
}

//This is the split function. It returns a new node, the middle data, and this
//node all in order.
template <class DATA>
bool node<DATA>::split(const DATA& new_data, DATA& middle_data, node<DATA> *& new_left, node<DATA> *& new_right)
{
    DATA* temp_array = new DATA[3];
    //assert(temp_array);
    for(int i=0;i < 2; ++i) {
        temp_array[i] = data[i];
    }
    temp_array[2] = new_data;

    //sort(temp_array.begin(), temp_array.end());
    sort_data(temp_array, 3);

    delete [] data;
    data = new DATA[2];
    data[0] = temp_array[2];
    data_count = 1;

    middle_data = temp_array[1];
    new_left = new node<DATA>(temp_array[0]);
    new_right = this;

    delete [] temp_array;
    temp_array = NULL;

    return true;
}

//ostream Extraction operator implemented here.
template <class DATA>
std::ostream& operator<<(std::ostream & out, const node<DATA> & obj)
{
    int i = 0;
    out << '(';
    for(; i < obj.data_count && i < 2; ++i)
    {
        if( i ) 
            out << ",";
        out << obj.data[i];
    }
    out << ')';
    return out;
}

template <class DATA>
std::ostream& node<DATA>::display_all(std::ostream & out, const node<DATA> * root, int & invoc) const
{
    if( !root ) return out;

    for(int i = 0; i < invoc; ++i)
        out << " -";
    out << *root << std::endl;
    ++invoc;

    for(int i = 0; i < 3; ++i)
        display_all( out, root -> child[i], invoc );
    --invoc;

    return out;
}

//Display all data in this subtree in sorted order.
template <class DATA>
std::ostream& node<DATA>::display_ordered(std::ostream & out, const node<DATA> * root) const
{
    if(!root) return out;
    if( root -> is_leaf() )
    {
        out << root -> data[0] << ", ";
        if( root -> data_count >= 2)
            out << root -> data[1] << ", ";
        return out;
    }

    display_ordered(out, root -> next_man(0));
    out << root -> data[0] << ", ";
    if( root -> data_count >= 2)
    {
        display_ordered(out, root -> next_man(1));
        out << root -> data[1] << ", ";
    }
    display_ordered(out, root -> next_man(2));

    return out;
}

//This function inserts new data into the node. It returns false if it's full.
template <class DATA>
bool node<DATA>::insert_here(const DATA& new_data)
{
    switch( data_count )
    {
        case 0:
            data[0] = new_data;
            ++data_count;
            break;
        case 1: //If data_count == 1 then we need to insert into the other spot
            data[1] = new_data;
            ++data_count;
            //sort(data.begin(), data.end());
            sort_data(data, 2);//Sort the data because now we have two datum
            break;
        default: //If data_count == 2 then this is a full node.
            return false;
    }
    return true;
}

//Returns the number of data points with the max number of data. It returns true
//if it is full, and false it is not full.
template <class DATA>
bool node<DATA>::is_full(void) const
{
    return (data_count >= 2);
}

//Does this node have children? It will return true if it does not have children
//and false if it does have children.
template <class DATA>
bool node<DATA>::is_leaf(void) const
{
    for(int i = 0; i < 3; ++i)
        if( child[i] ) return false;
    return true;
}

//Traverse manually to the next child.
template <class DATA>
node<DATA>* node<DATA>::next_man(int child_index) const
{
    return child[child_index];
}

//Returns the index of the next child based on the data.
template <class DATA>
int node<DATA>::which_child(const DATA& obj) const
{
    if( is_leaf() ) return -1;

    switch( data_count )
    {
        case 1:
            if( obj > data[0] )
                return 2;
            if( obj <= data[0] )
                return 0;
        case 2:
            if( obj > data[1] )
                return 2;
            if( obj <= data[0] )
                return 0;
            else
                return 1;
    }
    return -1;
}

//Traverse based on the data.
template <class DATA>
node<DATA>* node<DATA>::next(const DATA& obj) const
{
    //IF there are no children we can return NULL. Since this is pass pointer
    //by value anyway it won't matter.
    if( is_leaf() ) return NULL;

    switch( data_count )
    {
        case 1:
            if( obj > data[0] )
                return child[2];
            if( obj <= data[0] )
                return child[0];
        case 2:
            if( obj > data[1] )
                return child[2];
            if( obj <= data[0] )
                return child[0];
            else
                return child[1];
    }
    return NULL;
}

//This function connects a node to the indicated child. It will do this whether
//there is a node already there, or even if new_child is NULL. It doesn't really
//check for anything.
template <class DATA>
bool node<DATA>::connect(node<DATA> * new_child, int child_index)
{
    child[child_index] = new_child;
    return true;
}


//DEFAULT CONSTRUCTOR
template <class DATA>
node<DATA>::node()
{
    data_count = 0;
    data = new DATA[2];

    child = new node<DATA>*[3];
    for(int i = 0; i < 3; ++i)
        child[i] = NULL;
}

//COPY CONSTRUCTOR. This copy constructor DOES copy children.
template <class DATA>
node<DATA>::node(const node<DATA> & obj)
{
    data_count = obj.data_count;

    data = new DATA[2];
    for(int i = 0; i < data_count; ++i)
        data[i] = obj.data[i];

    child = new node<DATA>*[3];
    for(int i = 0; i < 3; ++i)
    {
        if( obj.child[i] )
            child[i] = new node<DATA>( *obj.child[i] );
        else
            child[i] = NULL;
    }
}

//CONSTRUCTOR. This constructor takes one datum and copies it to the node.
//This is useful for splitting nodes.
template <class DATA>
node<DATA>::node(const DATA& obj)
{
    data_count = 1;

    data = new DATA[2];
    data[0] = obj;

    child = new node<DATA>*[3];
    for(int i = 0; i < 3; ++i)
        child[i] = NULL;
}

//DESTRUCTOR. This destructor only deletes one node, not the children of this
//node.
template <class DATA>
node<DATA>::~node()
{
    if( data )
    {
        delete [] data;
        data = NULL;
    }

    if( child )
    {
        for(int i = 0; i < 3; ++i)
        {
            if( child[i] )
            {
                delete child[i];
                child[i] = NULL;
            }
        }

        delete [] child;
        child = NULL;
    }
}

//Removes all nodes in this subtree.
template <class DATA>
void node<DATA>::remove_all(node<DATA> *& root)
{
    if( !root ) return;
    for(int i = 0; i < 3; ++i)
    {
        if( root -> child[i] )
            remove_all( root -> child[i] );
    }
    delete root;
    root = NULL;
    return;
}



//SORTING functions
//Lazy sort. It works though and the data sets are very small.
template <class DATA>
bool sort_data(DATA* data, int len)
{
    //Temp holder
    DATA prev;
    for (int i = 0; i < len - 1; i++) {
        // Last i elements are already in place
        for (int j = 0; j < len - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                prev = data[j+1];
                data[j+1] = data[j];
                data[j] = prev;
            }
        }
    }

    return true;
}

// //Test if the array is sorted.
// template <class DATA>
// bool sorted(DATA* data, int len)
// {
//     for( int i = 1; i < len; ++i)
//     {
//         if( data[i-1] > data[i] )
//             return false;
//     }
//     return true;
// }