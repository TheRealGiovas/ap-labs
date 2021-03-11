#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
char *  REPORT_FILE  = "packages_report.txt";
#define LENGTH 100000
void analizeLog(char *logFile, char *report);
void analizarLinea(char *line, int length, int currentPackage);
void oldest(char* date1, char* name);
void separarFecha(char* fecha, int esPrincipal);
void separarHora(char* hora, int esPrincipal);



struct package {

    char* Name;

    char* InstallDate;

    char* LastUpdateDate;

    int HowManyUpdates;

    char* RemovalDate;

    char* Type;

    int removed;

    int upgraded;

};

struct package* packageArray[LENGTH];




int _TotalInstalledPackages = 0;
int _TotalRemovedPackages   = 0;
int _TotalUpgradedPackages  = 0;
int _TotalCurrentInstalled  = 0;

char* _OldestPackageName;            
char* _OldestPackageFecha; 

int _OldestPackageYear;
int _OldestPackageMes;
int _OldestPackageDia;
int _OldestPackageHora;
int _OldestPackageMinuto;

char listaPaquetes[LENGTH];



char* _NewestPackageName;            
char* _NewestPackageFecha;

int _NewestPackageYear;
int _NewestPackageMes;
int _NewestPackageDia;
int _NewestPackageHora;
int _NewestPackageMinuto;

char* _tmpName;            
char* _tmpFecha; 

int tmpYear;
int tmpMes;
int tmpDia;
int tmpHora;
int tmpMinuto;


char* _PackagesWithNoUpgradesList[LENGTH];
int _ALPM_SCRIPTTLETCount = 0;
int _ALPM_Count = 0;        
int _PACMAN_Count =0; 

int pointerPackagesList = 0;

void llenarPackagesSinUpgrades(){
    for (int i = 0; i < LENGTH; i++)
    {
         _PackagesWithNoUpgradesList[i] = NULL;
    }
    
       int primer = 0;

    for(int y = 0; y < LENGTH; y++){
		if(packageArray[y] != NULL){
            char * tmp;
            int wasUpgraded = packageArray[y]->upgraded;

            if(wasUpgraded == 0){

                if(primer == 0){

                primer = 1;
                tmp = packageArray[y] ->Name;

                strcat(listaPaquetes,tmp);
                
                }else{

                tmp = packageArray[y] ->Name;
                strcat(listaPaquetes, " , ");
                strcat(listaPaquetes,tmp); 



                }

		    }
	    }
    } 

}


void getListaPackagesSinUpgrades(){
    int primer = 0;

    for(int y = 0; y < LENGTH; y++){
		if(packageArray[y] != NULL){
            char * tmp;
            int wasUpgraded = packageArray[y]->upgraded;

            if(wasUpgraded == 0){

                if(primer == 0){

                primer = 1;
                tmp = packageArray[y] ->Name;

                strcat(listaPaquetes,tmp);
                
                }else{
                
                tmp = packageArray[y] ->Name;
                strcat(listaPaquetes, " , ");
                strcat(listaPaquetes,tmp); 



                }

		    }
	    }
    }  

}


void escribir(char * report){



    
    FILE * fp = fopen(report,"w");
	fprintf(fp,"Pacman Packages Report\n");
	fprintf(fp,"----------------------\n");
	fprintf(fp,"-Installed  : %d\n",_TotalInstalledPackages);
	fprintf(fp,"-Removed  : %d\n",_TotalRemovedPackages);
	fprintf(fp,"-Upgraded  : %d\n",_TotalUpgradedPackages);
	fprintf(fp,"-Current installed  : %d\n",_TotalCurrentInstalled);
	fprintf(fp,"\n");
    fprintf(fp,"-------------\n");
    
    fprintf(fp,"General Stats   \n");
	fprintf(fp,"--------------\n");
	fprintf(fp,"- Oldest package               : %s\n",_OldestPackageName);
	fprintf(fp,"- Newest package               : %s\n",_NewestPackageName);
	fprintf(fp,"- Package with no upgrades     : %s\n", listaPaquetes);
	fprintf(fp,"- [ALPM-SCRIPTTLET] type count : %d\n",_ALPM_SCRIPTTLETCount);
    fprintf(fp,"- [ALPM] count                 : %d\n",_ALPM_Count);
    fprintf(fp,"- [PACMAN] count               : %d\n",_PACMAN_Count);
	fprintf(fp,"\n");

	fprintf(fp,"List of packages\n");
    fprintf(fp,"----------------------\n");

	for(int y = 0; y < LENGTH; y++){
		if(packageArray[y] != NULL){
			fprintf(fp,"- Package Name              : %s\n",packageArray[y]->Name);
			fprintf(fp,"\t- Install date      : %s\n",packageArray[y]->InstallDate);
			fprintf(fp,"\t- Last update date  : %s\n",packageArray[y]->LastUpdateDate);
			fprintf(fp,"\t- How many updates  : %d\n",packageArray[y]->HowManyUpdates);
			fprintf(fp,"\t- Removal date      : %s\n",packageArray[y]->RemovalDate);
		}
	}	
	fclose(fp);
}


