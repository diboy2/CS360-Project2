/*
NAME: 
STUDENT ID: 
*/

#include <cstdlib>
#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include "Timer.h"

using namespace std;

class Search{

	//directions range from 1 2 3 4
	// 1 = left
	// 2 = up
	// 3 = down
	// 4 = right
	public: 
		int currentX;
		int currentY;
		vector<int>directions;
		Search(){
			currentX=0;
			currentY=0;
		}	      	  	
	
		void setPosition(int x, int y){
			currentX = x;
			currentY = y;
		}
		
		void pushVector(int direction){
			directions.push_back(direction);
		}

};

class Cell{
		
		public:
			int value;
			int row;
			int column;
			bool reachable;
			bool reaching;
			bool hasfb;
			bool hasff;
			int reachingCost;
			int totalCost;
			Cell(int i, int j, int v){
				row = i;
				column = j;
				value = v;
				reachable = false;
				reaching = false;
				reachingCost = 0;
				totalCost = 0;
				hasfb = false;
				hasff = true;
			}
};

vector< vector<Cell> > grid;


void GenerateResults(bool doesSolutionExists, 
	bool isSolutionUnique,
	int solutionLength,
	int numBlackHoles,
	int numWhiteHoles,
	int numffmoves,
	int numfbmoves,
	int puzzleScore) {

	//Solution

	cout << "Solution: Did Not Attempt ";
	cout<<endl;

	//Unique

	cout << "Unique: Did Not Attempt ";
	cout <<endl;

	//Solution Length

	cout << "Solution length: Did Not Attempt" << endl;

	//# of black holes

	cout << "# of black holes: " << numBlackHoles << endl;

	//# of white holes

	cout << "# of white holes: " << numWhiteHoles << endl;

	//# of forced forward moves

	cout << "# of forced forward moves: " << numffmoves << endl;

	//# of forced backward moves

	cout << "# of forced backward moves: " << numfbmoves << endl;

	//Puzzle score

	cout << "Puzzle score: " << puzzleScore << endl;

}



void EvaluateReachableCells(int rows, int columns){
	vector<Cell> nodes;
	grid[0][0].reachable = true;
	nodes.push_back(grid[0][0]);
	int i = 0;
	
	while(!nodes.empty()){	
		int parentRow = nodes[0].row;
		int parentColumn = nodes[0].column;
		int parentValue = nodes[0].value;
		//Evaluate right direction
		bool childReachable;
		int position = parentValue + parentRow;
		
		if(position<rows){
			childReachable = grid[position][parentColumn].reachable;
			if(!childReachable){
				grid[position][parentColumn].reachable = true;
				nodes.push_back(grid[position][parentColumn]);
			}
		}
		
		position = parentValue + parentColumn;
		
		if(position<columns){
			childReachable = grid[parentRow][position].reachable;
			if(!childReachable){
				grid[parentRow][position].reachable = true;
				nodes.push_back(grid[parentRow][position]);
			}
		}
		
		position =  parentRow-parentValue ;
		
		if(position>0){
			childReachable = grid[position][parentColumn].reachable;
			if(!childReachable){
				grid[position][parentColumn].reachable = true;
				nodes.push_back(grid[position][parentColumn]);
			}
		}

		position = parentColumn-parentValue;
		
		if(position>=0){
			childReachable = grid[parentRow][position].reachable;
			if(!childReachable){
				grid[parentRow][position].reachable = true;
				nodes.push_back(grid[parentRow][position]);
			}
		}
		
		
		nodes.erase(nodes.begin());
		
	}
	

}


