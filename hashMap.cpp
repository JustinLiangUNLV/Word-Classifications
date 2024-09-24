#include "hashMap.h"
#include <cmath>
#include <string>
using namespace std;

template <class t1, class t2>
const hashMap<t1, t2>& hashMap<t1, t2>::operator=(const hashMap<t1, t2>& rhs)
{
    if (this != &rhs)
    {
        deallocateMap(); // call deallocate map function
        copyMap(rhs); // call copy map function, pass in rhs object
    }
    return *this;
}

template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
{
    size_t loadFactor1 = items1 / capacity; // calculate the load factor for items1
    size_t loadFactor2 = items2 / capacity; // calculate the load factor for items2
    double loadFactorCapacity = 0.2; // create a temp variable for the load factor capacity

    if (loadFactor1 >= loadFactorCapacity || loadFactor2 >= loadFactorCapacity) // if either load factors are greater than or equal to .2
    {
        resize(capacity); // call reisze function and pass in current capacity
    }

    size_t collision = 0; // initialize collision counter
    size_t index1 = ((h1(key) + collision * h2(key)) % capacity); // calculate the initial index for table1 and table2
    size_t index2 = ((h2(key) + collision * h1(key)) % capacity);

    while (true) // infinite loop
    {
        for (node* i = table1[index1]; i != nullptr; i = i->link) // set node i to be the value of table1[index1], loop til nullptr, increment i by setting to next link
        {
            if (i->key == key) // if node i finds a matching key within the LL
            {
                return i->value; // return that key's value
            }
        }
        for (node* j = table2[index2]; j != nullptr; j = j->link) // set node j to be the value of table2[index2], loop til nullptr, increment j by setting to next link
        {
            if (j->key == key) // if j finds a matching key within the LL
            {
                return j->value; // return that key's value
            }
        }

        // if the matching key was not found in either table
        node* n; // create a temp node pointer n
		n = new node; // give it the value of the new node
        n->key = key; // set key
        n->value = t2(); // set value

        if(collision % 2 == 0) // if the collision is an even count
        {
            int count1 = 0; // create temp count variable
            for(node* i = table1[index1]; i != nullptr; i = i->link) // loop through the LL
            {
                count1++; // count how many nodes there are
            }
            if(count1 < 2) // if there are less than 2
            {
                n->link = table1[index1]; // head insert n
                table1[index1] = n; // set the value of table1[index1] to n
                items1++; // increment the counter
                return n->value; // return the node that was created
            }
        }
        else // if the collision counter is odd, then insert into table2 to simulate alternating in the video
        {
            int count2 = 0; // create temp count vairable
            for(node* j = table2[index2]; j != nullptr; j = j->link) // loop through LL
            {
                count2++; // see how many nodes there are
            }
            if(count2 < 2) // if there are less than 2 nodes
            {
                n->link = table2[index2]; // head insert the n ndoe
                table2[index2] = n; // set the value of table2[index2] to n
                items2++; // increment items2
                return n->value; // return the node that was created
            }
        }
        delete n; // deallocate dynamic variables
        collision++; // increment collision counter
        index1 = ((h1(key) + collision * h2(key)) % capacity); // calculate new index
        index2 = ((h2(key) + collision * h1(key)) % capacity); // calculate new index
    }

}

template <class t1, class t2>
void hashMap<t1, t2>::resize(size_t amount)
{
    node** x = table1; // create temp x to hold the original table1
    node** y = table2; // create temp y to hold the original table2
    size_t oldCapacity = capacity; // hold the old capacity
    capacity += amount; // increment the capacity by the amount passed in
    table1 = new node* [capacity]; // resize table1 and table2 with new capacity
    table2 = new node* [capacity];
    for (size_t i = 0; i < capacity; i++) // loop til we hit capacity
    {
        table1[i] = nullptr; // set the table's indexes to point to nullptr
        table2[i] = nullptr;
    }

    for (size_t i = 0; i < oldCapacity; i++) // loop til end of old capacity
    {
        node* source = x[i]; // create a temp source node to point to the value held in x[i], old table1
        while (source != nullptr) // while the source doesn't hit nullptr
        {
            node* next = source->link; // create a temp next node to point the source->link
            size_t tempIndex = h1(source->key) % capacity; // calculate new index
            source->link = table1[tempIndex]; // have source which is holding to old node, point to the new resized table at the new index
            table1[tempIndex] = source; // allow table1[index1] to be the source
            source = next; // set source to the next so it can break out of this loop
        }

        source = y[i]; // similar steps for the second table that is held in y
        while (source != nullptr) // while source != nullptr
        {
            node* next = source->link; // create a temp node that points to source->link
            size_t tempIndex = h2(source->key) % capacity; // new index
            source->link = table2[tempIndex]; // head insert using new index and new table2
            table2[tempIndex] = source; // set the value to the source
           	source = next; // set to source->link to break out of loop
        }
    }

    delete[] x; // we are then going to delete these old tables
    delete[] y;
}

