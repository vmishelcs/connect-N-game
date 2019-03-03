//Author: Vladimir Mishel
//Date: 2/11/2019

/*
Develop an algorithm to implement the Connect N game.
*/

#include <iostream>
#include <iomanip>

using namespace std;

bool InitializeBoard(int** connectNBoard,
                     int numRowsInBoard);

bool DisplayBoard(int** connectNBoard,
                  int numRowsInBoard);

bool MakeMove(int** connectNBoard,
              int numRowsInBoard,
              int playeID,
              int columnChosen);

bool CheckWinner(int** connectNBoard,
                 int numRowsInBoard,
                 int numConnect,
                 int columnChosen,
                 int playerID);

#define MINSIZE 8
#define MAXSIZE 25

int main()
{
	int numRows;			//size of the board (numRows x numRows)
	int numToConnect;		//how many gamepieces one needs to connect to win

	int playerID = 1;		//Red gets the first turn
	int moveTries = 0;		//counter for turns

	//variable for choosing where user wants to place his piece
	int columnChosen;

	//variable for checking if the game board is full
	int fillcount;	

	//variable for permitting a turn
	bool allowturn = true;

	//variable for ending the game
	bool endgame = false;

	//initialize matrix pointer
	int **myConnectNBoard = nullptr;
	
	int i;	//index
	int x;	//index

	//max and min gamepieces to connect to win
	const int MINCONNECTNUM = 4;
	      int MAXCONNECTNUM;

	//prompt user to enter board size
	do
	{
		cout<<"Enter the number of squares along each edge of board"<<endl;
		cin>>numRows;

		//check if user entered a number
		if(cin.fail())
		{
			cerr<<"ERROR: The value provided was not a number"<<endl;
			cin.clear();
			cin.ignore(1024, '\n');
			numRows = 0;
		}

		//check if numRows is in range
		else if( numRows < MINSIZE || numRows > MAXSIZE )
		{
			cerr<<"ERROR: Board size too large (>"<<MAXSIZE<<")\n";
			cerr<<"ERROR: Or board size too small (<"<<MINSIZE<<")"<<endl;
			cin.ignore(1024, '\n');
			numRows = 0;
		}
	} while(numRows == 0);

	//initialize a number for MAXCONNECTNUM after getting numRows
	MAXCONNECTNUM = numRows - MINCONNECTNUM;

	//prompt user to enter number of game pieces to connect
	do
	{
		cout<<"Enter the number of pieces that must form a line to win"<<endl;
		cin>>numToConnect;

		//check if user entered a number
		if(cin.fail())
		{
			cerr<<"ERROR: The value provided was not a number"<<endl;
			cin.clear();
			cin.ignore(1024, '\n');
			numToConnect = 0;
		}

		//check if numToConnect is in range
		else if( numToConnect < MINCONNECTNUM || numToConnect > MAXCONNECTNUM )
		{
			cerr<<"ERROR: Number to connect is too large (>"<<MAXCONNECTNUM<<")\n";
			cerr<<"ERROR: OR Number to connect is too small (<"<<MINCONNECTNUM<<")"<<endl;
			cin.ignore(1024, '\n');
			numToConnect = 0;
		}
	}while(numToConnect == 0);



//DYNAMICALLY ALLOCATE THE GAME BOARD---------------------------------------------------------------------------------------------------------------------------------------------------------------



	try
	{
		//create an array of pointers to the rows
		myConnectNBoard = new int*[numRows];

		//now for every pointer, create an array corresponding to that pointer
		for(i=0; i<numRows; i++)
		{
			myConnectNBoard[i] = new int[numRows];
		}
	}
	//catch any errors
	catch(exception& x)
	{
		cerr<<"ERROR: Could not initialize the game board"<<endl;
		//delete any memory that may have been allocated
		if(myConnectNBoard != nullptr)
		{
			//check all row pointers
			for(i=0; i<numRows; i++)
			{
				if(myConnectNBoard[i] != nullptr)
				{
					delete [] myConnectNBoard[i];
				}
			}
			delete [] myConnectNBoard;
		}
		//get rid of dangling pointer
		myConnectNBoard = nullptr;
		return 1;
	}

	//check if the game board initiallized correctly
	if(!InitializeBoard(myConnectNBoard, numRows))
	{
		cerr<<"ERROR: Could not initialize the game board"<<endl;
		//wipe game board from memory
		for(i=0; i<numRows; i++)
		{
			delete [] myConnectNBoard[i];
		}
		delete [] myConnectNBoard;

		//get rid of dangling pointer
		myConnectNBoard = nullptr;
		return 2;
	}

	//display the game board
	DisplayBoard(myConnectNBoard, numRows);



//GAME LOOP-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


	//keep asking for turns while there isn't a winner
	do
	{
		fillcount = 0;
		//check if the top rows are full
		for(x=0; x<numRows; x++)
		{
			if(myConnectNBoard[0][x] != 0)
			{
				fillcount++;
			}
		}
		//if the game board is full, exit the program
		if(fillcount == numRows)
		{
			cerr<<"ERROR: Game board full!"<<endl;
			cerr<<"Exiting game..."<<endl;

			//wipe game board from memory
			for(i=0; i<numRows; i++)
			{
				delete [] myConnectNBoard[i];
			}
			delete [] myConnectNBoard;

			//get rid of dangling pointer
			myConnectNBoard = nullptr;
			return 3;
		}

		//loop for choosing a turn
		do
		{
			//if player tries to make 3 illegal moves, take away player's turn
			if(moveTries >= 3)
			{
				cout<<"TOO MANY ILLEGAL MOVES"<<endl;
				//switch playerID to allow other player to make a turn
				if(playerID == 1)
				{
					cout<<"Red has forfeited a turn"<<endl;
					playerID = 2;
					allowturn = true;
					DisplayBoard(myConnectNBoard, numRows);
				}
				else if(playerID == 2)
				{
					cout<<"Black has forfeited a turn"<<endl;
					playerID = 1;
					allowturn = true;
					DisplayBoard(myConnectNBoard, numRows);
				}
				moveTries = 0;
				break;
			}

			//prompt the correct player to make a move
			if(playerID == 1 && allowturn == true)
			{
				cout<<"Red moves:"<<endl;
			}
			else if(playerID == 2 && allowturn == true)
			{
				cout<<"Black moves:"<<endl;
			}

			//ask for column
			cout<<"Enter the column number where you want to put your piece"<<endl;
			cin>>columnChosen;

			//if columnChosen is not a number, output an error
			if(cin.fail())
			{
				cerr<<"ERROR: The value provided was not a number"<<endl;
				cin.clear();
				cin.ignore(1024, '\n');
				moveTries++;	//increment tries
				allowturn = false;
			}

			//if columnChosen is not in range, output an error
			else if(columnChosen < 0 || columnChosen > numRows-1)
			{
				cerr<<"ERROR: Column "<<columnChosen<<" is not on the board: try again"<<endl;
				cerr<<"ERROR: Column number should be >= 0 and < "<<numRows<<endl;
				cin.ignore(1024, '\n');
				moveTries++;	//increment tries
				allowturn = false;
			}

			//if columnChosen is already full, output an error
			else if(myConnectNBoard[0][columnChosen] != 0)
			{
				cerr<<"ERROR: Column "<<columnChosen<<" is already completely full try again"<<endl;
				cin.ignore(1024, '\n');
				moveTries++;	//increment tries
				allowturn = false;
			}

			else
			{
				//execute make move function to change the values in the matrix
				MakeMove(myConnectNBoard, numRows, playerID, columnChosen);
				//check if the game has been won
				endgame = CheckWinner(myConnectNBoard, numRows, numToConnect, columnChosen, playerID);

				if(endgame == true)
				{
					break;
				}

				//reset moveTries for the other player
				moveTries = 0;
				//switch playerID to allow other player to make a turn
				if(playerID == 1)
				{
					playerID = 2;
				}
				else if(playerID == 2)
				{
					playerID = 1;
				}
				//exit the turn loop
				allowturn = true;

				if(allowturn == true)
				{		
					//display the game board
					DisplayBoard(myConnectNBoard, numRows);
				}
			}
		}while(allowturn == false);
	}while(endgame == false);

	//output victory messages
	if(playerID == 1)
	{
		cout<<"Red player has won\n";
	}
	else if(playerID == 2)
	{
		cout<<"Black player has won\n";
	}

	//wipe game matrix from memory
	for(i=0; i<numRows; i++)
	{
		delete [] myConnectNBoard[i];
	}
	delete [] myConnectNBoard;

	//get rid of dangling pointer
	myConnectNBoard = nullptr;

	return 0;
}



