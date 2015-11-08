#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include "MoveTable.h"
#include <iostream>
using namespace std;

class BitBoard {
private:
	unsigned long long m_bits;
	int fibonacci[14] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377};
	int fib_index[378];
public:
	BitBoard() {}
	~BitBoard() {}
	unsigned long long getIndex() { return m_bits; }
	void setIndex( unsigned long long index ) { m_bits = index; }
	bool isValidIndex( unsigned long long index )
	{
		int tile;
		for( int i=0; i<6; i++ ) {
			tile = index & 0xf;
			index = index >> 4;
			if( tile>13 ) { return false; }
		}
		return true;
	}
	void setArrayBoard( int arrayBoard[2][3] )
	{
		fib_index[0] = 0;	fib_index[1] = 1;	fib_index[2] = 2;	fib_index[3] = 3;	fib_index[5] = 4;	fib_index[8] = 5;	fib_index[13] = 6;
		fib_index[21] = 7;	fib_index[34] = 8;	fib_index[55] = 9;	fib_index[89] = 10;	fib_index[144] = 11;	fib_index[233] = 12;	fib_index[377] = 13;
		m_bits = 0ULL;
		for( int i=0; i<2; i++ ) {
			for( int j=0; j<3; j++ ) {
				m_bits = m_bits << 4;
				m_bits = m_bits | fib_index[arrayBoard[i][j]];
			}
		}
	}
	void getArrayBoard( int arrayBoard[2][3] )
	{
		unsigned long long bits = m_bits;
		
		for( int i=1; i>=0; i-- ) {
			for( int j=2; j>=0; j-- ) {
				arrayBoard[i][j] = fibonacci[(bits & 0xf)];
				bits = bits >> 4;
			}
		}
	}
	void showBoard()
	{
		int arrayBoard[2][3];
		getArrayBoard(arrayBoard);
		for( int i=0; i<2; i++ ) {
			for( int j=0; j<3; j++ ) {
				cout << arrayBoard[i][j] << " ";
			}
			cout << endl;
		}
		cout << "=============" << endl;
	}
	bool up( MoveTable& moveTable, double& dScore )
	{
		// 5 4 3 2 1 0
		// 5 2, 4 1, 3 0
		unsigned long long backup = m_bits;
		unsigned long long int col[3] = { (((m_bits&0xf000)>>8)|(m_bits&0xf)), (((m_bits&0xf0000)>>12)|((m_bits&0xf0)>>4)), (((m_bits&0xf00000)>>16)|((m_bits&0xf00)>>8)) };
		int newCol[3];
		int newBoard[6];
		for( int i=0; i<3; i++ ) {
			newCol[i] = moveTable.up(col[i],dScore);
			newBoard[i] = (newCol[i] & 0xf);
			newBoard[i+3] = ((newCol[i] & 0xf0)>>4); 
		}
		m_bits = 0ULL;
		for( int i=5; i>=0; i-- ) {
			m_bits = m_bits << 4;
			m_bits = m_bits | newBoard[i];
		}
		if( m_bits==backup ) { return false; }
		else { return true; }
	}
	bool down( MoveTable& moveTable, double& dScore )
	{
		unsigned long long backup = m_bits;
		unsigned long long int col[3] = { (((m_bits&0xf000)>>8)|(m_bits&0xf)), (((m_bits&0xf0000)>>12)|((m_bits&0xf0)>>4)), (((m_bits&0xf00000)>>16)|((m_bits&0xf00)>>8)) };
		int newCol[3];
		int newBoard[6];
		for( int i=0; i<3; i++ ) {
			newCol[i] = moveTable.down(col[i],dScore);
			newBoard[i] = (newCol[i] & 0xf);
			newBoard[i+3] = ((newCol[i] & 0xf0)>>4); 
		}
		m_bits = 0ULL;
		for( int i=5; i>=0; i-- ) {
			m_bits = m_bits << 4;
			m_bits = m_bits | newBoard[i];
		}
		if( m_bits==backup ) { return false; }
		else { return true; }
	}
	bool left( MoveTable& moveTable, double& dScore )
	{
		unsigned long long backup = m_bits;
		unsigned long long int row[2] = { ((m_bits&0xfff000)>>12), (m_bits&0xfff) };
		m_bits = 0ULL;
		for( int i=0; i<2; i++ ) {
			m_bits = m_bits << 12;
			m_bits = m_bits | moveTable.left(row[i],dScore);
		}
		if( m_bits==backup ) { return false; }
		else { return true; }
	}
	bool right( MoveTable& moveTable, double& dScore )
	{
		unsigned long long backup = m_bits;
		unsigned long long int row[2] = { ((m_bits&0xfff000)>>12), (m_bits&0xfff) };
		m_bits = 0ULL;
		for( int i=0; i<2; i++ ) {
			m_bits = m_bits << 12;
			m_bits = m_bits | moveTable.right(row[i],dScore);
		}
		if( m_bits==backup ) { return false; }
		else { return true; }
	}
};

#endif
