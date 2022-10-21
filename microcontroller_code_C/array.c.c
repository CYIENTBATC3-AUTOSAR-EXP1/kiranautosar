#include <stdio.h>
int sum=0;

int main()

{

    unsigned int row,column,a[4][4]; //Declaring variables

    printf("enter array elements: \n");

    for(row=0;row<4;row++)

    {

        for(column=0;column<4;column++)

        scanf("%d",&a[row][column]); //taking array elements

    }

    printf("sum= ");

    for(row=0;row<4;row++)

  {

        for(column=0;column<4;column++)

        {

            if((row-column)<=1)

            {

               sum=sum+a[row][column];  //logic for sum

            }

        }

    }

    printf("%d",sum); //printing sum

    return 0;

}