//FUNCTION DEFINITIONS------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



bool InitializeBoard(int** connectNBoard,
                     int numRowsInBoard)
{
	int j;				//index
	int k;				//index
	int rowcheck = 0;	//variable for checking number of rows

	//check the array of pointers to see if any of them are null and check number of rows
	if(connectNBoard == nullptr)
	{
		cerr<<"ERROR: cannot initialize the board because the pointer to the board is NULL"<<endl;
		return false;
	}
	else
	{
		for(j=0; j<numRowsInBoard; j++)
		{
			if(connectNBoard[j] == nullptr)
			{
				cerr<<"ERROR: cannot initialize the board because the pointer to the board is NULL"<<endl;
				return false;
			}
			rowcheck++;
		}
	}

	if(rowcheck<MINSIZE || rowcheck>MAXSIZE)
	{
		cerr<<"ERROR: illegal number of rows in board"<<endl;
		return false;
	}

	//initialize the entire matrix to 0
	for(j=0; j<numRowsInBoard; j++)
	{
		for(k=0; k<numRowsInBoard; k++)
		{
			connectNBoard[j][k] = 0;
		}
	}
	return true;
}

bool DisplayBoard(int** connectNBoard,
                  int numRowsInBoard)
{
	int x;	//index
	int y;	//index
	int c;	//index

	//print the row of column indeces
	cout<<endl<<endl<<setw(6);
	for(c=0; c<numRowsInBoard; c++)
	{
		cout<<c<<setw(3);
	}
	cout<<endl;

	//for all rows
	for(y=0; y<numRowsInBoard; y++)
	{
		//print row index
		cout<<setw(3)<<y;

		//for all columns
		for(x=0; x<numRowsInBoard; x++)
		{
			//if this spot is taken by RED, print R
			if(connectNBoard[y][x] == 1)
			{
				cout<<setw(3)<<'R';
			}
			//if this spot is taken by BLACK, print B
			else if(connectNBoard[y][x] == 2)
			{
				cout<<setw(3)<<'B';
			}
			//otherwise print o for an empty spot
			else
			{
				cout<<setw(3)<<'o';
			}
		}
		cout<<endl;
	}
	return true;
}

