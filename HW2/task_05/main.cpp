#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <stdio.h>
#include <stdint.h>

/*
// Задача x.x
*/

// На локальной машине, чтобы поверить
#define MAKETEST
#ifdef MAKETEST
#include <cassert>

typedef char byte;
//typedef bool bit;

class IInputStream
{
    std::ifstream& _in;

public:
    IInputStream( std::ifstream& in) : _in(in) {}
    
    bool Read( byte& value )
    {
        if( _in.eof() )
            return false;
        _in.get(value);
        return true;
    }
};

class IOutputStream
{
    std::ofstream& _out;

public:
    IOutputStream( std::ofstream& out ) : _out(out) {}
    
    void Write( byte value )
    {
        _out << value;
    }
};

#else
#include "Huffman.h"
#endif

class bitistream
{
    // Поток
    IInputStream& _stream;

    // Количество байт
    byte _currentByte;

    // Количество бит в текущем байте
    char _bitsRead;

public:
    bitistream( IInputStream & stream ) : _stream(stream), _bitsRead(0) {}

    // Чтение бит. Вернет false в случае конца чтения
    bool read(  bool& value )
    {
        if( _bitsRead == 0 ) {
            byte tmp = 0;
            if( !_stream.Read(tmp) )
                return false;
            
            _currentByte = 0;
            for( int i = 0; i < 8; ++i )
            {
                _currentByte <<= 1;
                _currentByte |= ( 1 & tmp );
                tmp >>= 1;
            }
        }
        value = ( _currentByte & 1 );
        _currentByte >>= 1;
        _bitsRead = ( ( _bitsRead + 1 ) % 8 );
        return true;
    }
};

class bitostream
{
    // Поток
    IOutputStream&  _stream;

    // Количество байт
    byte _currentByte;

    // Количество бит в текущем байте
    char _bitsWritten;

public:
    bitostream( IOutputStream& stream ) : _stream(stream), _bitsWritten(0) {}

    // Запись битов. Вернуть true, если успешно
    bool write( const bool& value )
    {
        _currentByte <<= 1;
        _currentByte |= ( (value) ? 1 : 0 );
        _bitsWritten++;
        if( _bitsWritten == 8 )
        {
            _stream.Write(_currentByte);
            _bitsWritten = 0;
            return true;
        }
        return false;
    }
};


struct Node
{
    byte value;
    long weight;
    Node* left;
    Node* right;

    Node() : value(0), weight(0), left(nullptr), right(nullptr) {}

    Node( byte value, long weight, Node* left, Node* right ) : value(value), weight(weight), left(left), 
                                                               right(right) {}

};

bool nodeComparator( Node* L, Node* R )
{
    return ( L->weight < R->weight );
}

// Строим дерево Хаффмана
Node* buildTree( std::map<byte, long> freq )
{
    std::list<Node *> list;
    for(  auto i = freq.begin(); i != freq.end(); ++i )
    {
        if( i->second != 0 )
        {
            Node* node = new Node( i->first, i->second, nullptr, nullptr );
            list.push_back(node);
        }
    }

    while( list.size() > 1 )
    {
        list.sort(nodeComparator);
        Node* left = list.front();
        list.pop_front();
        Node* right = list.front();
        list.pop_front();
        Node* father = new Node( 0, ( left->weight + right->weight ), left, right );
        list.push_back(father);
    }
    Node* tree = list.front();
    return tree;
}


// Получаем таблицу кодов символов
void getCodes( Node* node, long code, std::map<byte, long>& codes )
{
    if( node->left == nullptr )
    {
        long result = 1;
        while( code != 1 )
        {
            result <<= 1;
            result |= ( code & 1 );
            code >>= 1;
        }
        codes[node->value] = result;
        return;
    }

    getCodes( node->left, code << 1, codes );
    getCodes( node->right, ( ( code << 1 ) + 1 ), codes );
}



void printTree( Node* node, int depth, const std::map<byte, long>& codes )
{
    if( node->left != nullptr )
    {
        printTree( node->right, depth + 1, codes );
        for( int i = 0; i < ( depth * 3 ); ++i )
            std::cout << ' ';
        std::cout << 0 << '\n';
        printTree( node->left, depth + 1, codes );
        return;
    }
    for( int i = 0; i < ( depth * 3 ); ++i )
        std::cout << ' ';
    std::cout << node->value << " (";
    long code = codes.at(node->value);
    while( code != 1 )
    {
        std::cout << ( code & 1 );
        code >>= 1;
    }
    std::cout << ")\n";
}

void deleteTree( Node* node )
{
    if( node == nullptr )
        return;
    deleteTree(node->right);
    deleteTree(node->left);
    delete node;
}

void countCodeLengths( const std::map<byte, long>& codes, std::map<byte, char>& codeLengths )
{
    for( auto i = codes.begin(); i != codes.end(); ++i )
    {
        codeLengths[i->first] = 0;
        if( i->second != 0 )
        {
            long code = i->second;
            while( code != 1 )
            {
                codeLengths[i->first]++;
                code >>= 1;
            }
        }
    }
}

void fixTree( Node* node )
{
    if(node == nullptr)
        return;

    if( ( ( node->left != nullptr ) && ( node->right == nullptr ) ) )
    {
        deleteTree(node->left);
        node->left = nullptr;
        return;
    }

    fixTree(node->left);
    fixTree(node->right);
}

