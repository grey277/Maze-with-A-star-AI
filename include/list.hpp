#ifndef LIST_H
#define LIST_H

#include <stdexcept>
#include <utility>
#include <cstddef>
#include <limits>
#include <iterator>

namespace uj {

    /**
     * Main list class
     */
    template <typename T>
    class list {
    public:
        typedef size_t size_type;
    private:

        /**
         * Class holding data info
         */
        class node {
        protected:
            /**
             * Field holding data info
             */
            T data;
            /**
             * Pointer to next element
             */
            node* next;
        public:
            /**
             * Node constructor
             */
            node() : data(),
                     next(0) { }
            //!Node constructor with only data
            /*!
             \param data Data to save in node
            */
            node(T data) : data(data),
                           next(0) { }
            /**
             * Node constructor with data and next element
             */
            node(T data, node* next) : data(data),
                                       next(next) { }

            /**
             * Node destructor
             */
            ~node() { }

            friend class iterator;
            friend class list;
        };

        /**
         * Private fields
         */
        node *head, *tail;
        size_type elementCount;

    public:
        /**
         * Iterator class
         */
        template <typename type = T>
        class listIterator : public std::iterator<std::forward_iterator_tag, T> {
        protected:
            /**
             * Field holding current element
             */
            node* current;

        public:
            /**
             * Empty constructor
             */
            listIterator() : current(0) { }
            /**
             * Constructor with element
             */
            listIterator(node* current) : current(current) { }

            /**
             * Destructor
             */
            ~listIterator() { }

            /**
             * Operator pre++, returning iterator to next element
             */
            listIterator<type>& operator++() {
                if(!current || !current->next)
                    throw std::out_of_range("Out of range");
                current = current->next;
                return *this;
            }
            /**
             * Operator post++, returning iterator to current element
             */
            listIterator<type> operator++(int) {
                if(!current || !current->next)
                    throw std::out_of_range("Out of range");
                listIterator<type> old = *this;
                current = current->next;
                return old;
                //return *this;
            }
            /**
             * Returning true if iterators are not pointing to same elements
             */
            bool operator!=(const list<T>::listIterator<type> & it) {
                if(!current || !it)
                    return false;
                return current->next != it.current->next;
            }
            /**
             * Returning true if iterators are pointing to same elements
             */
            bool operator==(const list<T>::listIterator<type> & it) {
                if(!current || !it)
                    return false;
                return current->next == it.current->next;
            }

            bool operator!() const {
                if(current == 0)
                    return true;
                return false;
            }

            operator listIterator<const T>() { return listIterator<const T>(current); }

            /**
             * Returning value of pointed element by iterator
             */
            type& operator*() {
                if(!current)
                    throw std::out_of_range("Current is nil");
                if(!current->next)
                    throw std::out_of_range("Out of range exception");
                return current->next->data;
            }
            type* operator->() {
                if(!current)
                    throw std::out_of_range("Current is nil");
                if(!current->next)
                    throw std::out_of_range("Out of range exception");
                return &(current->next->data);
            }

            friend class list;
        };

        //typedefs
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef listIterator<T> iterator;
        typedef listIterator<const T> const_iterator;
        typedef ptrdiff_t difference_type;
        typedef std::forward_iterator_tag iterator_category;
        //end of typedefs

    private:
        static bool inline defaultComp(value_type x, value_type y) { return x < y; }
        static bool inline defaultBinPred(value_type x, value_type y) { return x == y; }
        //default comparer used in sort and marge
    public:

        //! empty container constructor (default constructor)
        /*!
         * Constructs an empty container, with no elements.
         */
        list() : head(new node()),
                 tail(head),
                 elementCount(0) { }

        //! fill constructor
        /*!
         Constructs a container with n elements. Each element is a copy of val.
         \param n size of container to create
         \param val value to insert to container, if empty its default value of value_type
         */
        list (size_type n, const value_type& val = value_type()) : head(new node()),
                                                                   tail(head),
                                                                   elementCount(0)
        {
            this->insert(this->begin(), n, val);
        }

        //! range constructor
        /*!
         * Constructs a container with as many elements as the range [first,last), with each element constructed from its corresponding element in that range, in the same order.
         \param first Iterator pointing to first element to copy
         \param last Iterator pointing to next element after element to copy
         */
        template <typename InputIterator>
        list (InputIterator first, InputIterator last) : head(new node()),
                                                         tail(head),
                                                         elementCount(0)
        {
            if(!first || !last)
                throw std::invalid_argument("Bad argument");
            while (first != last) {
                this->push_back(*first);
                first++;
            }
        }

        //!Copy constructor
        /*!
         * Constructs a container with a copy of each of the elements in x, in the same order.
         \param other List to copy from
         */
        list (const list& other) : head(new node()),
                                   tail(head),
                                   elementCount(0)
        {
            const_iterator begin = other.begin();
            while(begin != other.end()) {
                this->push_back(*begin);
                begin++;
            }
        }

