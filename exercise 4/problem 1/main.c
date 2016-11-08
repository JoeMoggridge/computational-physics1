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

double Input_Double (void);

//the following functions correspond to the 4 equations we are trying to solve. for example "dx/dt = f1"
double f1 (situation sit);
double f2 (situation sit);
double f3 (situation sit, double M);
double f4 (situation sit, double M);


int main()
{
    situation sit;//new variable of type situation

    //boundary conditions
    sit.t=0;
    printf ("What is the mass of the larger body?\n");
    double M= Input_Double();
    printf ("What is the x coordinate of the starting position?\n");
    sit.x= Input_Double();
    printf ("What is the y coordinate of the starting position?\n");
    sit.y= Input_Double();
    printf ("What is the x coordinate of the starting velocity?\n");
    sit.v_x= Input_Double();
    printf ("What is the y coordinate of the starting velocity?\n");
    sit.v_y= Input_Double();

    printf ("how long would you like to simulate for?\n");
    double max_time= Input_Double();
    printf ("What time-step would you like to use?\n");
    double h= Input_Double();

    //output to file
    FILE *orbit;
    orbit= fopen ("orbit.txt", "w");
    if (orbit==NULL)
    {
        printf ("Could not open file, ending program...\n\n");
        return 1;
    }
    else
    {//file succesfully opened, program continues.

        fprintf( orbit, "time\tx\ty\tv_x\tv_y\n"); //column headings.

        while (sit.t < max_time)
        {
            fprintf( orbit, "%lf\t%lf\t%lf\t%lf\t%lf\n", sit.t, sit.x,sit.y, sit.v_x, sit.v_y);//output line
            sit = Runge_Kutta(sit, M, h);//calculate next line
        }

    }



    return 0;
}

situation Runge_Kutta (situation sit, double  M, double h)
{//takes an initial situation and calculates the situation at the next timestep.

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
double Input_Double (void)
{
    /*
    double inputted_double;
    if (scanf ("%lf", &inputted_double)==1)
        return inputted_double;
    else
    {
        printf("You did not enter a number, try again.\n");
        Input_Double();//does not work since scanf buffer is not cleared.
    }
    */

    char tempstr [20];
    errno=0;

    fgets (tempstr, 20, stdin);//recieve a string from the user. max length of 20 characters.

    //first, check for general error
    if (errno!=0|| tempstr==NULL)
    {
        printf ("failed to read input, try again.\n");
        Input_Double();
    }
    //second, check the string contains a newline character. If it does not then tempstr was likely not long enough.
    /*this does not work*/
    int i;
    int big_enough= false;
    char** str_end;
    for (i=0; i<20; i++)
    {
        if (tempstr[i]== "\n")
        {
            big_enough= true;//notate that the newline character has been found

            //create a pointer to the null character. This will be used when converting the string to a double
            char* ptr;
            ptr= &tempstr[i];
            str_end= &ptr;
        }
    }
    if (big_enough==false)
    {//failed to find newline character
        printf ("Number may have been too long, try again\n");
        Input_Double();
    }


    else
    {//string succesfully read in, proccess it.

        //convert to double
        double inputted_double;
        errno=0;
        inputted_double = strtod(tempstr, str_end);//upon failing to convert charcaters, 'errno' will be changed.

        //check that strtod() worked
        if (errno!=0|| str_end==NULL)//if str_end is a null pointer then something very weird has happened. Might as well catch it here.
        {//strtod() failed.
            printf( "Failed to convert string. Possibly you did not enter a  number?\nTry again\n");
            Input_Double();
        }
        else//strtod() succeeded.
            return inputted_double;//if strod() worked then return the double
    }
}

//the following functions correspond to the 4 equations we are trying to solve. for example "dx/dt = f1"
double f1 (situation sit)
{//dx/dt=
    return sit.v_x;
}
double f2 (situation sit)
{//dy/dt=
    return sit.v_y;
}
double f3 (situation sit, double M)
{//dv_x/dt=
    return -G*M*sit.x/(pow((sit.x*sit.x+sit.y*sit.y),1.5));
}
double f4 (situation sit, double M)
{//dv_y/dt=
    return -G*M*sit.y/(pow((sit.x*sit.x+sit.y*sit.y),1.5));
}

