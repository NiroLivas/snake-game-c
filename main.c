#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>

/* September 4, 2026
Hey there!

I'm Niro, a first-year IT college student.

I learned the basics of C in a day and decided to put what
I learned into practice by making this Snake game. (The Tic Tac Toe one came prior)

I also applied my previous programming experience with:
- Luau (5 years)
- Python (1 year)
- JavaScript (1 year)

This project was made primarily through my own learning,
with some help from documentation and AI, particularly
when working with the WINAPI part.

There may be bugs or areas where the code could be improved,
but that's part of the learning process!

Have fun!
*/

int snake[100][2] = {{0,0}};
int highscore = 0;
int speed = 3;

int appleSpawned = 0;
int snakeLength = 15;

int option;
int appleX;
int appleY;

char direction = 'd';

const char controls[4][2] = {
	{'w', 'W'},
	{'s', 'S'},
	{'a', 'A'},
	{'d', 'D'},
};

char keyToMove(int key) {
	// WASD
	for(int i = 0; i < 4; i++) {
		if(key == controls[i][0] || key == controls[i][1]) 
			return controls[i][0];
	}
	
	// Arrow Keys
	switch(key) {
        case 72: return 'w'; // Up
        case 80: return 's'; // Down
        case 75: return 'a'; // Left
        case 77: return 'd'; // Right
	}
	
	return '\0';
}

DWORD WINAPI inputThread(LPVOID arg) {
    while (1) {
        if (_kbhit()) {
            int input = _getch();

            if (input == 0 || input == 224) {
                input = _getch();
            }

            char key = keyToMove(input);

            if (key != '\0') {
				if(snakeLength == 1) {
					direction = key;
					continue;
				}
            	
                if(((direction == 'w' && key != 's') ||
                    (direction == 's' && key != 'w') ||
                    (direction == 'a' && key != 'd') ||
                    (direction == 'd' && key != 'a'))) {
                    
                    direction = key;
                }
            }
        }
    }
    return 0;
}

int initiateSpeed() {
	if(speed == 1)
		return 1000000;
	else if (speed == 2)
		return 800000;
	else if (speed == 3)
		return 600000;
	else if (speed == 4)
		return 400000;
	else if (speed == 5)
		return 200000;
}

void speedConfiguration() {
	system("cls");
	puts("< * ---- Snake Game by Niro ---- * ");
	printf("Configuration - Speed (Currently: %d)\n", speed);
		
	puts("0 - Back");
	puts("1 - Very Slow");
   	puts("2 - Slow");
   	puts("3 - Normal");
   	puts("4 - Fast");
   	puts("5 - Very Fast");
   	printf("Input: ");
   	
   	scanf("%d", &option);
   	
   	if(option == 0)
   		return;
   	else if (option > 0 && option < 6)
   		speed = option;
   		
   	speedConfiguration();
}

int menu() {
	system("cls");
	puts("< * ---- Snake Game by Niro ---- * >");
	puts("Controls: WASD and ARROW KEYS");
	printf("Highscore: %d\n\n", highscore);
    puts("1 - Start");
    puts("2 - Speed Configuration");
    puts("3 - Exit");
    printf("Input: ");
    scanf("%d", &option);
    
    if(option == 2) {
    	speedConfiguration();
    	menu();
	} else if(option == 3) 
		return 0;	
}

void createApple() {
	if(appleSpawned == 0) {
		appleX = rand() % 20;
		appleY = rand() % 5;
		
		// Check if apple spawns on snake
		int interceptingBody = 0;
		for (int i = snakeLength - 1; i > 0; i--) {
		    snake[i][0] = snake[i - 1][0];
		    snake[i][1] = snake[i - 1][1];
		    
		    if(appleX == snake[i][0] && appleY == snake[i][1]) {
		    	interceptingBody = 1;
		    	break;
			}
		}
		
		if(interceptingBody) 
			createApple();
		else
			appleSpawned = 1;
	}
}

