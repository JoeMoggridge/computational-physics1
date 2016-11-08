#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    FILE *datafile;
    double K_x[4],K_y[4],K_Vx[4],K_Vy[4],t=0,t_total,x,y,V_x,V_y,h=10,M,G=6.77E-11,days;
    //declare variables

    printf("Please enter starting position of orbiting body\nx-coord: ");
    scanf("%lf",&x);
    printf("y-coord: ");
    scanf("%lf",&y);
    printf("\nPlease enter initial velocity of orbiting body\nx-direction: ");
    scanf("%lf",&V_x);
    printf("y-direction: ");
    scanf("%lf",&V_y);
    printf("\nPlease enter mass of the stationary body, in kg: ");
    scanf("%lf",&M);
    printf("\nPlease enter the length of time you wish to track the orbit, in days: ");
    scanf("%lf",&days);
    //receives all required data from the user
    t_total=days*24*60*60;//converts time from days to seconds

    printf("All necessary data obtained, beginning modelling\n");

    datafile=fopen("datafile.txt","w");

if(datafile!=NULL)//checks file opened correctly
{
    printf("Modelling...\n");

    for(t=0; t<=t_total; t+=h)//for loop runs for as long as user inputted, in increments of 100s
    {
            fprintf(datafile,"%lf\t%lf\t%lf\t%lf\t%lf\n",t,x,y,V_x,V_y);//writes data to file

            K_x[0]=V_x;
            K_y[0]=V_y;
            K_Vx[0]=-G*M*x/pow((x*x)+(y*y),1.5);
            K_Vy[0]=-G*M*y/pow((x*x)+(x*x),1.5);

            K_x[1]=V_x+(h*K_Vx[0]*0.5);
            K_y[1]=V_y+(h*K_Vy[0]*0.5);
            K_Vx[1]=-G*M*(x+(h*K_x[0]*0.5))/pow(((x+(h*K_x[0]*0.5))*(x+(h*K_x[0]*0.5))+((y+(h*K_y[0]*0.5))*y+(h*K_y[0]*0.5))),1.5);
            K_Vy[1]=-G*M*(y+(h*K_y[0]*0.5))/pow(((y+(h*K_y[0]*0.5))*(y+(h*K_y[0]*0.5))+(x+(h*K_x[0]*0.5)*(x+(h*K_x[0]*0.5)))),1.5);

            K_x[2]=V_x+(h*K_Vx[1]*0.5);
            K_y[2]=V_y+(h*K_Vy[1]*0.5);
            K_Vx[2]=-G*M*(x+(h*K_x[1]*0.5))/pow(((x+(h*K_x[1]*0.5))*(x+(h*K_x[1]*0.5))+((y+(h*K_y[1]*0.5))*y+(h*K_y[1]*0.5))),1.5);
            K_Vy[2]=-G*M*(y+(h*K_y[1]*0.5))/pow(((y+(h*K_y[1]*0.5))*(y+(h*K_y[1]*0.5))+(x+(h*K_x[1]*0.5)*(x+(h*K_x[1]*0.5)))),1.5);

            K_x[3]=V_x+(h*K_Vx[2]);
            K_y[3]=V_y+(h*K_Vy[2]);
            K_Vx[3]=-G*M*(x+(h*K_x[2]))/pow(((x+(h*K_x[2]*0.5))*(x+(h*K_x[2]*0.5))+((y+(h*K_y[2]*0.5))*y+(h*K_y[2]*0.5))),1.5);
            K_Vy[3]=-G*M*(y+(h*K_y[2]))/pow(((y+(h*K_y[2]*0.5))*(y+(h*K_y[2]*0.5))+(x+(h*K_x[2]*0.5)*(x+(h*K_x[2]*0.5)))),1.5);

            x=x+(h*(K_x[0]+(2*K_x[1])+(2*K_x[2])+K_x[3])/(float)6);
            y=y+(h*(K_y[0]+(2*K_y[1])+(2*K_y[2])+K_y[3])/(float)6);
            V_x=V_x+(h*(K_Vx[0]+(2*K_Vx[1])+(2*K_Vx[2])+K_Vx[3])/(float)6);
            V_y=V_y+(h*(K_Vy[0]+(2*K_Vy[1])+(2*K_Vy[2])+K_Vy[3])/(float)6);
//All the calculations

    }
    fclose(datafile);

    printf("Modelling complete.");//closes file, and informs user of the success
return 0;

}

else//if file didn't open correctly, informs user

printf("File did not open correctly, please try again.");

return 0;

}
