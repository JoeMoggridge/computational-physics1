/////////////////////////////////////////////////////////////////
//Joseph Moggridge Computational Physics exercise 4 problem 1
//
//calculate the orbit of a body using runge kutta
//////////////////////////////////////////////////////////////

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

situation Runge_Kutta (situation sit, double  M, double h);

int At_Start_Pos (situation sit, situation initial);
int Hit_Earth (situation sit);
void Calc_orbit (situation initial, double M, double max_time, double h);

//the following functions correspond to the 4 equations we are trying to solve. for example "dx/dt = f1"
double f1 (situation sit);
double f2 (situation sit);
double f3 (situation sit, double M);
double f4 (situation sit, double M);


int main()
{
    situation initial;
    double distance;

    //boundary conditions
    initial.t=0;
    double M = 6e24;//mass of the earth
    /*
    printf ("What is the x coordinate of the starting position? (km)\n");
    initial.x= 1000*Input_Double();
    printf ("What is the y coordinate of the starting position? (km)\n");
    initial.y= 1000*Input_Double();
    printf ("What is the x coordinate of the starting velocity?(km/s)\n");
    initial.v_x= 1000*Input_Double();
    printf ("What is the y coordinate of the starting velocity? (km/s)\n");
    initial.v_y= 1000*Input_Double();
    */

    //simulate for 10 days
    double max_time= 24*3600*10;
    //recalculate every minute
    double h= 60;

    //cycle through many possible starting conditions to investigate eccentricity.
    for (initial.x=-10000000; initial.x<10000000; initial.x= initial.x+ 1000)
    {
        for (initial.y=-10000000; initial.y<10000000; initial.y= initial.y+ 1000)
        {
            for (initial.v_x=6000; initial.v_x<15000; initial.v_x= initial.x+ 10)
            {
                for (initial.v_y=6000; initial.v_y<15000; initial.v_y= initial.x+ 10)
                {

                    Calc_orbit(initial, M, max_time, h);
                    printf(".");

                }
            }
        }

    }
    return 0;
}

int At_Start_Pos (situation sit, situation initial)
{
    //returns true if probe is close to (within 500m of) its starting position
    if (abs(sit.x-initial.x)<500 && abs(sit.x-initial.x)<500)
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


void Calc_orbit (situation initial, double M, double max_time, double h)
{//for given inital conditions, calculates an orbit. if  a complete orbit is succesfully generated then it is outputted to a text file

    situation sit;//new variable of type situation
    sit = initial;
    double perihelion= INFINITY, aphelion=0;//set each of these to (bad) initial guesses. they will be improved later.
    double distance;

    //create
    FILE *eccentric;
    eccentric= fopen ("eccentric.txt", "a");//append any new orbit found to the text file
    if (eccentric==NULL)
    {
        printf ("Could not open file, ending program...\n\n");
        return 1;
    }
    else
    {
        //file succesfully opened, program continues.


        while (sit.t < max_time && (At_Start_Pos(sit, initial)==false || sit.t<36000) && Hit_Earth (sit)==false)//terminate ater a long time or when object returns to its starting position, or when the object crashes
        {
            sit = Runge_Kutta(sit, M, h);//calculate next line

            if (At_Start_Pos(sit, initial)==true && sit.t>36000)
            {
                break;
            }
            if (Hit_Earth==true)
            {
                break;
            }

            distance= sqrt(sit.x*sit.x+sit.y*sit.y);
            if (distance < perihelion)
                perihelion=distance;//revise my guess for perhelion
            if (distance > aphelion)
                aphelion= distance;//revise my guess for aphelion.

        }
        if (At_Start_Pos(sit, initial)==true)//if an orbit was succesfullly generated, as opposed to a weird slingshot or something.
        {
            double eccentricity = (aphelion- perihelion)/(aphelion+perihelion);
            printf("!");
            fprintf(eccentric, "%lf\t%lf%lf\t%lf\t%lf\t%lf%lf\t%lf\n", initial.x, initial.y, initial.v_x, initial.v_y, perihelion, aphelion, eccentricity, sit.t);
        }
        fclose(eccentric);
    }
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
    k1v_x= f3(sit, M);
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
    return -G*M*sit.x/(pow((sit.x*sit.x+sit.y*sit.y),1.5));
}
double f4 (situation sit, double M)
{
    //dv_y/dt=
    return -G*M*sit.y/(pow((sit.x*sit.x+sit.y*sit.y),1.5));
}