void EvaluateReachingCells(int rows, int columns){
	vector<Cell> nodes;

	int targetRow = rows-1;
	int targetColumn = columns-1;
	int reachingCost = 1;
	//Evaluate last row
	for(int i = 0; i<columns; i++){
		if(targetColumn == (i + grid[targetRow][i].value)){
			grid[targetRow][i].reaching = true;
			grid[targetRow][i].reachingCost = reachingCost;
			nodes.push_back(grid[targetRow][i]);
		}
	}
	//Evaluate last column
	for(int i = 0; i<rows; i++){
		if(targetRow == (i + grid[i][targetColumn].value)){
			grid[i][targetColumn].reaching = true;
			grid[i][targetColumn].reachingCost = reachingCost;
			nodes.push_back(grid[i][targetColumn]);
		}
	}

	//Evaluate all nodes
	while(!nodes.empty()){	
		targetRow = nodes[0].row;
		targetColumn = nodes[0].column;
		//Evaluate nodes from the left 

		for(int i = 0; i<targetColumn; i++){
			if(targetColumn == (i + grid[targetRow][i].value)){
				if(!grid[targetRow][i].reaching){
					grid[targetRow][i].reaching = true;
					grid[targetRow][i].reachingCost = grid[targetRow][targetColumn].reachingCost +1;
					nodes.push_back(grid[targetRow][i]);
				}
			}
		}

		//Evaluate nodes from the right
		for(int i = targetColumn+1; i<columns; i++){
			if(targetColumn == (i - grid[targetRow][i].value)){
				if(!grid[targetRow][i].reaching){
					grid[targetRow][i].reaching = true;
					grid[targetRow][i].reachingCost = grid[targetRow][targetColumn].reachingCost +1;
					nodes.push_back(grid[targetRow][i]);
				}
			}
		}

		//Evaluate nodes from up
		for(int i = 0; i<targetRow; i++){
			if(targetRow == (i + grid[i][targetColumn].value)){
				if(!grid[i][targetColumn].reaching){ 
					grid[i][targetColumn].reaching = true;
					grid[i][targetColumn].reachingCost = grid[targetRow][targetColumn].reachingCost +1;
					nodes.push_back(grid[i][targetColumn]);
				}
			}
		}

		//Evaluate nodes from down
		for(int i = targetRow+1; i<rows; i++){
			if(targetRow == (i - grid[i][targetColumn].value)){
				if(!grid[i][targetColumn].reaching){ 
					grid[i][targetColumn].reaching = true;
					grid[i][targetColumn].reachingCost = grid[targetRow][targetColumn].reachingCost +1;
					nodes.push_back(grid[i][targetColumn]);
				}
			}
		}

		nodes.erase(nodes.begin());

	}

}

void ChangeParentsToReaching(int nodeValue, int nodeRow, int nodeColumn, int rows, int columns){
	vector <Cell> nodes;

	nodes.push_back(grid[nodeRow][nodeColumn]);
	int targetRow;
	int targetColumn;
	while(!nodes.empty()){	
		targetRow = nodes[0].row;
		targetColumn = nodes[0].column;
		//Evaluate nodes from the left 

		for(int i = 0; i<targetColumn; i++){
			if(targetColumn == (i + grid[targetRow][i].value)){
				if(!grid[targetRow][i].reaching){
					grid[targetRow][i].reaching = true;
					grid[targetRow][i].reachingCost = grid[targetRow][targetColumn].reachingCost +1;
					nodes.push_back(grid[targetRow][i]);
				}
			}
		}

		//Evaluate nodes from the right
		for(int i = targetColumn+1; i<columns; i++){
			if(targetColumn == (i - grid[targetRow][i].value)){
				if(!grid[targetRow][i].reaching){
					grid[targetRow][i].reaching = true;
					grid[targetRow][i].reachingCost = grid[targetRow][targetColumn].reachingCost +1;
					nodes.push_back(grid[targetRow][i]);
				}
			}
		}

		//Evaluate nodes from up
		for(int i = 0; i<targetRow; i++){
			if(targetRow == (i + grid[i][targetColumn].value)){
				if(!grid[i][targetColumn].reaching){ 
					grid[i][targetColumn].reaching = true;
					grid[i][targetColumn].reachingCost = grid[targetRow][targetColumn].reachingCost +1;
					nodes.push_back(grid[i][targetColumn]);
				}
			}
		}

		//Evaluate nodes from down
		for(int i = targetRow+1; i<rows; i++){
			if(targetRow == (i - grid[i][targetColumn].value)){
				if(!grid[i][targetColumn].reaching){ 
					grid[i][targetColumn].reaching = true;
					grid[i][targetColumn].reachingCost = grid[targetRow][targetColumn].reachingCost +1;
					nodes.push_back(grid[i][targetColumn]);
				}
			}
		}

		nodes.erase(nodes.begin());

	}
}

