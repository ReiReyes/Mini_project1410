#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> 
#include <unistd.h> 

void clearScreen() {
#ifdef _WIN32
    Sleep(500);       
    system("cls");     
#else
    sleep(0.5);          
    system("clear");   
#endif
}

int clearBoard(int size, char board[11][11])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = ' ';
        }
    }
    return 0;
}

void printBoard(int size, char board[11][11]) {
    printf("\n  ");
    for(int i =1; i <= size; i++){
        printf(" %d  ",i);
    }
    printf("\n");
    for(int i = 0; i < size; i++){
        printf("%c ", i+65);
        for(int j = 0; j < size; j++){
            if(j < size-1){
                printf(" %c |", board[i][j] );
            }else{
                printf(" %c", board[i][j]);
            }
        }
        printf("\n");
        printf("   ---");
        for(int i = 1; i < size-1; i++){
            printf("----");
        }
        
        printf("--\n");
    }
}

    char isGameOver(int size, char board[11][11]) {
        for (int i = 0; i < size; i++) { // check rows
            char first = board[i][0];
            if (first == ' ') continue;
            int win = 1;
            for (int j = 1; j < size; j++) {
                if (board[i][j] != first) {
                    win = 0;
                    break;
                }
            }
        if (win) return first;
        }

        for (int i = 0; i < size; i++) {//check for collums
            char first = board[0][i];
            if (first == ' ') continue;
            int win = 1;
            for (int j = 1; j < size; j++) {
                if (board[i][j] != first) {
                    win = 0;
                    break;
                }
            }
        if (win) return first;
        }
        

        char first = board[0][0];   // check for diagonals 
            if (first != ' '){
                int win = 1;
                for (int i = 1; i < size; i++) {
                    if (board[i][i] != first) {
                        win = 0;
                        break;
                    }
                }
            if (win) return first;
        }
        
    
        first = board[0][size - 1]; // check for diagonals from other way
        if (first != ' ') {
            int win = 1;
            for (int i = 1; i < size; i++) {
                if (board[i][size - 1 - i] != first) {
                    win = 0;
                    break;
                }
            }
            if (win) return first;
        }

    
    return ' '; // no winner yet
}


void Gamepvp(int grid,  char board[11][11]){
    int player = 1;
    clearBoard(grid, board);
    while(1){
        clearScreen();
        printBoard(grid, board);
        int row, col;
        char rowchar;
        
        printf("Player %d, please enter a row and column (e.g., 1 A): ", player);
        while (scanf("%d %c", &col, &rowchar) != 2) {
                printf("Invalid input, please try again: ");
                while (getchar() != '\n');
        }
        
        row = rowchar - 64;
        if (row < 1 || row > grid || col < 1 || col > grid || board[row - 1][col - 1] != ' ') {
                printf("Invalid move, please try again.\n");
        } else {
                if (player == 1) {
                    board[row - 1][col - 1] = 'X';
                    player = 2;
                } else {
                    board[row - 1][col - 1] = 'O';
                    player = 1;
                }
            }

        if (isGameOver(grid, board) != ' '){
            clearScreen();
            printf("Game over!\n");
            printBoard(grid, board);
            char winner = isGameOver(grid, board);
            if (winner != ' ') {
                printf("Player %c wins!\n", winner);
            } else {
                printf("It's a draw!\n");
            }
            break;
        }
    }
        
}


int main(){
    int option, always = 1, grid;
    char board[11][11];
    
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            board[i][j] = ' ';
        }
    }
    
    printf("Welcome to the Tic-Tac-Toe Game\n");
    while(always){
        printf("(1) Player vs Player");
        printf("\n(2) Player vs AI");
        printf("\n(3) Exit the game");
        printf("\nSelect the option you would like: ");
        scanf("%d", &option);
        
        switch (option)
        {
        case 1:
            printf("Which grid size do you want (put n for nxn size grid): ");
            scanf("%d", &grid);
            Gamepvp(grid, board);
            
            break;
        
        case 2:
            
            break;
        case 3:
            printf("\nThank You for playing");
            always = 0;
            break;

        default:
            break;
        }
    } 

    return 0;
}