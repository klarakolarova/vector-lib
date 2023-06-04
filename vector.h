#ifndef vector_h
#define vector_h

#include <iostream>
#include <string>

using namespace std;

template <typename T>

class Vector {

public:
    //ITERATOR CLASSES
    class ConstIterator;
    class Iterator;

    //USING STATEMENTS
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;

private:
    static constexpr size_t min_sz = 5;
    size_t sz;
    size_t max_sz;
    value_type* values;

public:

    friend difference_type operator-(const const_iterator&, const const_iterator&);

    // CONSTRUCTORS & DESTRUCTORS ///////////////////////////////////

    //Size constructor
    Vector(size_t n): sz(0), max_sz(n > min_sz ? n : min_sz), values(new value_type[max_sz]){
    }

    //Default Constructor
    Vector(): Vector(min_sz) {
    }

    //Copy constructor
    Vector (const Vector& muster): Vector(muster.sz) {
        sz = muster.sz;
        for (size_t i = 0; i<sz; i++) {
            values[i] = muster.values[i];
        }
    }

    //Constructor with parameter list
    Vector (initializer_list<value_type> list): Vector(list.size())  {
        size_t i = 0;
        for (const auto& elem: list) {
            values[i] = elem;
            sz++;
            i++;
        }
    }

    //Destructor
    ~Vector() {
        delete[] values;
    }


    // METHODS - SIZE ///////////////////////////////////////////////

    //size()
    size_t size() const {
        return sz;
    }

    //capacity()
    size_t capacity() const {
        return max_sz;
    }

    //empty()
    bool empty() const {
        if (sz==0) return true;
        else return false;
    }

    // METHODS - CONTENTS ////////////////////////////////////////////////////

    //clear()
    void clear() {
        sz = 0;
    }

    //reserve(size_t n)
    void reserve(size_t n) {
        if (n > max_sz) {
            value_type* new_values = new value_type[n];
            for (size_t i = 0; i < sz ; i++) {
                new_values[i] = values[i];
            }
            max_sz = n;
            delete[] values;
            values = new_values;
        }
    }

   //push back(value_type new_value)
    void push_back(value_type new_value) {
        if (sz == max_sz) {
            reserve(2*max_sz); //Could throw bad_alloc
        }

        values[sz] = new_value;
        sz++;
    }

    //pop back()
    void pop_back() {
        if (sz == 0) {
            throw runtime_error ("pop_back() invalid: Vector is already empty");
        }
        sz--;
    }

    //shrink_to_fit (max_sz==sz or max_sz == min_sz)
    void shrink_to_fit() {
        if (sz<min_sz) {
            value_type* new_values = new value_type[min_sz];
            for (size_t i = 0; i<sz; i++) {
                new_values[i] = values[i];
            }

            max_sz = min_sz;
            delete[] values;
            values = new_values;
        }

        else {
            value_type* new_values = new value_type[sz];
            for (size_t i = 0; i<sz; i++) {
                new_values[i] = values[i];
            }

            max_sz = sz;
            delete[] values;
            values = new_values;
        }
    }

    // METHODS - ITERATORS ///////////////////////

    //begin iterator (forward)
    iterator begin() {
        return values;
    }

    //end iterator (forward)
    iterator end() {
        return values+sz;
    }

    //const begin iterator (forward on const objects)
    const_iterator begin() const {
        return values;
    }

    //const end iterator  (forward on const objects)
    const_iterator end() const {
        return values+sz;
    }

    //OPERATORS ////////////////////////////////

    //operator []
    value_type& operator[] (size_t index) {
        if (index>=sz) {
            throw runtime_error ("Invalid index");
        }
        else return values[index];
    }

    //operator[] const
    const value_type& operator[] (size_t index) const {
        if (index >= sz) {
            throw runtime_error ("Invalid index");
        }
        else return values[index];
    }

    //operator =  (Kopierzuweisungsoperator)
    Vector& operator= (const Vector& rop) {
        this->sz = rop.sz;
        this->max_sz = rop.max_sz;
        value_type* new_values = new value_type[max_sz];

        for (size_t i = 0; i<sz; i++) {
            new_values[i] = rop.values[i];
        }

        delete[] values;
        this->values = new_values;
        return *this;
    }


    // METHODS - INSERT, ERASE ////////////////////////////