void EvaluateUnreachableUnreachingCells(int rows, int columns){
	vector <Cell> nodes;
	for(int i = 0; i<rows;i++){
		for(int j = 0; j<columns;j++){
			if(!grid[i][j].reachable && !grid[i][j].reaching){
				nodes.push_back(grid[i][j]);
			}
		}
	}
	int nodeRow = 0;
	int nodeColumn = 0;
	while(!nodes.empty()){
		nodeRow = nodes.at(0).row;
		nodeColumn = nodes.at(0).column;
		
		//Evaluate nodes from the left
		bool foundNodeReaching = false;
		for(int i = 0; i<nodeColumn && !foundNodeReaching ; i++){				
				if(grid[nodeRow][i].reaching){
					grid[nodeRow][nodeColumn].value=nodeColumn-i;
					grid[nodeRow][nodeColumn].reaching = true;
					grid[nodeRow][nodeColumn].reachingCost = grid[nodeRow][i].reachingCost + 1;
					ChangeParentsToReaching(grid[nodeRow][nodeColumn].value, grid[nodeRow][nodeColumn].row, grid[nodeRow][nodeColumn].column, rows, columns);
					foundNodeReaching = true;
				}	
		}
				//Evaluate nodes from the right
		for(int i = nodeColumn+1; i<columns && !foundNodeReaching ; i++){
				if(grid[nodeRow][i].reaching){
					grid[nodeRow][nodeColumn].value=i-nodeColumn;
					grid[nodeRow][nodeColumn].reaching = true;
					grid[nodeRow][nodeColumn].reachingCost = grid[nodeRow][i].reachingCost + 1;
					ChangeParentsToReaching(grid[nodeRow][nodeColumn].value, grid[nodeRow][nodeColumn].row, grid[nodeRow][nodeColumn].column, rows, columns);
					foundNodeReaching = true;
				}
		}

		//Evaluate nodes from up
		for(int i = 0; i<nodeRow && !foundNodeReaching ; i++){
			if(grid[i][nodeColumn].reaching ){ 
					grid[nodeRow][nodeColumn].value=nodeRow-i;
					grid[nodeRow][nodeColumn].reaching = true;
					grid[nodeRow][nodeColumn].reachingCost = grid[i][nodeColumn].reachingCost + 1;
					ChangeParentsToReaching(grid[nodeRow][nodeColumn].value, grid[nodeRow][nodeColumn].row, grid[nodeRow][nodeColumn].column, rows, columns);
					foundNodeReaching = true;
			}
		}

		//Evaluate nodes from down
		for(int i = nodeRow+1; i<rows && !foundNodeReaching ; i++){
			if(grid[i][nodeColumn].reaching ){ 
				grid[nodeRow][nodeColumn].value=i-nodeRow;
				grid[nodeRow][nodeColumn].reaching = true;
				grid[nodeRow][nodeColumn].reachingCost = grid[i][nodeColumn].reachingCost + 1;
				ChangeParentsToReaching(grid[nodeRow][nodeColumn].value, grid[nodeRow][nodeColumn].row, grid[nodeRow][nodeColumn].column, rows, columns);
				foundNodeReaching = true;
			}
		}
		
		nodes.erase(nodes.begin());
	}
}

bool EvaluateChildren( int previousValue, int nodeRow, int nodeColumn,int rows, int columns){
	vector<Cell> nodes;

						//up child
	int position = nodeRow - previousValue;
	if(position>=0){
		nodes.push_back(grid[position][nodeColumn]);
	}
						//bottom child
	position = nodeRow + previousValue;
	if(position<rows){
		nodes.push_back(grid[position][nodeColumn]);
	}
						//left child
	position = nodeColumn - previousValue;
	if(position>=0){
		nodes.push_back(grid[nodeRow][position]);
	}
						//right child
	position = nodeColumn + previousValue;
	if(position<columns){
		nodes.push_back(grid[nodeRow][position]);
	}
	bool childrenStillReachable = true;
	int targetRow=0;
	int targetColumn;
	while(!nodes.empty()){	
		targetRow = nodes[0].row;
		targetColumn = nodes[0].column;
		//Evaluate nodes from the left 
		bool nodeReachable = false;
		for(int i = 0; i<targetColumn; i++){
			if(targetColumn == (i + grid[targetRow][i].value) && i!=nodeColumn){
				nodeReachable = true;
			}
		}

		//Evaluate nodes from the right
		for(int i = targetColumn+1; i<columns; i++){
			if(targetColumn == (i - grid[targetRow][i].value) && i!=nodeColumn){
				nodeReachable = true;
			}
		}

		//Evaluate nodes from up
		for(int i = 0; i<targetRow; i++){
			if(targetRow == (i + grid[i][targetColumn].value) && i!=nodeRow){
				nodeReachable = true;
			}
		}

		//Evaluate nodes from down
		for(int i = targetRow+1; i<rows; i++){
			if(targetRow == (i - grid[i][targetColumn].value) && i!=nodeRow){
				nodeReachable = true;
			}
		}
		if(!nodeReachable){
			childrenStillReachable = false;
		}

		nodes.erase(nodes.begin());

	}
	
	return childrenStillReachable;

}

