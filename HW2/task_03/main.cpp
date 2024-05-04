#include <iostream>
#include <deque>
#include <vector>
#include <fstream>
#include <queue>
/*
// Задача 3
Постройте B-дерево минимального порядка t и выведите его по слоям. В качестве ключа используются числа, 
лежащие в диапазоне [0..2^32-1]
//// Требования:
* B-дерево должно быть реализовано в виде шаблонного класса.
* Решение должно поддерживать передачу функции сравнения снаружи.

//// Формат ввода:
Сначала вводится минимальный порядок дерева t. Затем вводятся элементы дерева.

//// Формат вывода:
Программа должна вывести B-дерево по слоям. Каждый слой на новой строке, элементы должны выводится в 
том порядке, в котором они лежат в узлах.
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <cassert>
#endif

//// Код

#if 0
template <typename T = size_t>  // Взял с семинара
bool defaultLess( const T &l, const T &r )
{
    return l < r;
}

template <typename T = size_t>
class BTree
{
    struct Node
    {
        size_t _order;
        bool _isLeaf;
    private:
        bool (*_cmp)( const T&, const T& );
    public:
        size_t _size;

        std::vector<T> _keys;
        Node **_children;

        Node() = delete;
        Node( const size_t& order, const bool& isLeaf, bool ( *cmp )( const T& l, const T& r ) ) : 
            _order(order), _isLeaf(isLeaf), _cmp(cmp), _size(0)
        {
            _keys.resize( ( ( 2 * _order ) - 1 ) );
            //_keys = new T[( ( 2 * _order ) - 1 )];
            _children = new Node*[( 2 * _order )];
        }
    
        ~Node()
        {
            for( size_t i = 0; i < _size; ++i )
                delete[] _children[i];
            delete[] _children;
            delete[] _keys;
        }

        void pushToNode( const T& key )
        {
            if( _isLeaf )
            {
                T buffer = key;
                for( size_t i = 0; ( ( i < _size ) ); ++i )
                    if( _cmp( buffer, _keys.at( i ) ) )
                        std::swap( buffer, _keys[i] );
                _keys[_size++] = buffer;
                return;
            }

            
            size_t i = 0;
            for( ; ( i < _size ); ++i )
            {
                if( _cmp( key, _keys.at(i) ) )
                {
                //    std::cout << "i: " << i << " Key: " << _keys[i] << std::endl; 
                    break;
                }
            }
            
            //size_t i = ( _size - 1 );

            //while( ( ( i >= 0 ) && _cmp( key, _keys[ i ] ) ) )
              //  --i;

            //std::cout << std::endl;
            //for(size_t j = 0; j < _size; ++j)
             //   std::cout << '(' << j << ", " << _keys[j] << ") ";
            //std::cout << std::endl;
            //i %= _size;
            //std::cout << _size << " i:" << i << " Key: " << _keys[i] << " push:" << key << std::endl;
            

            if( _children[( i )]->_size == ( ( 2 * _order ) - 1 ) )
            {
                reBuild( ( i ), _children[( i )] );
                if( _cmp( _keys[( i )], key ) )
                    ++i;
            }
            _children[( i )]->pushToNode(key);
        }

        void reBuild( const size_t& I, Node *node )
        {
            Node* buffer = new Node( node->_order, node->_isLeaf, _cmp );
            buffer->_size = ( _order - 1 );

            for( size_t j = 0; j < ( _order - 1 ); ++j )
                buffer->_keys[j] = node->_keys[j + _order];
            
            if( !node->_isLeaf )
                for( size_t j = 0; j < _order; ++j )
                    buffer->_children[j] = node->_children[( j + _order )];
            
            node->_size = ( _order - 1 );
            for( size_t j = _size; j >= ( I + 1 ); --j )
            {
                //std::cout << j << std::endl;
                _children[( j + 1 )] = _children[j];
            }
            _children[( I + 1 )] = buffer;
            //for(size_t i = 0; i < _size; ++i)
            //    std::cout << _keys[i] << ' ';
            //std::cout << std::endl;

            for( size_t j = _size; j > I; --j )  // Вот тут мог быть выход за границы
                _keys[( j + 1 )] = _keys[j];
            _keys[( I + 1 )] = _keys[I];

            _keys[I] = node->_keys[( _order - 1 )];
            _size = ( _size + 1 );
        }
    };

    size_t _order;
    bool (*_cmp)( const T&, const T& );
    Node *_root;

    /** @brief printNode - выведет ноду и сохранит указатели на ее листья, если есть.
     * */
    void printNode( std::deque<Node*>& deq, Node* node )
    {
        for( size_t i = 0; i < node->_size; ++i)
        {
            std::cout << node->_keys[i] << ' ';
            if( !node->_isLeaf )
                deq.push_back(node->_children[i]);
        }
        if( !node->_isLeaf )
            deq.push_back(node->_children[(node->_size)]);
    }

