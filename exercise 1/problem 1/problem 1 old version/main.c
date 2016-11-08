//Joseph Moggridge. computational physics excersise 1 problem 1.

//recieves input from the user about a quadratic equation. solves the equation.
//if the user instead enters a lower order equation, then that will also be solved.

//capital letters in comments indicate 'ToDo' items

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double input_double (void);

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

    if (A==1000000.5 || B==1000000.5 ||C==1000000.5)
    {
        printf("one of the coefficents you enetered was not read in correctly.\n Possibly you di not eneter a number?\n");
        return 1;
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
    if (scanf ("%lf", &inputted_double)==1)
        return inputted_double;
    else
        return 1000000.5;

    //what follows is my attempt to use fgets() to get input from the user, hence avoiding the many pitfalls of scanf().
    /*char text[20]={"qqqqq qqqqq qqqqq qq"};
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
