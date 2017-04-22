/* Header para el protocolo IIC */
/* Dado que el m�dufo de IIC del S08 no funciona como debe implementamos el
 * protocolo por software para realizar la misma funci�n 
 * */

/* Definimos cuales pines utilizaremos */
#define SCL		PTAD_PTAD2
#define	SDA		PTAD_PTAD1

void assemblyDelay(void);
	/* Crea un peque�o delay mediante instrucciones de Assembly */

void IIC_Init(void);
	/* Inicia los pines SDA y SCL para que funcionen como puertos de la comunicaci�n */

void  IIC_start_bit (void);
	/* Genera un startbit para iniciar la comunicaci�n */

void  IIC_stop_bit(void);
	/* Genera un stopbit para parar la comunicaci�n */

void IIC_send_byte(unsigned char dato);
	/* Manda un dato a trav�s del puerto SDA */


void assemblyDelay(void){
	/* Genera un pequie�o delay entre el cambio de subida
	 * y de bajada del SCL del protocolo */
	__asm{
		NOP		; Desperdicia tiempo
		NOP		; Desperdicia tiempo
		NOP		; Desperdicia tiempo
		NOP		; Desperdicia tiempo
		NOP		; Desperdicia tiempo
	}
}

void IIC_Init(void){
	/* Coloca lso pines en estado l�gico zero al inicio */
   SCL = 0;
   SDA = 0;
   /* Estos son los pines que se utilizar�n como entradas y 
    * Salidas de la comunicaci�n */
   PTBDD_PTBDD0=1;
   PTBDD_PTBDD1=1;
}

void  IIC_start_bit (void){
	/* Para generar un start bit hay que poner ambos pines de la comunicaci�n
	 * en estado alto y despues dejar caer el pin de datos a cero*/
   SCL = 1;
   SDA = 1;
   assemblyDelay();
   /* Hacemos que el pin cambie a zero y esperar un tiempo para bajar el otro pin*/ 
   SDA = 0;
   assemblyDelay();
   /* Pin del CLK a zero l�gico */
   SCL = 0;
   assemblyDelay();
}

void  IIC_stop_bit(void){
	/* El stop bit es quien indica cuando termina la tranmisi�n de datos 
	 * Esto es colocar el SCL en alto y desp�s el SDA en alto */
   SCL = 1;
   assemblyDelay();
   /* Ponemos SDA en alto */
   SDA = 1;
   assemblyDelay();
   /* Se corta la comunicacci�n y dejamos ambos pines en zero l�gico */
   SCL = 0;
   SDA = 0;
}

void IIC_send_byte(unsigned char dato) {
	unsigned char temp = dato;
	unsigned char i = 8;
	
	do {
		if (temp & 0x80) {
			SDA = 1;
		} else {
			SDA = 0;
		}
		/* Generamos un flaco de subida  */
		SCL = 1; 
		assemblyDelay();
		
		/* Generamos un flaco de bajada */
		SCL = 0;
		assemblyDelay();
		
		/* Recorremos el dato para obtener el siguiente valor */
		temp <<= 1;

	/* Le restamos al iterador cada ves que hagamos esta rutina, llegamos
	 * a zero  apagamos el pin de datos y dejamos el SCL como estaba */
	} while (--i);
	SDA = 0; 
}
