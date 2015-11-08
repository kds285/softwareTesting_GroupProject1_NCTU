#include "Fib2x3Solver.h"
#include <fstream>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

void Fib2x3Solver::initialize()
{
	m_moveTable.initialize();
	m_exceptedGrade = new double[16777215];
	m_bIsSameNode = new bool[16777215];
	m_bBestMove = new int[16777215];
	for( int i=0; i<16777215; i++ ) {
		m_bIsSameNode[i] = false;
		m_exceptedGrade[i] = -1.0f;
		m_bBestMove[i] = -1;
	}
	loadDB();
	//expandAllSolution();
}

double Fib2x3Solver::evaluteExpectedScore(int arrayboard[2][3])
{
	for( int i=0; i<2; i++ ) {
		for( int j=0; j<3; j++ ) {
			if( arrayboard[i][j]>377 || arrayboard[i][j]<0 ) { return -1; }
		}
	}
	BitBoard board;
	board.setArrayBoard(arrayboard);
	return m_exceptedGrade[board.getIndex()];
	return 0;
}

// 0: up
// 1: right
// 2: down
// 3: left
// -1: cannot move
int Fib2x3Solver::findBestMove(int arrayboard[2][3])
{
	for( int i=0; i<2; i++ ) {
		for( int j=0; j<3; j++ ) {
			if( arrayboard[i][j]>377 ) { return -1; }
		}
	}	
	BitBoard board;
	board.setArrayBoard(arrayboard);
	if( m_bBestMove[board.getIndex()]==UP ) { return 0; }
	else if( m_bBestMove[board.getIndex()]==DOWN ) { return 2; }
	else if( m_bBestMove[board.getIndex()]==RIGHT ) { return 1; }
	else if( m_bBestMove[board.getIndex()]==LEFT ) { return 3; }
	else { return -1; }
}

/**********************************
You can implement any additional functions
you may need.
**********************************/
void Fib2x3Solver::expandAllSolution()
{
	/*int arrayBoard[2][3] = {1,1,1,1,0,3};
	BitBoard board;
	double dScore;
	board.setArrayBoard(arrayBoard);
	
	while( 1 ) {
		// show board
		board.getArrayBoard(arrayBoard);
		cout << "get Score: " << dScore << endl;
		for( int i=0; i<2; i++ ) {
			for( int j=0; j<3; j++ ) {
				cout << arrayBoard[i][j] << " ";
			}
			cout << endl;
		}
		dScore = 0.0f;
		char input;
		cin >> input;
		switch( input ) {
		case 'w':	board.up(m_moveTable,dScore);	break;
		case 'a':	board.left(m_moveTable,dScore);	break;
		case 's':	board.down(m_moveTable,dScore);	break;
		case 'd':	board.right(m_moveTable,dScore);	break;
		default:	break;
		}
	}*/
	
	/*// search for all
	BitBoard iniBitBoard;
	int arrayBoard[2][3];
	vector<unsigned long long int> vIndex[14];
	int fib_index[378];
	fib_index[0] = 0;	fib_index[1] = 1;	fib_index[2] = 2;	fib_index[3] = 3;	fib_index[5] = 4;	fib_index[8] = 5;	fib_index[13] = 6;
	fib_index[21] = 7;	fib_index[34] = 8;	fib_index[55] = 9;	fib_index[89] = 10;	fib_index[144] = 11;	fib_index[233] = 12;	fib_index[377] = 13;
	for( unsigned long long int index=16777214; index>0; index-- ) {
		if( !iniBitBoard.isValidIndex(index) ) { continue; }
		
		// check is same number
		iniBitBoard.setIndex(index);
		iniBitBoard.getArrayBoard(arrayBoard);
		int min = 100;
		for( int i=0; i<2; i++ ) {
			for( int j=0; j<3; j++ ) {
				if( fib_index[arrayBoard[i][j]]<min ) { min = fib_index[arrayBoard[i][j]]; }
			}
		}
		vIndex[min].push_back(index);
	}
	
	for( int number=13; number>=0; number-- ) {
		// show progress
		cout << number << ": ";
		int set=0;
		for( int i=0; i<16777215; i++ ) {
			if( m_exceptedGrade[i]!=-1.0f ) { set++; }
		}
		cout << "now doing:" << double(set/7529536.0f)*100 << "%" << endl;
		cout << vIndex[number].size() << endl;
		
		for( unsigned int i=0; i<vIndex[number].size(); i++ ) {
			unsigned long long int index = vIndex[number][i];
			if( m_exceptedGrade[index]!=-1.0f ) { continue; }
			
			// setting root node
			iniBitBoard.setIndex(index);
			Node *rootNode = new Node;
			rootNode->setParent(NULL);
			rootNode->setBitBoard(iniBitBoard);
			expand(rootNode);
			setScore(rootNode);
			deleteNode(rootNode);
		}
	}*/
	
	BitBoard iniBitBoard;
	int arrayBoard[2][3] = {0,0,0,0,0,0};
	iniBitBoard.setArrayBoard(arrayBoard);
	Node *rootNode = new Node;
	rootNode->setParent(NULL);
	rootNode->setBitBoard(iniBitBoard);
	expand(rootNode);
	setScore(rootNode);
	deleteNode(rootNode);
	
	/*int set=0;
	for( int i=0; i<16777215; i++ ) {
		if( m_exceptedGrade[i]!=-1.0f ) { set++; }
	}
	cout << "total search: " << double(set/7529536.0f)*100 << "%" << endl;*/
	
	// write result to database
	saveDB();
}