bool MakeMove(int** connectNBoard,
              int numRowsInBoard,
              int playeID,
              int columnChosen)
{
	int y;	//index

	//if the column chosen is not on the board, out illegal move
	if(columnChosen > numRowsInBoard-1 || columnChosen < 0)
	{
		cerr<<"Illegal move"<<endl;
		return false;
	}

	//otherwise check for move validity
	else
	{
		for(y=numRowsInBoard-1; y>=0; y--)
		{
			//if we are at the top of the column and the spot is not empty, output that an illegal move has been made
			if(y==0 && connectNBoard[y][columnChosen]!=0)
			{
				cerr<<"Illegal move"<<endl;
				return false;
			}

			//otherwise, if playerID is 1, make the bottommost spot of columnChosen 1
			//if the playerID is 2, make the bottommost spot of columnChosen 2
			else if(connectNBoard[y][columnChosen] == 0)
			{
				if(playeID == 1)
				{
					cout<<"Red has moved"<<endl;
				}
				else if(playeID == 2)
				{
					cout<<"Black has moved"<<endl;
				}
				connectNBoard[y][columnChosen] = playeID;
				return true;
			}
		}
	}
	return false;
}

bool CheckWinner(int** connectNBoard,
                 int numRowsInBoard,
                 int numConnect,
                 int columnChosen,
                 int playerID)
{
	int y;	//index

	//variable for finding the newest spot that has been filled up
	int newestRow = 0;
	//variable for telling whether to look for Red or Black game pieces
	int lookingFor = 0;

	//counting gamepieces variables
	int beforeCount = 0;
	int afterCount = 0;
	int totalCount = 0;

	//column counter
	int columnCount;
	//row counter
	int rowCount;
	//iteration counter for diagonal counting
	int iterNum;
	int iterAllowed;

	//check if columnChosen is valid
	if(columnChosen > numRowsInBoard-1 || columnChosen < 0)
	{
		cerr<<"ERROR: invalid column chosen, cannot check for winner"<<endl;
		return false;
	}

	//check if the matrix pointer exists
	else if(connectNBoard == nullptr)
	{
		cerr<<"ERROR: cannot print the board because the pointer to the board is NULL"<<endl;
		return false;
	}

	//check every row's pointer
	else
	{
		for(y=0; y<numRowsInBoard; y++)
		{
			if(connectNBoard[y] == nullptr)
			{
				cerr<<"ERROR: cannot print the board because the pointer to the board is NULL"<<endl;
				return false;
			}
		}

		//find in which row the newest gamepiece has been placed
		for(y=0; y<numRowsInBoard; y++)
		{
			if(connectNBoard[y][columnChosen] != 0)
			{
				newestRow = y;
				lookingFor = connectNBoard[y][columnChosen];
				break;
			}
		}

		/*COUNTING HORIZONTALLY*/

		//count the game pieces of the color that we're looking for to the right
		for(columnCount=columnChosen+1; columnCount<numRowsInBoard; columnCount++)
		{
			//if the next piece is not the number we're looking for stop counting
			if(connectNBoard[newestRow][columnCount] != lookingFor)
			{
				break;
			}
			//otherwise increment counter
			else
			{
				afterCount++;
			}
		}

		//count the game pieces of the color that we're looking for to the left
		for(columnCount=columnChosen-1; columnCount>=0; columnCount--)
		{
			//if the previous piece is not the number we are looking for, stop counting
			if(connectNBoard[newestRow][columnCount] != lookingFor)
			{
				break;
			}
			//otherwise increment counter
			else
			{
				beforeCount++;
			}
		}

		//get line length
		totalCount = afterCount + beforeCount + 1;
		//check if the player has made a sufficiently long line
		if(totalCount >= numConnect)
		{
			return true;
		}
		//otherwise reset the counters and try counting vertically
		else
		{
			beforeCount = 0;
			afterCount = 0;
			totalCount = 0;
		}

		/*COUNTING VERTICALLY*/

		//count game pieces of the color that we're looking for below
		for(rowCount=newestRow+1; rowCount<numRowsInBoard; rowCount++)
		{
			if(connectNBoard[rowCount][columnChosen] != lookingFor)
			{
				break;
			}
			else
			{
				afterCount++;
			}
		}

		//count game pieces of the color that we're looking for above
		for(rowCount=newestRow-1; rowCount>=0; rowCount--)
		{
			if(connectNBoard[rowCount][columnChosen] != lookingFor)
			{
				break;
			}
			else
			{
				beforeCount++;
			}
		}

		//get line length
		totalCount = afterCount + beforeCount + 1;
		//check if the player has made a sufficiently long line
		if(totalCount >= numConnect)
		{
			return true;
		}
		//reset counters and try counting diagonally
		else
		{
			beforeCount = 0;
			afterCount = 0;
			totalCount = 0;
		}

		/*COUNTING DIAGONALLY (TOP LEFT - BOTTOM RIGHT)*/

		//GOING DOWN TO THE RIGHT FROM NEWEST SPOT
		//set iteration counter to 0
		iterNum = 0;
		//find out how far down right we can go
		iterAllowed = numRowsInBoard - columnChosen;

		for(rowCount=newestRow+1; rowCount<numRowsInBoard; rowCount++)
		{
			//iterate how many time we need to go right, to keep going diagonally
			iterNum++;
			//if we are off the board, break
			if(iterNum >= iterAllowed)
			{
				break;
			}
			//if we reach the end of the line of the gamepieces that we're looking for, break
			else if(connectNBoard[rowCount][columnChosen+iterNum] != lookingFor)
			{
				break;
			}
			//otherwise increase afterCount
			else
			{
				afterCount++;
			}
		}

		//GOING UP TO THE LEFT FROM NEWEST SPOT
		//set iteration counter to 0
		iterNum = 0;
		//find out how far left we can go
		iterAllowed = -columnChosen;

		for(rowCount=newestRow-1; rowCount>=0; rowCount--)
		{
			//iterate how many times we need to go left
			iterNum--;
			//if we are off the board, break
			if(iterNum < iterAllowed)
			{
				break;
			}
			//if we reach the end of the line of the gamepieces that we're looking for, break
			else if(connectNBoard[rowCount][columnChosen+iterNum] != lookingFor)
			{
				break;
			}
			//otherwise increase beforeCount
			else
			{
				beforeCount++;
			}
		}

		//get line length
		totalCount = beforeCount + afterCount + 1;
		//check if the player has made a sufficiently long line
		if(totalCount >= numConnect)
		{
			return true;
		}
		else
		{
			beforeCount = 0;
			afterCount = 0;
			totalCount = 0;
		}

		/*COUNTING DIAGONALLY (BOTTOM RIGHT - TOP LEFT)*/

		//GOING DOWN TO THE LEFT FROM NEWEST SPOT
		//set iteration counter to 0
		iterNum = 0;
		//find out how far left we can go
		iterAllowed = -columnChosen;

		for(rowCount=newestRow+1; rowCount<numRowsInBoard; rowCount++)
		{
			//iterate how many times we need to go left
			iterNum--;
			//if we are off the board, break
			if(iterNum < iterAllowed)
			{
				break;
			}
			//if we reach the end of the line of the gamepieces that we're looking for, break
			else if(connectNBoard[rowCount][columnChosen+iterNum] != lookingFor)
			{
				break;
			}
			//otherwise incrememnt beforeCount
			else
			{
				beforeCount++;
			}
		}

		//GOING UP TO THE RIGHT FROM NEWEST SPOT
		//set iteration counter to 0
		iterNum = 0;
		//find out how far right we can go
		iterAllowed = numRowsInBoard - columnChosen;

		for(rowCount = newestRow-1; rowCount>=0; rowCount--)
		{
			//iterate how many times we need to go right
			iterNum++;
			//if we are off the board, break
			if(iterNum >= iterAllowed)
			{
				break;
			}
			//if we reach the end of the line of the gamepieces that we're looking for, break
			else if(connectNBoard[rowCount][columnChosen+iterNum] != lookingFor)
			{
				break;
			}
			//otherwise increment afterCount
			else
			{
				afterCount++;
			}
		}

		//get line length
		totalCount = beforeCount + afterCount + 1;
		//check if the player has made a sufficiently long line
		if(totalCount >= numConnect)
		{
			return true;
		}
		//otherwise reset counters
		else
		{
			beforeCount = 0;
			afterCount = 0;
			totalCount = 0;
		}
	}
	return false;
}
