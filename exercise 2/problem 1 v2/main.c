///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge computational physics exercise 1 problem 1.                                                  //
//                                                                                                               //
//Calculating the derivative of sin(x) using the differential formula.                                           //
//3 major operations occur:                                                                                      //
// -a)the user enters a value of x and h and values of Ff and Fc are calculated.                                 //
// -b)h is fixed at a (user entered) value and x is varied. The error in Ff and Fc is outputted to a text file.  //
// -c)x is fixed at pi/4 and h is varied. The error in Ff and Fc is ouputted to a text file.                     //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define FAIL 1000000.5

double calc_Fc (double x, double h);
double calc_Ff (double x, double h);
double input_double (void);

int main()
{
    //part a-------------------------------------------------------------------------------
    //calculate the derivative of sinx using the central and forward difference formula's for a given x and h.

    printf("part a\n\n");

    printf("at what x value would you like to calculate the derivative?\n");
    double x= input_double();

    //calculate a recommended value for h
    double h_rec= x*pow(10, -7.5);//e^0.5 for double precision works out to be around 10^-7.5

    printf( "What value of h would you like to use?\nIt is reccomended that you use a value close to %e.\n" , h_rec);
    double h= input_double();

    if (h == FAIL || x == FAIL)//check that values were correctly read in.
    {
        printf ("Numbers were not read in correctly. Possibly you did not enter a number? \nEnding program ....\n\n");
        return 1;
    }

    printf( "the forward difference formula calculates cos(%lf) to be: %lf\n",x, calc_Ff(x,h));
    printf( "the central difference formula calculates cos(%lf) to be: %lf\n",x, calc_Fc(x,h));
    printf( "the actual value is: %lf\n",cos(x));


    //part b ----------------------------------------------------------------------
    //for a fixed h, vary x and observe how the errors in Ff and Fc change.
    //columns will be outputed to a text file which can then be plotted.

    double ncrmnt=0.01;
    printf("\npart b\n\n");

    printf( "What value of h would you like to use?\n");
    h=input_double();

    if (h == FAIL )//check that h was correctly read in.
    {
        printf ("h was not read in correctly. Possibly you did not enter a number? \nEnding program ....\n\n");
        return 1;
    }

    //create file for part b

    //create a filename that incorporates the value of h.

    char filename [50]={"vary_x for h="};
    char tempstring1 [20];
    sprintf(tempstring1, "%e", h);
    strcat (filename, tempstring1);
    char tempstring2 [6]= {".txt"};
    strcat( filename, tempstring2);

    FILE *partb;
    partb= fopen ( filename, "w");
    if (partb== NULL)//check file was succesfully opened
    {//File failed to open
        printf( "file was not opened correctly");
        return 1;
    }
    else
    {//File succesfully opened, program continues:

        fprintf (partb,"x\tsin(x)\tFf\tFc\tcos(x)-Ff\tcos(x)-Fc\n");//create column headings

        for (x=0; x<2*M_PI; x= x+ncrmnt)//output lines provided x<2pi
        {//output a single line of data
            fprintf (partb, "%e\t%e\t%e\t%e\t%e\t%e\n", x, sin(x), calc_Ff(x,h), calc_Fc(x,h), cos(x)-calc_Ff(x,h), cos(x)-calc_Fc(x,h));
        }

        printf( "data for part b has been written to file\n");

        fclose (partb);
    }


    //part c  ---------------------------------------------------------------------
    //for x=pi/4, vary h and investigate changes in the error of Ff and Fc.
    //a text file with columns of log(h), log(f_error), and log(c_error) will be produced.

    printf("\npart c\n\n");

    //create file for part c of question
    FILE *partc;
    partc= fopen ("Vary_h.txt", "w");
    if (partc== NULL)//check file was correctly opened
    {//file was not opened
        printf( "file was not opened correctly");
        return 1;
    }
    else
    {//file succesfully opened. program continues:

        printf("calculating for part c....\n");

        x= M_PI_4;
        double abs_Ff_error,abs_Fc_error ;

        fprintf (partc,"log(h)\tlog(f_error)\tlog(c_error)\n");//create column headings

        for (h=pow(10, -18); h<0.1; h= h*1.5)//h varies between 10^-18 and 10^-1.
        {//output a single line of data
            abs_Ff_error= fabs(cos(x)-calc_Ff(x,h));
            abs_Fc_error= fabs(cos(x)-calc_Fc(x,h));
            fprintf (partb, "%e\t%e\t%e\n", log10(h), log10(abs_Ff_error),log10(abs_Fc_error) );
        }

        printf( "data for part c has been written to file\n");

        fclose (partc);
    }

    return 0;
}

double calc_Fc (double x, double h)
{
    return ((sin(x+h)-sin(x-h))/(2*h));
}

double calc_Ff (double x, double h)
{
    return ((sin(x+h)-sin(x))/(h));
}

double input_double (void)
{//returns a user entered double. in the case of an error, returns FAIL (ie 1000000.5)
    //unfortunately this prevents the user from succesfullly entering 100000.5
    double inputted_double;
    if (scanf ("%lf", &inputted_double)==1)
        return inputted_double;
    else
        return FAIL;
}

