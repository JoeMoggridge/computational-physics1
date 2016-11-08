//////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge computational Physics exercise 2 problem 3.              //
//                                                                          //
//Calculate Pi by using monte carlo integration.                            //
//The unit circle has area pi and the unit square has area 4.               //
//Hence by randomly generating points we can calculate pi/4.                //
//The program allows the user to choose how many points to generate,        //
//and then investigates how many points produce how good a fit.             //
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define FAIL 1000000.5

double Input_Double (void);
double Calc_Result (long int max_generate);

int main()
{
    srand(time(NULL));//provide seed for later rand() function. makes it more random.

    //------------------------------------------------------------------------------
    //first output a value of Pi as calculated by a given (user entered) number of points.
    printf("How many random numbers would you like to generate?\n");
    long int max_generate;//max_generate is long and unsigned in order to allow it to store very large numbers.
    max_generate= Input_Double();
    if (max_generate== FAIL)
    {//check for error
        printf("\nError recieving user input.\nEnding program....\n\n");
        return 1;
    }
    double result= Calc_Result( max_generate);
    printf("The answer is %lf\n", result);

    //-------------------------------------------------------------------------------
    //next investigate how close to pi the result is as the number of points increases.
    //two columns will be written to a text file: the value of N and the calculated value of PI-actual pi for that given N.
    printf("\nInvestigating N:\n\n");

    FILE *plot_data;
    plot_data= fopen( "investigating_N.txt", "w");
    if (plot_data==NULL)//check that the file was succesfully opened
    {//file failed to open
        printf("File was not succesfully opened.\nEnding program...\n\n");
        return 1;
    }
    else
    {//file succesfully opened, continue:

        fprintf(plot_data, "log(N)\tlog(calc_Pi-Pi)\n");//create column headings

        for (max_generate=10; max_generate<1000000000; max_generate= max_generate*1.5)
        {
            printf("%lf\n", log(max_generate));
            fprintf(plot_data, "%lf\t", log(max_generate));//output N
            result= Calc_Result( max_generate)- M_PI;//calculate PI
            fprintf(plot_data, "%lf\n", log10(fabs(result)));//output the calculated value of PI
        }
        printf("Data has been written to a file.\n");
    }

    return 0;
}

double Input_Double (void)
{//returns a double entered by the user. on failure returns FAIL (ie 1000000.5)
    //this means that user cannot choose a value of 1000000.5, which is unfortunate, but i cant see a way around it.
    double inputted_double;
    if (scanf ("%lf", &inputted_double)==1)
        return inputted_double;
    else
        return FAIL;
}

double Calc_Result (long int max_generate)
{//calculates pi using a given number of randomly generated points

    long int n_in=0;
    int i;
    typedef struct{double x,y;} coordinate;//define a new type 'coordinate' to save having to maniplute two variables.
    coordinate number;//create a variable of type coordinate, as yet unitialised.

    for (i=0; i<=max_generate;i++)
    {
        //create a random coordinate between -1 and 1.
        number.x = 2*rand()/(double)RAND_MAX-1;
        number.y = 2*rand()/(double)RAND_MAX-1;

        //check whether random co-ordinate is within the circle
        if (fabs((number.x*number.x)+(number.y*number.y))<=1)
            n_in++;

    }
    //geometric fact: n_in/maxgenerated = pi/4.
    //we can use this to calcualte pi:
    double result= 4*n_in/(double)max_generate;
    return result;
}
