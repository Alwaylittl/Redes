//============================================================================
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
Este método va a imprimir la MAC
*/
void imprimirMACR (unsigned char MACaddr[6]){

int j = 0;

cout<<"La MAC es: ";
for(j = 0; j<5; j++){
    printf("%.2X:", MACaddr[j]);
   
}
     printf("%.2X", MACaddr[5]);
     cout<<""<<endl;

}


void conseguirMAC (interface_t &iface, char *nombre){

    setDeviceName(&iface, nombre);//Devuelve la interfaz que se identifica con el nombre del dispositivo que hemos pasado por parámetro
    GetMACAdapter(&iface);//Obtenemos la MAC de la interfaz
         
}

int OpenPort(interface_t &iface){
    int Puerto=OpenAdapter(&iface);
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

void EnviarTrama(interface_t iface,unsigned char mac_src[6],unsigned char mac_dst[6],unsigned char type[2], unsigned char *car){
    unsigned char *frame;
    __fpurge(stdin);
    
    frame = BuildFrame(mac_src,mac_dst,type,car); //Construimos la trama
    SendFrame(&iface,frame,1);
    delete car;
    free(frame);
}

unsigned char RecibirCaracter(interface_t iface){
  
   apacket_t trama ;
    unsigned char dato ;
    trama = ReceiveFrame (&iface);

    if(trama.packet != NULL){

    dato = trama.packet[14];


    }else{

        return 0;
    }

    return dato;

}



int main()
{
  interface_t iface;
  pcap_if_t *avail_ifaces=NULL;
  char *dispositivo = NULL;

  
  
  unsigned char mac_src[6]={0x00, 0x00, 0x00, 0x00,0x00, 0x00};
  unsigned char mac_dst[6]={0x00, 0x01, 0x02, 0x03,0x04, 0x05};
  unsigned char type[2]={0x30,0x00};
   
 

 printf("\n----------------------------\n");
 printf("------ SESION 1 - FRC ------\n");
 printf("----------------------------\n");


//Listado de Interfaces
avail_ifaces=GetAvailAdapters(); 
printf("Interfaces disponibles:\n");
imprimirDispositivos ( avail_ifaces);

//Seleccion de Interfaz
printf("Seleccione Interfaz: ") ;
dispositivo = seleccionarInterfaz( avail_ifaces);
printf ("Interfaz elegida: %s\n", dispositivo);

//Obtenemos la direccion MAC del dispositivo seleccionado
conseguirMAC(iface, dispositivo);

//Abrimos el puerto para poder enviar la trama
OpenPort(iface);

//Seleccionamos caracter a enviar
unsigned char *car = new unsigned char();
    cout<<"Seleccione un caracter: ";
    cin>>car;

//Enviamos la trama con [MAC_src y MAC_dst] como parametros ademas del caracter a enviar
EnviarTrama(iface,mac_src,mac_dst,type,car);

//Recibimos el caracter que nos han enviado
char recibido = (char)RecibirCaracter(iface);
printf("%c es el caracter recibido\n",recibido);

//Imprimimos la MAC de la interfaz que pasamos por la parámetro
imprimirMACR(iface.MACaddr);

 return 0;
}
