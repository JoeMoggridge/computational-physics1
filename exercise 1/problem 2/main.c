/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge. computational physics excersie 1 problem 2.                                            //
//                                                                                                         //
//Approximates sin(x) by using a large sum.                                                                //
//If an infinite sum could be taken then it would perfectly match sin(x).                                  //
//Outputs data to a txt file which will show how sine like the approximation is for various values of N.   //
//Data to allow sinx (as calculated by the function in math.h) to be plotted is also included.             //
//The program then prompts the user to calculate sine of a given x.                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned long int factorial(unsigned long int number);
double calculate_fx (int N, double x);
double calculate_sum (int N, double x);
double input_double (void);

int main()
{
    double N, x;

    //first, create a plot of how good an approximation various values of N give.
    FILE *plot;
    char filename[15]= {"plot_data.txt"};
    plot= fopen(filename ,"w"); //open the file for writng to.
    if (plot!=NULL )// check that the file was actually opened
    {
        //create column headings
        fprintf(plot, "x    \t");//create x column heading
        fprintf(plot, "sin (x)\t");//create the heading for built in sin function
        for (N=0;N<20;N=N+2)
        {
            fprintf(plot, "N=%lf\t", N);// create the headings for the other columns
        }
        fprintf(plot, "\n");

        //output data
        for (x=-10; x<=10; x=x+0.1)
        {
            fprintf(plot, "%lf\t",x);//output the x value

            fprintf(plot, "%lf\t",sin(x));//output the built in sin function

            for (N=0;N<20;N=N+2) //output the values that various values of N calculate
                fprintf(plot, "%lf\t",calculate_sum(N,x));

            fprintf(plot, "\n");//next row
        }

        fclose(plot);
        printf("data has been written to file.\n");
    }
    else
    {
        printf("file was not created succesfully.\n");
        return 1;
    }

    //calculate sine of a specific value
    printf("to test a pariticular value of the expansion,\nfirst enter how many terms you would like to calculate:\n");
    N= input_double();
    printf("Next enter what value of x you want to calculate on:\n");
    x= input_double();
    printf( "the function calculated sinx to be %lf \n", calculate_sum(N,x));

    return 0;
}

double input_double (void)
{//Continuosly asks for input from the user until a number is enetered.
    double inputted_double=0;
    while (scanf("%lf", &inputted_double)!=1)
    {
        //ONCE THIS STUCTURE IS ENTERED THE PROGRAM BECOMES STUCK AND CONTINUOSLY EXECUTES HERE.
        printf("the value you enetered is not a number, try again\n");
    }
    return inputted_double;
}
unsigned long int factorial(unsigned long int number)
{//this is a recursive function that calculates the factorial of a number.
//apparently math.h does not include a factorial function.
    if (number <= 1)//base case
        return 1;
    return number * factorial(number - 1);
    //when debugging this function appears to fail to return correctly.
    //however, it gives the right answer. wtf?!
}


double calculate_fx (int N, double x)
{//calculate f(x) for a given x and N
    double fx;

    double test= factorial(2*N+1);//check that the factorial function is returning the correct value fro debuuging

    fx=((pow(-1.0,(double)N)*pow(x, (double)2*N+1))/factorial(2*N+1));
    return fx;
}
double calculate_sum (int N, double x)
{//this is a recursive function that calculates the sum of N terms of f(x)

    if (N==0) //base case
    {
        //sin(x) roughly equal to x for the last term
        return x;
    }
    else
        return calculate_fx(N,x) + calculate_sum (N-1, x);
    //note that I effectively sum from N to zero rather than 0 to N.
    //However, addtition is commutative, so this is ok.

}
