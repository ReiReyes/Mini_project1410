#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> 
#include <unistd.h> 

int MALLOC_COUNT = 0;
int FREE_COUNT = 0;

#define malloc(x) (MALLOC_COUNT++, malloc(x))
#define free(x) (FREE_COUNT++, free(x))

void clearScreen() {
#ifdef _WIN32         
    system("cls");     
#else      
    system("clear");   
#endif
}
const int MAX_SIZE = 11;
void mySleep(){
    Sleep(1000); 
    sleep(1);
}

void clearBoard(char **board, int size){
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = ' ';
        }
    }
}
void printBoard(int size, char **board) {
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


char isGameOver(int size, char **board) {
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
            if (board[j][i] != first) {
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

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(board[i][j] == ' '){
                return ' '; // no winner yet
            }
        }
    
    }
return 'D'; // Draw
}

int wantToPlay(int keepPlaying){
    while (1){
        printf("\n(1) Yes\n(2) No \nDo you want a rematch: ");
        scanf("%d", &keepPlaying);
        if(keepPlaying == 2){
            keepPlaying = 0;
            clearScreen();
            return keepPlaying;
            
        }else if(keepPlaying == 1){
            return keepPlaying;
        }else{
            printf("\nInvalid Input, \n%d is not a valid option", keepPlaying);
        }
    }
    return 0;
}

int* aiMoveDecision(int size, char **board){
    int playerWin = 0, aiMove[2];
    int *ptr = aiMove;

    for (int i = 0; i < size; i++) { //row winning
        playerWin=0;
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 'O') {
                playerWin++;
            }
            if (board[i][j] == 'X') {
                playerWin--;
            }
        }
        if(playerWin ==  size-1){
            for (int j = 0; j < size; j++) {
                if(board[i][j] == ' '){
                    aiMove[0] = i;
                    aiMove[1] = j;
                    return ptr;
                }
            }
        }
    }

    for (int i = 0; i < size; i++) { // column winning
        playerWin=0;
        for (int j = 0; j < size; j++) {
            if (board[j][i] == 'O') {
                playerWin++;
            }
            if (board[j][i] == 'X') {
                playerWin--;
            }
        }
        if(playerWin ==  size-1){
            for (int j = 0; j < size; j++) {
                if(board[j][i] == ' '){
                    aiMove[0] = j;
                    aiMove[1] = i;
                    return ptr;
                }
            }
        }
    }

    playerWin = 0;
    for (int i = 0; i < size; i++) { //Diagonal winning
        if (board[i][i] == 'O') {
            playerWin++;
        }
        if (board[i][i] == 'X') {
            playerWin--;
        }
    }
    if(playerWin == size-1){
        for (int i = 0; i < size; i++) {
            if (board[i][i] == ' ') {
                aiMove[0] = i;
                aiMove[1] = i;
                return ptr;
            }
    }
    }

    playerWin = 0;
    for (int i = 0; i < size; i++) { //AntiDiagonal winning
        if (board[i][size - 1 - i] == 'O') {
            playerWin++;
        }
        if (board[i][size - 1 - i] == 'X') {
            playerWin--;
        }
    }
    if(playerWin == size-1){
        for (int i = 1; i < size; i++) {
            if (board[i][size - 1 - i] == ' ') {
                aiMove[0] = i;
                aiMove[1] = size - 1 - i;
                return ptr;
            }
    }
    }

    for (int i = 0; i < size; i++) { //row blocking
        playerWin=0;
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 'X') {
                playerWin++;
            }
            if (board[i][j] == 'O') {
                playerWin--;
            }
        }
        if(playerWin ==  size-1){
            for (int j = 0; j < size; j++) {
                if(board[i][j] == ' '){
                    aiMove[0] = i;
                    aiMove[1] = j;
                    return ptr;
                }
            }
        }
    }

    for (int i = 0; i < size; i++) { // column blocking
        playerWin=0;
        for (int j = 0; j < size; j++) {
            if (board[j][i] == 'X') {
                playerWin++;
            }
            if (board[j][i] == 'O') {
                playerWin--;
            }
        }
        if(playerWin ==  size-1){
            for (int j = 0; j < size; j++) {
                if(board[j][i] == ' '){
                    aiMove[0] = j;
                    aiMove[1] = i;
                    return ptr;
                }
            }
        }
    }

    playerWin = 0;
    for (int i = 0; i < size; i++) { //Diagonal Blocking
        if (board[i][i] == 'X') {
            playerWin++;
        }
        if (board[i][i] == 'O') {
            playerWin--;
        }
    }
    if(playerWin == size-1){
        for (int i = 0; i < size; i++) {
            if (board[i][i] == ' ') {
                aiMove[0] = i;
                aiMove[1] = i;
                return ptr;
            }
    }
    }

    playerWin = 0;
    for (int i = 0; i < size; i++) { //AntiDiagonal Blocking
        if (board[i][size - 1 - i] == 'X') {
            playerWin++;
        }
        if (board[i][size - 1 - i] == 'O') {
            playerWin--;
        }
    }

    if(playerWin == size-1){
        for (int i = 0; i < size; i++) {
            if (board[i][size - 1 - i] == ' ') {
                aiMove[0] = i;
                aiMove[1] = size - 1 - i;
                return ptr;
            }
    }
    }
    for (int i = 0; i < size; i++) { //row best move
        playerWin=0;
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 'O') {
                playerWin++;
            }
            if (board[i][j] == 'X') {
                playerWin = 0;
                break;
            }
        }
        if(playerWin != 0){
            for (int j = 0; j < size; j++) {
                if(board[i][j] == ' '){
                    aiMove[0] = i;
                    aiMove[1] = j;
                    return ptr;
                }
            }
        }
    }

    for (int i = 0; i < size; i++) { // column Best move
        playerWin=0;
        for (int j = 0; j < size; j++) {
            if (board[j][i] == 'O') {
                playerWin++;
            }
            if (board[j][i] == 'X') {
                playerWin = 0;
                break;
            }
        }
        if(playerWin != 0){
            for (int j = 0; j < size; j++) {
                if(board[j][i] == ' '){
                    aiMove[0] = j;
                    aiMove[1] = i;
                    return ptr;
                }
            }
        }
    }

    playerWin = 0;
    for (int i = 0; i < size; i++) { //Diagonal Best Move
        if (board[i][i] == 'O') {
            playerWin++;
        }
        if (board[i][i] == 'X') {
            playerWin = 0;
            break;
        }
    }
    if(playerWin != 0){
        for (int i = 0; i < size; i++) {
            if (board[i][i] == ' ') {
                aiMove[0] = i;
                aiMove[1] = i;
                return ptr;
            }
    }
    }

    playerWin = 0;
    for (int i = 0; i < size; i++) { //AntiDiagonal Best Move
        if (board[i][size - 1 - i] == 'O') {
            playerWin++;
        }
        if (board[i][size - 1 - i] == 'X') {
            playerWin = 0;
            break;
        }
    }
    if(playerWin != 0){
        for (int i = 1; i < size; i++) {
            if (board[i][size - 1 - i] == ' ') {
                aiMove[0] = i;
                aiMove[1] = size - 1 - i;
                return ptr;
            }
    }
    }

    while(1){
        int move1 = rand()%size;
        int move2 = rand()%size;
        if(board[move1][move2] == ' '){
            aiMove[0] = move1;
            aiMove[1] = move2;
            return ptr;
        }
    }
    return ptr;
}
void Gamepva(int grid,  char **board, int points[2]){
    int player = 1;
    clearBoard(board, grid);
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
                mySleep();
                
        } else {
                if (player == 1) {
                    board[row - 1][col - 1] = 'X';
                    player = 2;
                }
        }
        if(player == 2 && isGameOver(grid, board) == ' '){
            int *aimove= aiMoveDecision(grid, board);
            board[*aimove][*(aimove+1)] = 'O';
            player = 1;
        }

        if (isGameOver(grid, board) != ' '){
            clearScreen();
            printf("Game over!\n");
            printBoard(grid, board);
            char winner = isGameOver(grid, board);
            if (winner != 'D') {
                printf("Player %c wins!\n", winner);
                if(winner == 'X'){
                    points[0]++;
                }else{
                    points[1]++;
                }
                
            } else {
                printf("It's a draw!\n");
            }
            break;
    }
}
}
void Gamepvp(int grid,  char **board, int points[2]){
    int player = 1;
    clearBoard(board, grid);
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
                mySleep();
                
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
            if (winner != 'D') {
                printf("Player %c wins!\n", winner);
                if(winner == 'X'){
                    points[0]++;
                }else{
                    points[1]++;
                }
                
            } else {
                printf("It's a draw!\n");
            }
            break;
        }
    }
        
}

