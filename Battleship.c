/* Project Name: Battleship
 * Author: Rudaina Zahid
 * Description: This program allows the user to play * a game of battleship against the computer
*/

//include libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//define board limits
#define BOARDROWS 9
#define BOARDCOLS 11

bool dumpComputer = true; //causes dump of computer board before game if true (for debugging) - normally false

//Game boards. 0 means empty, non-zero means ship is there, number is size, -tive means hit on a ship
int playerBoard[BOARDROWS+1][BOARDCOLS+1];
int computerBoard[BOARDROWS+1][BOARDCOLS+1];
int compShotBoard[BOARDROWS+1][BOARDCOLS+1];

//function prototypes
bool rangeCheck(int userInput, int min, int max);
bool overEdgeCheck(int orient, int col, int row, int size);
bool overlapCheck(int colInput, int rowInput, int size, int orientInput, int board[BOARDROWS+1][BOARDCOLS+1]);
int getRand(int lowval,int highval);

//this function generates a random number between two given values
int getRand(int lowval,int highval){
    return (rand()%(highval+1-lowval) + lowval);
}

//this function lets the user and computer guess and take shots at each other's boats
int getShot(bool fromUser, int board[BOARDROWS+1][BOARDCOLS+1]){
    int row,col;
    
    while(1){
        if(fromUser){
            printf("Give a shot (row, col):");
            scanf("%d%d",&row,&col);
            if(col<1 || col > BOARDCOLS || row < 1 || row > BOARDROWS)
                printf("Invalid input\n");
            else  //valid input
                break;
        }
        else { //computer generated
            row=getRand(1,BOARDROWS);
            col=getRand(1,BOARDCOLS);
            if(compShotBoard[row][col]==0) { //make sure haven't shot here before
                compShotBoard[row][col]=1; //valid shot
                break;
            }
        }
    } //will leave this loop with valid input
    if(board[row][col] != 0){
        if(board[row][col]>0)
            board[row][col]= -1*board[row][col]; //make sure is -tive
        return -1*board[row][col]; //a hit!
    }
    return 0; //miss
}
            
//this function checks if all ships have been hit
bool allShipsNotHit(int board[BOARDROWS+1][BOARDCOLS+1]){
    int i, j;
    for(i=1;i<=BOARDROWS;i++){
        for(j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>0) //ship and not hit
                return(true);
        }
    }
    return (false); //no ships found, all have been hit
}

//this function checks whether any ships are left on the board
bool noneLeft(int valueToFind,int board[BOARDROWS+1][BOARDCOLS+1]){
    int i, j;
    for(i=1;i<=BOARDROWS;i++){
        for(j=1;j<=BOARDCOLS;j++){
            if(board[i][j]==valueToFind) //ship and not hit
                return(false);
        }
    }
    return (true); //no ships found, all hit
}

//this function checks whether the user's input is within the range of min, max
bool rangeCheck(int userInput, int rangeMin, int rangeMax)
{
   //declare variable
    bool rangeValid;       //tracks whether the user's input is in range

    //this if statement checks if the user's input is within the given range
    if(userInput >= rangeMin && userInput <= rangeMax)    //valid input
    {
        rangeValid = true;
    }
    else                                                  //invalid input
    {
        rangeValid = false;
    }

    return rangeValid; //return the result of the range check
}

//this function checks whether the ship placement selected goes over the edge of the board
bool overEdgeCheck(int row, int col, int orient, int size)
{
    //declare variable
    bool overEdgeValid = true;      //tracks whether the ship goes over the edge of the board

    //this if statement checks the orientation of the ship 
    if(orient == 0)                             //horizontal ship orientation
    {
        //this if statement checks whether the ship goes past the columns of the board
        if((col + size - 1) > BOARDCOLS)
        {
            overEdgeValid = false;
        }
    }
    else if (orient == 1)                       //vertical ship orientation
    {
        //this if statement checks whether the ship goes past the rows of the board
        if((row + size - 1) > BOARDROWS)
        {
            overEdgeValid = false;
        }
    }

    return overEdgeValid;        //return the result of the over board edge check
}

