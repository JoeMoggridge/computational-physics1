/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge computational physics ex3 problem 2                                                             //
//                                                                                                                 //
//Investiagating equations of motion for felix baunhoffers jump.                                                   //
//The program calculates felix baunhoffers changing velocity and height using both Eulers and annalytical methods. //                          //
//The program makes allowances for the varying density of air.                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

situation felix; //new global variable of type 'situation', will contain information about felix baumgartners situation.
situation felix_analytic;//contains information about felix's situation as calculated analytically

void Calc_Euler ( double k, double mass, double dt );


int main()
{
    //Drag force is given by kv^2. where k is given by 0.5*A*C_d*p_zero*exp(-height/scale_height)
    //A =cross sectional area of object
    //C_d = coefficent of drag of object. Roughly 1 to 1.3 for a skydiver
    //air density = p_zero*exp(height/scale_height)
    double A=0.6, C_d=1.2, p_zero= 1.2,scale_height=7640, k; //drag stuff.
    double start_height=39450, mass= 80, dt=0.01;//other initialisations
    felix.t=0;
    felix.y=start_height;
    felix.v=0;

    FILE *data;
    data= fopen("felix_baumgartner.txt", "w");
    if (data==NULL)
    {//check file was succesfully opened
        printf("Failed to open file.\nEnding program...\n\n");
        return 1;
    }
    else
    {
        fprintf ( data, "t\ty\tv\t\n");//column headings
        printf("writing to file...\n");
        while (felix.y>0 )
        {
            k= A*C_d*p_zero*exp(-felix.y/scale_height)/2;//exponent is there to allow for the changing density of the air
            fprintf (data, "%lf\t%lf\t%lf\t%lf\t%lf\n", felix.t, felix.y, felix.v, felix_analytic.y, felix_analytic.v);//output data.
            Calc_Euler( k,mass,dt);//next situation
        }
        printf("Data has been written to file\n");
    }

    return 0;
}

void Calc_Euler ( double k, double mass, double dt )
{//calcualte one iteration of eulers method.
    felix.v= felix.v-(dt*(g+ k*felix.v*abs(felix.v)/mass));
    felix.y= felix.y+ (dt*felix.v);
    felix.t= felix.t+dt;
}