        //! This destroys all container elements, and deallocates all the storage capacity allocated by the list container.
        ~list() {
            clear();
            if(head)
                delete head;
        }

        //! Copies all the elements from other into the container.
        /*!
         \param other A list object of the same type (i.e., with the same template parameter T
         \return *this
         */
        list & operator=(const list & other) {
            this->clear();
            node *otherNode = other.head;
            while(otherNode && otherNode->next) {
                otherNode = otherNode->next;
                this->insert(this->end(), otherNode->data);
            }
            return (*this);
        }

        reference operator[](int n) {
            iterator it = this->begin();
            for(int i = 0; i < n; i++) {
                it++;
            }
            return *it;
        }

        //! Returns whether the list container is empty
        /*!
         \return true if list is empty otherwise false
         */
        bool empty() const { return !elementCount; }
        //! Return number of elements in list
        /*
         \return Number of elements in list
         */
        size_type size() const { return elementCount; }

        //! Return max size of list
        /*
         \return Maximum elements that list can hold
         */
        size_type max_size() const { return std::numeric_limits<size_type>::max(); }

        //! Returns an iterator pointing to the first element in the list container.
        /*!
         \return iterator to first element in list
         */
        iterator begin() { return listIterator<value_type>(head); }

        //!
        /*!
         \return const iterator to first element in list
         */
        const_iterator begin() const { return listIterator<const value_type>(head); }
        //!
        /*!
         \return iterator to last element in list
         */
        iterator end() { return listIterator<value_type>(tail); }

        //!
        /*!
         \return const iterator to last element in list
         */
        const_iterator end() const { return listIterator<const value_type>(tail); }

        //access elements
        //! Returns a reference to the first element in the list container.
        /*
         \return A reference to the first element in the list container.
         */
        reference front() {
            if(head && head->next)
                return head->next->data;
            else
                throw std::out_of_range("List is empty");
        }

        //! Returns a const reference to the first element in the list container.
        /*
         \return A const reference to the first element in the list container.
         */
        const_reference front() const {
            if(head && head->next)
                return head->next->data;
            else
                throw std::out_of_range("List is empty");
        }

        /**
         * Return last element in list, but not delete him from list
         */
        reference back() {
            if(head && tail && head != tail)
                return tail->data;
            else
                throw std::out_of_range("List is empty");
        }

        /**
         * Return const last element in list
         */
        const_reference back() const {
            if(head && tail && head != tail)
                return tail->data;
            else
                throw std::out_of_range("List is empty");
        }

        //modf elements
        /**
         * Replace current content of list with another
         */
        template <class InputIterator>
        void assign (InputIterator first, InputIterator last) {
            if(!first || !last)
                throw std::invalid_argument("Bad argument");
            clear();
            while (first != last) {
                this->push_back(*first);
                first++;
            }
        }

        /**
         * Replace current content of list with another
         */
        void assign (size_type n, const value_type& val) {
            clear();
            this->insert(this->begin(), n, val);
        }

        /**
         * Insert element in front of list
         */
        void push_front(const value_type& value) {
            this->insert(this->begin(), value); // need to opt
        }

        /**
         * Delete first element in list
         */
        void pop_front() {
            this->erase(this->begin()); // need to opt
        }

        /**
         * Insert element in back of list
         */
        void push_back (const value_type& val) {
            this->insert(this->end(), val); // need to opt
        }

        /**
         * Delete last element in list
         */
        void pop_back() {
            list<value_type>::iterator it = this->begin();
            while(it.current && it.current->next && it.current->next->next) {
                it++;
            }
            if(it.current && it.current->next) {
                it.current->next = 0;
                elementCount--;
                tail = it.current;
            }

        }

        /**
         * Insert element on iterator position
         */
        iterator insert(iterator pos, const value_type & value) {
            if(!pos)
                throw std::invalid_argument("Bad argument");
            node* curr = pos.current;
            node* next = curr->next;
            curr->next = new node(value, next);
            if(!next)
                tail = curr->next;
            //curr->next = next;
            elementCount++;
            return iterator(curr->next);
        }

        /**
         * Insert n elements from position to list with val value
         */
        void insert (iterator position, size_type n, const value_type& val) {
            if(!position)
                throw std::invalid_argument("Bad argument");
            for(size_type i = 0; i < n; i ++) {
                position = this->insert(position, val);
            }
        }

        /**
         * Insert all elements between first and last iterator to list
         */
        template <class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last) {
            if(!position || !first || !last)
                throw std::invalid_argument("Bad argument");
            while(first != last) {
                position = this->insert(position, *first);
                first++;
            }
        }

