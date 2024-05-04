#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <stdio.h>
#include <stdint.h>

/*
// Задача 5
Напишите две функции для создания архива из одного файла и извлечения файла из архива.
Метод архивирует данные из потока original
'''C++
void Encode(IInputStream& original, IOutputStream& compressed);
'''
Метод восстанавливает оригинальные данные
'''C++
void Decode(IInputStream& compressed, IOutputStream& original);
'''
где:
'''C++
typedef char byte;

interface IInputStream {
// Возвращает false, если поток закончился
virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
virtual void Write(byte value) = 0;
};
'''
В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных. Дерево Хаффмана требуется хранить
эффективно - не более 10 бит на каждый 8-битный символ. В контест необходимо отправить .cpp файл содержащий
функции Encode, Decode, а также включающий файл Huffman.h. Тестирующая программа выводит размер сжатого
файла в процентах от исходного.
Пример минимального решения:
'''C++
#include "Huffman.h"

static void copyStream(IInputStream&input, IOutputStream& output) {
byte value;
while(input.Read(value)) { output.Write(value); }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
copyStream(original, compressed);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
copyStream(compressed, original);
}
'''
*/

// На локальной машине, чтобы поверить
//#define MAKETEST
#ifdef MAKETEST
#include <cassert>

typedef char byte;
class IInputStream
{
    std::ifstream& _in;
public:
    IInputStream( std::ifstream& in) : _in(in) {}
    bool Read( byte& value ) { return ( ( _in.eof() ) ? false : ( (bool)( _in.get(value) ) ) ); }
};

class IOutputStream
{
    std::ofstream& _out;
public:
    IOutputStream( std::ofstream& out ) : _out(out) {}
    void Write( const byte& value ) { _out << value; }
};
#else
#include "Huffman.h"
#endif

class BitIstream
{
    IInputStream& _stream;
    byte _currentByte;
    char _bitsRead;

public:
    BitIstream( IInputStream& stream ) : _stream(stream), _bitsRead(0) {}

