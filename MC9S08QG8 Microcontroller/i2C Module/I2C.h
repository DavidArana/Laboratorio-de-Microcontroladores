/* Header para el módulo IIC del S08 */
#include <mc9s08qg8.h>	/* Libreria del microcontrolador a utilizar */
#define IICPorts	SOPT2_IICPS

/*D7 D6 D5 D4 Backlight Enable RW RS  */
void assemblyDelay(void);
	/* Genera un pequeño delay durante el procesamiento de instrucciones */

void clear_flag_IIC(void);
	/* Limpia el bit acosiado con la bandera IICIF del modulo */

void init_IIC(void);
	/* Esta funcion inicializa el módulo de IIC */

void IIC_send_byte(unsigned char data);
	/* Mandamos un dato por el bus */

void IIC_start_condition(void);
	/* Generamos un startbit para comenzar la comunicación */

void IIC_stop_condition(void);
	/* Generamos un stopbit para cerrar la comunicación */

void assemblyDelay(void){
	/* Genera un pequeño delay durante el procesamiento de instrucciones */
	__asm{
		NOP		; Desperdicia tiempo
		NOP		; Desperdicia tiempo
		NOP		; Desperdicia tiempo
		NOP		; Desperdicia tiempo
		NOP		; Desperdicia tiempo
	}
}

void init_IIC(void){
	/* Inicializamos el modulo del S08 */
	IICPorts = 0;	/* Coloca SDA y SCL en los puertos PTAD */
	IICA = 0xAA;	/* Para evitar que se coloqie como esclavo */
	IICC = 0x00;	/* Lo ponemos en cero pos si acaso */
	IICF = 0x00;	/* Ajustamos el baudrate a 100Kbps */
	IICC = 0xC0;	/* Prendemos el módulo de IIC */

}

void clear_flag_IIC(void){
	/* Apagamos la flag asociada a la interrupción */
	(void)IICS;			/* Leemos el registro */
	IICS_IICIF = 1;		/* Escribimos uno para limpiar el bit */
}

void IIC_send_byte(unsigned char data) {
	/* Manda un byte por el bus de datos */
	IICD = data; 		/* Mandamos el byte */
}

void IIC_start_condition(void) {
	/* Genera un startbit para comenzar la comunicación */
	IICC_TX = 1;
	if (IICC_MST) { 	/* Verificamos si estamos en modo maestro  */
		IICC_RSTA = 1; 	/* Volvemos a generar un addressing  */
	} else {
		IICC_MST = 1; 	/* Colocamos el modulo como maestro  */
	}
}

void IIC_stop_condition(void) {
	/* Genera un stopbit para cerrar la comunicación */
    IICC_MST = 0; 	/* Mandamos el stopbit */
    IICC_TXAK = 0; 	/* Le mandamos un ACK */
    IICC_TX = 0; 	/* Comezamos la transmision  */
}

void start_communication(unsigned char salve){
	IIC_start_condition();
	IIC_send_byte(salve);
}

interrupt 17 void IIC_INTERRUPT(void){
	clear_flag_IIC();	
}

