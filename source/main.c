#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
	size_t number;
	bool merged;
}tile;

tile grid[4][4];

enum shift{LEFT, RIGHT, UP, DOWN};
bool step(int way);
bool spawnnew(void);

int main(void){
	srand(time(0));
	
	putgrid(1, 0, 0);
	putgrid(1, 1, 0);
	putgrid(1, 2, 0);
	putgrid(1, 3, 0);
	
	putgrid(1, 0, 1);
	putgrid(1, 1, 1);
	putgrid(1, 2, 1);
	putgrid(1, 3, 1);

	putgrid(1, 0, 2);
	putgrid(1, 1, 2);
	putgrid(1, 2, 2);
	putgrid(1, 3, 2);
	
	putgrid(1, 0, 3);
	putgrid(1, 1, 3);
	putgrid(1, 2, 3);
	putgrid(1, 3, 3);
	
	printgrid();
	step(RIGHT);
	printf("\nshifted right:\n\n");
	printgrid();
	step(LEFT);
	printf("\nshifted left:\n\n");
	printgrid();
	step(UP);
	printf("\nshifted up:\n\n");
	printgrid();
	step(DOWN);
	printf("\nshifted down:\n\n");
	printgrid();
	return 0;
}

bool step(int way){
	bool eat = true;
	switch(way){
		case RIGHT:
			for(int row = 0; row < 4; row++){
				while(eat){
					eat = false;
					for(int col = 3; col >= 1; col--){
						if(grid[row][col].number == 0){
							if(grid[row][col-1].number > 0){
								grid[row][col].number = grid[row][col-1].number;
								grid[row][col].merged = grid[row][col-1].merged;
								grid[row][col-1].merged = false;
								grid[row][col-1].number = 0;
								eat = true;
							}
						}else if(grid[row][col].number == grid[row][col-1].number){
							if(!(grid[row][col].merged | grid[row][col-1].merged)){
								grid[row][col-1].number = 0;
								grid[row][col].number++;
								grid[row][col].merged = true;
								eat = true;	
							}
						}
					}
				}
				eat = true;
			}
			break;
		case DOWN:
			for(int col = 0; col < 4; col++){
				while(eat){
					eat = false;
					for(int row = 3; row >= 1; row--){
						if(grid[row][col].number == 0){
							if(grid[row-1][col].number > 0){
								grid[row][col].number = grid[row-1][col].number;
								grid[row][col].merged = grid[row-1][col].merged;
								grid[row-1][col].merged = false;
								grid[row-1][col].number = 0;
								eat = true;
							}
						}else if(grid[row][col].number == grid[row-1][col].number){
							if(!(grid[row][col].merged | grid[row-1][col].merged)){
								grid[row-1][col].number = 0;
								grid[row][col].number++;
								grid[row][col].merged = true;
								eat = true;	
							}
						}
					}
				}
				eat = true;
			}
			break;
		case LEFT:
			for(int row = 0; row < 4; row++){
				while(eat){
					eat = false;
					for(int col = 0; col <= 2; col++){
						if(grid[row][col].number == 0){
							if(grid[row][col+1].number > 0){
								grid[row][col].number = grid[row][col+1].number;
								grid[row][col].merged = grid[row][col+1].merged;
								grid[row][col+1].merged = false;
								grid[row][col+1].number = 0;
								eat = true;
							}
						}else if(grid[row][col].number == grid[row][col+1].number){
							if(!(grid[row][col].merged | grid[row][col+1].merged)){
								grid[row][col+1].number = 0;
								grid[row][col].number++;
								grid[row][col].merged = true;
								eat = true;	
							}
						}
					}
				}
				eat = true;
			}
			break;
		case UP: 
			for(int col = 0; col < 4; col++){
				while(eat){
					eat = false;
					for(int row = 0; row <= 2; row++){
						if(grid[row][col].number == 0){
							if(grid[row+1][col].number > 0){
								grid[row][col].number = grid[row+1][col].number;
								grid[row][col].merged = grid[row+1][col].merged;
								grid[row+1][col].merged = false;
								grid[row+1][col].number = 0;
								eat = true;
							}
						}else if(grid[row][col].number == grid[row+1][col].number){
							if(!(grid[row][col].merged | grid[row+1][col].merged)){
								grid[row+1][col].number = 0;
								grid[row][col].number++;
								grid[row][col].merged = true;
								eat = true;	
							}
						}
					}
				}
				eat = true;
			}
			break;
		default:
			break;
	}
	
	
	for(int col = 0; col < 4; col++){
		for(int row = 0; row < 4; row++){
			grid[row][col].merged = false;
		}
	}
			
	return true;
}

void printgrid(void){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(grid[y][x].number){
				printf("%i\t", 1 << grid[y][x].number);
			}else{
				printf("0\t");
			}
			
		}
		printf("\n\n");
	}
}

void putgrid(size_t num, int x, int y){
	grid[y][x].number = num;
	grid[y][x].merged = false;
}

bool spawnnew(void){
	
	int spawn_x = 0;
	int spawn_y = 0;
	bool indexnotgood = true;
	
	while(indexnotgood){
		spawn_x++;
		if(spawn_x > 3){
			if(spawn_y < 3){
				spawn_y++;
				spawn_x = 0;
			}else{
				return false;
			}
		}
		indexnotgood = grid[spawn_y][spawn_x].number;
	}
	
	grid[spawn_y][spawn_x].number = 1;
	return true;
	
};
