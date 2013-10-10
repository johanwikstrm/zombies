#include "constants.h"

char * kindstr(int kind){
	switch(kind){
		case EMPTY:
		return "EMPTY";
		break;
		case HUMAN:
		return "HUMAN";
		break;
		case ZOMBIE:
		return "ZOMBIE";
		break;
		case INFECTED:
		return "INFECTED";
		break;
	}
	return "UNKNOWN KIND";
}
