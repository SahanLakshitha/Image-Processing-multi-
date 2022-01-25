#include <pthread.h>
#include <iostream>
#include"lodepng.h"
#include"lodepng.cpp"
using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	unsigned int error;
	unsigned int encError;
	unsigned char* png;
	unsigned int h;
	unsigned int w;

	int Arraysize;

	int i;
	int matrix;
	struct Markers
	{
		int start;
		int end;
		int no;
	};

const char* filename = "Allied.png";
const char* newFileName= "blur.png";

void *tFunction1(void *arg)
{
	error = lodepng_decode32_file(&png, &w, &h, filename);


	pthread_mutex_lock(&mutex);
	if(error){
		cout << "Error " << error << ": " << lodepng_error_text(error);
	}

	Arraysize = 4*h*w;

	matrix = (8 + ((h - 1) * 4)) + (h * w);


	pthread_mutex_unlock(&mutex);

	return NULL;
}

void *tFunction2(void *arg)
{
	pthread_mutex_lock(&mutex);
    cout << "Processing....";

	int** pMatrix = new int*[matrix];
    for(int q = 0; q < matrix; ++q){
    	pMatrix[q] = new int[6];
	}

	int count = 0;

	for(int y = h+1; y >= 0; y--)
	{
		for(int x = 0; x < w+1; x++)
		{

				pMatrix[count][0] = x;
				pMatrix[count][1] = y;
			count++;
		}

	}

	count = 0;

	for(i = 0; count < matrix; i=i+4)
	{
		if((pMatrix[count][0] == 0) || (pMatrix[count][1] == 0) || (pMatrix[count][0] == w+1) || (pMatrix[count][1] == h+1))
		{
			pMatrix[count][2] = 0;
			pMatrix[count][3] = 0;
			pMatrix[count][4] = 0;
			pMatrix[count][5] = 0;
			i=i-4;
			count++;
		}
		else
		{
			pMatrix[count][2] = png[0+i];
			pMatrix[count][3] = png[1+i];
			pMatrix[count][4] = png[2+i];
			pMatrix[count][5] = png[3+i];
			count++;
		}
	}


    count = 0;

    int tRed = 0;
    int tBlue = 0;
    int tGreen = 0;

    int R[matrix] = {};
    int G[matrix] = {};
    int B[matrix] = {};
    int RGB[Arraysize] = {};

    i = 0;

    int q = 0;

    int dRed = 9;
    int dGreen = 9;
    int dBlue = 9;

    int s = 0;
    int arrCount = 0;

    for (int x = 0; x <= matrix-(h+w); x++)
	{
        for (int y = 0; y < 6; y++)
		{
        	if(!((pMatrix[count][2] == 0) && (pMatrix[count][3] == 0) && (pMatrix[count][4] == 0) && (pMatrix[count][5] == 0)))
			{

            	if(y == 2)
				{
            		tRed += pMatrix[x][y] * 1;
            		if(pMatrix[x][y] == 0){
						dRed -= 1;
					}
					tRed += pMatrix[x+1][y] * 1;
					if(pMatrix[x+1][y] == 0)
					{
						dRed -= 1;
					}
					tRed += pMatrix[x-1][y] * 1;
					if(pMatrix[x-1][y] == 0)
					{
						dRed -= 1;
					}
            		for (int a = 0; a < matrix; a++)
					{
        				if((pMatrix[x][0] == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] + 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dRed -= 1;
							}
							tRed += pMatrix[a][y] * 1;
						}
					}
            		for (int a = 0; a < matrix; a++)
					{
        				if((pMatrix[x][0] == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] - 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dRed -= 1;
							}
							tRed += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] + 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] + 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dRed -= 1;
							}
							tRed += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] - 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] - 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dRed -= 1;
							}
							tRed += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] - 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] + 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dRed -= 1;
							}
							tRed += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] + 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] - 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dRed -= 1;
							}
							tRed += pMatrix[a][y] * 1;
						}
					}
				}

				if(y == 3)
				{
					tGreen += pMatrix[x][y] * 1;
            		if(pMatrix[x][y] == 0)
					{
						dGreen -= 1;
					}
					tGreen += pMatrix[x+1][y] * 1;
					if(pMatrix[x+1][y] == 0)
					{
						dGreen -= 1;
					}
            		tGreen += pMatrix[x-1][y] * 1;
					if(pMatrix[x-1][y] == 0)
					{
						dGreen -= 1;
					}
            		for (int a = 0; a < matrix; a++)
					{
        				if((pMatrix[x][0] == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] + 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dGreen -= 1;
							}
							tGreen += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if((pMatrix[x][0] == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] - 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dGreen -= 1;
							}
							tGreen += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] + 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] + 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dGreen -= 1;
							}
							tGreen += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] - 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] - 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dGreen -= 1;
							}
							tGreen += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] - 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] + 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dGreen -= 1;
							}
							tGreen += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] + 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] - 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dGreen -= 1;
							}
							tGreen += pMatrix[a][y] * 1;
						}
					}
				}

				if(y == 4)
				{
            		tBlue += pMatrix[x][y] * 1;
            		if(pMatrix[x][y] == 0)
					{
						dBlue -= 1;
					}
					tBlue += pMatrix[x+1][y] * 1;
					if(pMatrix[x+1][y] == 0)
					{
						dBlue -= 1;
					}
					tBlue += pMatrix[x-1][y] * 1;
					if(pMatrix[x-1][y] == 0)
					{
						dBlue -= 1;
					}
            		for (int a = 0; a < matrix; a++)
					{
        				if((pMatrix[x][0] == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] + 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dBlue -= 1;
							}
							tBlue += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if((pMatrix[x][0] == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] - 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dBlue -= 1;
							}
							tBlue += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] + 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] + 1)))
						{
							if(pMatrix[a][y] == 0)
							{
								dBlue -= 1;
							}
							tBlue += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] - 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] - 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dBlue -= 1;
							}
							tBlue += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] - 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] + 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dBlue -= 1;
							}
							tBlue += pMatrix[a][y] * 1;
						}
					}

            		for (int a = 0; a < matrix; a++)
					{
        				if(((pMatrix[x][0] + 1) == pMatrix[a][0]) && (pMatrix[a][1] == (pMatrix[x][1] - 1)))
						{
            				if(pMatrix[a][y] == 0)
							{
								dBlue -= 1;
							}
							tBlue += pMatrix[a][y] * 1;
						}
					}
				}
            }
        }
        if(!((tRed == 0) && (tGreen == 0) && (tBlue == 0)))
		{
        	if(dRed != 0)
			{
				RGB[arrCount] = tRed/dRed;
				arrCount++;
			}
        	if(dGreen != 0)
			{
				RGB[arrCount] = tGreen/dGreen;
				arrCount++;
			}
        	if(dBlue != 0)
			{
        		RGB[arrCount] = tBlue/dBlue;
        		arrCount++;
			}
			RGB[arrCount] = 255;
			arrCount++;
        	s += 4;
			q++;
		}

        count++;

		dRed = 9;
		dGreen = 9;
		dBlue = 9;


        tRed = 0;
        tGreen = 0;
        tBlue = 0;

    }

	for(i = 0; i < Arraysize; i=i+4)
	{

			png[0+i] = RGB[i];
			png[1+i] = RGB[1 + i];
			png[2+i] = RGB[2 + i];
			png[3+i] = RGB[3 + i];
	}

	cout <<endl;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(void)
{
	pthread_t pth, pth0, pth1, pth2, pth3;

	pthread_create(&pth, NULL, tFunction1, NULL);
	pthread_join(pth, NULL );


	struct Markers Positions[4];
	Positions[0].start = 0;
	Positions[0].end = (Arraysize/4) -1;
	Positions[0].no = 1;
	Positions[1].start =Arraysize/4;
	Positions[1].end = (Arraysize/2) -1;
	Positions[1].no = 2;
	Positions[2].start =Arraysize/2;
	Positions[2].end = (Arraysize*3/4) -1;
	Positions[2].no = 3;
	Positions[3].start =Arraysize*3/4;
	Positions[3].end = Arraysize;
	Positions[3].no = 4;


    pthread_create(&pth0,NULL,tFunction2,&Positions[0]);
	pthread_create(&pth1,NULL,tFunction2,&Positions[1]);
	pthread_create(&pth2,NULL,tFunction2,&Positions[2]);
	pthread_create(&pth3,NULL,tFunction2,&Positions[3]);
	pthread_join(pth0, NULL );
    pthread_join(pth1, NULL );
    pthread_join(pth2, NULL );
    pthread_join(pth3, NULL );

    encError = lodepng_encode32_file(newFileName, png, w, h);
	if(encError)
	{
		cout << "Error " << error << ": " << lodepng_error_text(encError);
	}
	free(png);
    cout << "Filter applied.";

	return 0;
}
