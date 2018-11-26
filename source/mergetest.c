#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2\SDL.h>

typedef struct{
	size_t number;
	bool merged;
	SDL_Texture * image;
	SDL_Rect drawto;
}tile;

enum shift{NOTHING, LEFT, RIGHT, UP, DOWN};
int currentshift = NOTHING;
bool step(int way);
bool spawnnew(void);
SDL_Window * window;
SDL_Renderer * renderer;
SDL_Event event;
SDL_Color clearcolor;
SDL_Surface * picture; 

tile grid[4][4];
SDL_Texture * numbers[12];
char filelist[12][17] = {
"numbers/0.bmp   ",
"numbers/2.bmp   ",
"numbers/4.bmp   ",
"numbers/8.bmp   ",
"numbers/16.bmp  ",
"numbers/32.bmp  ",
"numbers/64.bmp  ",
"numbers/128.bmp ",
"numbers/256.bmp ",
"numbers/512.bmp ",
"numbers/1024.bmp",
"numbers/2048.bmp"
};

size_t screenwidth = 640;
size_t screenheight = 480;
bool quit = false;
void loop(void);

int main(int argc, char ** argv){

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL Was Unable To Start\n");
		return 1;
	}
	
	window = SDL_CreateWindow("Game Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenwidth, screenheight, SDL_WINDOW_RESIZABLE);
	if(window == NULL){
		printf("SDL Could Not Initialize Window\n");
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		printf("SDL Could Not Initialize Renderer\n");
		return 1;
	}
	
	for(int iter = 0; iter < 12; iter++){
		picture = SDL_LoadBMP(filelist[iter]);
		if(picture == NULL){
			printf("Couldnt Load Image %s\n", filelist[iter]);
			return 1;
		}
		numbers[iter] = SDL_CreateTextureFromSurface(renderer, picture);
		if(numbers[iter] == NULL){
			printf("Couldnt Make Texture\n");
			return 1;
		}
	}
		
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
				grid[y][x].image = numbers[0];
		}
	}
	adjusttilescale();
	setclearcolor(255,255,255);
	spawnnew();
	spawnnew();
	while(!quit){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_CLOSE:
						quit = true;
						break;
					case SDL_WINDOWEVENT_RESIZED:
						SDL_GetWindowSize(window, &screenwidth, &screenheight);
						adjusttilescale();
					default: 
						break;
				}
			}else if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					case SDLK_UP:
						currentshift = UP;
						break;
					case SDLK_DOWN:
						currentshift = DOWN;
						break;
					case SDLK_RIGHT:
						currentshift = RIGHT;
						break;
					case SDLK_LEFT:
						currentshift = LEFT;
						break;
					default: 
						break;
				}
			}
		}
		if(!quit){
			quit = !step(currentshift);
		}
		
		loop();
		currentshift = NOTHING;
	}
	
	printf("game over\n");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	close();

	return 0;
}

void loop(void){
	clear();
	drawgrid();
	flip();
}

void drawbox(int x, int y, int w, int h){
	SDL_Rect temp = (SDL_Rect) {.x = x, .y = y, .w = w, .h = h};
	SDL_RenderDrawRect(renderer, &temp);
}
void drawline(int p1x, int p1y, int p2x, int p2y){
	SDL_RenderDrawLine(renderer, p1x, p1y, p2x, p2y);
}

void setclearcolor(Uint8 r, Uint8 g, Uint8 b){
	clearcolor.r = r;
	clearcolor.g = g;
	clearcolor.b = b;
}

void setcolor(Uint8 r, Uint8 g, Uint8 b){
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void clear(void){
	setcolor(clearcolor.r, clearcolor.g, clearcolor.b);
	SDL_RenderClear(renderer);
}

void flip(void){
	SDL_RenderPresent(renderer);
}

void adjusttilescale(void){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
				grid[y][x].drawto.x = x * (screenwidth >> 2);
				grid[y][x].drawto.y = y * (screenheight >> 2);
				grid[y][x].drawto.w = screenwidth >> 2;
				grid[y][x].drawto.h = screenheight >> 2;
		}
	}
}

void drawgrid(void){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			SDL_RenderCopy(renderer, grid[y][x].image, NULL, &(grid[y][x].drawto));
		}
	}
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
	
	bool spawned;
	if(way != NOTHING){
		spawned = spawnnew();
		if(!spawned){
			return false;
		}
	}
	updategrid();	
	return true;
}

void updategrid(void){
	for(int col = 0; col < 4; col++){
		for(int row = 0; row < 4; row++){
			grid[row][col].merged = false;
			grid[row][col].image = numbers[grid[row][col].number];
		}
	}
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
	
	size_t indexnotgood = false;
	int spawn_y;
	int spawn_x;
	
	for(spawn_y = 0;spawn_y < 4;spawn_y++){
		for(spawn_x = 0;spawn_x < 4;spawn_x++){
			indexnotgood = grid[spawn_y][spawn_x].number;
			if(!indexnotgood){
				break;
			}
		}	
		if(!indexnotgood){
			break;
		}
	}
	
	if(indexnotgood){
		
		return false;
	}else{
		grid[spawn_y][spawn_x].number = 1;
		updategrid();	
		return true;
	}
	
};
