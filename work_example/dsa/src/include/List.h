#include <cassert>      // assert
#include <cstddef>      // ptrdiff_t
#include <iterator>     // iterator
#include <iostream>
#include <string>
#include <stdexcept>
#include <typeinfo>
#include <type_traits>  // remove_cv
#include <utility>      // swap
#include <vector>


//<editor-fold LIST CLASS DECLARATION
template <class U>
class List{
private:
    // The Node class is used to manage the memory of each element in the list as well as the Node it points to.
    // This is done so as to allow the List class to act purely as an interface to each Node without the user
    // having to know or worry about individual Nodes. List is not a friend of Node and can only access Node's
    // private methods via the ForwardIterator claass. This is done because the only way to reach an element beyond the
    // first in a list is to iterate from the m_head Node. This is because Node is set up in a way that the only way to
    // access later Nodes is via the first, all pointers to those Nodes are embedded in previous Nodes (which is how
    // Node manages resources). Therefore, it makes sense to only allow ForwardIterator to be friends with Node and have
    // List access Node through ForwardIterator. The only public methods in Node are a 'default' constructor and a
    // destructor.
    class Node;


    // m_head is the default constructed Node and via it's implementation in List should never be used to store an element. Instead
    // it should only ever point to the first element in the list or to a nullptr. This allows for the front insertion of the first
    // element, as well as the deletion of the first element.
    Node m_head;
    int m_size;

public:
    // ForwardIterator is defined as a public class because it needs to be callable outside of List, e.g supplying an iterator to
    // other functions. However, the creation of an iterator using a Node is restricted to ForwardIterator and friends only as other
    // classes shouldn't have access to Node. ForwardIterator does have a public constructor which will construct an iterator with a
    // nullptr Node. This can be passed into functions outside of List as a parameter and then a copy constructor can be used to
    // set the iterator equal to a Node, i.e list.begin()...
    class ForwardIterator;


    // We have a default constructor which sets the size to 0 but will create the head Node which can be used later on to insert elements.
    // We also have a constructor which creates a list of given size and sets all of the elements to a given value. This would be useful
    // for if we want to implement a method that takes a user given function and applies it to all elements in a list, without first having to
    // loop over said list to insert the elements.
    //
    // Size and is_init are fairly self explanitory. Is_init is mainly used in list itself for testing but could be useful outside of list.
    List() : m_size(0) {};
    List(int size, const U& value) : m_size(size){
        // This will only be executed if m_size > 0
        if(m_size){
            ForwardIterator itr(m_head);
            for(ptrdiff_t i = 0; i < m_size; i++){
                itr.insert_back(value);
                ++itr;
            }

        }
    };
    const int& size(){return m_size;}
    bool is_init(){return bool(m_size);}


    // insert_back and insert_front check to make sure that the position an element is being inserted isn't beyond the end of the List.
    // If that check passes they then iterate to the Node equivalent to the given position and perform a front or back insertion. Both
    // operations boil down to the same thing, a back insertion. This is because a singly linked list only has forward iterators and so once
    // we reach a given position Node we can only insert after it.
    //
    // However, we can simulate a front insertion by inserting a new Node with the given value and then swapping the two Nodes values. This is
    // performed in the Node class so that once again our list class only needs to interact with the ForwardIterator rather than performing
    // the insertion using the Node class.
    void insert_back(const U& value, const int position){
        assert(m_size > position && "Cannot insert element beyond the end of a list");

        ForwardIterator itr(m_head);
        for(ptrdiff_t i = 0; i <= position; i++){
            ++itr;
        }
        itr.insert_back(value);

        m_size++;
    }
    void insert_front(const U& value, const int position){
        assert(m_size > position && "Cannot insert element beyond the end of a list");

        ForwardIterator itr(m_head);
        for(ptrdiff_t i = 0; i <= position; i++){
            ++itr;
        }
        itr.insert_front(value);

        m_size++;
    }

    // Remove_at runs into the same difficulties as insert_front, in that we can't iterate backwards
    // and so to remove a Node at a given position we need to actually iterate to the Node before it
    // and perform a remove_back operation. This would be fine for all Nodes except the first accessable
    // Node in a list. The solution was to have each list contain a head Node which points to the first
    // Node in the list and then perform a remove_back on the head Node, thereby remove the first element
    // in a list.
    void remove_at(const int position){
        assert(m_size > position && "Cannot remove element beyond the end of a list");
        assert(m_size != 0 && "Cannot remove element from an empty list");

        ForwardIterator itr(m_head);
        for(ptrdiff_t i = 0; i <= position - 2; i++){
            ++itr;
        }
        itr.remove_back();

        m_size--;
    }