void reinstaldoUpdate(char * nombre){

for(int i = 0; i < LENGTH; i++){
        if(packageArray[i] != NULL){
            char * nombreTemp = packageArray[i]->Name;
            //Busca el paquete
            if (strcmp(strdup(nombre), strdup(nombreTemp)) == 0){
                
                //Le suma 1 al total de updates           
                    
                    int actualUpdates = packageArray[i] -> HowManyUpdates;
                    
                    
                    packageArray[i] -> HowManyUpdates = actualUpdates + 1;
                    
                    
                    packageArray[i] -> removed = 0;
                    
                        
                }                
            }
            
        }
   

}

void remover(char* nombre, char* fecha){

for(int i = 0; i < LENGTH; i++){
        if(packageArray[i] != NULL){
            char * nombreTemp = packageArray[i]->Name;
            //Busca el paquete
            if (strcmp(strdup(nombre), strdup(nombreTemp)) == 0){
                
                //Le asigna la remove date, updates++ y lo flagea como removed                 
                    
                    int actualUpdates = packageArray[i] -> HowManyUpdates;
                    
                    packageArray[i] -> removed = 1;
                    packageArray[i] -> HowManyUpdates = actualUpdates + 1;
                    packageArray[i] -> RemovalDate = strdup(fecha);
                    
                    
                        
                }                
            }
            
        }
            
}


int buscarRepetido(char*nombre){

    for(int i = 0; i < LENGTH; i++){
        if(packageArray[i] != NULL){
            char * nombreTemp = packageArray[i]->Name;
            if (strcmp(strdup(nombre), strdup(nombreTemp)) == 0){
               // printf(" %s  y %s \n", nombre, nombreTemp);
                return 1;
                    
                    }                
            }
            
        }
            return 0;
}

int Actualizar(char*nombre, char* fecha){

    for(int i = 0; i < LENGTH; i++){
        if(packageArray[i] != NULL){
            char * nombreTemp = packageArray[i]->Name;
            //Busca el paquete
            if (strcmp(strdup(nombre), strdup(nombreTemp)) == 0){
                int upgrade = packageArray[i] -> upgraded;
                //Checa que él paquete esté actualizado, sí no lo marca
                    if (upgrade == 0){
                    
                    int actualUpdates = packageArray[i] -> HowManyUpdates;
                    
                    packageArray[i] -> upgraded = 1;
                    packageArray[i] -> HowManyUpdates = actualUpdates + 1;
                    packageArray[i] -> LastUpdateDate = strdup(fecha);
                    return 1;
                    
                    }    
                }                
            }
            
        }
            return 0;
}


void traversePaquetes(){
	int i;
	for(i = 0; i < LENGTH; i++){
		if(packageArray[i] != NULL){


			printf("- Package Name        : %s\n", packageArray[i]->Name);
            printf("  - Install date      : %s\n", packageArray[i]->InstallDate);
            printf("  - Last update date  : %s\n", packageArray[i]->LastUpdateDate);
            printf("  - How many updates       : %d\n", packageArray[i]->HowManyUpdates);
            printf("  - Removal date      : %s\n", packageArray[i]->RemovalDate);
		}
	}
	printf("\n");
}

void infoPaquete(char * nombre){
	int i;
	for(i = 0; i < LENGTH; i++){
		if(packageArray[i] != NULL){

            char * nombreTemp = packageArray[i]->Name;
            if (strcmp(strdup(nombre), strdup(nombreTemp)) == 0){

			printf("- Package Name        : %s\n", packageArray[i]->Name);
            printf("  - Install date      : %s\n", packageArray[i]->InstallDate);
            printf("  - Last update date  : %s\n", packageArray[i]->LastUpdateDate);
            printf("  - How many updates       : %d\n", packageArray[i]->HowManyUpdates);
            printf("  - Removal date      : %s\n", packageArray[i]->RemovalDate);
            
            }
		}
	}
	printf("\n");
}