void Encode( IInputStream& original, IOutputStream& compressed )
{

    std::map<byte, long> freq;

    // Чтение файла
    std::string file;
    byte value = 0;
    while( original.Read(value) )
    {
        freq[value]++;
        file += value;
    }
    freq[value]--; // Убираем символ конца файла
    file.erase(file.length() - 1);

    // Строим коды символов
    std::map<byte, long> codes;

    Node* tree = buildTree(freq);  // Строим код Хаффмана по дереву
    getCodes(tree, 1, codes);       // Получаем таблицу кодов
    deleteTree(tree);               // Стираем дерево

    // Считаем длину каждого кода и находим максимальный
    std::map<byte, char> codeLengths;
    countCodeLengths(codes, codeLengths);
    char maxlen = 0; // Максимальная длина кода
    for(  auto i = codeLengths.begin(); i != codeLengths.end(); ++i )
        if( i->second > maxlen )
            maxlen = i->second;

    // Считаем размер алфавита и количество пустых бит
    size_t encodedSize = 0;
    byte alphabetSize = 0;
    for( auto i = freq.begin(); i != freq.end(); ++i )
    {
        if( i->second != 0 )
            alphabetSize++;
        encodedSize = ( ( encodedSize + ( i->second * codeLengths[i->first] ) ) % 8 );
    }
    byte restBits = ( 8 - encodedSize ); // Кол-во неиспользованных бит

    // Подсчет количества байтов для хранения одного кода
    byte bytesPerCode = ( (maxlen % 8 == 0) ? (maxlen / 8) : (maxlen / 8 + 1) );

    bitostream bout(compressed);

/******************************/
    // Запись кодированного файла
    compressed.Write(restBits);
    compressed.Write(bytesPerCode);
    compressed.Write(alphabetSize);
    for( auto i = codes.begin(); i != codes.end(); ++i )
    {
        if( freq[i->first] != 0 )
        {
            long code = i->second;
            int j = 0;
            compressed.Write(byte(i->first));
            while( code != 1 )
            {
                bout.write(code & 1);
                code >>= 1;
                ++j;
            }
            for( ; j < bytesPerCode * 8; ++j ) // Забивание до кратности bytesPerCode
                bout.write(0);
        }
    }
    // Кодировка файла
    for( size_t i = 0; i < file.length(); ++i )
    {
        long code = codes[file[i]];
        while( code != 1 )
        {
            bout.write(code & 1);
            code >>= 1;
        }
    }
    for( int i = 0; i < restBits; ++i ) // Забивание до кратнотсти bytesPerCode
        bout.write(0);
}



void Decode( IInputStream& compressed, IOutputStream& decoded )
{
    // Считывание информации про закодированный файл
    byte restBits, bytesPerCode, tmp;
    compressed.Read(restBits);
    compressed.Read(bytesPerCode);
    compressed.Read(tmp);
    unsigned alphabetSize = (unsigned char) tmp;
    if(alphabetSize == 0) // Сползло все
        alphabetSize = 256;

    // Строим дерево Хаффмана
    Node* tree = new Node( 0, 0, nullptr, nullptr );
    bitistream bin(compressed);
    for( int i = 0; i < alphabetSize; ++i )
    {
        byte symbol;
        Node* node = tree;
        compressed.Read(symbol);
        for( int j = 0; j < 8 * bytesPerCode; ++j )
        {
            bool bit1;
            bin.read(bit1);
            if(bit1 == 0)
            {
                if( node->left == nullptr )
                    node->left = new Node( symbol, 0, nullptr, nullptr );
                node = node->left;
            }
            else
            {
                if( node->right == nullptr )
                    node->right = new Node( symbol, 0, nullptr, nullptr );
                node = node->right;
            }
        }
    }
    fixTree(tree);

    // Чтение файла
    std::string file;
    byte symbol;
    while( compressed.Read(symbol) )
    {
        file += symbol;
    }
    file.erase(file.length() - 1);

    // Разкодировка файла
    Node* node = tree;
    for( int i = 0; i < file.length() - 1; ++i )
    {
        for( int j = 7; j >= 0; --j )
        {
            if( ( (file[i] >> j) & 1 ) == 0 )
                node = node->left;
            else
                node = node->right;
            if( node->left == nullptr )
            {
                decoded.Write(node->value);
                node = tree;
            }
        }
    }
    for( int j = 7; j >= restBits; --j )
    {
        if( ( ( file[file.length() - 1] >> j ) & 1 ) == 0 )
            node = node->left;
        else
            node = node->right;
        if( node->left == nullptr )
        {
            decoded.Write(node->value);
            node = tree;
        }
    }

    deleteTree(tree);
}

//// Тест
#ifdef MAKETEST
int main()
{
    std::ifstream _original("test.txt");
    std::ofstream _compressed1("encoded.txt");
    IInputStream original(_original);
    IOutputStream compressed1(_compressed1);
    Encode(original, compressed1);
    _compressed1.close();
    _original.close();

    std::ifstream _compressed2("encoded.txt");
    std::ofstream _decoded("decoded.txt");
    IInputStream compressed2(_compressed2);
    IOutputStream decoded(_decoded);
    Decode(compressed2, decoded);
    _compressed2.close();
    _decoded.close();
    return 0;
}
#endif