    // These next functions just call the insert and remove functions with specific positions. This is so that
    // any changes made to the structure of the list only need to be accounted for in the insert and remove functions,
    // hopefully leading to less future bugs.
    void push_back(const U& value){
        insert_back(value,  m_size - 1);
    }
    void push_front(const U& value){
        insert_back(value, 0);
    }
    void pop_back(){
        remove_at(m_size - 1);
    }
    void pop_front(){
        remove_at(0);
    }


    // The at function returns a constant reference to the value at a given position in a list. This is done
    // by iterating to that position and dereferencing the iterator. The ForwardIterator class has an overloaded
    // derefence operator which returns the value of a given Node.
    const U& at(const int position){
        assert(m_size > position && "Cannot read element beyond the end of a list");
        ForwardIterator itr(m_head);
        for(ptrdiff_t i = 0; i <= position; i++){
            ++itr;
        }
        return *itr;
    }
    // std::vector<int> search(const U& value){
    //     ForwardIterator itr(m_head);
    //     std::vector<int> searchPosition;
    //     int count = 0;
    //
    //     while(!(itr.isDone())){
    //         if(*itr == value){
    //             searchPosition.push_back(count++);
    //             ++itr;
    //         }else{
    //             count++;
    //             ++itr;
    //         }
    //         return searchPosition;
    //     }
    // }

    //void erase()
    // void print(){
    //     ForwardIterator itr(m_head);
    //     if(m_head == nullptr){
    //         std::cout << "null" << std::endl;
    //     }
    //     for(ptrdiff_t i = 0; i < m_size; i++){
    //         std::cout << *itr << std::endl;
    //         ++itr;
    //     }
    //     std::cout << std::endl;
    // }
};

//</editor-fold>

//<editor-fold NODE CLASS DECLARATION
template <class U>
class List<U>::Node{
private:
    // The only classes that can actually interact with Node are the ForwardIterator and the List class.
    // This means that if at any point some other class is given access to List's private members then it still can
    // only access Node's default constructor. The destrcutor is also public as it needs to be callable when Node goes out of scope
    // or gets deleted.
    //
    // To initialise Node with a value we make it an explicit initialisation as we need to ensure that
    // Node is of the same type as the List containing it and to stop and undefined behaviour.
    friend List<U>::ForwardIterator;
    explicit Node(const U& value) : m_next(nullptr), m_value(value){};


    // These member variables are the core of Node, everything else could be implemented further up the chain in List or using
    // iterators. However, to be able to use an iterator we must have a node class/struct and each node object must contain a value
    // and point to the next node. The only exception to this is when the final node in a list points to a nullptr indicating that there
    // are no more nodes.
    Node*    m_next;
    U        m_value;


    // The insert functions are used to connect an existing Node to a new Node with a given input value. This allows us to insert
    // elements into a list. There are two ways in which an insert could occur; the element can either be inserted before or after a
    // given Node. These two options are given as insert_front and insert_back respectively.
    //
    // Insert_front creates a new node that points to the Node we are inserting an element in front of. The new Node is then given the
    // value currently held by the Node we are inserting in front of. Finally, the old Node has it's value set to the user supplied value
    // and the pointer to the new Node is set to null. This means that when the new Node pointer goes out of scope it's destructor is not
    // called as otherwise no new Node would be inserted. However, ownership of the new Node now falls to the old Node and so care must
    // be taken to avoid memory leaks. This is addressed in the destructor.
    //
    // Insert_back works similarly to insert_front except the value of the new Node is set to the user supplied value and the old Node
    // retains its value. This is equivalent to inserting after the old Node.
    void insert_front(const U& value){
        Node* tmp = new Node(this->m_value);
        tmp->m_next = this->m_next;
        this->m_next = tmp;
        this->m_value = value;
        tmp = nullptr;
    }
    void insert_back(const U& value){
        Node* tmp = new Node(value);
        tmp->m_next = this->m_next;
        this->m_next = tmp;
        tmp = nullptr;
    }

    // The remove function is used to remove an existing Node. This method is assumed to be used by the List and ForwardIterator
    // classes and hence the Nodes are assumed to be connected to other Nodes. Since each Node only knows the Node to which it
    // connects in one direction, we can only implement one form of remove function and that is to remove the Node after a selected
    // Node. For example, if we had a list containing five elements then calling remove_back(1) would remove the third element in the
    // list.
    //
    // remove_back is implemented by creating a temporary pointer to the Node that the next Node points to. Therefore, if the current Node
    // points to a nullptr, i.e it is the end of a list, then remove_back should not be called and instead we call assert.
    // We then delete the Node that the current Node points to and set the current Node to point to the temporary node. This recconects
    // the list. Finally we set the temporary Node pointer to nullptr.
    void remove_back(){
        assert(m_next != nullptr && "Cannot remove nullptr node");
        Node* tmp = m_next->m_next;
        delete m_next;
        m_next = tmp;
        tmp = nullptr;
    }
public:
    // This constructor is used by any class not friends with Node to be able to pass a Node to an iterator.
    explicit Node() : m_next(nullptr){};


