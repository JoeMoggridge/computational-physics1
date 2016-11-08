/////////////////////////////////////////////////////////////////
//Joseph Moggridge Computational Physics exercise 4 problem 2
//
//calculate the orbit of a body using runge kutta
//set earth at origin and moon at (385000000, 0)
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

situation Runge_Kutta (situation sit, double  M_e, double M_m, double h);

double Input_Double (void);
int At_Start_Pos (situation sit, situation initial);
int Hit_Earth (situation sit);
int Hit_Moon (situation sit);

//the following functions correspond to the 4 equations we are trying to solve. for example "dx/dt = f1"
double f1 (situation sit);
double f2 (situation sit);
double f3 (situation sit, double M_e, double M_m);
double f4 (situation sit, double M_e, double M_m);


int main()
{
    situation sit;//new variable of type situation
    situation initial;

    //boundary conditions
    initial.t=0;
    double M_e= 6e24;
    double M_m = 7.348e22;

    double closest_approach= INFINITY;
    //printf ("What is the x coordinate of the starting position? (km)\n");
    initial.x= -7000000;
    //printf ("What is the y coordinate of the starting position? (km)\n");
    initial.y= 0;
    //printf ("What is the x coordinate of the starting velocity?(km/s)\n");
    initial.v_x= 0;
    //printf ("What is the y coordinate of the starting velocity? (km/s)\n");
    initial.v_y= 11900;

    /*
    printf ("how long would you like to simulate for? (days)\n");
    double max_time= 24*3600*Input_Double();
    printf ("What time-step would you like to use? (minutes) \n");
    double h= 60*Input_Double();
    */

    //simulate for 50 days
    double max_time= 24*3600*50;
    //recalculate every minute
    double h= 60;

    sit = initial;

    //output to file
    FILE *orbit;
    orbit= fopen ("start_velocities.txt", "w");
    if (orbit==NULL)
    {
        printf ("Could not open file, ending program...\n\n");
        return 1;
    }
    else
    {
        //file succesfully opened, program continues.

        fprintf( orbit, "v_y\tclosest approach\n"); //column headings.

        for (initial.v_y=10000; initial.v_y<12000; initial.v_y= initial.v_y+1)
        {
            closest_approach= INFINITY;
            initial.x= -6378000;
            initial.y= 0;
            initial.v_x= 0;
            initial.t=0;
            sit = initial;

            while (sit.t < max_time )//terminate ater a long time or when object returns to its starting position, or when the object crashes
            {
                sit = Runge_Kutta(sit, M_e, M_m, h);//calculate next line

                if (At_Start_Pos(sit, initial)==true && sit.t>3600)
                {
                    printf(" object has returned to its starting position.\n");
                    break;
                }
                if (Hit_Earth(sit)==true)
                {
                    printf("object has crashed into the earth.\n");
                    break;
                }
                if (Hit_Moon(sit)==true)
                {
                    printf("object has crashed into the moon.\n");
                    break;
                }

                if (fabs(sqrt((sit.x -385000000)*(sit.x-385000000) + sit.y*sit.y)) < closest_approach)
                    closest_approach= fabs(sqrt((sit.x -385000000)*(sit.x-385000000) + sit.y*sit.y));
            }
            printf(".");
            if (Hit_Moon( sit)== false )
                fprintf(orbit, "%lf\t%lf\n", initial.v_y, closest_approach);
        }
    }
    printf("closest approach was: %e\n", closest_approach);

    return 0;
}

int At_Start_Pos (situation sit, situation initial)
{
    //returns true if probe is close to (within 500m of) its starting position
    if (abs(sit.x-initial.x)<100 && abs(sit.x-initial.x)<100)
        return true;
    else
        return false;

}
int Hit_Earth (situation sit)
{
    //returns true if object has hit earth
    if (sit.x*sit.x+sit.y*sit.y < 6.378e6*6.378e6)//radius of earth is 6e6
        return true;
    else
        return false;


}
int Hit_Moon (situation sit)
{
    //returns true if object has hit moon
    if ((sit.x - 385000000)*(sit.x- 385000000) + sit.y*sit.y < 1.737e6*1.737e6)//radius of moon is 1737000m
        return true;
    else
        return false;

}

situation Runge_Kutta (situation sit, double  M_e, double M_m, double h)
{
    //takes an initial situation and calculates the situation at the next timestep.

    situation result;

    situation temp;//this will be used to pass values to the various functions which calculate the k's

    //calculate k1's
    double k1x, k1y, k1v_x, k1v_y;
    k1x= f1(sit);
    k1y= f2(sit);
    k1v_x= f3(sit, M_e, M_m);
    k1v_y= f4(sit, M_e, M_m);

    //calculate k2's
    double k2x, k2y, k2v_x, k2v_y;
    temp.x =sit.x+ h*(k1x)/2;
    temp.y =sit.y+ h*(k1y)/2;
    temp.v_x =sit.v_x+ h*(k1v_x)/2;
    temp.v_y =sit.v_y+ h*(k1v_y)/2;
    k2x= f1(temp);
    k2y= f2(temp);
    k2v_x= f3(temp, M_e, M_m);
    k2v_y= f4(temp, M_e, M_m);

    //calculate k3's
    double k3x, k3y, k3v_x, k3v_y;
    temp.x =sit.x+ h*(k2x)/2;
    temp.y =sit.y+ h*(k2y)/2;
    temp.v_x =sit.v_x+ h*(k2v_x)/2;
    temp.v_y =sit.v_y+ h*(k2v_y)/2;
    k3x= f1(temp);
    k3y= f2(temp);
    k3v_x= f3(temp, M_e, M_m);
    k3v_y= f4(temp, M_e, M_m);

    //calculate k4's
    double k4x, k4y, k4v_x, k4v_y;
    temp.x =sit.x+ h*(k3x);
    temp.y =sit.y+ h*(k3y);
    temp.v_x =sit.v_x+ h*(k3v_x);
    temp.v_y =sit.v_y+ h*(k3v_y);
    k4x= f1(temp);
    k4y= f2(temp);
    k4v_x= f3(temp, M_e, M_m);
    k4v_y= f4(temp, M_e, M_m);

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
double f3 (situation sit, double M_e, double M_m)
{
    //dv_x/dt=
    double distance= sqrt(pow(sit.x-385000000,2 )+ sit.y*sit.y);
    return -G*M_e*sit.x/(pow((sit.x*sit.x+sit.y*sit.y),1.5))- G* M_m*(sit.x- 385000000)/pow(distance, 3);
}
double f4 (situation sit, double M_e, double M_m)
{
    //dv_y/dt=
    double distance= sqrt(pow(sit.x-385000000,2 )+ sit.y*sit.y);
    return -G*M_e*sit.y/(pow((sit.x*sit.x+sit.y*sit.y),1.5))- G* M_m*(sit.y)/pow(distance, 3);
}