int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o \n");
	return 1;
    }
    
    if(argc >= 5){
        printf("parametro %s\n", REPORT_FILE);
        REPORT_FILE = argv[4];
    }else{
        printf("Error not enough parameters\n ");
        return 0;
    }

    
    struct stat buffer;
    int exist = stat(argv[2],&buffer);
    
    if(exist != 0){
        printf("No input file was found\n ");
        return 0;
    }    

    analizeLog(argv[2], REPORT_FILE);

    return 0;
}


void analizeLog(char *logFile, char *report) {

    

    printf("Generating Report from: [%s] log file\n", logFile);

    // Implement your solution here.
    
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    
    fp = fopen(logFile, "r");

    int currentPackage = 0;

    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
      //  printf("Retrieved line of length %zu:\n", read);
        //printf("%s", line);
        analizarLinea(line,read,currentPackage);
        currentPackage++;

    }

   
    printf("Report is generated at: [%s]\n", report);
    fclose(fp);
    llenarPackagesSinUpgrades();
   // getListaPackagesSinUpgrades();
    remove(report);
    escribir(report);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);



   
}


void analizarLinea(char *line, int length, int currentPackage){

    //printf("line %s \n",line);

     // tokenizamos el array

     //printf("ALTO");
    const char s[2] = " ";
    
    char *token;

    char* result[200];

    for (int  i = 0; i < 200; i++)
    {
        result[i] = NULL;
    }
    
 
    /* get the first token */
    token = strtok(line, s);

    int i = 0;
   
    /* walk through other tokens */
    while( token != NULL ) {
    //  printf("Palabra : %s\n", token );
      result[i] = token;
      token = strtok(NULL, s);
      //printf("Palabra : %s\n", result[i]);
      i++;
      
    }



    char * hora = strdup(result[0]);
    char * fecha = strdup(result[0]);
    int type2 = 0;
  //  char *ptr;
    char fechaChida[15];
    char horaChida[15];
    for (int i = 0; i < 15; i++)
    {
        fechaChida[i] = '\0';
    }
    
    
    for (int i = 0; i < 15; i++)
    {
        horaChida[i] = '\0';
    }
    
    char * tempFecha;
    char * tempHora;
    char * tempType;
    char * tempInstruction;
    char * tempNombre;


    char * temporalhora = (char *) malloc(50);
    char * horatemporal = strdup(hora);
    char * horatemporal2 = strdup(hora);

    for (int i = 0; i < 20; i++)
    {
        if(hora[i] == 'T'){

            for (int j = 0; j < 11; j++)
            {
               fechaChida[j] = horatemporal[j];
            }
            tempFecha = strdup(fechaChida);

            int x = 0;

            for (int j = 12; j < 17; j++)
            {  
               horaChida[x] = horatemporal2[j];
               x++;
            }
            horaChida[x] = ']';
            //printf("hora chida%s\n",horaChida);
            tempHora =  strdup(horaChida);
           // printf("hora chida%s\n",result[1]);

            type2 = 1;




                

               
            
        }
    }


                
    if(type2 ==1){
       
            if( result[4] == NULL){
                return;
            }
            tempType = strdup(result[1]);
            tempInstruction = strdup(result[2]);
            tempNombre = strdup(result[3]);

            result[0] = strdup(tempFecha);
            result[1] = strdup(tempHora);
            result[2] = strdup(tempType);
            result[3] = strdup(tempInstruction);
            result[4] = strdup(tempNombre);


    }else{
       

    }
      

            
    
            if(strcmp(result[2],"[PACMAN]") == 0){
                _PACMAN_Count++;
            }
            
            if(strcmp(result[2],"[ALPM]") == 0){
                _ALPM_Count++;
            }
            
            if(strcmp(result[2],"[ALPM-SCRIPTLET]") == 0){
                _ALPM_SCRIPTTLETCount++;
            }

    if(strcmp(result[3],"upgraded") == 0 ||strcmp(result[3],"installed") == 0 || strcmp(result[3],"removed") == 0 ){
       

   


        //Inicializamos el nombre del paquete
        
        
        if (strcmp(result[3],"installed") == 0){
            
            int llegao = 0;

            char *nombre = result[4];


            llegao = buscarRepetido(nombre);

            if ( llegao == 1 )
            {
                _TotalRemovedPackages--;
                _TotalCurrentInstalled++;
                reinstaldoUpdate(nombre);

                 char fechaInstal[50];

            
            for (int i = 0; i < 50; i++)
            {
                fechaInstal[i] = '\0';
                
            }

            strcat(fechaInstal,result[0]);
            strcat(fechaInstal," ");
            strcat(fechaInstal,result[1]);

            char fecha[51];

            

            for (int current = 1; current < 17; current++)
            {
                if(fechaInstal[current] != '\0'){

                    fecha[current] = fechaInstal[current];

                   
                
                }else{
                    break;
                }
                /* code */
            }
            

                oldest(fecha, nombre);

            }else{
            

            //Inicializamos la fecha de instalación

            char fechaInstal[50];

            
            for (int i = 0; i < 50; i++)
            {
                fechaInstal[i] = '\0';
                
            }

            strcat(fechaInstal,result[0]);
            strcat(fechaInstal," ");
            strcat(fechaInstal,result[1]);

            char otroexample[51];

            

            for (int current = 1; current < 17; current++)
            {
                if(fechaInstal[current] != '\0'){

                    otroexample[current] = fechaInstal[current];

                   
                
                }else{
                    break;
                }
                /* code */
            }
            

           

            //Inicializamos la fecha de update

            char* ultimoUpdate = otroexample;

            
            

            //Inicializamos número de updates a 0     

            int cuantosUpdates = 0;

            char* fechaRemovido  = "-";

            char* tipo = result[2];


            //Actualiza el conteo
            
            _TotalInstalledPackages++;
            _TotalCurrentInstalled++;

            //Compara las fechas más viejas y más nuevas para ver sí el paquete es uno de los 2
            //sí lo es entonces lo marca como newest u oldest según sea

            oldest(otroexample,nombre);

                    
            struct package* newPackage = (struct package*) malloc(sizeof(struct package));

            newPackage -> upgraded = 0;

            newPackage -> removed = 0;

            newPackage->Name = strdup(nombre);
            newPackage->InstallDate = strdup(otroexample);
            newPackage->LastUpdateDate = strdup(otroexample);
            newPackage->HowManyUpdates = cuantosUpdates;
            newPackage->RemovalDate = strdup(fechaRemovido);
            newPackage->Type = strdup(tipo);

            // { strdup(nombre),strdup(fechaInstal),strdup(ultimoUpdate),strdup(cuantosUpdates),strdup(fechaRemovido),strdup(tipo)};

            packageArray[currentPackage] = newPackage;
           // infoPaquete(nombre);
            }
        }

        if (strcmp(result[3],"upgraded") == 0){

            char fechaInstal[50];

            
            for (int i = 0; i < 50; i++)
            {
                fechaInstal[i] = '\0';
                
            }

            strcat(fechaInstal,result[0]);
            strcat(fechaInstal," ");
            strcat(fechaInstal,result[1]);

            char  fecha[51];

            for (int current = 1; current < 17; current++)
            {
                if(fechaInstal[current] != '\0'){

                    fecha[current] = fechaInstal[current];

                }else{
                    break;
                }
                /* code */
            }
            


        char *nombre = result[4];

        //infoPaquete(nombre);

        int wasNeverUpgradedBefore = Actualizar(nombre, fecha);

        if (wasNeverUpgradedBefore == 1)
        {
            _TotalUpgradedPackages++;
        }
        

      
      //  printf("fecha de actualizacion %s   %s\n", fecha, nombre);
            
        }


        if (strcmp(result[3],"removed") == 0){
            
        char fechaInstal[50];

            
            for (int i = 0; i < 50; i++)
            {
                fechaInstal[i] = '\0';
                
            }

            strcat(fechaInstal,result[0]);
            strcat(fechaInstal," ");
            strcat(fechaInstal,result[1]);

            char  fecha[51];

            for (int current = 1; current < 17; current++)
            {
                if(fechaInstal[current] != '\0'){

                    fecha[current] = fechaInstal[current];

                }else{
                    break;
                }
                /* code */
            }
            


        char *nombre = result[4];
        //infoPaquete(nombre);
        // - Asignar remove date y a la flag fijarla como removido, totalupdates++

        remover(nombre, fecha);

        _TotalRemovedPackages++;
        _TotalCurrentInstalled--;


        }
        
       
        
        
    }



    

    


    
}