void Fib2x3Solver::expand( Node *node )
{	
	if( m_bIsSameNode[node->getBitBoard().getIndex()]==true ) { return; }
	
	//node->getBitBoard().showBoard();
	
	m_bIsSameNode[node->getBitBoard().getIndex()] = true;
	node->expandChild(m_moveTable);
	vector<Node*> vChildren = node->getChildNode();
	
	for( unsigned int i=0; i<vChildren.size(); i++ ) { expand(vChildren[i]); }
}

void Fib2x3Solver::setScore( Node *node )
{
	vector<Node*> vChildren = node->getChildNode();
	unsigned long long myIndex = node->getBitBoard().getIndex();
	
	if( m_exceptedGrade[myIndex]!=-1 ) { return; }
	
	if( vChildren.size()==0 ) {
		m_exceptedGrade[myIndex] = 0.0f;
		m_bBestMove[myIndex] = -1;
		return;
	}
	
	int numDirection[2][4];
	double dScore[2][4];	// up, down, left, right
	for( int i=0; i<2; i++ ) {
		for( int j=0; j<4; j++ ) {
			numDirection[i][j] = 0;
			dScore[i][j] = 0.0f;
		}
	}
	
	for( unsigned int i=0; i<vChildren.size(); i++ ) {
		unsigned long long childIndex = vChildren[i]->getBitBoard().getIndex();
		
		if( m_exceptedGrade[childIndex]==-1 ) { setScore(vChildren[i]); }
		if( vChildren[i]->MOVE_TYPE==UP ) { dScore[vChildren[i]->ADD_TYPE][UP] += m_exceptedGrade[childIndex]; numDirection[vChildren[i]->ADD_TYPE][UP]++; }
		else if( vChildren[i]->MOVE_TYPE==DOWN ) { dScore[vChildren[i]->ADD_TYPE][DOWN] += m_exceptedGrade[childIndex]; numDirection[vChildren[i]->ADD_TYPE][DOWN]++; }
		else if( vChildren[i]->MOVE_TYPE==LEFT ) { dScore[vChildren[i]->ADD_TYPE][LEFT] += m_exceptedGrade[childIndex]; numDirection[vChildren[i]->ADD_TYPE][LEFT]++; }
		else if( vChildren[i]->MOVE_TYPE==RIGHT ) { dScore[vChildren[i]->ADD_TYPE][RIGHT] += m_exceptedGrade[childIndex]; numDirection[vChildren[i]->ADD_TYPE][RIGHT]++; }
	}
	
	// calculate four direction
	double dAvg[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for( int i=0; i<4; i++ ) {
		if( numDirection[0][i]==0 ) { continue; }
		dAvg[i] = (dScore[0][i]/numDirection[0][i]) * 0.75 + (dScore[1][i]/numDirection[1][i]) * 0.25;
		
		if( dAvg[i]+node->m_dScore[i]>m_exceptedGrade[myIndex] ) {
			m_exceptedGrade[myIndex] = dAvg[i]+node->m_dScore[i];
			m_bBestMove[myIndex] = i;
		}
	}
}

void Fib2x3Solver::deleteNode( Node *node )
{
	vector<Node*> vChildren = node->getChildNode();
	for( unsigned int i=0; i<vChildren.size(); i++ ) {
		deleteNode(vChildren[i]);
	}
	delete node;
}

void Fib2x3Solver::saveDB()
{
	ofstream fout("5D.db",ios::out|ios::binary);
	if( !fout ) {
		printf("Save tuple failed.\n");
		return;
	}
	fout.write(reinterpret_cast<char*>(m_exceptedGrade), sizeof(double)*16777215);
	fout.write(reinterpret_cast<char*>(m_bBestMove), sizeof(double)*16777215);
	fout.close();
}

void Fib2x3Solver::loadDB()
{
	ifstream fin("../../Fib2x3Solver/5D.db",ios::in|ios::binary);
	if( !fin ) {
		printf("Load tuple failed.\n");
		return;
	}
	fin.read(reinterpret_cast<char*>(m_exceptedGrade), sizeof(double)*16777215);
	fin.read(reinterpret_cast<char*>(m_bBestMove), sizeof(double)*16777215);
	fin.close();
}