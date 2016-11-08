//////////////////////////////////////////////////////////////////////////////////////////////////////
//Joseph Moggridge Computational Physics exercise 4 problem 3                                       //
//                                                                                                  //
//a second attemt to solve this problem much later (15/11/2013)                                     //                                                                        //
//calculate the paths of 2 bodies which are interacting gravitationally.                            //
//use runge kutta numeriacal method to solve differential eqns for the motion of the two bodies     //
//we have 8 eqations to solve                                                                       //
//dx/dt   = attracted.v_x                                                                           //
//dy/dt   = attracted.v_y                                                                           //
//dv_x/dt = -G*(attractor.mass)*(attracted.x - attractor.x)/(pow(distance,3))                       //
//dv_y/dt = -G*(attractor.mass)*(attracted.x - attractor.x)/(pow(distance,3))                       //
//because each of these equations must be applied to both bodies                                    //
//ie we first assume that one is the attractor, and then the other.                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define G 6.673E-11
#define true 1
#define false 0

typedef struct
{
    double x;
    double y;
    double v_x;
    double v_y;
    double mass;
} situation;

double Runge_Kutta ( double z ,char* equation, situation attractor, situation attracted, double h);
//this function takes an ODE of the form:
//d(z)/dt = equation
//with boundary condition:
//z[t_0]= <correct bit of the relevant situation z>
//and uses runge kutta to calculate the next timstep.

int collision (situation earth, situation moon);
//checks if earth and moon have collided


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
    double h= 60;

    //define the equations we are trying to solve
    char* f1 = "attracted.v_x;";//dx/dt= f1
    char* f2 = "attracted.v_y;";//dy/dt= f2
    char* f3 = "-G*(attractor.mass)*(attracted.x - attractor.x)/(pow(distance,3));";//dv_x/dt= f3
    char* f4 = "-G*(attractor.mass)*(attracted.y - attractor.y)/(pow(distance,3));";//dv_y/dt= f4

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

            //calculate pull of the earth on the moon
            temp_moon.x = Runge_Kutta(earth.x, f1, earth, moon, h);
            temp_moon.y = Runge_Kutta(earth.y, f2, earth, moon, h);
            temp_moon.v_x = Runge_Kutta(earth.v_x, f3, earth, moon, h);
            temp_moon.v_y = Runge_Kutta(earth.v_y, f4, earth, moon, h);

            //calculate pull of the moon on the earth
            temp_earth.x = Runge_Kutta( moon.x, f1, moon, earth, h);
            temp_earth.y = Runge_Kutta( moon.y, f2, moon, earth, h);
            temp_earth.v_x = Runge_Kutta( moon.v_x, f3, moon, earth, h);
            temp_earth.v_y = Runge_Kutta( moon.v_y, f4, moon, earth, h);

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

double Runge_Kutta ( double z ,char* equation, situation attractor, situation attracted, double h)
{
    double temp= z;


    double k1= *equation;

    situation increment = {z+(h*k1/2)}

    temp = ;
    double k2= *equation;

    temp = z+(h*k2/2);
    double k3= *equation;

    temp = z+(h*k3);
    double k4= *equation;

    return z+ (h/6)*(k1+ 2*k2+2*k3+k4);
}