void oldest(char* date1, char* name){

    //Verificar sí ya existe algún paquete newest, sí no existe newest entonces tampoco existe oldest

    if(_NewestPackageFecha == NULL){

        _NewestPackageFecha = strdup(date1);
        _NewestPackageName = strdup(name);

        _OldestPackageFecha = strdup(date1);
        _OldestPackageName = strdup(name);

         //
        //asignar la fecha del paquete a las variables que ya tenemos de apoyo
        //
        const char s[2] = " ";
        
        char *token;

        char* result[200];

        for (int  i = 0; i < 200; i++)
        {
            result[i] = NULL;
        }
        

        

        /* get the first token */
        token = strtok(date1, s);
        
        int i = 0;
    
        /* walk through other tokens */
        while( token != NULL ) {
        //  printf("Palabra : %s\n", token );
        result[i] = token;
        //printf("Palabra : %s,%d\n", result[i],i);
        token = strtok(NULL, s);
        //printf("Palabra : %s\n", result[i]);
        i++;  
        
        }

       // printf("Palabra : %s\n", result[1]);
        //asignamos oldest fecha
        separarFecha(strdup(result[0]),2);

        
        //asignamos oldest hora
        separarHora(strdup(result[1]),2);


         //asignamos oldest fecha
        separarFecha(strdup(result[0]),1);

        
        //asignamos oldest hora
        separarHora(strdup(result[1]),1);
      
    // printf("%s","si llegó");
    }else{

        //
        // Sí ya hay un oldest y un newest entonces hay que acompararlo
        //
        const char s[2] = " ";
        
        char *token;

        char* result[200];

        for (int  i = 0; i < 200; i++)
        {
            result[i] = NULL;
        }
        

        

        /* get the first token */
        token = strtok(date1, s);
        
        int i = 0;
    
        /* walk through other tokens */
        while( token != NULL ) {
        //  printf("Palabra : %s\n", token );
        result[i] = token;
       // printf("Palabra : %s,%d\n", result[i],i);
        token = strtok(NULL, s);
        //printf("Palabra : %s\n", result[i]);
        i++;  
        
        }

         //asignamos fecha
        separarFecha(strdup(result[0]),0);

        
        //asignamos  hora
        separarHora(strdup(result[1]),0);

        //
        //Sí alguna la fecha es mayor al del oldest entonces lo ponemos como oldest
        //

        if(tmpYear > _OldestPackageYear){

        separarFecha(strdup(result[0]),1);
        
        separarHora(strdup(result[1]),1);
        
        _OldestPackageFecha = strdup(date1);
        _OldestPackageName = strdup(name);
        
        }else if(tmpYear == _OldestPackageYear){

                if(tmpMes > _OldestPackageMes){
                    separarFecha(strdup(result[0]),1);
        
                    separarHora(strdup(result[1]),1);
                     
                    _OldestPackageFecha = strdup(date1);
                    _OldestPackageName = strdup(name);

                    }else if(tmpMes == _OldestPackageMes){
                        
                            if(tmpDia > _OldestPackageDia){

                                separarFecha(strdup(result[0]),1);
        
                                separarHora(strdup(result[1]),1);

                                 _OldestPackageFecha = strdup(date1);
                                 _OldestPackageName = strdup(name);
                                }else if(tmpDia == _OldestPackageDia){
                                        
                                    if(tmpHora > _OldestPackageHora){

                                        separarFecha(strdup(result[0]),1);
        
                                        separarHora(strdup(result[1]),1);                                        
                                        _OldestPackageFecha = strdup(date1);
                                        _OldestPackageName = strdup(name);                                        

                                        }else if(tmpHora == _OldestPackageHora){
                                            
                                                if(tmpMinuto > _OldestPackageMinuto){

                                                    separarFecha(strdup(result[0]),1);
        
                                                    separarHora(strdup(result[1]),1);

                                                    _OldestPackageFecha = strdup(date1);
                                                    _OldestPackageName = strdup(name);

                                                }
                                        }    
                                    }    
                        }

        }
        
        
        if(tmpYear < _NewestPackageYear){

        separarFecha(strdup(result[0]),1);
        
        separarHora(strdup(result[1]),1);
        
        _NewestPackageFecha = strdup(date1);
        _NewestPackageName = strdup(name);
        
        }else if(tmpYear == _NewestPackageYear){

                if(tmpMes < _NewestPackageMes){
                    separarFecha(strdup(result[0]),1);
        
                    separarHora(strdup(result[1]),1);
                     
                    _NewestPackageFecha = strdup(date1);
                    _NewestPackageName = strdup(name);

                    }else if(tmpMes == _NewestPackageMes){
                        
                            if(tmpDia < _NewestPackageDia){

                                separarFecha(strdup(result[0]),1);
        
                                separarHora(strdup(result[1]),1);

                                 _NewestPackageFecha = strdup(date1);
                                 _NewestPackageName = strdup(name);
                                }else if(tmpDia == _NewestPackageDia){
                                        
                                    if(tmpHora < _NewestPackageHora){

                                        separarFecha(strdup(result[0]),1);
        
                                        separarHora(strdup(result[1]),1);                                        
                                        _NewestPackageFecha = strdup(date1);
                                        _NewestPackageName = strdup(name);                                        

                                        }else if(tmpHora == _NewestPackageHora){
                                            
                                                if(tmpMinuto < _NewestPackageMinuto){

                                                    separarFecha(strdup(result[0]),1);
        
                                                    separarHora(strdup(result[1]),1);

                                                    _NewestPackageFecha = strdup(date1);
                                                    _NewestPackageName = strdup(name);

                                                }
                                        }    
                                    }    
                        }

        }

    }


}


