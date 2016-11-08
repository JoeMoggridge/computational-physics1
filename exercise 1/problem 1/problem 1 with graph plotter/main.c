//Joseph Moggridge. computational physics excersise 1 problem 1.

//recieves input from the user about a quadratic equation. solves the equation.
//if the user instead enters a lower order equation, then that will also be solved.
//creates a 3d plot of the graph with the z axis being the complex component

//capital letters in comments indicate 'ToDo' items

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

double input_double (void);
void create_plot(double *eqn, double eqn_order, double lwr, double uppr, double ncrmnt);

int main()
{
    double A,B,C,solution1, solution2;

    printf( "Solving a cubic of the form ax^3+bx^2+cx+d=0\n");
    printf("Input the values of a,b and c:\n");

    //recieve input from the user about the coefficents.
    printf("a=");
    A= input_double();

    printf("b=");
    B= input_double();

    printf("c=");
    C= input_double();

    //output plottable data to a file to graphically visualize function
    {
        printf( "\nThe program will output complex data to a file,\nwhich can then be plotted in order to visualise the function.\n");
        double lwr, uppr, ncrmnt;
        double eqn [3] = {A, B, C};// create an array containing the coefficents.
        //this will be passed to the 'create_plot' function.
        printf("What is the smallest value you would like to plot?\n");
        lwr =input_double();
        printf("What is the biggest value you would like to plot?\n");
        uppr =input_double();
        printf("What increment would you like between x values?\n");
        ncrmnt =input_double();
        if (uppr<lwr)
        {
            printf("upper bound must be bigger than lower bound.\n");
            return 5;
        }
        else if (ncrmnt>abs(uppr-lwr))
        {
            printf("Increment must be smaller than the gap between the upper and lower bounds.\n");
            return 5;
        }
        else
            create_plot( eqn, 2, lwr, uppr, ncrmnt);
    }


    //if the equation is not quadratic, then solve it.
    if (A==0 && B==0)
    {
        printf ("\nEquation is a horizontal line.\n There are no solutions.\n");
        return 0;
    }
    else if ( A==0 && B!=0 )
    {
        printf("\nEquation is linear, not quadratic.\n");
        solution1= -C/B;
        printf("The line will interest the x-axis at x= %lf\n", solution1);
        return 0;
    }

    //if it really is quadratic, then solve it:
    else if ((B*B)-4*A*C <0)//two imaginary solutions
    {
        printf( "\nThere is no real solution.\n");
        //let the two solutions be of the form p+qi and p-qi
        double p= -B/(2*A);
        double q= sqrt(-(B*B-4*A*C))/(2*A);
        printf("Imaginary solutions are %lf + %lfi and %lf - %lfi\n", p, q, p, q );
    }
    else if ((B*B)-4*A*C == 0)//one repeated real solution
    {
        solution1 = (-B)/(2*A);
        printf("\nThere is only one solution: x= %lf\n", solution1);
    }
    else//two distinct real solutions
    {
        solution1 = (-B+sqrt(B*B-4*A*C))/(2*A);
        solution2 = (-B-sqrt(B*B-4*A*C))/(2*A);
        printf("\nThere are two solutions: x= %lf and x= %lf\n", solution1, solution2);
    }
    return 0;
}
double input_double (void)
{
    double inputted_double;
    scanf ("%lf", &inputted_double);
    return inputted_double;
    /*THIS NEXT BIT IS COMMENTED BECAUSE IT DOESNT WORK
    //this is a misnomer since "sscanf()" reads in a float type.
    //however i think that generally the user will not want to enter a number to a vey high degree of accuracy.
    char text[20]={"qqqqq qqqqq qqqqq qq"};
    if ( fgets(text, sizeof text, stdin) )
    {
      int number;
      if ( sscanf(text, "%f", &number) == 1 )//THIS BIT DOESNT WORK. I THINK THAT IT CONTINUES READING EVEN AFTER THE FIRST NON DIGIT ELEMENT.
      {
         return number;
      }
      else
      {
          printf("you did not enter an number, try again:\n");
          input_double();
      }
    }*/
}

void create_plot(double *eqn, double eqn_order, double lwr, double uppr, double ncrmnt)
{//output plottable data to a file
    FILE *plot;
    char filename[20]= {"plot_data.txt"};
    plot= fopen(filename ,"w"); // create a file for writng to.
    if (plot!=NULL )// check that the file was actually opened
    {
        complex double x_c= lwr + lwr*I;

        double fx=0;
        while (creal(x_c)<uppr)
        {


            for (x_c= lwr+lwr*I; cimag(x_c)<uppr; x_c= x_c+ ncrmnt*I)
            {
                //output real part
                fprintf(plot, "%lf\t",creal(x_c));

                //output imaginary part
                fprintf(plot, "%lf\t",cimag(x_c));


                //calculate fx
                int complex k= eqn_order+ 0*I;
                int i=0;
                fx=0;
                //experiment to see what is breaking;
                double test1 = creal(cpow(x_c,k));
                printf ("test 1 done");
                double test2 = cabs(cpow(x_c,k));
                 printf ("test 2 done");
                double test3= eqn[i]*test2;
                 printf ("test 3 done");
                while (creal(k)>=0)
                {
                    fx= fx + (eqn[i]*cabs(cpow(x_c, k)));//this breaks totally
                    k= k-1;
                    i++;
                }
                printf("thinking");

                //output fx
                fprintf(plot, "%lf\n",fx);
            }
            //next x value
            x_c= x_c+ ncrmnt;
        }
        fclose(plot);
        printf("data has been written to file.\n");
    }
    else
        printf("file was not created succesfully.\n");
}