// Main Game
int startGame() {
	direction = 'd';
    srand(time(NULL));
    createApple();
    
    int sleepSpeed = initiateSpeed();
    int applesEaten = 0;
    int dead = 0;
    
    snakeLength = 1;
    
    for(int i = 0; i < 100; i++) {
    	snake[i][0] = 0;
    	snake[i][1] = 0;
	}

	while(1) {
		system("cls");
		
		// Move body
		for (int i = snakeLength - 1; i > 0; i--) {
			if(direction == '\0')
				break;
		    snake[i][0] = snake[i - 1][0];
		    snake[i][1] = snake[i - 1][1];
		}
		
		// Move Head
		if(direction == 'w')
			snake[0][1]--;
		else if(direction == 'a')
			snake[0][0]--;
		else if(direction == 's')
		    snake[0][1]++;
		else if(direction == 'd')
		    snake[0][0]++;

		// Drawing
		puts("*--------------------*");
		
		for (int y = 0; y < 5; y++) {
			putchar('|');
			
			for (int x = 0; x < 20; x++) {
    			char c = ' ';
    			// Snake
    			if(x == snake[0][0] && y == snake[0][1])
    				c = '@';
        		else {
        			// Creating Apple
    				if(x == appleX && y == appleY) {
    					c = '$';
					}          
					else
            			c = ' ';	
				}	           
    			for(int i = 1; i < snakeLength; i++) {
					if (x == snake[i][0] && y == snake[i][1]) 
						c = 'o';
				} 
				putchar(c);
    		}
    		putchar('|');
    		putchar('\n');
		}
		for (int i = 1; i < snakeLength; i++) {
			if (snake[0][0] == snake[i][0] &&
				snake[0][1] == snake[i][1]) {
				dead = 1;
				break;
			}
		}
		
		// Eating Apple
		if(snake[0][0] == appleX && snake[0][1] == appleY) {
		    appleSpawned = 0;
		    applesEaten++;
		
		    snakeLength++;
		
		    createApple();
		}

		puts("*--------------------*");
		printf("Score: %d\n", applesEaten);
		printf("Highscore: %d\n\n", highscore);
//		puts("-----Debugging-----");
//		printf("AppleX: %d\n", appleX);
//		printf("AppleY: %d\n\n", appleY);
//		printf("Snake X: %d\n", snake[0][0]);
//		printf("Snake Y: %d", snake[0][1]);

		// Death Logic
		if((snake[0][0] == 20 || snake[0][0] == -1) || (snake[0][1] == 5 || snake[0][1] == -1)) {
			puts("\n\n >>>>> You perished from a brick wall. <<<<<");
			break;
		} else if(dead) {
			puts("\n\n >>>>> You ran into your own body. <<<<<");
			break;
		}

		usleep(sleepSpeed);
	}
	
	if(applesEaten > highscore) {
		int newHighscore = applesEaten;
		
		printf("\n!! New Highscore: %d (Previous highscore: %d)\n", newHighscore, highscore);
		
		highscore = applesEaten;
	} else
		printf("Highscore: %d\n", highscore);
		
		
    puts("1 - Retry");
    puts("2 - Speed Configuration");
    puts("3 - Exit");
    printf("Input: ");
    scanf("%d", &option);
    
    if(option == 1) {
    	return 1;
	} else if(option == 2) {
    	return 2;
	} else if(option == 3) {
		return 3;
	}
}

int main() {
	
	// Get Data
	FILE *readData = fopen("data.txt", "r");
	
	fscanf(readData, "Highscore: %d\nSpeed: %d", &highscore, &speed);
	fclose(readData);
	
	// For user input
	CreateThread(
        NULL,
        0,
        inputThread,
        NULL,
        0,
        NULL
    );

    // Menu
  	int result;
	int gameResult;
	
	while ((result = menu()) != 0) {
	    gameResult = startGame();
	
	    while (gameResult == 1) {
	        gameResult = startGame();
	    }
	
	    if (gameResult == 2) {
	        speedConfiguration();
	    } else if (gameResult == 3) {
	        break;
	    }
	}
	
	FILE *data = fopen("data.txt", "w");
	fprintf(data, "Highscore: %d\n", highscore);
	fprintf(data, "Speed: %d\n", speed);
	fclose(data);
	
	return 0;
}