/* 8/15/17 23tree. This data structure is similar to a BST in sorting and 
 * placement, but differs because it is always balanced. A BST only places nodes
 * according to how the new data relates to data already in the tree. But a 
 * 23tree places data in the same way, but splits nodes and pushes data up the 
 * tree to ensure it is always balanced.
 */
#include <cstring>
#include <cctype>
#include <iostream>

#ifndef TWO_TRHEE_TREE_
#define TWO_TRHEE_TREE_
int cnt1;
//Sorting functions, we have Bubble sort and check if it's sorted functions 
template <class DATA>
bool sort_data(DATA* data, int len);
template <class DATA>
bool sorted(DATA* data, int len);

//Friend functions. They needed to be declared before the classes
template <class DATA>
class node;
template <class DATA>
std::ostream& operator<<(std::ostream & out, const node<DATA> & obj);

template <class DATA>
class tree;

template <class DATA>
std::ostream& operator<<(std::ostream & out, const tree<DATA> & obj);

//NODE class. This class handles almost all of the moving and insertion.
template <class DATA>
class node
{
    public:
        node();
        node(const node<DATA> & obj);
        node(const DATA& obj);
        ~node();
        //Operators
        std::ostream& display_all(std::ostream & out, const node<DATA> * root, int & invoc) const;
        std::ostream& display_ordered(std::ostream & out, const node<DATA> * root) const;
        friend std::ostream& operator<< <DATA>(std::ostream & out, const node<DATA> & data);
        //Public functions
        void remove_all(node *& root);
        bool insert(node<DATA> *& root, const DATA & data);
        bool search(node<DATA> *& root, const DATA & obj);
        void ret_val();
    private:
        bool is_full(void) const;
        bool is_leaf(void) const;
        node<DATA>* next_man(int child) const; //Traverse manually to the next child
        node<DATA>* next(const DATA& data) const;//Traverse based on the data.
        int which_child(const DATA& data) const;
        bool connect(node<DATA> * new_child, int child_num);

        bool split(const DATA& new_data, DATA& middle_data, node<DATA> *& new_left, node<DATA> *& new_right);
        bool absorb(DATA& middle_data, node<DATA> *& new_left, node<DATA> *& new_right);
        bool insert_here(const DATA& data);
        bool insert(const DATA& data, node<DATA> * root, DATA& middle_data, node<DATA> *& new_left, node<DATA> *& new_right);

        DATA* data; /* Array of 2 datum */
        node<DATA> ** child; /* Array of 3 children. If there are 4 we split */
        int data_count;
};

template <class DATA>
class tree
{
    public:
        tree();
        tree(const tree<DATA>& obj);
        ~tree();
        bool insert(const DATA& data);
        bool search(const DATA& data);
        void ret_val();
        std::ostream& display_all(std::ostream & out) const;
        std::ostream& display_ordered(std::ostream & out) const;
        friend std::ostream& operator<< <DATA>(std::ostream & out, const tree & obj);
        tree<DATA>& operator=(const tree<DATA> & obj);
    private:
        node<DATA> * root;
};

#include "two_three_node.cpp"
#include "two_three_tree.cpp"

#endif // TWO_TRHEE_TREE_