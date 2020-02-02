

#include <string>
#include <utility>
#include <iostream>

template <class T>
struct Node
{
    T data;
    Node<T> *nextNode;
};

template <class T>
class LinkedList
{
public:

    LinkedList() : firstNode(nullptr), listSize(0) {}

    LinkedList(const LinkedList<T> &origList) : firstNode(nullptr), listSize(0)
    {
        insert(origList, 0);
    }

    void insert(const T &value, const size_t index)
    {
        Node<T> *node = new Node<T>{value, nullptr};

        auto nodes{findNode(index)};

        // if inserting in the first spot, move reference to first node
        if (nodes.first == nullptr)
        {
            firstNode = node;
        }
        else
        {
            nodes.first->nextNode = node;
        }

        node->nextNode = nodes.second;
        ++listSize;
        nodes = findNode(index);
        return;
    }

    void insert(const LinkedList<T> &origList, const size_t index)
    {

        auto origNode = origList.firstNode;
        auto nodes{findNode(index)};
        auto prevNode{nodes.first};

        while (origNode != nullptr)
        {
            auto node = new Node<T>{origNode->data, nodes.second};
            ++listSize;
            if (prevNode == nullptr)
            {
                firstNode = node;
            }
            else
            {
                prevNode->nextNode = node;
            }
            origNode = origNode->nextNode;
            prevNode = node;
        }

    }

    void remove(const size_t index)
    {
        // goal is to remove nodes.second
        //
        // returns prevIndex @ nodes.first, and index @ nodes.second
        auto nodes{findNode(index)};

        // if nodes.second is nullptr, then pointed off the end of the list
        if (nodes.second == nullptr)
        {
            throw std::runtime_error("LinkedList::remove() index does not exist");
        }

        if (nodes.first == nullptr)
        {
            // removing firstNode, so no node points to it
            firstNode = nodes.second->nextNode;
        }
        else
        {
            // removing any other node, including final node
            nodes.first->nextNode = nodes.second->nextNode;
        }

        delete nodes.second;
        --listSize;
        return;
    }

    size_t size() const
    {
        return listSize;
    }

    T &operator[] (const size_t index)
    {
        auto nodes{findNode(index)};
        return nodes.second->data;
    }

    ~LinkedList()
    {
        auto node{firstNode};
        while (node != nullptr)
        {
            auto nextNode{node->nextNode};
            delete node;
            node = nextNode;
        }
    }

    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const LinkedList<U> &list);


private:
    Node<T> *firstNode;
    size_t listSize;

    std::pair<Node<T>*, Node<T>*> findNode(const size_t index) const
    {
        auto node{firstNode};
        Node<T> *prevNode{nullptr};

        for(size_t i = 0 ; i < index ; i++)
        {
            if (node == nullptr)
            {
                throw std::runtime_error("LinkedList index " + std::to_string(index) + " out of range");
            }
            prevNode = node;
            node = node->nextNode;
        }

        return std::make_pair(prevNode, node);
    }

};

template <class T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T> &list)
{
    auto curNode{list.firstNode};
    os << "{ ";
    while (curNode != nullptr)
    {
        os << curNode->data << ", ";
        curNode = curNode->nextNode;
    }
    os << " }";
    return os;
}

int main()
{
    LinkedList<int> list;

    // should test adding and removing from first index, last index, middle index

    for (int i = 0; i < 10; i++)
    {
        list.insert(i,i);
        std::cout << "insert " << i << " at index " << i << " " << list << std::endl;
    }
    // list has values 0-9

    std::cout << "original " << list << std::endl << std::endl;
    list.remove(9);
    std::cout << "remove last index (9) " << list << std::endl << std::endl;
    list.insert(-1, 5);
    std::cout << "insert -1 @ index 5 " << list << std::endl << std::endl;
    list.remove(5);
    std::cout << "remove index 5 " << list << std::endl << std::endl;
    list.insert(-1, 0);
    std::cout << "insert -1 @ index 0 " << list << std::endl << std::endl;
    list.remove(0);
    std::cout << "remove index 0 " << list << std::endl << std::endl;

    auto secondList{list};
    std::cout << "copied list " << secondList << std::endl;
    secondList.insert(-1, 5);
    std::cout << "insert -1 @ index 5 " << secondList << std::endl;
    std::cout << "original list unchanged " << list << std::endl;

    auto thirdList{list};
    thirdList.insert(list,0);
    std::cout << "insert list copy at beginning of orig list" << thirdList << std::endl << std::endl;

    auto fourthList{list};
    fourthList.insert(list,5);
    std::cout << "insert list copy into orig list at index 5" << fourthList << std::endl << std::endl;

    auto fifthList{list};
    fifthList.insert(list,list.size());
    std::cout << "insert list copy at end of original list" << fifthList << std::endl << std::endl;

    return 0;
}

