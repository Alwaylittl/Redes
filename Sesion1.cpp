//============================================================================
////============================================================================
// ----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES -----------
// ---------------------------- CURSO 2022/23 --------------------------------
// ----------------------------- SESION1.CPP ---------------------------------
//============================================================================
//Realizado por Adrián Lancho Durán y Javier Arias Rodriguez
// Grupo 2: Lunes, de 10:00 a 11:30 h


#include <stdio.h>
#include <stdio_ext.h>
#include <iostream>
#include "linkLayer.h"


using namespace std;

/*
Este método va a imprimir o mostrar por pantalla cada uno de los dispositivos que se encuentran en nuestro PC,
los cuáles se encuentran almacenados en la variable "pcap_if_t *avail_ifaces2".
*/
void imprimirDispositivos ( pcap_if_t *avail_ifaces2){
int contador  = 0;

while ( avail_ifaces2 != NULL ){
    cout<< "[" << contador <<"] "<< avail_ifaces2->name<<endl;
    avail_ifaces2 = avail_ifaces2->next;
    contador = contador + 1;

}
}

/*
Este método primeramente lo que va a realizar es que va a pedir al usuario cuando se ejecute que elija 
una opción (entre los dispositivos que se encuentran en nuestro PC los cuáles hemos mostrado por pantalla
anteriormente, van a estar enumerados del 0 al 7). Una vez que el usuario ha elegido la opción, este método 
va a devolver el dispositivo que se identifica con la opción del 0 al 7 que hemos elegido.
*/
char *  seleccionarInterfaz( pcap_if_t *avail_ifaces){

int opcion;
char *dispositivo= NULL;
    
cin>>opcion;

int i = 0;
while(i != opcion){
    avail_ifaces = avail_ifaces->next;
    i++;
}

dispositivo = avail_ifaces->name;

return dispositivo;


}

/*
Este método va a imprimir la MAC origen
*/
void imprimirMACFuente (unsigned char MACaddr[6]){

int j = 0;

cout<<"La MAC es: ";
for(j = 0; j<5; j++){
    printf("%.2X:", MACaddr[j]);
   
}
     printf("%.2X", MACaddr[5]);
     cout<<""<<endl;

}

/*
Este método va a realizar una serie de funciones o tareas, como se puede observar le pasamos por parámetros la interfaz que 
va a ser un parámetro de entrada/salida y el nombre del dispositivo.

1.setDeviceName: Devuelve la interfaz que se identifica con el nombre del dispositivo que hemos pasado por parámetro.
2. getMACAdapter: Obtenemos la MAC de la interfaz.
3. imprimirMACR: Imprimimos la MAC de la interfaz que pasamos por la parámetros

*/
void conseguirMAC (interface_t &iface, char *nombre){

    setDeviceName(&iface, nombre);
    GetMACAdapter(&iface);
    imprimirMACFuente(iface.MACaddr);
         
}

/*
Este módulo lo que va a devolver es un 0 y va a imprimir el mensaje que el puerto se ha abierto correctamente, en caso contrario 
saltaría un mensaje de que al abrir el puerto se ha producido un error, en ese caso, va a devolver un 1.
*/
int OpenPort(interface_t *iface){
    int Puerto=OpenAdapter(iface);
        if(Puerto != 0)
        {
            printf("Error al abrir el puerto\n");
            getch();
            return(1);
        }
        else
            printf("Puerto abierto correctamente\n");
            return 0;
}

/*
Este módulo llamado enviarTrama tiene como parámetros la interfaz que no va a ser modificada, la MAC fuente de la interfaz elegida,
la MAC destino de la interfaz elegida, el tipo de la trama y el dato de la trama es decir el carácter.
Este método va a enviar la trama que hemos construido con los siguientes parámetros que son:

1- MAC fuente de la interfaz elegida
2- MAC destino de la interfaz elegida
3- Tipo de la trama
4. Dato de la trama (Carácter)

Una vez enviada la trama para comprobar que se ha enviado correctamente, la tenemos que capturar a través de la herramienta Wireshark
*/
void EnviarTrama(interface_t iface,unsigned char mac_src[],unsigned char mac_dst[],unsigned char type[],unsigned char caracter){
   
    unsigned char *dato = new unsigned char();
    dato[0] = caracter;

  
    unsigned char *frame = BuildFrame(mac_src,mac_dst,type,dato); //Construimos la trama
    SendFrame(&iface,frame,1);//Enviamos la trama

    delete dato;
    free(frame);

}

/*
Este método va a imprimir la MAC destino
*/
void imprimirMACDestino (unsigned char MACaddr[6]){

int j = 0;

cout<<"La MAC destino es: ";
for(j = 0; j<5; j++){
    printf("%.2X:", MACaddr[j]);
   
}
     printf("%.2X", MACaddr[5]);
     cout<<""<<endl;

}

/*
Este módulo llamado RecibirCaracter tiene como único parámetro la interfaz, principalmente este método va a recibir la trama con el método "ReceiveFrame",
que anteriormente hemos enviado con el método "SendFrame", una vez que hemos obtenido la trama, obtenemos el caracter que hemos enviado con anterioridad 
y lo devolvemos, en el caso contrario que no hayamos recibido ninguna trama, no obtendremos el caracter y por lo tanto devolverá 0.
*/
unsigned char RecibirCaracter(interface_t iface){

    apacket_t trama ;
    unsigned char dato ;
    trama = ReceiveFrame (&iface);

    if(trama.packet != NULL){

    dato = trama.packet[14];

   } else {
      return 0;
   }

   return dato;

}