public:
    BTree() = delete;
    BTree( const size_t& order, bool ( *cmp )( const T& l, const T& r ) = defaultLess ) : _order(order), 
        _cmp(cmp), _root(nullptr) {}

    ~BTree() = default;

    void printLevels()
    {
        if( _root == nullptr )
            return;

        std::deque<Node*> deq;
        printNode( deq, _root );
        std::cout << std::endl;

        size_t N = deq.size(), j = 1;

        while( !deq.empty() )
        {
            Node* node = deq.front();
            deq.pop_front();

            printNode( deq, node );
            if( (j++) == N )
            {
                std::cout << std::endl;
                N = deq.size();
                j = 0;
            }       
        }
        std::cout << std::endl;
    }

    void push( const T& key )
    {
        if ( _root == nullptr )
        {
            _root = new Node( _order, true, _cmp );
            _root->_keys[0] = key;
            _root->_size = 1;
            return;
        }
        //std::cout << "--------------------------" << std::endl;
        //std::cout << "Root->Key[0]: " << _root->_keys[0] << " key: " << key << std::endl; 
        if ( _root->_size != ( ( 2 * _order ) - 1 ) )
        {
            //std::cout << "Key: " << key << " added current root" << std::endl;
            _root->pushToNode(key);
        //std::cout << _root->_keys[0] << std::endl;

            return; //( (void)( _root->pushToNode(key) ) );
        }
        //std::cout << "Key: " << key << " Make new node" << std::endl;
        Node *node = new Node( _order, false, _cmp );
        node->_children[0] = _root;
        node->reBuild( 0, _root );
        node->_children[( ( _cmp( node->_keys[0], key ) ? 1 : 0 ) )]->pushToNode(key);
        _root = node;

        //std::cout << _root->_keys[0] << std::endl;
    }
};
#endif

struct Node
    {
        size_t _order;
        bool _isLeaf;
        size_t _size;

        std::vector<int> _keys;
        std::vector<Node*> _children;

        Node() = delete;
        Node( const size_t& order, const bool& isLeaf ) : 
            _order(order), _isLeaf(isLeaf), _size(0)
        {
            _keys.resize( ( ( 2 * _order ) - 1 ) );
            _children.resize(( 2 * _order ));
        }
        ~Node(){}
};

template<typename T = int>
class BTree
{
public:
    BTree(size_t order) : _root(nullptr), _order(order) {}

    void push(const T& key)
    {
        if (_root == nullptr)
        {
            _root = new Node(_order, true);
            _root->_keys[0] = key;
            _root->_size = 1;
        }
        else
        {
            if (_root->_size == (2 * _order - 1))
            {
                Node* newRoot = new Node(_order, false);
                newRoot->_children[0] = _root;
                splitChild(newRoot, _root, 0);
                _root = newRoot;
            }
            insertNonFull(_root, key);
        }
    }

    void printByLevel()
    {
        if (_root == nullptr)
        {
            std::cout << "Tree is emptyn";
            return;
        }

        std::queue<Node*> q;
        q.push(_root);

        while (!q.empty())
        {
            int nodeCount = q.size();

            while (nodeCount > 0)
            {
                Node* node = q.front();
                q.pop();

                for (size_t i = 0; i < node->_size; ++i)
                {
                    std::cout << node->_keys[i] << " ";
                }

                if (!node->_isLeaf)
                {
                    for (size_t i = 0; i <= node->_size; ++i)
                    {
                        q.push(node->_children[i]);
                    }
                }

                --nodeCount;
            }

            std::cout << std::endl;
        }
    }

private:
    Node* _root;
    size_t _order;

