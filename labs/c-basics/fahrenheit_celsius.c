#include <stdio.h>
#include <stdlib.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

//argc, significa "argument count". Osea cuantos argumentos recibió la consola. al menos va a recibir uno, pues el primer argumento es
//el nombre del archivo a abrir.

//argv es él array con todos los argumentos que se le pasan por consola a la función main


int main(int argc, char *argv[])
{
    
    
    if(argc==2){

        int degrees = atoi(argv[1]);        

        printf("Fahrenheit: %3d, Celcius: %6.1f\n", degrees, (5.0/9.0)*(degrees-32));

    } else if(argc == 4){

        int fahr;

        int lower = atoi(argv[1]);

        int upper = atoi(argv[2]);

        int step = atoi(argv[3]);

        for (fahr = lower; fahr <= upper; fahr = fahr + step)
	        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
        
    }
    
    
    return 0;
}