        /**
         * Delete element from iterator position
         */
        iterator erase(iterator pos) {
            if(!pos)
                throw std::invalid_argument("Bad argument");
            node* curr = pos.current;
            if(curr && curr->next) {
                //node *tmp = curr->next;
                curr->next = curr->next->next;
                //delete tmp;
                if(!curr->next && tail != curr)
                    tail = curr;

                elementCount--;
            } else {
                //curr->setNext(0); no need, already nullptr
            }

            return iterator(curr);
        }

        /**
         * Delete element from first to last position
         */
        iterator erase (iterator first, iterator last) {
            if(!first || !last)
                throw std::invalid_argument("Bad argument");
            while(first != last) {
                first = this->erase(first);
            }
            return first;
        }

        /**
         * Swap elements of 2 lists
         */
        void swap (list& x) {
            uj::list<value_type> tmp;
            tmp = *this;
            *this = std::move(x);
            x = std::move(tmp);
        }

        /**
         * Resize container to N elements, if val is given also they are initialized with it
         */
        void resize (size_type n, value_type val = value_type()) {
            while(size() < n)
                this->push_back(val);
            while(size() > n)
                this->pop_back();
        }


        /**
         * Delete all elements in list
         */
        void clear() {
            if(!head)
                return;
            node* tmp = head->next;
            node* tmp2 = tmp;
            while(tmp) {
                tmp2 = tmp->next;
                delete tmp;
                tmp = tmp2;
            }
            head->next = 0;
            tail = head;
            elementCount = 0;
        }

        /**
         * Splice all elements from other list to this list
         */
        void splice(iterator it, list<value_type> & other) { // not actual std splice implementation
            if(!it)
                throw std::invalid_argument("Bad argument");
            node* tmp = other.head;
            iterator itmp = it;
            while(tmp->next) {
                tmp = tmp->next;
                itmp = this->insert(itmp, tmp->data);
            }
            other.clear();
        }

        /**
         * Splice 1 element pointed by i to this list from other list into position
         */
        void splice (iterator position, list& other, iterator i) {
            if(!position || !i)
                throw std::invalid_argument("Bad argument");
            this->insert(position, *i);
            other.erase(i);
        }

        /**
         * Splice all elements from other list in range from first to last into this list at position
         */
        void splice (iterator position, list& other, iterator first, iterator last) {
            if(!position || !first || !last)
                throw std::invalid_argument("Bad argument");
            iterator tmp;
            while(first != last) {
                position = this->insert(position, *first);
                tmp = first++;
                other.erase(tmp);
            }
        }

        /**
         * Remove all elements with val value
         */
        void remove (const value_type& val) { // repair
            iterator begin = this->begin();
            iterator end = this->end();
            while (begin != end) {
                if(*begin == val) {
                    begin = erase(begin);
                } else {
                    begin++;
                }
            }
        }

        /**
         * Remove element if condition is true
         */
        template <class Predicate>
        void remove_if (Predicate pred) {
            iterator begin = this->begin();
            iterator end = this->end();
            while (begin != end) {
                if(pred(*begin)) {
                    begin = erase(begin);
                } else {
                    begin++;
                }
            }
        }

        /**
         * Remove all duplicates with predicate
         */
        template <class BinaryPredicate = decltype(defaultBinPred)>
        void unique (BinaryPredicate binary_pred = defaultBinPred) {
            iterator begin = this->begin();
            iterator tbegin = begin++;
            while(begin != this->end()) {
                if(tbegin == this->end())
                    return;
                if(binary_pred(*tbegin, *begin)) {
                    begin = this->erase(begin);
                } else {
                    tbegin = begin++;
                }
            }
        }

        /**
         * Marge all elements from other list to this list using given Compare
         */
        template <typename Compare = decltype(defaultComp)>
        void merge (list& other, Compare comp = defaultComp) {
            iterator thisBegin = this->begin();
            iterator otherBegin = other.begin();
            while(!other.empty()) {
                if(comp(*thisBegin, *otherBegin)) {
                    this->insert(thisBegin, *otherBegin);
                    otherBegin = other.erase(otherBegin);
                }
                thisBegin++;
            }
        }

        /**
         * Sort all elements in list using given Compare
         */
        template <typename Compare = decltype(defaultComp) >
        void sort (Compare comp = defaultComp) {
            iterator begin = this->begin();
            iterator tbegin = begin;
            iterator tmp;
            while(begin != this->end()) {
                tbegin = begin;
                tmp = tbegin;
                while(++tbegin != this->end()) {
                    if(comp(*tmp, *tbegin)) {
                        value_type t = *tmp;
                        tmp.current->next->data = *tbegin;
                        tbegin.current->next->data = t;
                    }
                }
                begin++;
            }
        }

        /**
         * Reverse all elements in list
         */
        void reverse() {
            uj::list<value_type> *tmp = new uj::list<value_type>();
            iterator begin = this->begin();
            iterator end = this->end();
            while(begin != end) {
                tmp->push_front(*begin);
                begin++;
            }
            *this = std::move(*tmp);
            delete tmp;
        }
    };
}

#endif //LIST_H
