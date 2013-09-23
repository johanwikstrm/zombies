//#include <random>
//#include <chrono>
#include <stdlib.h>
#include <iostream>

#define WIDTH 10
#define HEIGHT 10

#define POP_DENSITY 0.05


using namespace std;

//void init(int * matrix, uniform_real_distribution<double> distr,mt19937_64 twister){
void init(int * matrix){
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH;x++){
            //if (distr(twister) < POP_DENSITY)
            if (drand48() < POP_DENSITY)
            {
                matrix[y*WIDTH+x] = 1;
            }	
        }
    }
}

//void move(int *matrix, uniform_real_distribution<double> distr,mt19937_64 twister){
void move(int *matrix){
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH;x++){
            int xy = y*WIDTH+x;
            if (matrix[xy] == 1)
            {
                //double r = distr(twister);
                double r = drand48();
                int xy2;
                if (r < 0.2) // left
                {
                    xy2 = y*WIDTH + (x-1+WIDTH)%WIDTH;
                }else if (r < 0.4) // right
                {
                    xy2 = y*WIDTH + (x+1)%WIDTH;
                }else if (r < 0.6) // up
                {
                    xy2 = ((y-1+HEIGHT)%HEIGHT)*WIDTH + x;
                }else if (r < 0.8) //down
                {
                    xy2 = ((y+1)%HEIGHT)*WIDTH + x;
                }
                // otherwise, no move

                if (matrix[xy2] != 1) // no collision
                {
                    matrix[xy] = 0;
                    matrix[xy2] = 1;
                }
            }	
        }
    }
}

void print(int *matrix){
    cout << "-----------------------------\n";
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            cout << " "<< matrix[y*WIDTH+x];
        }
        cout << endl;
    }
}


int main(int argc, char const *argv[])
{
    int *matrix;
    /**
      unsigned seed = chrono::system_clock::now().time_since_epoch().count();
      mt19937_64 twister (seed);
      uniform_real_distribution<double> distr(0,1);
      */
    matrix = (int*)calloc(WIDTH*HEIGHT,sizeof(int));
    //init(matrix,distr,twister);
    init(matrix);
    print(matrix);

    for (int i = 0; i < 5; i++) {
        //move(matrix,distr,twister);
        move(matrix);
        print(matrix);
    }

    free(matrix);
    return 0;
}
