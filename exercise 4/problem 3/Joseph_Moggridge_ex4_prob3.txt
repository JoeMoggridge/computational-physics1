//////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge Computational Physics exercise 4 problem 3               //
//                                                                          //
//calculate the paths of 2 bodies which are interacting gravitationally.    //
//*****************DOESN'T WORK********************                         //
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define G 6.673E-11
#define true 1
#define false 0

typedef struct
{
    double v_x;
    double v_y;
    double x;
    double y;
    double mass;
} situation;

situation Runge_Kutta ( situation attractor, situation attracted, double h);
//by not being explicit about which variable is the earth and which is the moon, i can use this one function to calculate both the pull of the earth on the moon and vice versa.

int collision (situation earth, situation moon);

//the following functions correspond to the 4 equations we are trying to solve. for example "dx/dt = f1"
double f1 ( situation attractor, situation attracted);
double f2 ( situation attractor, situation attracted);
double f3 ( situation attractor, situation attracted);
double f4 ( situation attractor, situation attracted);


int main()
{
    situation earth;//new variable of type situation
    situation moon;
    //these varibles will store the new situation temporarily so that (for example) the pull of the moon will not be calculated using the future position of the moon as opposed to the current postion of the moon
    situation temp_earth;
    situation temp_moon;

    //boundary conditions
    double time=0;
    earth.mass= 6e24;
    moon.mass= 7.347e22;

    earth.x= 0;
    earth.y= 0;
    earth.v_x= 0;
    earth.v_y= -12.55;

    moon.x= 385000000;
    moon.y= 0;
    moon.v_x= 0;
    moon.v_y= 1022;

    //simulate for 100 days
    double max_time= 24*3600*100;
    //recalculate every minute
    double h= 1;


    //output to file
    FILE *orbit;
    orbit= fopen ("two_bodies.txt", "w");
    if (orbit==NULL)
    {
        printf ("Could not open file, ending program...\n\n");
        return 1;
    }
    else
    {
        //file succesfully opened, program continues.
        int sample=0;//will be used so that the program does not generate reams of data

        fprintf( orbit, "earth.x\tearth.y\tmoon.x\tmoon.y\n"); //column headings.

        while (time < max_time)//terminate ater a long time or when earth and moon collide
        {
            if (sample==0)
                fprintf( orbit, "%lf\t%lf\t%lf\t%lf\n", earth.x, earth.y, moon.x, moon.y);//output line
            sample++;
            if (sample>=600)
                sample=0;


            temp_moon = Runge_Kutta(earth, moon, h);//calculate pull of the earth on the moon
            temp_earth = Runge_Kutta(moon, earth, h);//calculate pull of the moon on the earth
            time=time+h;//next time step

            //store the new situation
            moon= temp_moon;
            earth= temp_earth;

            if (collision(earth, moon)==true)
            {
                printf("object has crashed into the earth.\n");
                break;
            }

        }
        printf("orbit has been writen to file\n");

    }

    return 0;
}


int collision (situation earth, situation moon)
{
     //returns true if earth and moon have collided
    if (fabs(earth.x - moon.x) < 6000000+ 1737000 && fabs(earth.y - moon.y) < 6000000+ 1737000)//if position of earth and moon differ by less than the sum of their radii.
        return true;
    else
        return false;

}

situation Runge_Kutta (situation attractor, situation attracted, double h)
{
    //takes an initial situation and calculates the situation at the next timestep.

    situation result;

    situation temp;//this will be used to pass values to the various functions which calculate the k's


    //calculate k1's
    double k1x, k1y, k1v_x, k1v_y;
    k1x= f1(attractor, attracted);
    k1y= f2(attractor, attracted);
    k1v_x= f3(attractor, attracted);
    k1v_y= f4(attractor, attracted);

    //calculate k2's
    double k2x, k2y, k2v_x, k2v_y;
    temp.x =attracted.x+ h*(k1x)/2;
    temp.y =attracted.y+ h*(k1y)/2;
    temp.v_x =attracted.v_x+ h*(k1v_x)/2;
    temp.v_y =attracted.v_y+ h*(k1v_y)/2;
    k2x= f1(attractor, temp);
    k2y= f2(attractor, temp);
    k2v_x= f3(attractor, temp);
    k2v_y= f4(attractor, temp);

    //calculate k3's
    double k3x, k3y, k3v_x, k3v_y;
    temp.x =attracted.x+ h*(k2x)/2;
    temp.y =attracted.y+ h*(k2y)/2;
    temp.v_x =attracted.v_x+ h*(k2v_x)/2;
    temp.v_y =attracted.v_y+ h*(k2v_y)/2;
    k3x= f1(attractor, temp);
    k3y= f2(attractor, temp);
    k3v_x= f3(attractor, temp);
    k3v_y= f4(attractor, temp);

    //calculate k4's
    double k4x, k4y, k4v_x, k4v_y;
    temp.x =attracted.x+ h*(k3x);
    temp.y =attracted.y+ h*(k3y);
    temp.v_x =attracted.v_x+ h*(k3v_x);
    temp.v_y =attracted.v_y+ h*(k3v_y);
    k4x= f1(attractor, temp);
    k4y= f2(attractor, temp);
    k4v_x= f3(attractor, temp);
    k4v_y= f4(attractor, temp);

    //calculate answer using the k's calculated above
    result.x= attracted.x+ (h/6)*(k1x+ 2*k2x+2*k3x+k4x);
    result.y= attracted.y+ (h/6)*(k1y+ 2*k2y+2*k3y+k4y);
    result.v_x= attracted.v_x+ (h/6)*(k1v_x+ 2*k2v_x+2*k3v_x+k4v_x);
    result.v_y= attracted.v_y+ (h/6)*(k1v_y+ 2*k2v_y+2*k3v_y+k4v_y);


    return result;
}

//the following functions correspond to the 4 equations we are trying to solve. for example "dx/dt = f1"
double f1 (situation attractor, situation attracted)
{
    //dx/dt=
    return attracted.v_x;
}
double f2 (situation attractor, situation attracted)
{
    //dy/dt=
    return attracted.v_y;
}
double f3 (situation attractor, situation attracted)
{
    //dv_x/dt=
    double distance= sqrt(pow((attracted.x-attractor.x),2)+pow((attracted.y-attractor.y),2));
    return -G*(attractor.mass)*(attracted.x - attractor.x)/(pow(distance,3));
}
double f4 (situation attractor, situation attracted)
{
    //dv_y/dt=
    double distance= sqrt(pow((attracted.x-attractor.x),2)+pow((attracted.y-attractor.y),2));
    return -G*(attractor.mass)*(attracted.y - attractor.y)/(pow(distance,3));
}