    // Earlier we mentioned that care needs to be taken to avoid memory leaks. For example, if we created a list of Nodes, due to the way in which
    // new Nodes/elements are inserted into the list we would end up with the top node in the list having ownership over all of the nodes beneath it.
    // Therefore, if we were delete this top Node without creating a a proper destructor then we would end up leaking the memory of all of the Nodes.
    //
    // To avoid this we must call delete iteratively on each Node connected to the top Node. This could be done from the list class since we would
    // know the size of the list and therefore easily avoid trying to double free memory. Doing this from the Node destructor means that we only need
    // to worry about deallocating memory once, in one Node. We check to see if the next Node is a nullptr, if it is we set this to be nullptr. If
    // however, the current Node points to an actual Node then we must delete that Node as well to avoid a memory leak. Doing so will call that Nodes
    // destrutor which in turn will call its next Nodes destructor and so on. This way we don't need to know the size of the list as each destructor
    // will call the next nodes destructor iteratively until the last node at which point each destructor will return thereby deallocating all of the
    // memory.
    //
    // One thing to be careful of is setting temporary Node pointers to null before the function using them returns as otherwise the temporary Node will be
    // deleted and the list will behave improperly. Restricting access to creating new Nodes will help prevent this and we want to really only create new Nodes
    // using the Node class.
    ~Node(){
        assert(this != nullptr && "Can't delete nullptr");
        if(this->m_next == nullptr){
        }else{
            delete this->m_next;
        }
    }

};
//</editor-fold>

//<editor-fold LIST::FORWARD_ITERATOR CLASS DECLARATION
template <class U>
class List<U>::ForwardIterator : public std::iterator<std::forward_iterator_tag, std::remove_cv<U>, std::ptrdiff_t, U*, U&> {
    private:
        // ForwardIterator needs to be friends with List in order to have access to the Node class, for which it is an iterator.
        // This makes
        friend class List<U>;


        // This is the heart of the iterator, it points to a Node and allows the iterator to access all of the Node's functions and
        // variables. This is the only way that any class that is not friends with Node can interact with a Node. Even friends of
        // ForwardIterator, who can access m_itr, couldn't call any of the Node's functions.
        Node* m_itr;

        // Only ForwardIterator and friends of it can construct an iterator with a Node. This is basically limited to List as it needs to
        // be able to create an iterator to the head Node. All classes outside of these two will have to call a method in List to create
        // an iterator and then work with that. This limits the creation of invalid iterators that don't point to a list element and makes
        // bugs easier to find and harder for a user to create.
        //
        // One such function would be list.begin which returns an iterator to the head Node of a list.
        explicit ForwardIterator(Node& Node) : m_itr(&Node){}

    public:
        // The default iterator is accessable to classes outside of List as a means to be able to pass an iterator to a function as a parameter.
        // Then when the user wanted to call said function they could call a list method that returns a valid iterator and pass that in.
        // The function would then call the default copy constructor for ForwardIterator and encapsulation would remain consistent.
        ForwardIterator() : m_itr(nullptr){}

        void swap(ForwardIterator& other) noexcept{
            using std::swap;
            swap(m_itr, other.m_itr);
        }

        // The following are general iterator functions that provide methods to move to the next Node, dereference a Nodes value, compare iterators,
        // and convert to constant iterators.
        //
        // Pre and post increment operator overloads
        ForwardIterator& operator++ (){
            assert(m_itr != nullptr && "Out-of-bounds iterator increment!");

            m_itr = m_itr->m_next;
            return *this;
        }
        ForwardIterator operator++ (int){
            assert(m_itr != nullptr && "Out-of-bounds iterator increment!");

            ForwardIterator tmp(m_itr);
            m_itr = m_itr->m_next;
            return tmp;
        }

        // Two-way comparison: v.begin() == v.cbegin() and vice versa
        bool operator == (const ForwardIterator& rhs) const{
            return m_itr == rhs.m_itr;
        }
        bool operator != (const ForwardIterator& rhs) const{
            return m_itr != rhs.m_itr;
        }

        // Dereference operator overloads
        const U& operator* () const{
            assert(m_itr != nullptr && "Invalid iterator dereference!");
            return m_itr->m_value;
        }
        const U& operator-> () const{
            assert(m_itr != nullptr && "Invalid iterator dereference!");
            return m_itr->m_value;
        }

        // One way conversion operator from iterator to const_iterator
        operator typename List<const U>::ForwardIterator() const{
            return typename List<const U>::ForwardIterator(m_itr);
        }


        // Here are the Node specific functions that allow List to insert and delete Nodes.
        void insert_front(const U& value){
                m_itr->insert_front(value);
            }
        void insert_back(const U& value){
                m_itr->insert_back(value);
        }
        void remove_back(){
            m_itr->remove_back();
        }
};

//</editor-fold>