/*
En este método hemos realizado un bucle "while" donde cada caracter que enviemos mediante el método "enviarTrama" va a ser recibido 
mediante el método "RecibirCaracter" este bucle "While" finalizara cuando pulsemos la tecla "escape" que equivale a el número 27.
*/
void Funcionamiento(interface_t iface,unsigned char mac_src[6],unsigned char mac_dst[6],unsigned char type[2]){

  unsigned char tecla;
  unsigned char resultado;

  cout<<"Pulse los caracteres a enviar: "<<endl;
  tecla = 0;

    while (tecla != 27)
    {
        resultado = RecibirCaracter(iface);

        if(resultado != 0){

            cout<<" "<<endl;
            printf("Recibido: %c \n", resultado);
        }
        
        if(kbhit()){

            tecla = getch();

            if( tecla !=27){

                EnviarTrama(iface, iface.MACaddr, mac_dst, type, tecla);
            
            }
        }
    }

}

/*
Este método va a devolver un entero que se va a corresponder con el número de grupo que hayamos elegido durante la ejecucuión
del programa. El número del grupo lo utilizaremos durante la implementación del "Modo Maestra" y "Modo Esclava".
*/
int numeroGrupo(  ){

int num_grupo;

printf("Introduzca el número del grupo: \n");
cin>>num_grupo;


return num_grupo;
}

/*
Este método va a imprimir por pantalla cada uno de los modos de estación que tenemos que implementar, estos
modos de estación van a ser "Modo Maestra" y "Modo Esclava".
*/
void imprimirEstacion(){

    cout<< "[" << 1 <<"] "<< "Modo Maestra"<<endl;
    cout<< "[" << 2 <<"] "<< "Modo Esclava"<<endl;
    
    
}

void modoMaestra(interface_t iface, int grupo, unsigned char type[2],unsigned char mac_src[6],unsigned char mac_brockcast[6]){

    cout<<"Esperando que se una la estación esclava"<<endl;

    unsigned char *trama;
    bool enc= false;
    apacket_t datos ;

    type[0] = 30 + grupo;
    type[1] = 1;
    unsigned char mac_orig[6];
    trama = BuildHeader( mac_src,mac_brockcast, type);
    SendFrame(&iface, trama, 0);

    while(!enc){

        datos = ReceiveFrame(&iface);

        if( datos.packet[12] == type[0] && datos.packet[13] == 2) {
            for(int i = 0;i<6; i++){
            mac_orig[i] = datos.packet[i];
            }
            enc=true;
        }



    }

    printf("Estacióm encontrada.");
    imprimirMACFuente(mac_orig);

}

void modoEsclavo(interface_t iface, int grupo, unsigned char type[2],unsigned char mac_src[6]){

    cout<<"Esperando que se una la estación maestra"<<endl;

    unsigned char *trama;
    bool enc= false;
    apacket_t datos ;

    type[0] = 30 + grupo;
    type[1] = 2;
    unsigned char mac_orig[6];


    while( !enc ){

        datos = ReceiveFrame(&iface);

        if( datos.packet[12] == 31 && datos.packet[13] == 1) {
            for(int i = 0;i<6; i++){
            mac_orig[i] = datos.packet[i];
            }
            trama = BuildHeader(mac_src, mac_orig, type);
            SendFrame(&iface, trama, 0);
             enc=true;

        }



    }
    
    printf("Estacióm encontrada.");
    imprimirMACFuente(mac_orig);

}



int main()
{
  interface_t iface;
  pcap_if_t *avail_ifaces=NULL;
  char *dispositivo = NULL;
  int num_grupo;
  int estacion;
  
  
  unsigned char mac_brockcast[6]={0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF};
  unsigned char type[2];
  
 

 printf("\n----------------------------\n");
 printf("------ SESION 3 - FRC ------\n");
 printf("----------------------------\n");


avail_ifaces=GetAvailAdapters(); 

printf("Interfaces disponibles:\n");
imprimirDispositivos ( avail_ifaces);

printf("Seleccione Interfaz: ") ;
dispositivo = seleccionarInterfaz( avail_ifaces);

printf ("Interfaz elegida: %s\n", dispositivo);

conseguirMAC(iface, dispositivo);//Imprime MACorigen 

//imprimirMACDestino(mac_dst);//Imprime MACDestino




/*
Comienzo de la tercera práctica:
*/


num_grupo = numeroGrupo();
OpenPort(&iface);//Abre correctamente el puerto
__fpurge(stdin);//Vacia lo que hay en el buffer de teclado
printf("Selecciona el modo de la estacion: \n");
imprimirEstacion();

cin>>estacion;


if(estacion == 1){
    modoMaestra(iface,num_grupo, type, iface.MACaddr, mac_brockcast);


}else{
   modoEsclavo(iface,num_grupo, type, iface.MACaddr);


}





//Funcionamiento(iface,iface.MACaddr,mac_dst,type);


CloseAdapter(&iface);

 return 0;
}