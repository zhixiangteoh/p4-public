#ifndef LIST_HPP
#define LIST_HPP
/* List.hpp
 *
 * doubly-linked, double-ended list with Iterator interface
 * EECS 280 Project 4
 */

#include <cassert>  //assert
#include <cstddef>  //NULL
#include <iostream>

template <typename T>
class List {
    // OVERVIEW: a doubly-linked, double-ended list with Iterator interface
   public:
    List() : first(nullptr), last(nullptr), sz(0) {
    }

    List(const List &other) : List() {  // copy-ctor
        copy_all(other);
    }

    List &operator=(const List &other) {  // overloaded assignment
        List temp(other);
        std::swap(first, temp.first);
        std::swap(last, temp.last);
        std::swap(sz, temp.sz);
        return *this;
    }

    ~List() {
        clear();
    }

    // EFFECTS:  returns true if the list is empty
    bool empty() const {
        return sz == 0;
    }

    // EFFECTS: returns the number of elements in this List
    // HINT:    Traversing a list is really slow.  Instead, keep track of the size
    //          with a private member variable.  That's how std::list does it.
    int size() const {
        return sz;
    }

    // REQUIRES: list is not empty
    // EFFECTS: Returns the first element in the list by reference
    T &front() {
        assert(!empty());
        return first->datum;
    }

    // REQUIRES: list is not empty
    // EFFECTS: Returns the last element in the list by reference
    T &back() {
        assert(!empty());
        return last->datum;
    }

    // EFFECTS:  inserts datum into the front of the list
    void push_front(const T &datum) {
        Node *new_node = new Node{first, nullptr, datum};
        if (empty()) {
            first = last = new_node;
        } else {
            first = first->prev = new_node;
        }
        ++sz;
    }

    // EFFECTS:  inserts datum into the back of the list
    void push_back(const T &datum) {
        Node *new_node = new Node{nullptr, last, datum};
        if (empty()) {
            first = last = new_node;
        } else {
            last = last->next = new_node;
        }
        ++sz;
    }

    // REQUIRES: list is not empty
    // MODIFIES: may invalidate list iterators
    // EFFECTS:  removes the item at the front of the list
    void pop_front() {
        assert(!empty());
        Node *victim = first;
        first = victim->next;
        if (first) {
            first->prev = nullptr;
        }
        delete victim;
        --sz;
    }

    // REQUIRES: list is not empty
    // MODIFIES: may invalidate list iterators
    // EFFECTS:  removes the item at the back of the list
    void pop_back() {
        assert(!empty());
        Node *victim = last;
        last = victim->prev;
        if (last) {
            last->next = nullptr;
        }
        delete victim;
        --sz;
    }

    // MODIFIES: may invalidate list iterators
    // EFFECTS:  removes all items from the list
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    // You should add in a default constructor, destructor, copy constructor,
    // and overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you can omit them. A user
    // of the class must be able to create, copy, assign, and destroy Lists

   private:
    // a private type
    struct Node {
        Node *next;
        Node *prev;
        T datum;
    };

    // REQUIRES: list is empty
    // EFFECTS:  copies all nodes from other to this
    void copy_all(const List<T> &other) {
        clear();
        for (Iterator it = other.begin(); it != other.end(); ++it) {
            push_back(*it);
        }
    }

    Node *first;  // points to first Node in list, or nullptr if list is empty
    Node *last;   // points to last Node in list, or nullptr if list is empty

    size_t sz;  // number of elements in the list

   public:
    ////////////////////////////////////////
    class Iterator {
        // OVERVIEW: Iterator interface to List

        // You should add in a default constructor, destructor, copy constructor,
        // and overloaded assignment operator, if appropriate. If these operations
        // will work correctly without defining these, you can omit them. A user
        // of the class must be able to create, copy, assign, and destroy Iterators.

       public:
        Iterator() : node_ptr(nullptr) {
        }

        // Your iterator should implement the following public operators: *,
        // ++ (prefix), default constructor, == and !=.
        T &operator*() const {
            return node_ptr->datum;
        }

        Iterator &operator++() {
            assert(node_ptr);
            node_ptr = node_ptr->next;
            return *this;
        }

        bool operator==(const Iterator &other) const {
            return node_ptr == other.node_ptr;
        }

        bool operator!=(const Iterator &other) const {
            return node_ptr != other.node_ptr;
        }

        // This operator will be used to test your code. Do not modify it.
        // Requires that the current element is dereferenceable.
        Iterator &operator--() {
            assert(node_ptr);
            node_ptr = node_ptr->prev;
            return *this;
        }

       private:
        Node *node_ptr;  // current Iterator position is a List node
        // add any additional necessary member variables here

        // add any friend declarations here
        friend class List;

        // construct an Iterator at a specific position
        Iterator(Node *p) : node_ptr(p) {
        }

    };  // List::Iterator
    ////////////////////////////////////////

    // return an Iterator pointing to the first element
    Iterator begin() const {
        return Iterator(first);
    }

    // return an Iterator pointing to "past the end"
    Iterator end() const {
        return Iterator();
    }

    // REQUIRES: i is a valid, dereferenceable iterator associated with this list
    // MODIFIES: may invalidate other list iterators
    // EFFECTS: Removes a single element from the list container
    //          Returns An iterator pointing to the element that followed the element
    //          erased by the function call
    Iterator erase(Iterator i) {
        if (i.node_ptr == first) {
            pop_front();
            return begin();
        } else if (i.node_ptr == last) {
            pop_back();
            return end();
        }

        Node *victim = i.node_ptr;
        Node *prev = victim->prev;
        Node *next = victim->next;
        prev->next = next;
        next->prev = prev;
        delete victim;
        --sz;
        return Iterator(next);
    }

    // REQUIRES: i is a valid iterator associated with this list
    // EFFECTS: inserts datum before the element at the specified position.
    //          returns an iterator to the the newly inserted element
    Iterator insert(Iterator i, const T &datum) {
        if (i.node_ptr == first) {
            push_front(datum);
            return begin();
        } else {
            Node *new_node = new Node{i.node_ptr, i.node_ptr->prev, datum};
            i.node_ptr->prev->next = new_node;
            i.node_ptr->prev = new_node;
            ++sz;
            return Iterator(new_node);
        }
    }

};  // List

////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.

#endif  // Do not remove this. Write all your code above this line.
