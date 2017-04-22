/* Header para el protocolo IIC */
/* Dado que el módufo de IIC del S08 no funciona como debe implementamos el
 * protocolo por software para realizar la misma función 
 * */

/* Definimos cuales pines utilizaremos */
#define SCL		PTAD_PTAD2
#define	SDA		PTAD_PTAD1

void assemblyDelay(void);
	/* Crea un pequeño delay mediante instrucciones de Assembly */

void IIC_Init(void);
	/* Inicia los pines SDA y SCL para que funcionen como puertos de la comunicación */

void  IIC_start_bit (void);
	/* Genera un startbit para iniciar la comunicación */

void  IIC_stop_bit(void);
	/* Genera un stopbit para parar la comunicación */

void IIC_send_byte(unsigned char dato);
	/* Manda un dato a través del puerto SDA */


void assemblyDelay(void){
	/* Genera un pequieño delay entre el cambio de subida
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
	/* Coloca lso pines en estado lógico zero al inicio */
   SCL = 0;
   SDA = 0;
   /* Estos son los pines que se utilizarán como entradas y 
    * Salidas de la comunicación */
   PTBDD_PTBDD0=1;
   PTBDD_PTBDD1=1;
}

void  IIC_start_bit (void){
	/* Para generar un start bit hay que poner ambos pines de la comunicación
	 * en estado alto y despues dejar caer el pin de datos a cero*/
   SCL = 1;
   SDA = 1;
   assemblyDelay();
   /* Hacemos que el pin cambie a zero y esperar un tiempo para bajar el otro pin*/ 
   SDA = 0;
   assemblyDelay();
   /* Pin del CLK a zero lógico */
   SCL = 0;
   assemblyDelay();
}

void  IIC_stop_bit(void){
	/* El stop bit es quien indica cuando termina la tranmisión de datos 
	 * Esto es colocar el SCL en alto y despés el SDA en alto */
   SCL = 1;
   assemblyDelay();
   /* Ponemos SDA en alto */
   SDA = 1;
   assemblyDelay();
   /* Se corta la comunicacción y dejamos ambos pines en zero lógico */
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