void NewChildrenReachable(int futureValue,int nodeRow, int nodeColumn, int rows , int columns){
	int position = nodeRow - futureValue;
		             //up child
	if(position>=0){
		grid[position][nodeColumn].reachable = true;
	}
						//bottom child
	position = nodeRow + futureValue;
	if(position<rows){
		grid[position][nodeColumn].reachable = true;
	}
						//left child
	position = nodeColumn - futureValue;
	if(position>=0){
		grid[nodeRow][position].reachable = true;
	}
						//right child
	position = nodeColumn + futureValue;
	if(position<columns){
		grid[nodeRow][position].reachable = true;
	}
}
void EvaluateBlackHoles(int rows, int columns){
	vector <Cell> nodes;
	for(int i = 0; i<rows;i++){
		for(int j = 0; j<columns;j++){
			if(grid[i][j].reachable && !grid[i][j].reaching){
				nodes.push_back(grid[i][j]);
			}
		}
	}	
	int nodeRow = 0;
	int nodeColumn = 0;
	while(!nodes.empty()){
		nodeRow = nodes.at(0).row;
		nodeColumn = nodes.at(0).column;
		//Evaluate nodes from the left
		bool changedNodeReaching=false;
		for(int i = 0; i<nodeColumn && !changedNodeReaching ; i++){				
				if(grid[nodeRow][i].reaching){
					int futureValue = nodeColumn-i;
					int previousValue = grid[nodeRow][nodeColumn].value;
					if(EvaluateChildren(previousValue, nodeRow, nodeColumn,rows, columns)){
						NewChildrenReachable(futureValue, nodeRow, nodeColumn, rows, columns);
						grid[nodeRow][nodeColumn].value=nodeColumn-i;
						grid[nodeRow][nodeColumn].reaching = true;
						grid[nodeRow][nodeColumn].reachingCost = grid[nodeRow][i].reachingCost + 1;
						ChangeParentsToReaching(grid[nodeRow][nodeColumn].value, grid[nodeRow][nodeColumn].row, grid[nodeRow][nodeColumn].column, rows, columns);
						changedNodeReaching=true;
					}
					
				}	
		}
		
		//Evaluate nodes from the right
		for(int i = nodeColumn+1; i<columns && !changedNodeReaching  ; i++){
				if(grid[nodeRow][i].reaching){
					int futureValue = i-nodeColumn;
					int previousValue = grid[nodeRow][nodeColumn].value;
					if(EvaluateChildren(previousValue, nodeRow, nodeColumn,rows, columns)){
						NewChildrenReachable(futureValue, nodeRow, nodeColumn, rows, columns);
						grid[nodeRow][nodeColumn].value=i-nodeColumn;
						grid[nodeRow][nodeColumn].reaching = true;
						grid[nodeRow][nodeColumn].reachingCost = grid[nodeRow][i].reachingCost + 1;
						ChangeParentsToReaching(grid[nodeRow][nodeColumn].value, grid[nodeRow][nodeColumn].row, grid[nodeRow][nodeColumn].column, rows, columns);
						changedNodeReaching=true;
					}
					
				}
		}
		
		//Evaluate nodes from up
		for(int i = 0; i<nodeRow && !changedNodeReaching  ; i++){
			if(grid[i][nodeColumn].reaching ){ 
				int futureValue = nodeRow-i;
				int previousValue = grid[nodeRow][nodeColumn].value;
				if(EvaluateChildren(previousValue, nodeRow, nodeColumn,rows, columns)){
					NewChildrenReachable(futureValue, nodeRow, nodeColumn, rows, columns);
					grid[nodeRow][nodeColumn].value=nodeRow-i;
					grid[nodeRow][nodeColumn].reaching = true;
					grid[nodeRow][nodeColumn].reachingCost = grid[i][nodeColumn].reachingCost;
					ChangeParentsToReaching(grid[nodeRow][nodeColumn].value, grid[nodeRow][nodeColumn].row, grid[nodeRow][nodeColumn].column, rows, columns); 
					changedNodeReaching=true;
				}
			}
		}

		//Evaluate nodes from down
		for(int i = nodeRow+1; i<rows && !changedNodeReaching; i++){
			if(grid[i][nodeColumn].reaching ){ 
				int futureValue = i-nodeRow;
				int previousValue = grid[nodeRow][nodeColumn].value;
				if(EvaluateChildren(previousValue, nodeRow, nodeColumn,rows, columns)){
					NewChildrenReachable(futureValue, nodeRow, nodeColumn, rows, columns);
					grid[nodeRow][nodeColumn].value=i-nodeRow;
					grid[nodeRow][nodeColumn].reaching = true;
					grid[nodeRow][nodeColumn].reachingCost = grid[i][nodeColumn].reachingCost;
					ChangeParentsToReaching(grid[nodeRow][nodeColumn].value, grid[nodeRow][nodeColumn].row, grid[nodeRow][nodeColumn].column, rows, columns); 
					changedNodeReaching=true;
				}

			}
		}

		nodes.erase(nodes.begin());
	}

}