//this function checks whether the ship placement selected overlaps with any previously 
//placed ships
bool overlapCheck(int row, int col, int orient, int size, int board[BOARDROWS+1][BOARDCOLS+1])
{
    //declare variables
    bool overlapValid = false;          //tracks whether the ship overlaps any other ships
    int currentCol, currentRow;         //tracks the number of the row/column being checked

    //this if statement checks the orientation of the ship
    if(orient == 0)                             //horizontal ship placement
    {
        //this for loop goes through the section of the row where the ship will be placed 
        //and checks if it is empty
        for(currentCol = col; currentCol <= (col + size - 1); currentCol++)
        {
            if(board[row][currentCol] != 0)
            {
                overlapValid = true;
            }
        }
    }
    else                                        //vertical ship placement
    {
        //this for loop goes through the section of the column where the ship will be placed 
        //and checks if it is empty
        for(currentRow = row; currentRow <= (row + size - 1); currentRow++)
        {
            if (board[currentRow][col] != 0)
            {
                overlapValid = true;
            }
        }
    }

    return overlapValid;        //return the result of the overlap check
}

//this function places the specified ship onto the selected board
void placeShip(int row, int col, int orient, int size, int board[BOARDROWS+1][BOARDCOLS+1])
{
    //declare variables
    int currentCol, currentRow;     //tracks the number of the row/column being checked

    //this if statement checks the orientation of the ship
    if(orient == 0)                             //horizontal ship orientation
    {
        //this for loop places the ship into the selected row
        for(currentCol = col; currentCol <= (col + size - 1); currentCol++)
        {
            board[row][currentCol] = size;
        }
    }
    else                                        //vertical ship orientation
    {
        //this for loop places the ship into the selected column
        for(currentRow = row; currentRow <= (row + size - 1); currentRow++)
        {
            board[currentRow][col] = size;
        }
    }
}

//this function places ships onto the user's and computer's boards based on their input
//(user: keyboard, computer: random)
void populateBoard(bool getUserInput, int board[BOARDROWS+1][BOARDCOLS+1])
{
    //declare variables

    //range variables
    int rowMin = 1, rowMax = BOARDROWS;         //min/max row number
    int colMin = 1, colMax = BOARDCOLS;         //min/max column number
    int orientMin = 0, orientMax = 1;           //min/max orientation selection number
    int sizeMin = 1, sizeMax = 5;               //min/max ship size

    //user input variables
    int rowInput, colInput, orientInput;        //user's row, column and orientation inputs

    //input validity tracking variables
    bool rowValid, colValid, orientValid;       //tracks whether the user's row, column and 
                                                //orientation inputs are valid
    bool overEdgeValid;                         //tracks whether a ship goes over the edge of the board
    bool overlapValid;                          //tracks whether a ship overlaps other ships

    //other variables
    bool shipPlaced;                            //tracks whether a ship has been placed
    int currentSize;                            //tracks the size of the ship being placed

    //this if statement checks if the user's input is needed to populate the board
    if(getUserInput == true)    //user's board will be populated
    {
        //Output the board row, column and ship orientation choices to the user
        printf("Rows are 1 - 9, Columns are 1 - 11\nOrientation is 0 for across, 1 for down\n");

        //this for loop is used to place 5 ships from sizes 5 - 1 onto the board
        for(currentSize = sizeMax; currentSize >= sizeMin; currentSize--)
        {
            shipPlaced = false;

            //this while loop runs until a ship of the current size has been placed
            while(shipPlaced == false)
            {
                //Ask the user to input the row, column and orientation for a ship. Read and store 
                //these values
                printf("Give starting row, starting column and orientation (3 inputs) for ship of size=%d:", currentSize);
                scanf("%d%d%d", &rowInput, &colInput, &orientInput);

                //Check the validity of each of the user's inputs
                rowValid = rangeCheck(rowInput, rowMin, rowMax);
                colValid = rangeCheck(colInput, colMin, colMax);
                orientValid = rangeCheck(orientInput, orientMin, orientMax);

                //this if statement checks whether ALL of the user's inputs were valid
                if(rowValid == true && colValid == true && orientValid == true) //all inputs valid
                {
                    //check whether the new ship goes over the edge of the board or overlaps a 
                    //previously placed ship
                    overEdgeValid = overEdgeCheck(rowInput, colInput, orientInput, currentSize);
                    overlapValid = overlapCheck(rowInput, colInput, orientInput, currentSize, playerBoard);

                    //this if statement checks whether the ship can be placed on the board
                    if(overEdgeValid == false)                      //ship goes over the edge of the board
                    {
                        //output an invalid message to the user
                        printf("Invalid Input\n");
                    }
                    else if(overlapValid == true)                   //ship overlaps a previous ship
                    {
                        //output a conflict message to the user
                        printf("Conflicts with ship already placed\n");
                    }
                    else                                            //ship can be placed
                    {
                        //place ship onto the board
                        placeShip(rowInput, colInput, orientInput, currentSize, playerBoard);    
                        shipPlaced = true;
                    }
                }
                else        //at least one of the user's inputs were invalid
                {
                    //output an invalid message to the user
                    printf("Invalid Input\n");
                }
            }
        }
    }
    else if(getUserInput == false)      //computer's board will be populated
    {
        //this for loop is used to place 5 ships from sizes 5-1 onto the board
        for(currentSize = sizeMax; currentSize >= sizeMin; currentSize--)
        {
            shipPlaced = false;

            //this while loop runs until a ship of the current size has been placed
            while(shipPlaced == false)
            {
                //randomly select the computer's input for row, column and orientation
                rowInput = getRand(rowMin, rowMax);
                colInput = getRand(colMin, colMax);
                orientInput = getRand(orientMin, orientMax);

                //check whether the new ship goes over the edge of the board or overlaps a 
                //previously placed ship
                overEdgeValid = overEdgeCheck(rowInput, colInput, orientInput, currentSize);
                overlapValid = overlapCheck(rowInput, colInput, orientInput, currentSize, computerBoard);

                //this if statement checks whether the ship can be placed on the board
                if(overEdgeValid == true && overlapValid == false)      //ship can be placed
                {
                    //place ship onto the board
                    placeShip(rowInput, colInput, orientInput, currentSize, computerBoard);
                    shipPlaced = true;
                }
            }
        }
    }
}

