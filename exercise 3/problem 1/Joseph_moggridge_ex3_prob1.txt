//////////////////////////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge computational physics exercise 3 problem 1.                                      //
//                                                                                                  //
//Solve the differential equations of freefall using both Eulers method and an analytical method.   //
//Assume constant air density and calculate data assuming a skydiver fell from 1000m.               //
//Output the data to a text file to allow it to be plotted                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define g 9.81

typedef struct
{
    double t;
    double y;
    double v;
}situation;

situation felix; //new global variable of type 'situation', will contain information about felix baunhoffers situation.
situation felix_analytic;

void Calc_Euler ( double k, double mass, double dt );

void Calc_Analytical (double k,double mass, double t, double start_height);

int main()
{
    //Drag force is given by kv^2. where k is given by 0.5*A*C_d*p_zero*exp(-height/scale_height)
    //A =cross sectional area of object
    //C_d = coefficent of drag of object. Roughly 1 to 1.3 for a skydiver
    double A=0.6, c_d=1.2, p_zero= 1.2, start_height=1000, mass= 80, dt=0.01;
    double k= A*c_d*p_zero/2;
    felix.t=0;
    felix.y=start_height;
    felix.v=0;

    FILE *data;
    data= fopen("felix_baumgartner.txt", "w");
    if (data==NULL)
    {//check file was succesfully opened
        printf("failed to open file.\nEnding program....\n\n");
        return 1;
    }
    else
    {
        fprintf ( data, "t\ty\tv\ty_analytic\tv_analytic\n");//column headings
        printf("writing to file....\n");
        while (felix.y>0 )
        {
            Calc_Analytical (k, mass, felix.t, start_height);//felix.t increments 2 lines down
            fprintf (data, "%lf\t%lf\t%lf\t%lf\t%lf\n", felix.t, felix.y, felix.v, felix_analytic.y, felix_analytic.v);//output data to file
            //It might seem surprising that the data is outputted before both functions have been called. consider the case at t=0 to see that it does work.
            Calc_Euler( k,mass,dt);
        }
        printf("data written to file\n");
    }

    return 0;
}

Calc_Euler ( double k, double mass, double dt )
{//calcualte one iteration of eulers method
    felix.v= felix.v-(dt*(g+ k*felix.v*abs(felix.v)/mass));
    felix.y= felix.y+ (dt*felix.v);
    felix.t= felix.t+dt;
}


void Calc_Analytical (double k,double mass, double t, double start_height)
{//calculate the answer analytically.
    felix_analytic.y= start_height-(mass/(2*k))*(log(pow(cosh(t*sqrt(k*g/mass)),2)));
    felix_analytic.v= -sqrt(mass*g/k)*tanh(t*sqrt(k*g/mass));
    felix_analytic.t=t;
}