void EvaluateWhiteHoles(int rows, int columns){
	vector <Cell> nodes;
	for(int i = 0; i<rows;i++){
		for(int j = 0; j<columns;j++){
			if(!grid[i][j].reachable && grid[i][j].reaching){
				nodes.push_back(grid[i][j]);
			}
		}
	}

	int nodeRow = 0;
	int nodeColumn = 0;
	int nodeValue;
	while(!nodes.empty()){
		nodeRow = nodes.at(0).row;
		nodeColumn = nodes.at(0).column;
		nodeValue = nodes.at(0).value;

		bool changedNodeReachable=false;
		//Evaluate nodes from the left
		for(int i = 0; i<nodeColumn && !changedNodeReachable ; i++){				
				if(grid[nodeRow][i].reachable){
					int futureValue = nodeColumn-i;
					int previousValue = grid[nodeRow][i].value;
					if(EvaluateChildren(previousValue, nodeRow, i,rows, columns)){
						NewChildrenReachable(futureValue, nodeRow, i, rows, columns);
						grid[nodeRow][i].value=futureValue;
						grid[nodeRow][i].reaching = true;
						grid[nodeRow][nodeColumn].reachable = true; //redundant already taken care off in New child
						ChangeParentsToReaching(grid[nodeRow][i].value, grid[nodeRow][i].row, grid[nodeRow][i].column, rows, columns);
						changedNodeReachable=true;
					}
					
				}	
		}

		//Evaluate nodes from the right
		for(int i = nodeColumn+1; i<columns && !changedNodeReachable  ; i++){
				if(grid[nodeRow][i].reachable){
					int futureValue = i-nodeColumn;
					int previousValue = grid[nodeRow][i].value;
					if(EvaluateChildren(previousValue, nodeRow, i,rows, columns)){
						NewChildrenReachable(futureValue, nodeRow, i, rows, columns);
						grid[nodeRow][i].value = futureValue;
						grid[nodeRow][i].reaching = true;
						grid[nodeRow][nodeColumn].reachable= true;
						ChangeParentsToReaching(grid[nodeRow][i].value, grid[nodeRow][i].row, grid[nodeRow][i].column, rows, columns);
						changedNodeReachable=true;
					}
					
				}
		}
		
		//Evaluate nodes from up
		for(int i = 0; i<nodeRow && !changedNodeReachable  ; i++){
			if(grid[i][nodeColumn].reachable ){ 
				int futureValue = nodeRow-i;
				int previousValue = grid[i][nodeColumn].value;
				if(EvaluateChildren(previousValue, i, nodeColumn,rows, columns)){
					NewChildrenReachable(futureValue, i, nodeColumn, rows, columns);
					grid[i][nodeColumn].value = futureValue;
					grid[i][nodeColumn].reaching = true;
					grid[nodeRow][nodeColumn].reachable = true;
					ChangeParentsToReaching(grid[i][nodeColumn].value, grid[i][nodeColumn].row, grid[i][nodeColumn].column, rows, columns);
					changedNodeReachable=true;
				}
			}
		}

		//Evaluate nodes from down
		for(int i = nodeRow+1; i<rows && !changedNodeReachable; i++){
			if(grid[i][nodeColumn].reachable){ 
				int futureValue = i-nodeRow;
				int previousValue = grid[i][nodeColumn].value;
				if(EvaluateChildren(previousValue, i, nodeColumn,rows, columns)){
					NewChildrenReachable(futureValue, i, nodeColumn, rows, columns);
					grid[i][nodeColumn].value = futureValue;
					grid[i][nodeColumn].reaching = true;
					grid[nodeRow][nodeColumn].reachable = true;
					ChangeParentsToReaching(grid[i][nodeColumn].value, grid[i][nodeColumn].row, grid[i][nodeColumn].column, rows, columns);
					changedNodeReachable = true;
				}

			}
		}
		
		nodes.erase(nodes.begin());
	}	

}

void addChildren(int nodeValue, int nodeRow, int nodeColumn, int rows, int columns, vector<Cell> &nodesToConsider ){

	int position = nodeRow - nodeValue;
		             //up child
	if(position>=0){
		nodesToConsider.push_back(grid[position][nodeColumn]);
	}
						//bottom child
	position = nodeRow + nodeValue;
	if(position<rows){
		nodesToConsider.push_back(grid[position][nodeColumn]);
	}
						//left child
	position = nodeColumn - nodeValue;
	if(position>=0){
		nodesToConsider.push_back(grid[nodeRow][position]);
	}
						//right child
	position = nodeColumn + nodeValue;
	if(position<columns){
		nodesToConsider.push_back(grid[nodeRow][position]);
	}

}