template <class t1, class t2>
void hashMap<t1, t2>::copyMap(const hashMap<t1, t2>& map)
{
    items1 = map.items1; // copy over items1 and items2
    items2 = map.items2;
    capacity = map.capacity; // copy over the capacity

    table1 = new node*[capacity]; // allow table1 and table2 to create an array of pointers with that copied capacity
    table2 = new node*[capacity];

    for (size_t i = 0; i < capacity; i++) // loop til capacity
    {
        table1[i] = nullptr; // set the values of table1[i] and table2[i] to nullptrs
        table2[i] = nullptr;
    }

    for (size_t i = 0; i < map.capacity; i++) // loop til copied capacity
    {
        node* source = map.table1[i]; // create a source node to hold the value of the copied map.table1[i]
        while (source != nullptr) // while the source doesn't hit nullptr
        {
            node* newNode;
            newNode = new node; //create a newNode that will be inserted into *this map
            newNode->key = source->key; // copy over the key
            newNode->value = source->value; // copy over the value
            newNode->link = table1[i]; // have the newNode->link to point to *this table1[i]
            table1[i] = newNode; // set *this table1[i] to the newNode
            source = source->link; // set source to the link to break out of loop
        }
    }

    for (size_t i = 0; i < map.capacity; i++) // loop til map.capacity
    {
        node* source = map.table2[i]; // set source to equal the copied table2[i]
        while (source != nullptr) // while source doesnt hit nullptr
        {
            node* newNode;
            newNode = new node; // create a new node
            newNode->key = source->key; // set newNode's key to hold key
            newNode->value = source->value; // copy over value
            newNode->link = table2[i]; // set newNode's link to equal *this table2[i]
            table2[i] = newNode; // set *this table2[i] to equal the newNode
            source = source->link; // set source to the link to break out of loop
        }
    }
}

template <class t1, class t2>
void hashMap<t1, t2>::deallocateMap()
{
    if (table1 != nullptr) // while table1 doesn't equal nullptr
    {
        for (size_t i = 0; i < capacity; i++) // loop through the capacity
        {
            node* current = table1[i]; // set a current equal to table1[i];
            while (current != nullptr) // while current doesn't hit nullptr
            {
                node* temp = current; // create a temp node that points to current
                current = current->link; // set current to the next link
                delete temp; //delete the temp
            }
        }
        delete[] table1; // delete the array of pointers
        table1 = nullptr; // set table1 to point to nothing
    }

    if (table2 != nullptr) // if table2 is not nullptr
    {
        for (size_t i = 0; i < capacity; i++) // loop til capacity
        {
            node* current = table2[i]; // set the current to equal to the current table2[i]
            while (current != nullptr) // while current is not nullptr;
            {
                node* temp = current; // create a temp node to point to same node as current
                current = current->link; // set current to point to next link
                delete temp; // delete the temp
            }
        }
        delete[] table2; // delete array of pointers
        table2 = nullptr; // set table2 to point to nothing
    }
}

template <class t1, class t2>
size_t hashMap<t1, t2>::h1(const string& s) const
{
    size_t sum = 0; // create a temp sum
    for (size_t i = 0; i < s.length(); i++) // loop til the string's length
    {
        sum += static_cast<size_t>((pow(10, i)) * s[i]); // calculate the sum according to pdf
    }
    return sum; // return the sum
}

template <class t1, class t2>
size_t hashMap<t1, t2>::h2(const string& s) const
{
    size_t sum = 0; // create temp sum
    for (size_t i = 0; i < s.length(); i++) // loop til string's length
    {
        sum += static_cast<size_t>((pow(10, i)) * s[s.length() - 1 - i]); // calculate the sum according to pdf
    }
    return sum; // return the sum back
}
