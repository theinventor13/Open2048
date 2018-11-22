#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

size_t grid[4][4] = {
{0,0,0,0},
{0,0,0,0},
{0,0,0,0},
{0,0,0,0}};

enum shift{LEFT, RIGHT, UP, DOWN};

bool step(int way);
bool spawnnew(void);

int main(void){
	srand(time(0));
	
	putgrid(2, 0, 0);
	putgrid(4, 1, 0);
	putgrid(2, 2, 0);
	putgrid(2, 3, 0);
	
	putgrid(2, 0, 1);
	putgrid(4, 1, 1);
	putgrid(4, 2, 1);
	putgrid(2, 3, 1);
	
	putgrid(16, 0, 2);
	putgrid(16, 1, 2);
	putgrid(32, 2, 2);
	putgrid(8, 3, 2);
	
	putgrid(4, 0, 3);
	putgrid(4, 1, 3);
	putgrid(2, 2, 3);
	putgrid(8, 3, 3);
	
	printgrid();
	step(LEFT);
	printf("\nshifted left:\n\n");
	printgrid();
	step(UP);
	printf("\nshifted up:\n\n");
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
						if(grid[row][col] == 0){
							if(grid[row][col-1] > 0){
								grid[row][col] = grid[row][col-1];
								grid[row][col-1] = 0;
								eat = true;
							}
						}else if(grid[row][col] == grid[row][col-1]){
							grid[row][col-1] = 0;
							grid[row][col] += grid[row][col];
							eat = true;
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
						if(grid[row][col] == 0){
							if(grid[row-1][col] > 0){
								grid[row][col] = grid[row-1][col];
								grid[row-1][col] = 0;
								eat = true;
							}
						}else if(grid[row][col] == grid[row-1][col]){
							grid[row-1][col] = 0;
							grid[row][col] += grid[row][col];
							eat = true;
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
						if(grid[row][col] == 0){
							if(grid[row][col+1] > 0){
								grid[row][col] = grid[row][col+1];
								grid[row][col+1] = 0;
								eat = true;
							}
						}else if(grid[row][col] == grid[row][col+1]){
							grid[row][col+1] = 0;
							grid[row][col] += grid[row][col];
							eat = true;
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
						if(grid[row][col] == 0){
							if(grid[row+1][col] > 0){
								grid[row][col] = grid[row+1][col];
								grid[row+1][col] = 0;
								eat = true;
							}
						}else if(grid[row][col] == grid[row+1][col]){
							grid[row+1][col] = 0;
							grid[row][col] += grid[row][col];
							eat = true;
						}
					}
				}
				eat = true;
			}
			break;
		default:
			break;
	}
	return true;
}

void printgrid(void){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			printf("%i \t",grid[y][x]);
		}
		printf("\n\n");
	}
}

void putgrid(size_t num, int x, int y){
	grid[y][x] = num;
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
		indexnotgood = grid[spawn_y][spawn_x];
	}
	
	grid[spawn_y][spawn_x] = 2;
	return true;
	
};