//this function displays a game board
void showBoard(int board[BOARDROWS+1][BOARDCOLS+1]){
    printf("   Cols\n");
    printf("    1  2  3  4  5  6  7  8  9 10 11\n");
    printf("     ________________________________\n");
    int i, j;
    for(i=1;i<=BOARDROWS;i++){
        printf("R%d|",i);
        for(j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>=0)
                printf(" ");
            printf("%d ",board[i][j]);
        }
        printf("|\n");
    }
    printf("  __________________________________\n");
}

//this function initializes and runs the battleship game
int main(int argc, char **argv)
{
    bool playerBoardOK,computerBoardOK;
    
    // initialize the seed randomly. 

     	srand(time(NULL)); 

    //init the boards
    int i, j;
    
    for(i=0;i<BOARDROWS;i++){
        for(j=0;j<BOARDCOLS;j++){
            playerBoard[i][j]=0;
            computerBoard[i][j]=0;
            compShotBoard[i][j]=0;
        }
    }

    populateBoard(true,playerBoard);
    populateBoard(false,computerBoard);
    printf("Your board is\n");
    showBoard(playerBoard);
    if(dumpComputer){
        printf("\n\nComputer board is\n");
        showBoard(computerBoard);
    }

    // play starts here
    playerBoardOK=true;
    computerBoardOK=true;
    while(playerBoardOK && computerBoardOK){
        int hit=getShot(true,computerBoard); //shot from user
        if(hit){
            printf("HIT on size %d\n",hit);
            if(noneLeft(hit,computerBoard))
                printf("Sunk computer's %d\n",hit);
        }
        else
            printf("MISS!!\n");
        hit=getShot(false,playerBoard);
        if(hit){
            printf("Computer hits! Size=%d\n", hit);
            if(noneLeft(hit,playerBoard))
                printf("Sunk your %d\n",hit);
        }
        else
            printf("Computer miss!\n");
        playerBoardOK = allShipsNotHit(playerBoard);
        computerBoardOK = allShipsNotHit(computerBoard);
//    showBoard(playerBoard); //debug
//    showBoard(computerBoard); //debug
   }
   if(playerBoardOK)
       printf("You win!\n");
    else
        printf("You lose.\n");
    showBoard(computerBoard);

	return 0;
}