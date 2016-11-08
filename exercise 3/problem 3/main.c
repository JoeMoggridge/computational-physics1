/////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge computational physics ex3 problem 3
//
//calculating the potential due to two rectangular rods of opposites charge in a rectangular box.
//program works fine when debugging, but when running normally, some elements become massive.
/////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

double **Relaxation (double **v_old, double **v_new);

int main()
{
    int i, x,y;

    //each array is 2 bigger than you'd expect. This is so the boundary's can be set at zero.

    //create v_old
    double **v_old=(double**) malloc( 32*sizeof(double));
    for (i=0; i<=31; i++)
        v_old [i] = (double*) malloc (17*sizeof(double));

    //create v_new
    double **v_new=(double**) malloc( 32*sizeof(double));
    for (i=0; i<=31; i++)
        v_new [i] = (double*) malloc (17*sizeof(double));

    //check arrays were created
    if (v_new==NULL ||v_old==NULL)
    {//allocation failed
        printf("memory allcocation failed.\nEnding Program....\n\n");

        {//free arrays
            for (i = 0; i < 32; i++)
            {
                free(v_new[i]);
                free(v_old[i]);
            }
            free(v_new);
            free(v_old);
        }

        return 1;
    }

    else
    {//arrays succesfully created, program continues

        printf("\nArrays created.\n");

        //initialise both arrays
        for (x=0; x<=31 ; x++)
        {
            for (y=0; y<=16; y++)
            {
                //if in conductors then set to appropriate values.
                if (x>6 && x<=9 && y>6 && y<=9)
                {
                    v_new[x][y]= -10;
                    v_old[x][y]= -10;
                }
                else if (x>21 && x<=24 && y>6 && y<=9)
                {
                    v_new[x][y]= 10;
                    v_old[x][y]= 10;
                }
                //if at edges then set to zero
                else if (x==31 || x==0 ||y==0|| y==16)
                {
                    v_new [x][y]=0;
                    v_old[x][y]= 0;
                }
                //else initialise to 1
                else
                {
                    v_new[x][y]= 1;
                    v_old[x][y]= 1;
                }

            }
        }

        printf("Arrays initialsied\n");

        //output the array
        FILE *initial_array;
        initial_array= fopen("initial_array.txt", "w");
        if (initial_array==NULL)
        {//file was not opened
            printf("\nInitial array file was not created, \nEnding program....\n\n");

            {//free arrays
            for (i = 0; i < 32; i++)
            {
                free(v_new[i]);
                free(v_old[i]);
            }
            free(v_new);
            free(v_old);
            }

            return 1;
        }
        else
        {//file succesfully created, program continues

            //output array
            for (y=0; y<17 ; y++)
            {
                fprintf (initial_array, "\n");


                for (x=0; x<32; x++)
                {
                    fprintf(initial_array, "%lf \t", v_new[x][y]);
                }
            }
            fclose(initial_array);
        }

        printf("initial array outputted\n");

        int num_converged=0;//records how many cells have converged.

        //relaxation
        while (num_converged < 544)
        {
            v_new = Relaxation(v_old, v_new);

            //reset num_converged
            num_converged=0;

            //check for convergence
            for (x=0; x<32 ; x++)
            {
                for (y=0; y<17; y++)
                {
                    if (abs(v_new[x][y]- v_old[x][y])<0.000001 && v_new[x][y]!=1 && v_new [1][1]<=0)//program would often finish before convergence had occured since most of the initial values would not change. The AND statement prevbents this. last staement is a messy way of making it properly converge.
                        num_converged++;
                }
            }

            //copy v_new into v_old
            for (x=0; x<32 ; x++)
            {
                for (y=0; y<17; y++)
                {
                    v_old[x][y]=v_new[x][y];
                }
            }

            printf("number of converged elements= %d \n", num_converged);
        }
        printf("Array has converged\n");

        //output the array
        FILE *final_array;
        final_array= fopen("potential.txt", "w");
        if (final_array==NULL)
        {//file was not opened
            printf("potential file was not created,\nEnding Program.\n\n");
            //program now jumps to end, hence it is unnecesary to free all array here.
        }
        else
        {//file succesfully created, program continues

            //output array
            for (y=0; y<17 ; y++)
            {
                fprintf (final_array, "\n");

                for (x=0; x<32; x++)
                {
                    fprintf(final_array, "%lf \t", v_new[x][y]);
                }
            }
            fclose(final_array);
            printf("final array outputted\n");
        }

    }


    {//free arrays
        for (i = 0; i < 4; i++)
        {
            free(v_new[i]);
            free(v_old[i]);
        }
        free(v_new);
        free(v_old);
    }
    return 0;
}

double **Relaxation (double **v_old, double **v_new)
{
    int x,y;
    for (x=0; x<=31 ; x++)
    {
        for (y=0; y<=16; y++)
        {
            //if in conductors then set to appropriate values.
            if (x>6 && x<=9 && y>6 && y<=9)
                v_new[x][y]= -10;
            else if (x>21 && x<=24 && y>6 && y<=9)
                v_new[x][y]= 10;
            //if at edges then set to zero
            else if (x==31 || x==0 ||y==0|| y==16)
                v_new [x][y]=0;
            //else do relaxation
            else
                v_new[x][y]= 0.25*(v_old[x+1][y]+v_old[x-1][y]+ v_old[x][y+1]+ v_old[x][y-1]);
        }
    }

    //output the  current array
    FILE *current_array;
    current_array= fopen("current_array.txt", "w");
    if (current_array==NULL)
    {//file was not opened
        printf("current file was not created! \n");
        //should be a diifererent return value here, but potentially the program could continue so i will ignore this exception.
    }
    else
    {//file succesfully created, program continues

        int i, j;
        //output array
        for (j=0; j<17 ; j++)
        {
            fprintf (current_array, "\n");

            for (i=0; i<32; i++)
            {
                fprintf(current_array, "%lf \t", v_new[i][j]);
            }

        }
        fclose(current_array);
    }

    return v_new;
}


