#include "movement.h"
extern coordinate actualCoordinate;
void down(){
	if(actualCoordinate.y > 0){
		actualCoordinate.y--;
		if(actualCoordinate.y == 2 || actualCoordinate.y == 0){
			if(actualCoordinate.x%5 < 2){
				while(map[actualCoordinate.x][actualCoordinate.y].segment == 'x'){
					actualCoordinate.x++;
				}
			}
			if(actualCoordinate.x%5 > 2){
				while(map[actualCoordinate.x][actualCoordinate.y].segment == 'x'){
					actualCoordinate.x--;
				}
			}
		}
	}

}

void up(){
	if(actualCoordinate.y < 4){
		actualCoordinate.y++;
		if(actualCoordinate.y == 2 || actualCoordinate.y == 4){
			if(actualCoordinate.x % 5 < 2){
				while(map[actualCoordinate.x][actualCoordinate.y].segment == 'x'){
			  		actualCoordinate.x++;
			  	}
			}
			 if(actualCoordinate.x % 5 > 2){
			  	while(map[actualCoordinate.x][actualCoordinate.y].segment == 'x'){
			  		actualCoordinate.x--;
			  	}
			 }
		}
	}
}
void left(){
	if(actualCoordinate.x > 0){
		actualCoordinate.x--;
		if(actualCoordinate.y % 2 == 0){
			if(actualCoordinate.x >= 6){
				actualCoordinate.x -= 4;
			}else{
				actualCoordinate.x = 2;
			}
		}
	}
}
void right(){
	if(actualCoordinate.x < 34){
			actualCoordinate.x++;
			if(actualCoordinate.y % 2 == 0){
				actualCoordinate.x += 4;
				if(actualCoordinate.x > 34){
					actualCoordinate.x = 32;
				}
			}
		}
}
