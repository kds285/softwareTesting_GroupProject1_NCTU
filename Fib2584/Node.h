#ifndef __NODE_H__
#define __NODE_H__

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#include "BitBoard.h"
#include <iostream>
#include <vector>
using namespace std;

class Node {
private:
	BitBoard m_bitBoard;
	Node* m_parent;
	vector<Node*> m_vChildNode;
public:
	int MOVE_TYPE;
	int ADD_TYPE;
	double m_dScore[4];
public:
	Node()
	{
		for( int i=0; i<4; i++ ) { m_dScore[i] = 0.0f; }
	}
	~Node() {}
	void setBitBoard( BitBoard bitBoard ) { m_bitBoard = bitBoard; }
	BitBoard getBitBoard() { return m_bitBoard; }
	vector<Node*> getChildNode() { return m_vChildNode; }
	void setParent( Node* parent ) { m_parent = parent; }
	void expandChild( MoveTable& moveTable )
	{
		Node* newNode;
		bool bCanMove;
		int arrayBoard[2][3];
		BitBoard bitBoard,newBoard;

		// move four direction
		for( int direction=0; direction<4; direction++ ) {

			newBoard = m_bitBoard;
			
			if( m_bitBoard.getIndex()==0ULL ) {
				bCanMove = true;
			} else {
				if( direction==UP ) { bCanMove = newBoard.up(moveTable,m_dScore[UP]); }
				else if( direction==DOWN ) { bCanMove = newBoard.down(moveTable,m_dScore[DOWN]); }
				else if( direction==LEFT ) { bCanMove = newBoard.left(moveTable,m_dScore[LEFT]); }
				else if( direction==RIGHT ) { bCanMove = newBoard.right(moveTable,m_dScore[RIGHT]); }
				newBoard.getArrayBoard(arrayBoard);
			}
			
			if( bCanMove ) {
				// generate a new number & child
				for( int i=0; i<2; i++ ) {
					for( int j=0; j<3; j++ ) {
						if( arrayBoard[i][j]==0 ) {
							// new child for 1
							arrayBoard[i][j] = 1;
							bitBoard.setArrayBoard(arrayBoard);
							newNode = new Node;
							newNode->setParent(this);
							newNode->setBitBoard(bitBoard);
							newNode->MOVE_TYPE = direction;
							newNode->ADD_TYPE = 0;
							m_vChildNode.push_back(newNode);
							
							// new child for 3
							arrayBoard[i][j] = 3;
							bitBoard.setArrayBoard(arrayBoard);
							newNode = new Node;
							newNode->setParent(this);
							newNode->setBitBoard(bitBoard);
							newNode->MOVE_TYPE = direction;
							newNode->ADD_TYPE = 1;
							m_vChildNode.push_back(newNode);
	
							arrayBoard[i][j] = 0;
						}
					}
				}
			}
		}
	}
};

#endif
