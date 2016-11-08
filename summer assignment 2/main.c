/* calculate the line of regression when given a file with data in the format x, y, x error, y error.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int NumberOfElements=3, NumberUsed=0;//these variables are properties of the array that will store the data

// the line of regression will have form y=ax+b
typedef struct coefficents
    {
        double a;
        double b;
        double a_error;
        double b_error;
    };
double** read_data(char filename[]);
struct coefficents Calculate_coefficents (double **array);
char* recievefilename (void);

int main()
{
    //input the filname
    char filename[100];
    filename = (char*) recievefilename ();
    if (filename==NULL)
      return 1;

    double **arrayofdata= read_data (filename);//read the data in the file and convert to an array
    if (arrayofdata==(double**)1)//if function returns error value (eg: 1), say so, and end program
    {
        printf( "something has gone wrong");
        return 1;
    }

    //calculate the coefficents
    struct coefficents output = Calculate_coefficents(arrayofdata);

    //output the results
    printf("line of regression is y=%lf(+/- %lf)x ", output.a, output.a_error);
    printf("+%lf(+/- %lf) \n", output.b, output.b_error);

    //free the array
    int i;
    for (i = 0; i < 4; i++)
        free(arrayofdata[i]);
    free(arrayofdata);

    return 0;
}

double** read_data(char filename[])
{/* Takes a filneame and returns an array with 4 columns....
The first column is the x values, with errors in the third column.
The second column is the y values with errors given by the fouth column.*/

    double x,y,xerror, yerror;
    double **array;//declare an array of pointers

    FILE *data;
    data= fopen(filename,"r"); //open the specified file for reading
    if (data!=NULL )// check that the file was actually opened
    {
        int  i,n;
        array= (double**) malloc(4*sizeof(double));//declare that there are 4 columns in the array

        for (n=0; n<4; n++)//declare that a variable number of rows exist. To begin with there will be 3.
            array[n]= (double*) malloc(NumberOfElements*sizeof(double));
        /* alternitively the last few steps can be thought of as creating a 1-dimensional array of pointers, which then all point to arrays.*/

        for (i=0; !feof(data); i++)
        {
            if (NumberUsed==NumberOfElements)//if all elements have been used then double the size of the array. ie make the columns twice as high
            {
                int n;
                for (n=0;n<4; n++)
                    array[n]= (double*)realloc(array [n],2*NumberOfElements*sizeof(double));//double the size of each column.

                //check that it worked
                if (array==NULL)
                {//if it didn't work then tell the user and return to 'int main()'.
                    double **pointer=(double**)1;
                    printf ("something has gone wrong!");
                    return pointer;//gtfo
                }
                else//if it worked, then notate the new size of the array.
                    NumberOfElements= 2*NumberOfElements;
            }
            fscanf( data, "%lf    %lf    %lf    %lf", &x, &y, &xerror, &yerror);//read in the data from a given line.

            //store that data in the correct place of the array:
            array [0][i]=x;
            array [1][i]=y;
            array [2][i]=xerror;
            array [3][i]=yerror;

            NumberUsed= NumberUsed+1;//notate that the latest line conatained information and that it has now been stored
        }
        fclose(data);
    }
    else
    {
        printf ( "the file could not be opened.\n");
        return (double**)1;
    }

    return array;
}

struct coefficents Calculate_coefficents (double**array)
{/* Takes the array of data and returns the coefficents of the line of regression. */

    //represent yerror^2 as 's' to make the notation simplr
    //'sum of (p^2/q)' is notated as sum_p_2_q
    double sum_y_s =0, sum_x_2_s =0, sum_x_s =0, sum_xy_s =0, sum_1_s =0;
    int n;
    for (n=0; n<NumberUsed; n++)
    {//calculte the sums.
        sum_y_s   = sum_y_s   +(      array[1][n]          /(array[3][n]*array[3][n]));
        sum_x_2_s = sum_x_2_s +( (array[0][n]*array[0][n]) /(array[3][n]*array[3][n]));
        sum_x_s   = sum_x_s   +(      array[0][n]          /(array[3][n]*array[3][n]));
        sum_xy_s  = sum_xy_s  +( (array[0][n]*array[1][n]) /(array[3][n]*array[3][n]));
        sum_1_s   = sum_1_s   +(           1               /(array[3][n]*array[3][n]));

    }
    struct coefficents results;
    //calculate a
    results.a= ((sum_y_s*sum_x_2_s)-(sum_x_s*sum_xy_s)) / ((sum_1_s*sum_x_2_s)- (sum_x_s*sum_x_s));
    //and error:
    results.a_error= sqrt(sum_x_2_s/((sum_1_s* sum_x_2_s)- (sum_x_s*sum_x_s)))     ;

    //calculate b
    results.b=((sum_1_s*sum_xy_s )-(sum_x_s*sum_y_s)) / ((sum_1_s*sum_x_2_s)-(sum_x_s*sum_x_s));
    //and error:
    results.b_error=  sqrt(sum_1_s/((sum_1_s* sum_x_2_s)- (sum_x_s*sum_x_s)));

    return results;
}

char *recievefilename (void)
{
   char input [100];
   printf("please enter the filname containing the data\n");
    if (scanf( "%99s",input )!==1);//the %99s prevents buffer overflow
   {
      printf("failed to obtain filename correctly")
      return NULL;
   }
   else
      return filename;
}
