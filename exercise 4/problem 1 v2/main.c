//////////////////////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge Computational Physics exercise 4 problem 1                                   //
//                                                                                              //
//calculate the path of a body using runge kutta.                                               //
//initial conditions are set and then the program calculates how the object moves over time.    //
//a text file is produced with the postion, velocity, and energy over time.                     //
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#define G 6.673E-11
#define true 1
#define false 0

typedef struct
{
    double t;
    double v_x;
    double v_y;
    double x;
    double y;
} situation;

situation Runge_Kutta (situation sit, double M, double h);

double Input_Double (void);
int At_Start_Pos (situation sit, situation initial);
int Hit_Earth (situation sit);

//the following functions correspond to the 4 equations we are trying to solve. for example "dx/dt = f1"
double f1 (situation sit);
double f2 (situation sit);
double f3 (situation sit, double M);
double f4 (situation sit, double M);


int main()
{
    situation sit;//new variable of type situation
    situation initial;
    double Energy, E_k, E_p;

    //boundary conditions
    initial.t=0;
    double M= 5.97e24;//mass of the earth
    initial.x= -7000000;//radius of earth is 6738000
    initial.y= 0;
    initial.v_x= 0;
    initial.v_y= 10400;

    //simulate for 50 days
    double max_time= 24*3600*50;
    //recalculate every minute
    double h= 0.1;

    sit = initial;

    FILE *orbit;
    orbit= fopen ("orbit.txt", "w");
    if (orbit==NULL)
    {//failed to open file
        printf ("Could not open file, ending program...\n\n");
        return 1;
    }
    else
    {
        //file succesfully opened, program continues.
        fprintf( orbit, "time\tx\ty\tv_x\tv_y\t E_k\t E_p\tenergy\n"); //column headings.

        E_k= pow(sit.v_x,2) + pow(sit.v_y,2);
        E_p= -G*M/(sqrt(sit.x*sit.x+sit.y*sit.y));
        Energy = E_p+ E_k;
        fprintf( orbit, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", sit.t, sit.x, sit.y, sit.v_x, sit.v_y, E_k, E_p, Energy);//output initial conditions

        int sample=0;//will be used so that the program does not generate reams of data.

        while (sit.t < max_time )
        {
            sit = Runge_Kutta(sit, M, h);//calculate next line

            //calculate energy, assuming for simplicity that probe has a mass of 1kg. Energy= gravitational energy+ kinetic energy
            //velocity= sqrt(v_x^2+v_y^2). however kinetic energy = velocity^2, so the "sqrt()" dissapears.
            E_k= pow(sit.v_x,2) + pow(sit.v_y,2);
            E_p= -G*M/(sqrt(sit.x*sit.x+sit.y*sit.y));
            Energy = E_p+ E_k;

            //output line once per "sample"
            if (sample==0)
                fprintf( orbit, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", sit.t, sit.x, sit.y, sit.v_x, sit.v_y, E_k, E_p, Energy);
            //next 3 lines eqivalent to: sample++ (mod sample)
            sample++;
            if (sample>=600)
                sample=0

            if (At_Start_Pos(sit, initial)==true && sit.t>3600)//if object has returned to its start position.
            {
                printf(" object has returned to its starting position.\n");
                break;
            }
            if (Hit_Earth(sit)==true)
            {
                printf("object has crashed into the earth.\n");
                break;
            }


        }
        printf("data has been written to file.\n");

    }

    return 0;
}

int At_Start_Pos (situation sit, situation initial)
{
    //returns true if probe is close to (within 100m of) its starting position
    if (abs(sit.x-initial.x)<100 && abs(sit.x-initial.x)<100)
        return true;
    else
        return false;

}
int Hit_Earth (situation sit)
{
    //returns true if object has hit earth
    if (sit.x*sit.x+sit.y*sit.y < 6e6*6e6)//radius of earth is 6e6
        return true;
    else
        return false;


}

situation Runge_Kutta (situation sit, double  M, double h)
{
    //takes an initial situation and calculates the situation at the next timestep.

    situation result;

    situation temp;//this will be used to pass values to the various functions which calculate the k's
    temp.t = sit.t+h/2;

    //calculate k1's
    double k1x, k1y, k1v_x, k1v_y;
    k1x= f1(sit);
    k1y= f2(sit);
    k1v_x= f3(sit, M );
    k1v_y= f4(sit, M);

    //calculate k2's
    double k2x, k2y, k2v_x, k2v_y;
    temp.x =sit.x+ h*(k1x)/2;
    temp.y =sit.y+ h*(k1y)/2;
    temp.v_x =sit.v_x+ h*(k1v_x)/2;
    temp.v_y =sit.v_y+ h*(k1v_y)/2;
    k2x= f1(temp);
    k2y= f2(temp);
    k2v_x= f3(temp, M);
    k2v_y= f4(temp, M);

    //calculate k3's
    double k3x, k3y, k3v_x, k3v_y;
    temp.x =sit.x+ h*(k2x)/2;
    temp.y =sit.y+ h*(k2y)/2;
    temp.v_x =sit.v_x+ h*(k2v_x)/2;
    temp.v_y =sit.v_y+ h*(k2v_y)/2;
    k3x= f1(temp);
    k3y= f2(temp);
    k3v_x= f3(temp, M);
    k3v_y= f4(temp, M);

    //calculate k4's
    double k4x, k4y, k4v_x, k4v_y;
    temp.t= sit.t+h;
    temp.x =sit.x+ h*(k3x);
    temp.y =sit.y+ h*(k3y);
    temp.v_x =sit.v_x+ h*(k3v_x);
    temp.v_y =sit.v_y+ h*(k3v_y);
    k4x= f1(temp);
    k4y= f2(temp);
    k4v_x= f3(temp, M);
    k4v_y= f4(temp, M);

    //calculate answer using th k's calculated above
    result.x= sit.x+ (h/6)*(k1x+ 2*k2x+2*k3x+k4x);
    result.y= sit.y+ (h/6)*(k1y+ 2*k2y+2*k3y+k4y);
    result.v_x= sit.v_x+ (h/6)*(k1v_x+ 2*k2v_x+2*k3v_x+k4v_x);
    result.v_y= sit.v_y+ (h/6)*(k1v_y+ 2*k2v_y+2*k3v_y+k4v_y);
    result.t = sit.t + h;

    return result;
}

//the following functions correspond to the 4 equations we are trying to solve. for example "dx/dt = f1"
double f1 (situation sit)
{
    //dx/dt=
    return sit.v_x;
}
double f2 (situation sit)
{
    //dy/dt=
    return sit.v_y;
}
double f3 (situation sit, double M)
{
    //dv_x/dt=
    return -G*M*sit.x/(pow(sit.x*sit.x+sit.y*sit.y , 1.5)) ;
}
double f4 (situation sit, double M)
{
    //dv_y/dt=
    return -G*M*sit.y/(pow( sit.x*sit.x+sit.y*sit.y , 1.5));
}



