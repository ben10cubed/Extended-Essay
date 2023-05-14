#include "two_three_tree.h"

//WRAPPER for the insert function.
template <class DATA>
bool tree<DATA>::insert(const DATA& data)
{
    node<DATA> * temp = root;
    if( !root -> insert(temp, data) )
        return false;
    root = temp;
    return true;
}

template <class DATA>
bool tree<DATA>::search(const DATA& data)
{
    node<DATA> *temp = root;
    if (!root->search(temp, data)) {
        return false;
    }

    return true;
}

template<class DATA>
void tree<DATA>::ret_val()
{
    node<DATA> * temp = root;
    root->ret_val();
}

//Display all nodes in the tree.
template <class DATA>
std::ostream& tree<DATA>::display_all(std::ostream & out) const
{
    int invoc = 0;
    root -> display_all(out, root, invoc);
    return out;
}

template <class DATA>
std::ostream& tree<DATA>::display_ordered(std::ostream & out) const
{
    root -> display_ordered(out, root);
    return out;
}

template <class DATA>
std::ostream& operator<<(std::ostream & out, const tree<DATA> & obj)
{
    obj.display_all(out);
    return out;
}

template <class DATA>
tree<DATA>& tree<DATA>::operator=(const tree<DATA> & obj)
{
    root -> remove_all(root);
    if( obj.root )
    root = new node<DATA>(*obj.root);
    return *this;
}

//DEFAULT CONSTRUCTOR
template <class DATA>
tree<DATA>::tree()
{
    root = new node<DATA>();
}

//COPY CONSTRUCTOR. This copies all the nodes from another tree into our tree.
template <class DATA>
tree<DATA>::tree(const tree<DATA>& obj)
{
    if( obj.root )
        root = new node<DATA>(*obj.root);
    else
        root = NULL;
}

//DESTRUCTOR
template <class DATA>
tree<DATA>::~tree()
{
    root -> remove_all(root);
    root = NULL;
}