void printScore(int points[2], int option){
    if(option != 2){
        printf("\nScores: \nPlayer 1: %d \nPlayer 2: %d\n", points[0], points[1]);
    }else{
        printf("\nScores: \nPlayer 1: %d \nAI: %d\n", points[0], points[1]);
    }
    
}

typedef struct {
    int games_played;
    int wins_player1;
    int wins_player2;
    int draws;
    int * win_patterns; // Dynamic array : tracks HOW games were won
    int pattern_count;
    int pattern_capacity;
    } GameStats ;

char** createBoard ( int size, char **out_data ){
    char *data = malloc(size * size * sizeof(char));
    char **board = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        board[i] = data + (i * size);
    }
    *out_data = data;
    return board;
}

int main(){
    srand(time(NULL));
    int option, always = 1, grid, keepPlaying, points[2] = {0,0};
    char *data;
    char **boardptr;
    printf("Welcome to the Tic-Tac-Toe Game\n");
    while(always){
        printf("(1) Player vs Player");
        printf("\n(2) Player vs AI");
        printf("\n(3) Exit the game");
        printf("\nSelect the option you would like: ");
        scanf("%d", &option);
        if (option == 2 || option == 1){
            while (1) {
                printf("\nWhich grid size do you want (put n for nxn size grid): ");

                if (scanf("%d", &grid) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    while (getchar() != '\n');
                    continue; 
                }
                while (getchar() != '\n'); 
                if (grid >= 3 && grid <= 15) {
                    boardptr = createBoard(grid, &data);
                    break; 
                } else {
                    printf("Invalid grid size. Must be between 3 and 15.\n");
                }
            }
        }
        
        
        switch (option)
        {
        case 1:
            keepPlaying = 1;
            points[0] = 0;
            points[1] = 0;
            while(keepPlaying){
                clearBoard(boardptr, grid);
                Gamepvp(grid, boardptr, points);
                printScore(points, option);
                keepPlaying = wantToPlay(keepPlaying);
                
            }
            free(boardptr);
            free(data);
            
            break;
        
        case 2:
            keepPlaying =1;
            points[0] = 0;
            points[1] = 0;
            while(keepPlaying){
                clearBoard(boardptr, grid);
                Gamepva(grid, boardptr, points);
                printScore(points, option);
                keepPlaying = wantToPlay(keepPlaying);
            }
            free(boardptr);
            free(data);
            break;
        case 3:
            printf("\nThank You for playing");
            always = 0;
            break;

        default:
            break;
        }
        
    } 
    printf("Mallocs: %d\nFrees: %d\n", MALLOC_COUNT, FREE_COUNT); // Memory allocation report erase when not needed

    return 0;
}