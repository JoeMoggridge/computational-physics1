#include <stdio.h>
#include <stdlib.h>
#include <math.h>
double x;
double f(double x)
{
double y;
y=pow(x,4)+pow(x,3)-12*pow(x,2)-2*x+10;

}

double f_dash(double x)
{
double y_dash;
y_dash=4*pow(x,3)+3*pow(x,2)-24*x-2;

if(y_dash=0)
{

}

}


int main()
{
double z;


printf(" This is a program which uses the Newton-Raphson method to find the numerical roots to a polynomial equation.\n Please enter a point at which the program is to begin.\n");
scanf("%lf", &x);

while(abs((z-x)>0.001))
{
z=x-(f(x)/f_dash(x));
x=z;

}

printf("The program has found a root!\n The root found is x=%lf", x);

return 0;

}
