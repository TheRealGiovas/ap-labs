#include <stdio.h>



/* month_day function's prototype*/
void month_day(int year, int yearday, char * pmonth[], int pday[]);

int main(int args, char *argv[]) {

int normal[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int biciesto[13] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static char *pmonth[] = {

       "January", "February", "March",
       "April", "May", "June",
       "July", "August", "September",
       "October", "November", "December"
};

int year = atoi(argv[1]);

int yearday= atoi(argv[2]);

    if(year <= 0 || year <=0){
        printf("Invalid Input");
    }

    if (year%4 == 0)
    {
        /* Es biciesto*/
        month_day(year,yearday, &pmonth,  &biciesto);
    }else{
        //no es biciesto
        month_day (year,yearday, &pmonth,  &normal);
    }
    
    
    return 0;
}

void month_day(int year, int yearday, char * pmonth[], int *pday){
    int month = 0;
    int day = yearday;
    
    for (int i = 0; i < 12; i++)
    {
        int tmp = pday[i];

        if (day <= tmp){
            month = i;
            day = tmp;
            break;
        }else{
            day -= tmp;
        }
    }
  printf("%s %d, %d\n", pmonth[month],day, year);
    

    
}
