#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2\SDL.h>
#define defaultcols 10
#define defaultrows 10
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
size_t gridcols = defaultcols;
size_t gridrows = defaultrows;
tile ** grid;
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

size_t screenwidth = 800;
size_t screenheight = 800;
bool quit = false;
void loop(void);

void makegrid(size_t sizey, size_t sizex){
	

	grid = (tile **)malloc(sizeof(tile *)*sizey);
	for(size_t iter = 0; iter < sizey;iter++){
		grid[iter] = (tile *)malloc(sizeof(tile)*sizex);
	}
	
	for(int y = 0; y < gridcols; y++){
		for(int x = 0; x < gridrows; x++){
			grid[y][x].number = 0;
			grid[y][x].merged = false;
			grid[y][x].image = numbers[0];
		}
	}
	adjusttilescale();
	
}

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
	makegrid(defaultcols, defaultrows);
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
				if(!gameover){
					switch(event.key.keysym.sym){
						case SDLK_UP:
							gameover = step(UP);
							break;
						case SDLK_DOWN:
							gameover = step(DOWN);
							break;
						case SDLK_RIGHT:
							gameover = step(RIGHT);
							break;
						case SDLK_LEFT:
							gameover = step(LEFT);
							break;
						default: 
							break;
					}
				}
			}
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
	for(int y = 0; y < gridcols; y++){
		for(int x = 0; x < gridrows; x++){
				grid[y][x].drawto.x = x * (screenwidth / gridcols);
				grid[y][x].drawto.y = y * (screenheight / gridrows);
				grid[y][x].drawto.w = screenwidth / gridcols;
				grid[y][x].drawto.h = screenheight / gridrows;
		}
	}
}

void drawgrid(void){
	for(int y = 0; y < gridcols; y++){
		for(int x = 0; x < gridrows; x++){
			SDL_RenderCopy(renderer, grid[y][x].image, NULL, &(grid[y][x].drawto));
		}
	}
}

bool step(int way){
	bool eat = true;
	switch(way){
		case RIGHT:
			for(int row = 0; row < gridrows; row++){
				while(eat){
					eat = false;
					for(int col = gridcols-1; col >= 1; col--){
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
								if(number == 11){
									gameover = true;
									strcpy(endmessage, "YOU WIN");
								}
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
			for(int col = 0; col < gridcols; col++){
				while(eat){
					eat = false;
					for(int row = gridrows-1; row >= 1; row--){
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
			for(int row = 0; row < gridrows; row++){
				while(eat){
					eat = false;
					for(int col = 0; col <= gridcols-2; col++){
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
			for(int col = 0; col < gridcols; col++){
				while(eat){
					eat = false;
					for(int row = 0; row <= gridrows-2; row++){
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
	
	updategrid();
	bool cancontinue;
	if(way != NOTHING){
		cancontinue = addtile() || canmakemove;
		return cancontinue;
	}
	return true;
}

void updategrid(void){
	for(int col = 0; col < gridcols; col++){
		for(int row = 0; row < gridrows; row++){
			grid[row][col].merged = false;
			grid[row][col].image = numbers[grid[row][col].number];
		}
	}
}

void printgrid(void){
	for(int y = 0; y < gridcols; y++){
		for(int x = 0; x < gridrows; x++){
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

bool addtile(void){
	
	size_t indexnotgood = false;
	int spawn_y;
	int spawn_x;
	
	for(spawn_y = 0;spawn_y < gridcols;spawn_y++){
		for(spawn_x = 0;spawn_x < gridrows;spawn_x++){
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
