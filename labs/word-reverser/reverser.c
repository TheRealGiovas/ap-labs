#include <stdio.h>
#include <stdlib.h>

int reverse(int lenght, char arr[]) {

    int tmp,i = 0;

    for (i = 0;  i < lenght/2; i++) {
        tmp = arr[i];
        arr[i] = arr[lenght - i - 1];
        arr[lenght - i - 1] = tmp;
    }

    printf("%s", arr);
            
    i = 0;
			
    while (arr[i] != '\0'){
                
        arr[i] = '\0';

        i++;
    }
    
    return 0;
}


int main(int argc, char **argv){
	
    int letra,i = 0;
	
    char arreglo[100];

    for(int i = 0 ; i < sizeof(arreglo) ; i++){
		arreglo[i] = '\0';
	}

    while ((letra = getchar()) != EOF) {


    	if (letra == '\n'){ 

       		i = reverse(i,arreglo);
			
    	}
    		
        arreglo[i] = letra;
    		
        i++;
    
    }

	return 0;
}