    //insert(pos, val)
    iterator insert (const_iterator pos, const_reference val) {
        auto diff = pos-begin();
        if (diff<0 || static_cast<size_type>(diff)>sz)
            throw runtime_error ("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
        if (sz>=max_sz)
            reserve (max_sz*2);
        for (auto i {sz}; i-- > current;)
            values[i+1]= values[i];
        values [current] = val;
        ++sz;
        return iterator{values+current};
    }

    //erase(pos)
    iterator erase(const_iterator pos) {
        auto diff = pos-begin();
        if (diff<0 || static_cast<size_type>(diff)>=sz)
            throw runtime_error ("Iterator out of bounds");
        size_type current {static_cast<size_type>(diff)};
        for (auto i {current}; i<sz-1; i++)
            values[i]=values[i+1];
        --sz;
        return iterator {values+current};
    }

    //CLASS ITERATOR ////////////////////////////////////////
    class Iterator {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;
    private:
        pointer ptr;
    public:

    //CONSTRUCTORS ////////////////////////////

    //Default Constructor
        Iterator(): ptr(nullptr) {}

    //Pointer Constructor
        Iterator(pointer ptr): ptr(ptr) {}

    //OPERATORS /////////////////////////////////

    //Operator *
        reference operator* () const {
            //if (ptr == nullptr) throw runtime_error ("Illegal dereference of nullptr");
            return *ptr;
        }

        //Operator -> (!const)
        pointer operator-> () const {
            return ptr;
        }

        //Operator ++(Prefix)
        iterator& operator++() {
            //if (ptr == nullptr) throw runtime_error ("Illegal increment of nullptr");
            ptr++;
            return *this;
        }

        //Operator ++ (Postfix)
        iterator operator++(int) {
            //if (ptr==nullptr) throw runtime_error ("Illegal increment of nullptr");
            return Iterator(ptr++);
        }

        //Type consversion to const_iterator()
        operator const_iterator() const {
            return ConstIterator(ptr);
        }

	     //operator ==
	     bool operator== (const ConstIterator& rop) const {
             if (rop == *this) return true; //calls == operator of ConstIterator
             else return false;
        }

        //operator !=
	     bool operator!= (const ConstIterator& rop) const {
             if (rop == *this) return false; //calls == operator of ConstIterator
             else return true;
	      }

    };

    //END OF CLASS ITERATOR ////////////////
    // /////////////////////////////////////


    //CLASS CONST_ITERATOR //////////////////////////////
    class ConstIterator {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::const_reference;
        using pointer = Vector::const_pointer;
        using difference_type =  Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;
    private:
        pointer ptr;
    public:

        //CONSTRUCTORS ///////////////

        //Default constructor
        ConstIterator(): ptr(nullptr) {}

        //Pointer constructor
        ConstIterator(pointer ptr): ptr(ptr) {}

        //OPERATORS /////////////////////

        //Operator *
        reference operator* () const {
            //if (ptr == nullptr) throw runtime_error ("Illegal dereference of nullptr");
            return *ptr;
        }

        //Operator ->
        pointer operator->() const {
            return ptr;
        }

        //Operator ++ (prefix)
        const_iterator& operator++ () {
            //if (ptr == nullptr) throw runtime_error ("Illegal increment of nullptr");
            ptr++;
            return *this;
        }

        //Operator ++ (postfix)
        const_iterator operator++(int) {
            //if (ptr == nullptr) throw runtime_error ("Increment of nullptr is not allowed");
            return ConstIterator(ptr++);
        }

        //operator ==
        bool operator== (const ConstIterator& rop) const {
            if (ptr == rop.ptr) return true; //Only this one compares underlying pointers
            else return false;
        }

        //operator !=
        bool operator!= (const ConstIterator& rop) const {
            if (*this==rop) return false; //Calls == operator of ConstIterator
            else return true;
        }

        //operator -
        Vector::difference_type operator-(const ConstIterator& rop) {
            return ptr-rop.ptr;
        }

    };
    //END OF CLASS CONST_ITERATOR ////////////////
    // /////////////////////////////////////


};
//END OF CLASS VECTOR //

//ADDITIONAL METHODS AND OPERATORS //////////////////

template <typename T>

//operator <<
ostream& operator<< (ostream& o, const Vector<T>& v) {
    o <<"[";
    bool first = true;
    for (auto it = v.begin(); it != v.end(); it++) {
        if (first == true) first = false;
        else o << ", ";
        o << *it;
    }
    o << "]";
    return o;
}

#endif