//sí es principal le mandan un 0    
void separarFecha(char* fecha, int esPrincipal){


        const char s[2] = "-";
        
        char *token;

        char* result[200];

        for (int  i = 0; i < 200; i++)
        {
            result[i] = NULL;
        }
        
    
        /* get the first token */
        token = strtok(fecha, s);

        char years[5];

        char y[5];
        for (int i = 1; i < 5; i++)
        {
            y[i]= '\0';
            years[i] = '\0';
        }
        
       // y = token;
        
        for (int i = 1; i < 5; i++)
        {
            y[i] = token[i];
            strcat(years, &y[i]);
         //   printf("valor %c",y[i]);

        }
       // printf("el año tioooo %d",atoi(years));

        int i = 0;
    
        /* walk through other tokens */
        while( token != NULL ) {
        //  printf("Palabra : %s\n", token );
        result[i] = token;
        token = strtok(NULL, s);
        //printf("Palabra : %s\n", result[i]);
        i++;  
        
        }
        
        //char year[] = ;
        
        //printf("nuestro year %s",result[0]);

        //si es 0 entonces es tmp
        if(esPrincipal == 0){
        tmpYear = atoi(years);
        tmpMes = atoi(result[1]);
        tmpDia = atoi(result[2]);
        }

        if(esPrincipal == 1){
        _OldestPackageYear = atoi(years);
        _OldestPackageMes = atoi(result[1]);
        _OldestPackageDia = atoi(result[2]);
        }

        if(esPrincipal == 2){
        _NewestPackageYear = atoi(years);
        _NewestPackageMes = atoi(result[1]);
        _NewestPackageDia = atoi(result[2]);
        }




}

//sí es principal le mandan un 0    
void separarHora(char* hora, int esPrincipal){       
                
        const char s[2] = ":";
        
        char *token;

        char* result[200];

        for (int  i = 0; i < 200; i++)
        {
            result[i] = NULL;
        }
        
    
        /* get the first token */
        token = strtok(hora, s);

        
       // printf("el año tioooo %d",atoi(years));

        int i = 0;
    
        /* walk through other tokens */
        while( token != NULL ) {
        //  printf("Palabra : %s\n", token );
        result[i] = token;

        token = strtok(NULL, s);
        //printf("Palabra : %s\n", result[i]);
        i++;  
        
        }

        
        if (esPrincipal == 0)
        {
            tmpHora = atoi(result[0]);
            tmpMinuto = atoi(result[1]);

        }

        if (esPrincipal == 1)
        {
            _OldestPackageHora = atoi(result[0]);
            _OldestPackageMinuto = atoi(result[1]);
        }

        if (esPrincipal == 2)
        {
            
            _NewestPackageHora = atoi(result[0]);
            _NewestPackageMinuto = atoi(result[1]);
        }
        
  
}
