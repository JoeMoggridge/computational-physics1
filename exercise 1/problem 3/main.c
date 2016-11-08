//////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge, computational physics excersise 1, problem 3.               //
//                                                                              //
//Recieves input from the user about the coefficents of a polynomial.
//A graph of the the polynomial is produced.                                   //
//The program attempts to solve the equation by using newton raphson iteration. //
//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include<math.h>

double input_double (void);
int check_derivative (double *fnctn, double x, double eqn_order);
double calculate_Xn (double x, double *eqn, double *d_eqn, double eqn_order);
void create_plot(double *eqn, double eqn_order, double lwr, double uppr, double ncrmnt);

int main()
{
    printf("Solving an equation by using Newton Raphson iteration.\n\n");

    printf("what is the order of the equation?\n");
    double const eqn_order = input_double();
    int i=0;
    printf("At what value would you like to start the iteration?\n");
    double start_value = input_double();
    int k= eqn_order;//Since 'eqn_order' is a constant, we will manipulate k instead.
    //This has the benefit that we can change k as much as we like without forgetting what the order of the equation is.

    //Declare the array 'Xn', which will represent the nth iteration of the array.
    //The zeroth element will be the current number of iterations and the 1st element will be the value of that iteration
    double *Xn;
    Xn = (double*)malloc(2 * sizeof(double));

    //Declare the array 'eqn' which stores the coefficents of the equation.
    //The 0th element stores the 1st coefficent, the 1st element stores the second coefficent..... etc.
    double *eqn;
    eqn = (double*) malloc((eqn_order+1)* sizeof(double));

    //Declare the array 'differentiated' which will store the coefficents of the differentiated equation.
    //Note that 'differentiated' is one element smaller than 'eqn', because the constant will dissapear.
    double *differentiated;
    differentiated = (double*)malloc(eqn_order * sizeof(double));

    //check that the allocation of all the arrays succeeded:
    if (differentiated == NULL || eqn== NULL || Xn == NULL)
    {//throw an exception if an array was not created
        printf("memory could not be allocated\n");
        //free all three arrays
        free (eqn);
        free (Xn);
        free (differentiated);
        return 1;
    }

    else
    { // Allocation succeeded. program continues:

        //recieve input from the user about the coefficents of the equation:
        while (i<=eqn_order)
        {
            printf("enter coefficent number %d: ", i+1);//it would take an excessive number of lines of code to make this say "1st" and "2nd" properly.
            eqn[i]= input_double();
            i++;
        }

        //diffferentiate the equation
        for (i=0; i<(eqn_order); i++)
        {
            differentiated[i]= k*eqn[i];
            k--;
        }

        //output plottable data to a file to graphically visualize function
        printf( "\nThe program will output data to a file,\nwhich can then be plotted in order to visualise the function.\n");
        double lwr, uppr, ncrmnt;
        printf("What is the smallest value you would like to plot?\n");
        lwr =input_double();
        printf("What is the biggest value you would like to plot?\n");
        uppr =input_double();
        printf("What increment would you like between x values?\n");
        ncrmnt =input_double();
        if (uppr<lwr)
        {
            printf("upper bound must be bigger than lower bound.\nProcess aborted.\n");
              //free all three arrays
        free (eqn);
        free (Xn);
        free (differentiated);
            return 5;
        }
        else if (ncrmnt>(uppr-lwr))
        {
            printf("Increment must be smaller than the gap between the upper and lower bounds.\nProcess aborted.\n");
              //free all three arrays
        free (eqn);
        free (Xn);
        free (differentiated);
            return 5;
        }
        else
            create_plot( eqn, eqn_order, lwr, uppr, ncrmnt);




        //check that the derivative at start_value is not zero, otherwise the method will fail
        if (check_derivative( differentiated, start_value, eqn_order)==1)
        {
            printf("The derivative at the chosen point is zero and hence the method will fail.\nAborting....\n");
            //free all three arrays
            free (eqn);
            free (Xn);
            free (differentiated);
            return 2;
        }

        else
        {//calculate the root.
            printf("\nIterating....\n");
            Xn [0] =0;
            Xn [1]= calculate_Xn(start_value, eqn, differentiated, eqn_order);//initial approximation
            double Xtemp=start_value;

            //iterate, stopping either when two subsequent iterations are essentially the same, or after 100 iterations
            for (i=0; i<=100, sqrt((Xn[1]-Xtemp)*(Xn[1]-Xtemp))>0.000001;i++)
            {
                //check that the derivative at the current point is not zero, or else the process will fail.
                if (check_derivative(Xn, Xtemp, eqn_order)==1)
                {
                    printf("The derivative at one of the iteration points is zero and hence the process will fail. \nAborting....\n");
                    //free all three arrays
                    free (eqn);
                    free (Xn);
                    free (differentiated);
                    return 1;
                }
                else
                {
                    //perform an iteration
                    Xtemp = Xn[1];
                    Xn[0]= Xn[0]+1;
                    Xn[1]= calculate_Xn (Xn[1], eqn, differentiated, eqn_order);
                    //for the purposes of testing, output the current iteration:
                    printf("%lf\n",Xn[1]);
                }
            }
        }
    }

    //output the results, firstly if process failed....
    if (Xn[1]== start_value)
    {
        printf("The starting value you chose is a solution! \n");
        //free all three arrays
        free (eqn);
        free (Xn);
        free (differentiated);
        return 4;
    }
    else if (Xn [0] == 100)
    {
        printf("100 iterations were performed and no root was found.\n");
        //free all three arrays
        free (eqn);
        free (Xn);
        free (differentiated);
        return 3;
    }

    //...and secondly if a root was found
    else
    {
        printf("A root was found: %lf \n", Xn [1]);
        printf("It took %lf iterations to find the root.\n\n", Xn [0]);

        //free all three arrays
        free (eqn);
        free (Xn);
        free (differentiated);
    }

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

int check_derivative (double *fnctn, double x, double eqn_order)
{//Returns 1 if the derivative is 0. Else, returns zero.
    int i=0;
    int n=eqn_order;
    double derivative=0;

    //iterate to find the derivative:
    while (n>0)
    {
        derivative = derivative + fnctn[i]*pow(x,(n-1));
        n--;
        i++;
    }

    //return, depending on the value of the derivative
    if (derivative==0)
        return 1;
    else
        return 0;
}

double calculate_Xn (double x, double *eqn, double *d_eqn, double eqn_order)
{
    int k= eqn_order;
    int i=0;
    double fx=0;
    double d_fx=0;

    //calculate f(x)
    while (k>=0)
    {
        fx= fx+ (eqn[i])*pow(x, k);
        k--;
        i++;
    }

    //calculate f'(x)
    k=eqn_order;
    i=0;
    while (k-1>=0)
    {
        d_fx= d_fx+ (d_eqn[i])*pow(x, k-1);
        k--;
        i++;
    }

    //calculate x-f(x)/f'(x)
    double Xn= x-(fx/d_fx);

    return Xn;
}

void create_plot(double *eqn, double eqn_order, double lwr, double uppr, double ncrmnt)
{//output plottable data to a file
    FILE *plot;
    char filename[20]= {"plot_data.txt"};
    plot= fopen(filename ,"w"); // create a file for writng to.
    if (plot!=NULL )// check that the file was actually opened
    {
        double x=lwr;
        double fx=0;
        while (x<uppr)
        {
            //output x
            fprintf(plot, "%lf\t",x);

            //calculate fx
            int k= eqn_order;
            int i=0;
            fx=0;
            while (k>=0)
            {
                fx= fx + eqn[i]*pow(x, k);
                k--;
                i++;
            }
            //output fx
            fprintf(plot, "%lf\n",fx);

            //move on
            x=x+ncrmnt;
        }
        fclose(plot);
        printf("data has been written to file.\n");
    }
    else
        printf("file was not created succesfully.\n");
}
//
