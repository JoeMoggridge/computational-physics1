//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge computational physics ex3 problem 3                                                              //
//                                                                                                                  //
//calculating the potential due to two rectangular rods of opposites charge in a rectangular box.                   //
//A box of infinite depth with a size of 30x15 contains two rectangular charged rods.                               //
//The two rods are each of size 3x3 and are located at (7.5, 7.5) and (22.5, 7.5).                                  //
//Since everything is infinite in depth, we will consider this a 2D problem.                                        //
//We calculate the potential every 1 units by relaxtation.                                                          //
//Hence an array storing a grid of the potential is produced.                                                       //
//The program then calculates the x and y components of the electric field and stores each in 2 more arrays.        //
//Finally, this data is written to a text file in a format which will allow Gnuplot to plot it as a vector field.   //                                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

double** Relaxation (double** v_old, double** v_new);
int Calc_E (double** potential);
void Plot_Gnu (double** E_x, double** E_y);

int main()
{
    int i, x,y;

    //each array is 2 bigger than you'd expect. This is so the boundary's can be set at zero.

    //create v_old
    double** v_old=(double**) malloc( 32*sizeof(double));
    for (i=0; i<=31; i++)
        v_old [i] = (double*) malloc (17*sizeof(double));

    //create v_new
    double** v_new=(double**) malloc( 32*sizeof(double));
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
        printf("Arrays created.\n");

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
                //else make an initial guess of 0.
                else
                {
                    v_new[x][y]= 0;
                    v_old[x][y]= 0;
                }
            }
        }
        printf("Arrays initialsied\n");

        int num_converged=0;//records how many cells have converged.
        int num_iterations=0;

        //relaxation
        while (num_converged < 544)
        {
            num_iterations++;

            v_new = Relaxation(v_old, v_new);

            //reset num_converged
            num_converged=0;

            //check for convergence
            for (x=0; x<32 ; x++)
            {
                for (y=0; y<17; y++)
                {
                    if (fabs(v_new[x][y] - v_old[x][y])<0.001 )
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
        printf("It took %d iterations to calculate the field\n", num_iterations);

        //output the array
        FILE *final_array;
        final_array= fopen("potential.txt", "w");
        if (final_array==NULL)
        {//file was not opened
            printf("potential file was not created,\nEnding Program.\n\n");
            //free arrays
            for (i = 0; i < 4; i++)
            {
                free(v_new[i]);
                free(v_old[i]);
            }
            free(v_new);
            free(v_old);

            return 1;
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

    //call function to calculate E. Check function succeeded.
    if (Calc_E (v_new)!=0)
    {
        printf("failed to calculate Electric field\n");
    }

    //free arrays
    for (i = 0; i < 32; i++)
    {
        free(v_new[i]);
        free(v_old[i]);
    }
    free(v_new);
    free(v_old);

    return 0;
}

double** Relaxation (double** v_old, double** v_new)
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

    return v_new;
}

int Calc_E (double** potential)
{//returns 0 on success

    int i,x,y;//used in several 'for' loops.

    //create E_x
    double** E_x=(double**) malloc( 32*sizeof(double));
    for (i=0; i<=31; i++)
        E_x [i] = (double*) malloc (17*sizeof(double));

    //create E_y
    double** E_y=(double**) malloc( 32*sizeof(double));
    for (i=0; i<=31; i++)
        E_y [i] = (double*) malloc (17*sizeof(double));

    //calculate E for every co-ordinate
    //E is approximately half the difference between the two neighbouring points.
    for ( y=1; y<16; y++)
        for ( x=1; x<31; x++)
        {
            E_x [x][y]= 0.5* (potential[x+1][y]-potential[x-1][y]);
            E_y [x][y]= 0.5* (potential[x][y+1]-potential[x][y-1]);
        }

    Plot_Gnu(E_x, E_y);

    //free arrays
    for (i = 0; i < 32; i++)
    {
        free(E_x[i]);
        free(E_y[i]);
    }
    free(E_x);
    free(E_y);


    return(0);
}

void Plot_Gnu (double** E_x, double** E_y)
{//takes the arrays of the field and uses Gnuplot to create a vector plot of the field.

    //change the two arrays into a form that Gnu plot likes:
    //x-coordinate  y-coordinate    x-component  y-component
    FILE *plot_data;
    plot_data= fopen("vector_field.txt", "w");
    if (plot_data==NULL)//check file was created
    {
        printf("failed to create 'vector_field.txt'\n");
        return;
    }
    else
    {//file created, program continues

        int x,y;
        for ( y=1; y<16; y++)
            for ( x=1; x<31; x++)
            {
                fprintf(plot_data, "%d\t%d\t%lf\t%lf\n",x,y, E_x[x][y], E_y[x][y]);
                //output data to a file in a form that Gnuplot likes
            }
        printf("vector_field.txt has been written.\n");
        return;
    }
}