bool HasForcedBackward(int nodeRow, int nodeColumn, int rows, int columns){
		int targetRow = nodeRow;
		int targetColumn = nodeColumn;
		//Evaluate nodes from the left 
		int numThatReachTarget = 0;
		for(int i = 0; i<targetColumn; i++){
			if(targetColumn == (i + grid[targetRow][i].value)){
				numThatReachTarget++;
			}
		}

		//Evaluate nodes from the right
		for(int i = targetColumn+1; i<columns; i++){
			if(targetColumn == (i - grid[targetRow][i].value)){
				numThatReachTarget++;
			}
		}

		//Evaluate nodes from up
		for(int i = 0; i<targetRow; i++){
			if(targetRow == (i + grid[i][targetColumn].value)){
				numThatReachTarget++;
			}
		}

		//Evaluate nodes from down
		for(int i = targetRow+1; i<rows; i++){
			if(targetRow == (i - grid[i][targetColumn].value)){
				numThatReachTarget++;
			}
		}

		if(numThatReachTarget==1){
			return true;
		}
		else{
			return false;
		}

}

void EvaluateForcedBackward(int rows, int columns){
	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			if(grid[i][j].reaching){
				grid[i][j].hasfb = HasForcedBackward(i,j,rows,columns);
			}
		}
	}
}

bool HasForcedForward(int nodeValue, int nodeRow, int nodeColumn, int rows, int columns){
	
		             //up child
	int options = 0;
	int position = nodeRow - nodeValue;
	if(position>=0){
		options++;
	}
						//bottom child
	position = nodeRow + nodeValue;
	if(position<rows){
		options++;
	}
						//left child
	position = nodeColumn - nodeValue;
	if(position>=0){
		options++;
	}
						//right child
	position = nodeColumn + nodeValue;
	if(position<columns){
		options++;
	}
	if(options==1){
		return true;
	}
	else{
		return false;
	}
}

void EvaluateForcedForward(int rows, int columns){
	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			if(grid[i][j].reachable){
				grid[i][j].hasff= HasForcedForward(grid[i][j].value,i,j,rows,columns);
			}
		}
	}
}

void FindSingleParentNode( int nodeRow, int nodeColumn, int rows, int columns,int & singleRow, int & singleColumn){
	int targetRow = nodeRow;
	int targetColumn = nodeColumn;
	for(int i = 0; i<targetColumn; i++){
			if(targetColumn == (i + grid[targetRow][i].value)){
				singleRow = targetRow;
				singleColumn = i;
			}
		}

		//Evaluate nodes from the right
		for(int i = targetColumn+1; i<columns; i++){
			if(targetColumn == (i - grid[targetRow][i].value)){
				singleRow = targetRow;
				singleColumn = i;
			}
		}

		//Evaluate nodes from up
		for(int i = 0; i<targetRow; i++){
			if(targetRow == (i + grid[i][targetColumn].value)){
				singleRow = i;
				singleColumn = targetColumn;
			}
		}

		//Evaluate nodes from down
		for(int i = targetRow+1; i<rows; i++){
			if(targetRow == (i - grid[i][targetColumn].value)){
				singleRow = i;
				singleColumn = targetColumn;
			}
		}
}

bool hasThreeParents(int nodeRow, int nodeColumn, int rows, int columns){
	int targetRow = nodeRow;
	int targetColumn = nodeColumn;
		//Evaluate nodes from the left 
	int numThatReachTarget = 0;
	for(int i = 0; i<targetColumn; i++){
		if(targetColumn == (i + grid[targetRow][i].value)){
			numThatReachTarget++;
		}
	}

		//Evaluate nodes from the right
	for(int i = targetColumn+1; i<columns; i++){
		if(targetColumn == (i - grid[targetRow][i].value)){
			numThatReachTarget++;
		}
	}

		//Evaluate nodes from up
	for(int i = 0; i<targetRow; i++){
		if(targetRow == (i + grid[i][targetColumn].value)){
			numThatReachTarget++;
		}
	}

		//Evaluate nodes from down
	for(int i = targetRow+1; i<rows; i++){
		if(targetRow == (i - grid[i][targetColumn].value)){
			numThatReachTarget++;
		}
	}

	if(numThatReachTarget>2){
		return true;
	}
	else{
		return false;
	}
}

bool childrenOkay(int nodeRow, int nodeColumn, int nodeValue, int rows, int columns){
	vector<Cell> nodes;

						//up child
	int position = nodeRow - nodeValue;
	if(position>=0){
		nodes.push_back(grid[position][nodeColumn]);
	}
						//bottom child
	position = nodeRow + nodeValue;
	if(position<rows){
		nodes.push_back(grid[position][nodeColumn]);
	}
						//left child
	position = nodeColumn - nodeValue;
	if(position>=0){
		nodes.push_back(grid[nodeRow][position]);
	}
						//right child
	position = nodeColumn + nodeValue;
	if(position<columns){
		nodes.push_back(grid[nodeRow][position]);
	}

	bool childrenOkay = true;

	while(!nodes.empty()){
		int nodeRow = nodes.at(0).row;
		int nodeColumn = nodes.at(0).column;

		if(!hasThreeParents(nodeRow, nodeColumn, rows, columns)){
			childrenOkay = false;
		}

		nodes.erase(nodes.begin());
	}
	return childrenOkay;
}



