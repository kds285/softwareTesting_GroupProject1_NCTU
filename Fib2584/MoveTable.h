#ifndef __MOVETABLE_H__
#define __MOVETABLE_H__

#include <cmath>
#include <iostream>
#include "MoveDirection.h"

using namespace std;

class MoveTable {
private:
	int **row; // 3
	int **col; // 2
	double **row_score;
	double **col_score;
	int fibonacci[14] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377};
public:
	MoveTable() {}
	~MoveTable() {}
	void initialize()
	{
		// row: 3 * 4 = 12(4096)
		// col: 2 * 4 = 8(256)
		row = new int*[2];
		col = new int*[2];
		row_score = new double*[2];
		col_score = new double*[2];
		for( int i=0; i<2; i++ ) { 
			row[i] = new int[4096];
			row_score[i] = new double[4096];
		}
		for( int i=0; i<2; i++ ) {
			col[i] = new int[256];
			col_score[i] = new double[256];
		}

		for( int i=0; i<4096; i++ ) {
			row_score[0][i] = row_score[1][i] = 0.0f;
			row[0][i] = rowMove(0,i,row_score[0][i]);	//right
			row[1][i] = rowMove(1,i,row_score[1][i]);	//left
		}
		for( int i=0; i<256; i++ ) {
			col_score[0][i] = col_score[1][i] = 0.0f;
			col[0][i] = colMove(0,i,col_score[0][i]);	//up
			col[1][i] = colMove(1,i,col_score[1][i]);	//down
		}
	}
	int right( int index, double& dScore )
	{
		dScore += row_score[0][index];
		return row[0][index];
	}
	int left( int index, double& dScore )
	{
		dScore += row_score[1][index];
		return row[1][index];
	}
	int up( int index, double& dScore )
	{
		dScore += col_score[0][index];
		return col[0][index];
	}
	int down( int index, double& dScore )
	{
		dScore += col_score[1][index];
		return col[1][index];
	}
	int rowMove( int direction, int index, double& dScore )
	{
		// row
		// 2 1 0
		int row[3] = { ((index&0xff0)>>8), ((index&0xf0)>>4), (index&0xf) };
		int newRow[3];
		int newIndex = 0;
		
		if( direction==0 ) {
			// right
			for( int i=0; i<3; i++ ) { newRow[i] = 0; }
			for( int i=2, j=2; i>=0; i-- ) { if( row[i]!=0 ) { newRow[j--] = row[i]; } }
			for( int i=2; i>0; i-- ) { if( merge2(newRow,i,i-1,dScore) ) { break; } }
			for( int i=0; i<3; i++ ) { row[i] = 0; }
			for( int i=2, j=2; i>=0; i-- ) { if( newRow[i]!=0 ) { row[j--] = newRow[i]; } }
		} else {
			// left
			for( int i=0; i<3; i++ ) { newRow[i] = 0; }
			for( int i=0, j=0; i<3; i++ ) { if( row[i]!=0 ) { newRow[j++] = row[i]; } }
			for( int i=0; i<2; i++ ) { if( merge2(newRow,i,i+1,dScore) ) { break; } }
			for( int i=0; i<3; i++ ) { row[i] = 0; }
			for( int i=0, j=0; i<3; i++ ) { if( newRow[i]!=0 ) { row[j++] = newRow[i]; } }
		}
		
		for( int i=0; i<3; i++ ) {
			newIndex = newIndex << 4;
			newIndex = newIndex | row[i];
		}

		return newIndex;
	}
	int colMove( int direction, int index, double& dScore )
	{
		//col
		// 1
		// 0
		int col[2] = { ((index&0xf0)>>4), (index&0xf) };
		int newCol[2];
		int newIndex = 0;

		if( direction==0 ) {
			// up
			for( int i=0; i<2; i++ ) { newCol[i] = 0; }
			for( int i=0, j=0; i<2; i++ ) { if( col[i]!=0 ) { newCol[j++] = col[i]; } }
			for( int i=0; i<1; i++ ) { if( merge2(newCol,i,i+1,dScore) ) { break; } }
			for( int i=0; i<2; i++ ) { col[i] = 0; }
			for( int i=0, j=0; i<2; i++ ) { if( newCol[i]!=0 ) { col[j++] = newCol[i]; } }
		} else {
			// down
			for( int i=0; i<2; i++ ) { newCol[i] = 0; }
			for( int i=1, j=1; i>=0; i-- ) { if( col[i]!=0 ) { newCol[j--] = col[i]; } }
			for( int i=1; i>0; i-- ) { if( merge2(newCol,i,i-1,dScore) ) { break; } }
			for( int i=0; i<2; i++ ) { col[i] = 0; }
			for( int i=1, j=1; i>=0; i-- ) { if( newCol[i]!=0 ) { col[j--] = newCol[i]; } }
		}
		
		for( int i=0; i<2; i++ ) {
			newIndex = newIndex << 4;
			newIndex = newIndex | col[i];
		}
		
		return newIndex;
	}
	bool merge2( int* row, int from, int to, double& dScore )
	{
		if( row[from]!=0 && row[to]!=0 && row[from]!=13 && row[to]!=13 ) {
			if( (row[from]==1 && row[to]==1) || abs(row[from]-row[to])==1 ) {
				row[to] = (row[from]>row[to])?(row[from]+1):(row[to]+1);
				row[from] = 0;
				dScore += fibonacci[row[to]];
				return true;
			}
		}

		return false;
	}
};

#endif
