#include <iostream>
#include <deque>
#include <vector>
#include <fstream>

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
        bool _isLeaf;
        std::vector<T> _items;
        std::vector<Node*> _leafs;

        Node( const bool& leaf = false ) : _isLeaf(leaf) {}
        ~Node() {
            _items.clear();
            _leafs.clear();
        }
    };

    size_t _order;
    bool (*_cmp)( const T&, const T& );
    Node* _root;

    void reBuild( Node* node, const size_t& position )
    {
        Node* bufferFullNode = node->_leafs[position];
        Node* bufferEmptyNode = new Node(bufferFullNode->_isLeaf);
        bufferEmptyNode->_items.resize( ( _order - 1 ) );

        for( size_t j = 0, J = ( _order - 1 ); j < J; ++j )
            bufferEmptyNode->_items[j] = bufferFullNode->_items[( j + _order )];

        if( !bufferFullNode->_isLeaf )
        {
            bufferEmptyNode->_leafs.resize(_order);
            for( size_t j = 0; j < _order; ++j )
                bufferEmptyNode->_leafs[j] = bufferFullNode->_leafs[( j + _order )];
            bufferFullNode->_leafs.resize(_order);
        }

        node->_leafs.resize( ( node->_leafs.size() + 1 ) );
        for( size_t j = ( node->_leafs.size() - 1 ), J = ( position + 1 ); j > J; --j )
            node->_leafs[j] = node->_leafs[( j - 1 )];
        node->_leafs[( position + 1 )] = bufferEmptyNode;

        node->_items.resize( ( node->_items.size() + 1 ) );
        for( size_t j = ( node->_items.size() - 1 ); j > position; --j )
            node->_items[j] = node->_items[( j - 1 )];
        node->_items[position] = bufferFullNode->_items[( _order - 1 )];

        bufferFullNode->_items.resize( ( _order - 1 ) );
    }

    void pushToNode( Node* node, const T& item )
    {
        int position = ( node->_items.size() - 1 );
        if( node->_isLeaf )
        {
            node->_items.resize( ( node->_items.size() + 1 ) );
            for( ; ( ( position >= 0 ) && ( _cmp( item, node->_items[position] ) ) ); --position )
                node->_items[( position + 1 )] = node->_items[position];
            return ( (void)( node->_items[( position + 1 )] = item ) );
        }

        for( ; ( ( position >= 0 ) && ( _cmp( item, node->_items[position] ) ) ); --position );

        if( ( node->_leafs[( position + 1 )]->_items.size() == ( ( 2 * _order ) - 1 ) ) )
        {
            reBuild( node, ( position + 1 ) );
            if( _cmp( node->_items[( position + 1 )], item ) )
                ++position;
        }
        pushToNode( node->_leafs[( position + 1 )], item );
    }

public:
    BTree( const size_t& order, bool ( *cmp )( const T& l, const T& r ) = defaultLess ) : _order(order),
        _cmp(cmp), _root(nullptr) {}
    ~BTree()
    {
        delete _root;
    }

    void push( const T& item )
    {
        if( _root == nullptr )
            _root = new Node(true);

        if( ( _root->_items.size() == ( ( 2 * _order ) - 1 ) ) )
        {
            Node* node = new Node(false);
            node->_leafs.push_back(_root);
            _root = node;
            reBuild( _root, 0 );
        }

        pushToNode( _root, item );
    }

    void printLevels()
    {
        if( _root == nullptr )
            return;

        std::deque<Node*> nodes;
        nodes.push_back(_root);
        while( !nodes.empty() )
        {
            std::deque<Node*> bufferNodes;
            while( !nodes.empty() )
            {
                Node* node = nodes.front();
                nodes.pop_front();

                for( size_t i = 0; i < node->_items.size(); ++i )
                    std::cout << node->_items[i] << ' ';
                for( size_t i = 0; i < node->_leafs.size(); ++i )
                    bufferNodes.push_back(node->_leafs[i]);
            }
            std::cout << std::endl;
            nodes = bufferNodes;
        }
    }
};

//// Тест
#ifdef MAKETEST
void test1()
{
    std::cout << "--------------------------" << std::endl;
    BTree<int> tree(2);
    for( int i = 0; i < 10; ++i )
        tree.push(i);
    tree.printLevels();
}

void test2()
{
    std::cout << "--------------------------" << std::endl;
    BTree<int> tree(4);
    for( int i = 0; i < 10; ++i )
        tree.push(i);
    tree.printLevels();
}

void test3()
{
    std::cout << "--------------------------" << std::endl;
    BTree<int> tree(5);
    for( size_t i = 9; i > 0; --i )
        tree.push(i);
    tree.push(0);
    tree.printLevels();
}

void test4()
{
    std::cout << "--------------------------" << std::endl;
    BTree<int> tree(2);
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
    BTree tree(4);
    std::ifstream in("./input/in11.txt", (std::ios::binary | std::ios::in));
    if( !in )
        throw FileNotOpen("input/in11.txt");
    size_t key = 0;
    while( in >> key )
        tree.push(key);
    tree.printLevels();
}
#endif

int main()
{
#ifdef MAKETEST
    test1();
    test2();
    test3();
    test4();
    test5();
    //test6();
#else
    size_t order = 0;
    std::cin >> order;
    BTree tree(order);
    size_t key = 0;
    while( std::cin >> key )
        tree.push(key);
    tree.printLevels();
#endif
    return 0;
}