void TryRemoveForcedBackward(int rows, int columns){
	vector<Cell> nodes;

	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			if(grid[i][j].hasfb){
				nodes.push_back(grid[i][j]);
			}
		}
	}

	while(!nodes.empty()){

		int targetRow = nodes[0].row;
		int targetColumn = nodes[0].column;

		// the only node that reaches the target;
		int singleNodeRow = targetRow;
		int singleNodeColumn = targetColumn;

		FindSingleParentNode(targetRow, targetColumn, rows, columns, singleNodeRow, singleNodeColumn);
		//Evaluate nodes from the left 
		
		bool foundNewParent = false;
		for(int i = 0; i<targetColumn && !foundNewParent ; i++){
			int nodeValue = grid[targetRow][i].value;
			if(!(singleNodeRow == targetRow && singleNodeColumn == i)){
				if(childrenOkay(targetRow, i, nodeValue, rows, columns)){
					int futureValue = targetColumn - i;
					if(!HasForcedForward(futureValue,targetRow, i, rows,columns)){
						grid[targetRow][i].value = futureValue;
						grid[targetRow][targetColumn].hasfb = false;
						foundNewParent = true;
					}
				}
			}
			
		}

		//Evaluate nodes from the right
		for(int i = targetColumn+1; i<columns && !foundNewParent; i++){
			int nodeValue = grid[targetRow][i].value;
			if(!(singleNodeRow == targetRow && singleNodeColumn == i)){
				if(childrenOkay(targetRow, i, nodeValue, rows, columns)){
					int futureValue = i-targetColumn;
					if(!HasForcedForward(futureValue,targetRow, i, rows,columns)){
						grid[targetRow][i].value = futureValue;
						grid[targetRow][targetColumn].hasfb = false;
						foundNewParent = true;
					}
				}
			}
		}

		//Evaluate nodes from up
		for(int i = 0; i<targetRow && !foundNewParent; i++){
			int nodeValue = grid[i][targetColumn].value;
			if(!(singleNodeRow == i && singleNodeColumn == targetColumn)){
				if(childrenOkay(i, targetColumn, nodeValue, rows, columns)){
					int futureValue = targetRow-i;
					if(!HasForcedForward(futureValue, i, targetColumn, rows,columns)){
						grid[i][targetColumn].value = futureValue;
						grid[targetRow][targetColumn].hasfb = false;
						foundNewParent = true;
					}
				}
			}
		}

		//Evaluate nodes from down
		for(int i = targetRow+1; i<rows && !foundNewParent; i++){
			int nodeValue = grid[i][targetColumn].value;
			if(!(singleNodeRow == i && singleNodeColumn == targetColumn)){
				if(childrenOkay(i, targetColumn, nodeValue, rows, columns)){
					int futureValue = i - targetRow;
					if(!HasForcedForward(futureValue, i, targetColumn, rows,columns)){
						grid[i][targetColumn].value = futureValue;
						grid[targetRow][targetColumn].hasfb = false;
						foundNewParent = true;
					}
				}
			}
		}

		nodes.erase(nodes.begin());
	}
}
void FindSingleChildNode(int nodeValue, int nodeRow, int nodeColumn, int rows, int columns,int & singleRow, int & singleColumn){
	int position = nodeRow - nodeValue;
		             //up child
	if(position>=0){
		singleRow=position;
		singleColumn=nodeColumn;
	}
						//bottom child
	position = nodeRow + nodeValue;
	if(position<rows){
		singleRow = position;
		singleColumn = nodeColumn;
	}
						//left child
	position = nodeColumn - nodeValue;
	if(position>=0){
		singleRow = nodeRow;
		singleColumn = position;
	}
						//right child
	position = nodeColumn + nodeValue;
	if(position<columns){
		singleRow = nodeRow;
		singleColumn = position;
	}
}
bool hasTwoOptions(int nodeValue, int nodeRow, int nodeColumn, int rows, int columns){
	
		             //up child
	int options = 0;
	int position = nodeRow - nodeValue;
	if(position>=0){
		options++;
	}
						//bottom child
	position = nodeRow + nodeValue;
	if(position<rows){
		options++;
	}
						//left child
	position = nodeColumn - nodeValue;
	if(position>=0){
		options++;
	}
						//right child
	position = nodeColumn + nodeValue;
	if(position<columns){
		options++;
	}
	if(options>1){
		return true;
	}
	else{
		return false;
	}
}


