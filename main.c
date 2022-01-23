#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>

#define SIZE 20
#define clear() printf("\033[H\033[J") 

char board[SIZE][SIZE];
char c;
int score = 1;
int k; // Counter for total no. of available tiles

int available[(SIZE*SIZE)+1][2];

struct Node {
    int x, y;
    struct Node* next;
    struct Node* prev;
};

struct Fruit {
    int x, y;
};

struct Node* head;
struct Fruit* fruit;
struct Node* tail;

void check();

void printBoard() {
    for (int i = 0; i < SIZE + 2; i++) printf("-");
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf("|");
        for (int j = 0; j < SIZE; j++) {
            printf("%c", board[i][j]);
        }
        printf("|\n");
    }
    for (int i = 0; i < SIZE + 2; i++) printf("-");
    printf("\n");
}

void setFruit() {
    int rnd = rand()%k;
    if (board[available[rnd][0]][available[rnd][1]] != ' ') setFruit();
    else {
        fruit->x = available[rnd][0];
        fruit->y = available[rnd][1];
        
        available[rnd][0] = available[k][0];
        available[rnd][1] = available[k--][0];

        board[fruit->x][fruit->y] = 'O';
    }
}

int makeMove(int i, int j) {
    
    int newX = (head->x + i)%SIZE, newY = (head->y + j)%SIZE;
    newX = newX < 0 ? SIZE-1 : newX;
    newY = newY < 0 ? SIZE-1 : newY;

    if (newX == head->next->x && newY == head->next->y) return 1;
    if (board[newX][newY] != ' ' && board[newX][newY] != 'O') return 0; // snake ate itself

    struct Node* newHead = (struct Node*)malloc(sizeof(struct Node));
    newHead->x = newX; newHead->y = newY;
    newHead->next = head;
    head->prev = newHead;
    board[head->x][head->y] = 'X';
    head = newHead;

    if (board[newX][newY] == 'O') {
        setFruit();
        score++;
    } else {
        board[tail->x][tail->y] = ' ';
        struct Node* temp = tail;
        tail = tail->prev;
        free(temp);
        board[tail->x][tail->y] = 'Y';
    }

    board[newX][newY] = 'A';

    return 1;
}

int isMove(char c) {
    return c == 'A' || c == 'D' || c == 'W' || c == 'S';
}

int getMove(char c) {
    return c == 'D' ? 0 : c == 'S' ? 1 : c == 'A' ? 2 : 3;
} 

void freeMemory() {
    struct Node* tmp;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }

}

void startGame() {
    memset(board, ' ', sizeof(board));

    head = (struct Node*)malloc(sizeof(struct Node));
    tail = head;
    fruit = (struct Fruit*)malloc(sizeof(struct Fruit));

    k = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            available[k][0] = i;
            available[k++][1] = j;
        } 
    }

    int start = rand() % k;

    head->x = available[start][0];
    head->y = available[start][1];
    head->next = tail;
    tail->prev = head;
    tail->x = head->x;
    tail->y = head->y;

    k--;
    available[start][0] = available[k][0];
    available[start][1] = available[k][1];
    board[head->x][head->y] = 'A';

    setFruit(); 
    int moves[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    char lastMove = 'A';

    int run = 1;
    while (run) {
        char move;
        scanf("%c", &move);
        
        if (move > 90) move -= 32;

        int moveMade = 1;
        if (isMove(move)) {
            moveMade = makeMove(moves[getMove(move)][0], moves[getMove(move)][1]);
        }

        lastMove = move;
        clear();
        printBoard(); 
        if (move == 'Q' || !moveMade) run = 0;
    }

    printf("Game Over! Your score: %d", score);
    // freeMemory();

}

void showScore() {
    printf("Your score was: %d", score);
}

void askForValidInput() {
    printf("Please enter a valid input\n");
    scanf("%c", &c);
    check();
}

void check() {
    switch(c) {
        case 'P':
        startGame();
        break;
        case 'Q':
        showScore();
        break;
        default:
        askForValidInput();
        break;
    }
}

int main()
{
    srand(time(0));
    printf("Welcome\nUse WASD keys to control the snake. It moves when you enter a direction. You can quit the game at any time by pressing Q.\n1. Enter P to play the game\n2.Enter Q to exit.\n");
    scanf("%c", &c);
    check();
    return 0;
}