    bool read(  bool& value )
    {
        if( _bitsRead == 0 )
        {
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

class BitOstream
{
    IOutputStream&  _stream;
    byte _currentByte;
    char _bitsWritten;

public:
    BitOstream( IOutputStream& stream ) : _stream(stream), _bitsWritten(0) {}

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
    byte _item;
    size_t _weight;
    Node* _l;
    Node* _r;

    Node( byte item = 0, size_t weight = 0, Node* left = nullptr, Node* right = nullptr ) : _item(item),
            _weight(weight), _l(left), _r(right) {}

    ~Node()
    {
        if( this->_l != nullptr )
            delete this->_l;
        if( this->_r != nullptr )
            delete this->_r;
    }
};

class HuffmanTreeEncode
{
    Node* _root;
    std::string _data;        // Данные из потока
    byte _abcSize;            // Размер алфавита
    byte _noUseBits;          // Неиспользованные биты
    byte _countBytesForCode;  // Байты для хранения одного кода
    size_t _maxSize;          // Максимальная длина кода
    /* Для HuffmanTreeDecode аналогично */
    std::map<byte, size_t> _frequencySymbol;
    std::map<byte, size_t> _codes;
    std::map<byte, char> _codesSize;

    void treeInit()
    {
        std::list<Node*> list;
        for(  auto i = _frequencySymbol.begin(); i != _frequencySymbol.end(); ++i )
            if( i->second != 0 )
                list.push_back( ( new Node( i->first, i->second, nullptr, nullptr ) ) );
        Node *left, *right;
        while( list.size() > 1 )
        {
            list.sort([]( Node* l, Node* r ){ return ( l->_weight < r->_weight ); });
            left = list.front();
            list.pop_front();
            right = list.front();
            list.pop_front();
            list.push_back( ( new Node( 0, ( left->_weight + right->_weight ), left, right ) ) );
        }
        _root = list.front();
    }

    void calcCodes( Node* node, size_t code = 1 )
    {
        if( node->_l == nullptr )
        {
            size_t result = 1;
            while( code != 1 )
            {
                result <<= 1;
                result |= ( code & 1 );
                code >>= 1;
            }
            _codes[node->_item] = result;
            return;
        }

        calcCodes( node->_l, ( code << 1 ) );
        calcCodes( node->_r, ( ( code << 1 ) + 1 ) );
    }
    
    void countCodesSize()
    {
        for( auto i = _codes.begin(); i != _codes.end(); ++i )
        {
            _codesSize[i->first] = 0;
            if( i->second != 0 )
                for( size_t code = i->second; code != 1; code >>= 1 )
                    _codesSize[i->first]++;
        }
    }

    void calcMaxSizeCode()
    {
        for(  auto i = _codesSize.begin(); i != _codesSize.end(); ++i )
            if( i->second > _maxSize )
                _maxSize = i->second;
    }

public:

    HuffmanTreeEncode() : _root(nullptr), _data(""), _abcSize(0), _noUseBits(0), _countBytesForCode(0),
                          _maxSize(0) {}

    ~HuffmanTreeEncode()
    {
        delete _root;
        _data.clear();
        _frequencySymbol.clear();
        _codes.clear();
        _codesSize.clear();
    }

    void calcFrequency( IInputStream& original )
    {
        byte value = 0;
        while( original.Read(value) )
        {
            _frequencySymbol[value]++;
            _data += value;
        }
        treeInit();
    }

    void calcCodesHuffmans()
    {
        calcCodes(_root);
        countCodesSize();
        calcMaxSizeCode();
    }

    void calcOptions()
    {
        size_t buffer = 0;
        for( auto i = _frequencySymbol.begin(); i != _frequencySymbol.end(); ++i )
        {
            if( i->second != 0 )
                ++_abcSize;
            buffer = ( ( buffer + ( i->second * _codesSize[i->first] ) ) % 8 );
        }
        _noUseBits = ( 8 - buffer );
        _countBytesForCode = ( ( ( _maxSize % 8 ) == 0 ) ? ( _maxSize / 8 ) : ( ( _maxSize / 8 ) + 1 ) );
    }

    void encode( IOutputStream& compressed )
    {
        BitOstream bout(compressed);
        compressed.Write(_noUseBits);
        compressed.Write(_countBytesForCode);
        compressed.Write(_abcSize);
        for( auto i = _codes.begin(); i != _codes.end(); ++i )
            if( _frequencySymbol[i->first] != 0 )
            {
                size_t j = 0;
                compressed.Write( ( (byte)(i->first) ) );
                for( size_t code = i->second; code != 1; code >>= 1, ++j)
                    bout.write( ( code & 1 ) );
                for( size_t count = ( _countBytesForCode * 8 ); j < count; ++j )
                    bout.write(0);
            }

        for( size_t i = 0; i < _data.length(); ++i )
            for( size_t code = _codes[_data[i]]; code != 1; code >>= 1)
                bout.write( ( code & 1 ) );
        for( size_t i = 0; i < _noUseBits; ++i )
            bout.write(0);
    }
};

void Encode( IInputStream& original, IOutputStream& compressed )
{
    HuffmanTreeEncode tree;
    tree.calcFrequency(original);
    tree.calcCodesHuffmans();
    tree.calcOptions();
    tree.encode(compressed);
}

class HuffmanTreeDecode
{
    Node* _root;
    byte _noUseBits;
    byte _countBytesForCode;
    uint8_t _abcSize;
    std::string _data;

    void readOptions( IInputStream& compressed )
    {
        compressed.Read(_noUseBits);
        compressed.Read(_countBytesForCode);
        byte tmp = 0;
        compressed.Read(tmp);
        _abcSize = ((uint8_t)(tmp));
        if(_abcSize == 0)
            _abcSize = 0xFF;
    }
    
    void fillTree( IInputStream& compressed )
    {
        _root = new Node( 0, 0, nullptr, nullptr );
        BitIstream bin(compressed);
        byte symbol;
        Node* node;

        for( size_t i = 0; i < _abcSize; ++i )
        {
            node = _root;
            compressed.Read(symbol);
            for( size_t j = 0, count = ( 8 * _countBytesForCode ); j < count; ++j )
            {
                bool bit;
                bin.read(bit);
                if( ( (bit) ? node->_r : node->_l ) == nullptr )
                    ( (bit) ? node->_r : node->_l ) = new Node( symbol, 0, nullptr, nullptr );
                node = ( (bit) ? node->_r : node->_l );
            }
        }
    }

    void repairTree( Node* node )
    {
        if( node == nullptr )
            return;

        if( ( ( node->_l != nullptr ) && ( node->_r == nullptr ) ) )
        {
            if( node->_l != nullptr )
                delete node->_l;
            node->_l = nullptr;
            return;
        }

        repairTree(node->_l);
        repairTree(node->_r);
    }

public:
    HuffmanTreeDecode() : _root(nullptr), _noUseBits(0), _countBytesForCode(0), _abcSize(0), _data("") {}
    ~HuffmanTreeDecode()
    {
        delete _root;
        _data.clear();
    }

    void buildHuffmanTree( IInputStream& compressed )
    {
        readOptions(compressed);
        fillTree(compressed);
        repairTree(_root);
    }

    void readDataFromFile( IInputStream& compressed )
    {
        byte symbol;
        while( compressed.Read(symbol) )
            _data += symbol;
    }

    void decode( IOutputStream& decoded )
    {
        Node* node = _root;
        for( int i = 0, count = ( _data.length() - 1 ); i < count; ++i )
            for( int j = 7; j >= 0; --j )
            {
                node = ( ( ( ( ( _data[i] >> j ) & 1 ) == 0 ) ) ? node->_l : node->_r );
                if( node->_l == nullptr )
                {
                    decoded.Write(node->_item);
                    node = _root;
                }
            }
        for( int j = 7; j >= _noUseBits; --j )
        {
            node = ( ( ( ( _data[( _data.length() - 1 )] >> j ) & 1 ) == 0 ) ? node->_l : node->_r );
            if( node->_l == nullptr )
            {
                decoded.Write(node->_item);
                node = _root;
            }
        }
    }
};

void Decode( IInputStream& compressed, IOutputStream& decoded )
{
    HuffmanTreeDecode tree;
    tree.buildHuffmanTree(compressed);
    tree.readDataFromFile(compressed);
    tree.decode(decoded);
}

//// Тест
#ifdef MAKETEST
class FileNotOpen : public std::exception
{
public:
    explicit FileNotOpen(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

int main()
{
    std::ifstream _original("testFile/test.txt");
    if( !_original )
        throw FileNotOpen( "testFile/test.txt" );
    std::ofstream _compressed1("testFile/encoded.txt");
    if( !_compressed1 )
        throw FileNotOpen( "testFile/encoded.txt" );

    IInputStream original(_original);
    IOutputStream compressed1(_compressed1);
    Encode(original, compressed1);
    _compressed1.close();
    _original.close();

    std::ifstream _compressed2("testFile/encoded.txt");
    if( !_compressed2 )
        throw FileNotOpen( "testFile/encoded.txt" );
    std::ofstream _decoded("testFile/decoded.txt");
    if( !_decoded )
        throw FileNotOpen( "testFile/decoded.txt" );
    IInputStream compressed2(_compressed2);
    IOutputStream decoded(_decoded);
    Decode(compressed2, decoded);
    _compressed2.close();
    _decoded.close();
    return 0;
}
#endif


