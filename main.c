#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <conio.h>



#define WIDTH 20
#define HEIGHT 15

void updateHistory();


struct lego { // อันที่จะร่วงลงมา
    int x, y; 
    int width, height;  
}*playingLego, legoHistory;

char keyInput;

int score = 0;
bool gameOver = false;

char map[HEIGHT][WIDTH] = {
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "####################"
};


void resetMap() {
    for (int i=0;i<HEIGHT;i++) {
        for (int y=0;y<WIDTH;y++) {
            if (y == 0 || y == WIDTH-1) {
                map[i][y] = '#';
            }else if (i==HEIGHT-1) {
                map[i][y] = '#';
            }else {
                map[i][y] = ' ';
            }
        }
        
    }
}
 
void displayMap() {
    for (int row=0;row<HEIGHT;row++) {
        printf("\t");
        for (int col=0;col<WIDTH;col++) {
            printf(" %c", map[row][col]);
        }
        printf("\n");
    }
    printf("x = %d y = %d", playingLego->x, playingLego->y);
    printf("\nscore = %d", score);
}

struct lego *newLego() {
    struct lego *l = malloc(sizeof(struct lego));
    
    l->height = rand() % 4 + 1;
    l->width = 5-l->height; //ถ้าสูง 4 ต้องกว้าง 1 ถ้าสูง 3 ต้องกว้าง 2
    l->x = rand() % (WIDTH-(l->width*2)) + l->width;
    l->y = 0;
    return l;
}   

void cleanHistory() {
    if (legoHistory.x != 0) {
        for (int i=0;i<legoHistory.height;i++) {
            map[legoHistory.y+i][legoHistory.x] = ' ';
        }
        for (int k=0;k<legoHistory.width;k++) {
            map[legoHistory.y+legoHistory.height-1][legoHistory.x+k] = ' ';
        }
    }
}

void updateHistory() {
    legoHistory.x = playingLego->x;
    legoHistory.y = playingLego->y;
    legoHistory.height = playingLego->height;
    legoHistory.width =playingLego->width;
}

bool collided() {
    //loop ตามแนวกว้าง
    for (int i=0;i<playingLego->width;i++) {
        if (map[playingLego->y+playingLego->height-1][playingLego->x+i] == '*' //ชน pixel อื่น หรือชนกำแพง
        || map[playingLego->y+playingLego->height-1][playingLego->x+i] == '#') {
            return true;
        }
    }
    return false;
    
}

bool isGameOver() {
    return playingLego->y == 0 && collided();
}


void updateMap() {

    

    playingLego->y++;
    
    if (collided()) {
        score += 10;
        if(playingLego->y == 1) { // ถ้าชนแล้ว ติดขอบ
            gameOver = true;
        }
        playingLego = newLego();
        updateHistory();
    }

    cleanHistory();
    updateHistory();

    for (int i=0; i<HEIGHT; i++) {
        for (int y=0; y<WIDTH; y++) {
            if (i == playingLego->y && y == playingLego->x) {
                
                for (int j=0;j<playingLego->height;j++) {
                    map[i+j][y] = '*';    
                }

                for (int k=0;k<playingLego->width;k++) {
                    map[i+playingLego->height-1][y+k] = '*';
                }
            }
            
        }
    }

}



void input() {

 
    if ( kbhit() ) {
 
        // Stores the pressed key in ch
        keyInput = getch();
        if (keyInput == 'a' && playingLego->x > 1) {
            playingLego->x--;
        }

        if (keyInput == 'd' && playingLego->x+playingLego->width-1 < WIDTH -1) {
            playingLego->x++;
        }
    }
    
}



int main() {
    playingLego = newLego();

    system("color a");

    while (!gameOver) {
        updateMap();
        input();
        displayMap();
        usleep(500000);
        system("cls");
    }
    
    system("color c");
    printf("your score = %d", score);
    printf("\nthanks for play me!");
    getch();
    

    return 0;
}