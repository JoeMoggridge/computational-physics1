//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge computational physics exercise 2 problem 2.                                                                         //
//                                                                                                                                      //
//Numerical integration by simpsons rule.                                                                                               //
//The timeperiod of a pendulum can be approximated by: T= 2*PI*sqrt(L/g).                                                               //
//In actual fact the formula for the timepriod of a pendulum is:                                                                        //
//            (  L  )   /pi/2                    1                                        (  L  )   /pi/2                               //
//  T= 4* sqrt| --- | * |       -------------------------------------    d(phi)  = 4* sqrt| --- | * |       f(phi) d(phi)               //
//            (  g  )   /0      sqrt( sin^2(0theta / 2)* sin^2(phi) )                     (  g  )   /0                                  //
//                                                                                                                                      //
//This is a difficult integral to solve. This program solves this integral by using Simpsons method.                                    //
//A text file is produced for various starting angles, in order to investigate how the starting angle affects the time period.          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define FAIL 1000000.5

double Calc_FPhi (double phi, double theta_zero );
double Calc_Integral(double theta_zero,double h);
double Input_Double (void);

int main()
{
    printf("Enter the value of h (must be less than 1.57):\n");
    double h= Input_Double();

    printf("Enter the length of the pendulum:\n");
    double L= Input_Double();

    printf("The program will create a text file with T for various theta_zero.\nEnter the increment to use between values of theta_zero\n");
    double ncrmnt= Input_Double();


    //check for errors:
    if (h == FAIL || ncrmnt == FAIL|| L == FAIL)
    {//error in reading in the values?
        printf ("Numbers were not read in correctly. Possibly you did not enter a number? \nEnding program ....\n\n");
        return 1;
    }
    if (h>1.57|| h<0|| ncrmnt> M_PI_4)
    {//catch other errors
        printf ("The chosen values will not work. \nEnding program ....\n\n");
        return 1;
    }

    //program continues
    else
    {
        double T, integral, theta_zero;
        double T_approx = 2*M_PI*sqrt(L/9.81);

        //create a filename incorporating L and h.
        char filename[50] ={"varying_theta_zero(L="};
        char tempstring1 [10];
        sprintf(tempstring1, "%lf", L);
        strcat ( filename, tempstring1);
        char tempstring2[5]={", h="};
        strcat(filename, tempstring2);
        sprintf(tempstring1, "%lf", h);
        strcat (filename, tempstring1);
        char tempstring3 [6]= {").txt"};
        strcat( filename, tempstring3);

        //create a file with the above filename and open for writing.
        FILE *plotfile;
        plotfile= fopen( filename, "w");
        if (plotfile==NULL)
        {//check the file was actually opened
            printf ("File was not succesfully created. \nEnding program ....\n\n");
            return 1;
        }
        else
        {//program continues

            fprintf(plotfile, "theta_zero\th=0.1\th=0.01\th=0.001\th=0.0001\th=0.00001\tT\n");//write column headings

            double Already_Checked= FALSE;//this will be used when outputting the accuracy of T_approx

            //ouptut two columns to the file. First column is theta_zero,  and the second is T as calculated by simpsons rule.
            for (theta_zero=0.1; theta_zero<M_PI; theta_zero= theta_zero+ncrmnt)
            {
                fprintf(plotfile, "%lf\t", theta_zero);//output theta_zero

                for (h=0.1; h>0.00001; h=h/10)
                {
                    integral = Calc_Integral (theta_zero, h);//calculate the integral using simpsons rule

                    T= 4*sqrt(L/9.81)*integral;//calculate T:
                    fprintf(plotfile,"%lf\t", T);//output T

                    if ( T > 1.1*T_approx && Already_Checked == FALSE)//check if the timeperiod differs from the approximate timeperiod by more than 10%
                    {
                        printf("\nThe small angle approximation calculates T= %lf\n", T_approx);
                        printf("The timeperiod differs significately from the approximate time period when\ntheta_zero> %lf\n ", theta_zero);//if it does, tell the user
                        Already_Checked= TRUE;//make it so that the previous lines will only be printed once.
                    }
                }
                fprintf(plotfile,"\n");



            }
            fclose(plotfile);
            printf("\nData has been written to a file.\n");
        }

    }
    return 0;
}

double Calc_FPhi (double phi, double theta_zero )
{
    return (1/(sqrt(1-(sin(theta_zero/2)*sin(theta_zero/2)*sin(phi)*sin(phi)))));
}

double Calc_Integral(double theta_zero, double h)
{//calculate the integral using simpsons rule:

    double integral=0, phi;

    for (phi=0; phi<=M_PI_2; phi=phi+M_PI_2)
    {//first and last terms
        integral= integral+ h/3*Calc_FPhi(phi, theta_zero);
    }
    for (phi=h; phi<M_PI_2; phi=phi+(2*h))
    {//odd terms
        integral= integral+ 4*h/3*Calc_FPhi(phi, theta_zero);
    }
    for (phi=2*h; phi<M_PI_2; phi=phi+(2*h))
    {//even terms
        integral= integral+ 2*h/3*Calc_FPhi(phi, theta_zero);
    }
    return integral;

}

double Input_Double (void)
{//returns a user entered double. in the case of an error, returns FAIL (ie 1000000.5).
    //unfortunately this prevents the user from succesfullly entering 100000.5
    double inputted_double;
    if (scanf ("%lf", &inputted_double)==1)
        return inputted_double;
    else
        return FAIL;
}


