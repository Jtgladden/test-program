#include <iostream>
#include <stdlib.h>
using namespace std;

//Player names


// Array for the board
char board[3][3] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};

//Variable Declaration
int choice;
int row, column;
char turn = 'X';
bool draw = false;
string name_X, name_O;

//Function to who the current status of the gaming board
void display_board()
{

    //Render game board LAYOUT
    cout <<name_X << " - [X]\t" << name_O << " - [O]\n\n";
    cout <<"\t\t       |       |       \n";
    cout <<"\t\t  "<<board[0][0]<<"    |   "<<board[0][1]<<"   |   "<<board[0][2]<<endl;
    cout <<"\t\t_______|_______|_______\n";
    cout <<"\t\t       |       |       \n";
    cout <<"\t\t  "<<board[1][0]<<"    |   "<<board[1][1]<<"   |   "<<board[1][2]<<endl;
    cout <<"\t\t_______|_______|_______\n";
    cout <<"\t\t       |       |       \n";
    cout <<"\t\t  "<<board[2][0]<<"    |   "<<board[2][1]<<"   |   "<<board[2][2]<<endl;
    cout <<"\t\t       |       |       \n";
}

//Function to get the player input and update the board
void player_turn()
{
    if(turn == 'X'){
        cout <<"\n\t"<< name_X <<"- [X] turn: ";
    }
    else if(turn == 'O'){
        cout <<"\n\t"<< name_O <<"- [O] turn: ";
    }

    //taking input from user
    //updating the board according to choice and reassigning the turn start

    cin >> choice; //Switch case to get which row and column will be updated
    switch(choice)
    {
    case 1:
        row = 0;
        column = 0;
        break;
    case 2:
        row = 0;
        column = 1;
        break;
    case 3:
        row = 0;
        column = 2;
        break;
    case 4:
        row = 1;
        column = 0;
        break;
    case 5:
        row = 1;
        column = 1;
        break;
    case 6:
        row = 1;
        column = 2;
        break;
    case 7:
        row = 2;
        column = 0;
        break;
    case 8:
        row = 2;
        column = 1;
        break;
    case 9:
        row = 2;
        column = 2;
        break;
    default:
        cout <<"Invalid Move";
    }

    if(turn == 'X' && board[row][column] != 'X' && board[row][column] != 'O'){
        //u;dating the position for 'X' symbol if not already occupied
        board[row][column] = 'X';
        turn = 'O';
    }
    else if(turn == 'O' && board[row][column] != 'X' && board[row][column] != 'O'){
        //updating the position for 'O' symbol if not already occupied
        board[row][column] = 'O';
        turn = 'X';
    }
    else{
        //If input position already filled
        cout << "box already filled! \n Please choose another! \n\n";
        player_turn();
    }
    /* Ends */
    display_board();
}

//Funciton to get the game statues. E.g Game won, game draw, game over

bool gameover()
{
    //checking the win for the simple Rows and Simple Column
    for(int i=0; i<3; i++)
    if(board[i][0] == board[i][1] && board[i][0] == board[i][2] || board[0][i] == board[1][i] && board[0][i] == board[2][i])
        return false;

    //checking the win for a botch diagonal
    if(board[0][0] == board[1][1] && board[0][0] == board [2][2] || board[0][2] == board[1][1] && board[0][2] == board[2][0])
        return false;

    //checking the game is in continue mode or not
    for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
    if(board[i][j] != 'X' && board[i][j] != 'O')
    return true;

    //checking the if game already draw
    draw = true;
    return false;
}

//Program Main Method

int main()
{
    cout <<"name for player 'X' :";
    cin >> name_X;
    cout <<"name for player 'O' :";
    cin >> name_O;
    cout <<"\t\t\tT I C K -- T A C -- T O E -- G A M E\t\t\t";
    cout <<"\n\t\t\t\tFOR 2 Players\n\t\t\t";
    while(gameover()){
        display_board();
        player_turn();
        gameover();
    }
    if(turn == 'X' && draw == false){
        cout<<"\n\nCongratulations! " << name_O <<" has won the game";
    }
    else if(turn == 'O' && draw == false){
        cout<<"\n\nCongratulations!" << name_X <<" has won the game";
    }
    else cout<<"\n\nGame Draw!!!\n\n";

    return 0;
}