void TryRemoveForcedForward(int minVal, int maxVal,int rows, int columns){
	vector <Cell> nodes;

	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			if(grid[i][j].hasff){
				nodes.push_back(grid[i][j]);
			}
		}
	}

	while(!nodes.empty()){

		int targetRow = nodes[0].row;
		int targetColumn = nodes[0].column;
		int targetValue = nodes[0].value;

		// the only node that reaches the target;
		int singleNodeRow = targetRow;
		int singleNodeColumn = targetColumn;

		FindSingleChildNode(targetValue, targetRow, targetColumn, rows, columns, singleNodeRow, singleNodeColumn);

		if(hasThreeParents(singleNodeRow, singleNodeColumn, rows, columns)){
			bool valueFound = false;
			for(int i = minVal; i<=maxVal && !valueFound; i++)
			{
				if(hasTwoOptions(i,targetRow,targetColumn, rows, columns)){
					grid[targetRow][targetColumn].value = i;
					grid[targetRow][targetColumn].hasff = false;
					valueFound = true;
				}
			}
		}

		nodes.erase(nodes.begin());
		
	}

}

void GeneratePuzzle(int nRows, int nColumns, int minVal, int maxVal)
{
	int rows;
	int columns;
	int minV;
	int maxV;
	bool doesSolutionExists = true;
	bool isSolutionUnique = true;
	int solutionLength = 0;
	int numBlackHoles = 0;
	int numWhiteHoles = 0;
	int numffmoves = 0;
	int numfbmoves = 0;
	int puzzleScore = 0;


	rows = nRows;
	columns = nColumns;
	minV = minVal;
	maxV = maxVal;
	
	

	for(int i = 0; i < rows; i++){

		grid.push_back(vector<Cell>());
		
		for(int j = 0; j<columns; j++){
			grid.at(i).push_back(Cell(i,j,rand() % (maxVal-minVal+1) + minVal));
		}
	}
	
	// Set Reachable Cells
	EvaluateReachableCells(rows,columns);
	// Set Reaching Cells
	EvaluateReachingCells(rows,columns);

	// Make unreachable, unreaching cells to white holes
	EvaluateUnreachableUnreachingCells(rows,columns);

	// Try removing black holes
	EvaluateBlackHoles(rows,columns);

	 //Try removing white holes
	EvaluateWhiteHoles(rows,columns);


	// Set ForcedBackward
	EvaluateForcedBackward(rows, columns);
	
	// Set ForcedForward;
	EvaluateForcedForward(rows, columns);

	// Remove ForcedBackward
	TryRemoveForcedBackward(rows, columns);

	// Remove ForcedForward
	TryRemoveForcedForward(minVal, maxVal, rows, columns);

	// Check again 
	// Set Reachable Cells
	EvaluateReachableCells(rows,columns);
	// Set Reaching Cells
	EvaluateReachingCells(rows,columns);

	// Set ForcedBackward
	EvaluateForcedBackward(rows, columns);
	
	// Set ForcedForward;
	EvaluateForcedForward(rows, columns);
	
	for(int i = 0; i<rows; i++){

	}

	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			cout << (grid[i][j]).value; 
		}
		cout << endl;
	}

	cout << endl;

	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			cout << (grid[i][j]).reachable; 
		}
		cout << endl;
	}
	cout << endl;

	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			cout << (grid[i][j]).reaching; 
		}
		cout << endl;
	}
	cout << endl;

	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			cout << (grid[i][j]).hasfb; 
		}
		cout << endl;
	}
	cout << endl;

	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			cout << (grid[i][j]).hasff; 
		}
		cout << endl;
	}
	cout << endl;

	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			if(!grid[i][j].reaching && grid[i][j].reachable){
				numBlackHoles++;
			}
			if(grid[i][j].reaching && !grid[i][j].reachable){
				numWhiteHoles++;
			}
			if(grid[i][j].hasfb){
				numfbmoves++;
			}  
			if(grid[i][j].hasff){
				numffmoves++;
			}  
		}
	}
	
	cout << endl;


	GenerateResults(doesSolutionExists, isSolutionUnique,solutionLength,numBlackHoles,numWhiteHoles,numffmoves,numfbmoves,puzzleScore);
	
	
}


int main(int argc, char **argv)
{

	if (argc != 5)
	{
		printf("Please specify the number of rows and columns and the minimum and maximum values for each cell (requires 4 parameters)\n");
		exit(0);
	}
	int nRows = atoi(argv[1]);
	int nColumns = atoi(argv[2]);
	int minVal = atoi(argv[3]);
	int maxVal = atoi(argv[4]);

	// Start the timer
	Timer t;
	t.StartTimer();
  	
	// Generate the puzzle
	printf("Generating a %dx%d puzzle with values in range [%d-%d]\n", nRows, nColumns, minVal, maxVal);
	
	GeneratePuzzle(nRows, nColumns, minVal, maxVal);
		
	// Print the elapsed time
    printf("Total time: %.6lf seconds\n", t.GetElapsedTime());

	return 0;
}