    void insertNonFull(Node* node, const T& key)
    {
        int i = node->_size - 1;

        if (node->_isLeaf)
        {
            while (i >= 0 && key < node->_keys.at(i))
            {
                node->_keys.at(i + 1) = node->_keys.at(i);
                --i;
            }

            node->_keys.at(i + 1) = key;
            node->_size++;
        }
        else
        {
            while (i >= 0 && key < node->_keys.at(i))
            {
                --i;
            }

            ++i;

            if (node->_children.at(i)->_size == 2 * _order - 1)
            {
                splitChild(node, node->_children[i], i);

                if (key > node->_keys.at(i))
                {
                    ++i;
                }
            }
            insertNonFull(node->_children[i], key);
        }
    }

    void splitChild(Node* parent, Node* child, size_t index)
    {
        Node* newChild = new Node(_order, child->_isLeaf);
        newChild->_size = _order - 1;

        for (size_t i = 0; i < _order - 1; ++i)
        {
            newChild->_keys[i] = child->_keys[i + _order];
        }

        if (!child->_isLeaf)
        {
            for (size_t i = 0; i < _order; ++i)
            {
                newChild->_children[i] = child->_children[i + _order];
            }
        }

        child->_size = _order - 1;

        for (int i = parent->_size; i > index; --i)
        {
            parent->_children[i + 1] = parent->_children[i];
        }

        parent->_children[index + 1] = newChild;

        for (int i = parent->_size - 1; i >= index; --i)
        {
            parent->_keys[i + 1] = parent->_keys[i];
        }

        parent->_keys[index] = child->_keys[_order - 1];
        parent->_size++;
    }

};


//// Тест
#ifdef MAKETEST
void test1()
{
    std::cout << "--------------------------" << std::endl;
    BTree tree(2);
    for( size_t i = 0; i < 10; ++i )
        tree.push(i);
    tree.printByLevel();
}

void test2()
{
    std::cout << "--------------------------" << std::endl;
    BTree tree(4);
    for( size_t i = 0; i < 10; ++i )
        tree.push(i);
    tree.printByLevel();
}
#if 0
void test3()
{
    std::cout << "--------------------------" << std::endl;
    BTree tree(5);
    for( size_t i = 9; i > 0; --i )
        tree.push(i);
    tree.push(0);
    tree.printLevels();
}

void test4()
{
    std::cout << "--------------------------" << std::endl;
    BTree tree(2);
    tree.push(5);
    tree.push(9);
    tree.push(3);
    tree.push(7);
    tree.push(1);
    tree.push(2);
    tree.push(8);
    tree.push(6);
    tree.push(0);
    tree.push(4);
/*
                                 ___
                                |_5_|
                            ___/     \___
                           |_2_|     |_8_|
                         _/    |     |    \_
                       _/      |     |      \_
                 ___ _/_    ___|___  |_______ \____
                |_0_|_1_|  |_3_|_4_| |_6_|_7_| |_9_| 
*/
    tree.printLevels();
}

void test5()
{
    std::cout << "--------------------------" << std::endl;
    BTree tree(2);
    for( size_t i = 0; i < 7; ++i )
        tree.push(i);
    tree.printLevels();
}

class FileNotOpen : public std::exception
{
public:
    explicit FileNotOpen(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

void test6()
{
    //std::cout << "--------------------------" << std::endl;
    BTree tree(4);
    std::ifstream in("./input/in11.txt", (std::ios::binary | std::ios::in));
    if( !in )
        throw FileNotOpen("input/in11.txt");

    size_t key = 0;
    while( in >> key)
    {
        tree.push(key);
    }
    tree.printLevels();

}
#endif
#endif

int main()
{
#ifdef MAKETEST
    test1();
    test2();
    //test3();
    //test4();
    //test5();
    //test6();
#else
    size_t order = 0;
    std::cin >> order;
    BTree tree(order);
    size_t key = 0;
    while( std::cin >> key )
    {
        tree.push(key);
    }
    tree.printLevels();
#endif
    return 0;
}


