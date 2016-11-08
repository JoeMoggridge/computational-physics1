#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define G 6.67384E-11
int main()
{
    double h,T,t,xe,ye,vxe,vye,xm,ym,vxm,vym,me,mm,xem,xme,yem,yme;//last 4 are to make the later code shorter, representing their positions relative to each other
    double k1xe,k1ye,k1vxe,k1vye,k2xe,k2ye,k2vxe,k2vye,k3xe,k3ye,k3vxe,k3vye,k4xe,k4ye,k4vxe,k4vye;
    double k1xm,k1ym,k1vxm,k1vym,k2xm,k2ym,k2vxm,k2vym,k3xm,k3ym,k3vxm,k3vym,k4xm,k4ym,k4vxm,k4vym;
    t=0;
    printf("This program will simulate the orbits of 2 objects of definable mass.");
    printf("\nEnter the desired time intervals:");
    scanf("%lf",&h);
    printf("\nEnter the desired amount of time:");
    scanf("%lf",&T);
    printf("\nEnter the mass of object 1:");
    scanf("%lf",&me);
    printf("\nEnter the desired starting x coordinate of object 1:");
    scanf("%lf",&xe);
    printf("\nEnter the desired starting y coordinate of object 1:");
    scanf("%lf",&ye);
    printf("\nEnter the desired x direction velocity of object 1:");
    scanf("%lf",&vxe);
    printf("\nEnter the desired y direction velocity of object 1:");
    scanf("%lf",&vye);
    printf("\nEnter the mass of object 2:");
    scanf("%lf",&mm);
    printf("\nEnter the desired starting x coordinate of object 2:");
    scanf("%lf",&xm);
    printf("\nEnter the desired starting y coordinate of object 2:");
    scanf("%lf",&ym);
    printf("\nEnter the desired x direction velocity of object 2:");
    scanf("%lf",&vxm);
    printf("\nEnter the desired y direction velocity of object 2:");
    scanf("%lf",&vym);
    FILE *output;
    output=fopen("output.csv","w");
    if(output!=NULL)//checks to see if the file was opened/created properly
    {
        fprintf(output,"t,xe,ye,vxe,vye,xm,ym,vxm,vym");
        while(t<T)
        {
            xem=xe-xm;//these all allows easier representation of one objects position relative to the other
            xme=xm-xe;
            yem=ye-ym;
            yme=ym-ye;
            fprintf(output,"\n%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",t,xe,ye,vxe,vye,xm,ym,vxm,vym);



            k1xe=vxe;
            k1ye=vye;
            k1vxe=-(mm*G*xem)/(pow((sqrt(xem*xem+yem*yem)),3));//the positions are now based on the other object and the force is calculated using the other objects mass
            k1vye=-(mm*G*yem)/(pow((sqrt(xem*xem+yem*yem)),3));

            k1xm=vxm;
            k1ym=vym;
            k1vxm=-(me*G*xme)/(pow((sqrt(xme*xme+yme*yme)),3));//the entire  set of k's must be performed for each body
            k1vym=-(me*G*yme)/(pow((sqrt(xme*xme+yme*yme)),3));



            k2xe=vxe+(h*0.5*k1vxe);
            k2ye=vye+(h*0.5*k1vye);
            k2vxe=-(mm*G*(xem+h*0.5*k1xe))/(pow((sqrt((xem+h*0.5*k1xe)*(xem+h*0.5*k1xe)+(yem+h*0.5*k1ye)*(yem+h*0.5*k1ye))),3));
            k2vye=-(mm*G*(yem+h*0.5*k1ye))/(pow((sqrt((xem+h*0.5*k1xe)*(xem+h*0.5*k1xe)+(yem+h*0.5*k1ye)*(yem+h*0.5*k1ye))),3));

            k2xm=vxm+(h*0.5*k1vxm);
            k2ym=vym+(h*0.5*k1vym);
            k2vxm=-(me*G*(xme+h*0.5*k1xm))/(pow((sqrt((xme+h*0.5*k1xm)*(xme+h*0.5*k1xm)+(yme+h*0.5*k1ym)*(yme+h*0.5*k1ym))),3));
            k2vym=-(me*G*(yme+h*0.5*k1ym))/(pow((sqrt((xme+h*0.5*k1xm)*(xme+h*0.5*k1xm)+(yme+h*0.5*k1ym)*(yme+h*0.5*k1ym))),3));



            k3xe=vxe+(h*0.5*k2vxe);
            k3ye=vye+(h*0.5*k2vye);
            k3vxe=-(mm*G*(xem+h*0.5*k2xe))/(pow((sqrt((xem+h*0.5*k2xe)*(xem+h*0.5*k2xe)+(yem+h*0.5*k2ye)*(yem+h*0.5*k2ye))),3));
            k3vye=-(mm*G*(yem+h*0.5*k2ye))/(pow((sqrt((xem+h*0.5*k2xe)*(xem+h*0.5*k2xe)+(yem+h*0.5*k2ye)*(yem+h*0.5*k2ye))),3));

            k3xm=vxm+(h*0.5*k2vxm);
            k3ym=vym+(h*0.5*k2vym);
            k3vxm=-(me*G*(xme+h*0.5*k2xm))/(pow((sqrt((xme+h*0.5*k2xm)*(xme+h*0.5*k2xm)+(yme+h*0.5*k2ym)*(yme+h*0.5*k2ym))),3));
            k3vym=-(me*G*(yme+h*0.5*k2ym))/(pow((sqrt((xme+h*0.5*k2xm)*(xme+h*0.5*k2xm)+(yme+h*0.5*k2ym)*(yme+h*0.5*k2ym))),3));



            k4xe=vxe+(h*k3vxe);
            k4ye=vye+(h*k3vye);
            k4vxe=-(mm*G*(xem+h*k3xe))/(pow((sqrt((xem+h*k3xe)*(xem+h*k3xe)+(yem+h*k3ye)*(yem+h*k3ye))),3));
            k4vye=-(mm*G*(yem+h*k3ye))/(pow((sqrt((xem+h*k3xe)*(xem+h*k3xe)+(yem+h*k3ye)*(yem+h*k3ye))),3));

            k4xm=vxm+(h*k3vxm);
            k4ym=vym+(h*k3vym);
            k4vxm=-(me*G*(xme+h*k3xm))/(pow((sqrt((xme+h*k3xm)*(xme+h*k3xm)+(yme+h*k3ym)*(yme+h*k3ym))),3));
            k4vym=-(me*G*(yme+h*k3ym))/(pow((sqrt((xme+h*k3xm)*(xme+h*k3xm)+(yme+h*k3ym)*(yme+h*k3ym))),3));



            xe=xe+((h/6)*(k1xe+(2*k2xe)+(2*k3xe)+k4xe));
            ye=ye+((h/6)*(k1ye+(2*k2ye)+(2*k3ye)+k4ye));
            vxe=vxe+((h/6)*(k1vxe+(2*k2vxe)+(2*k3vxe)+k4vxe));
            vye=vye+((h/6)*(k1vye+(2*k2vye)+(2*k3vye)+k4vye));

            xm=xm+((h/6)*(k1xm+(2*k2xm)+(2*k3xm)+k4xm));
            ym=ym+((h/6)*(k1ym+(2*k2ym)+(2*k3ym)+k4ym));
            vxm=vxm+((h/6)*(k1vxm+(2*k2vxm)+(2*k3vxm)+k4vxm));
            vym=vym+((h/6)*(k1vym+(2*k2vym)+(2*k3vym)+k4vym));

            t=t+h;
        }
        fprintf(output,"\n%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",t,xe,ye,vxe,vye,xm,ym,vxm,vym);
        fclose(output);
    }
    else

    {
        printf("File error.");
    }
    return 0;
}
