#ifndef __FIB2X3SOLVER_H__
#define __FIB2X3SOLVER_H__

// include anything you might use
#include <iostream>
#include <map>
#include "Fib2584/Node.h"
#include "Fib2584/BitBoard.h"
#include "Fib2584/MoveTable.h"

using namespace std;

class Fib2x3Solver
{
public:
	Fib2x3Solver() { initialize(); }
	// initialize solver
	void initialize();
	// do some action when game over
	double evaluteExpectedScore(int board[2][3]);
	int findBestMove(int board[2][3]);
	
	/**********************************
	You can implement any additional functions
	or define any variables you may need.
	**********************************/
	void expandAllSolution();
	void expand( Node *node );
	void setScore( Node *node );
	void deleteNode( Node *node );
	void saveDB();
	void loadDB();
private:
	MoveTable m_moveTable;
	double *m_exceptedGrade;
	bool *m_bIsSameNode;
	int *m_bBestMove;
};

static double Calculate(int a, int b, int c, int d, int e, int f)
{	
	static Fib2x3Solver solver;
	int board[2][3] = {{a,b,c},{d,e,f}};
	
	return solver.evaluteExpectedScore(board);
}

#endif
