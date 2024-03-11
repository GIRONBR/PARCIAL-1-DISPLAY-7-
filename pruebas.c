
	#include <stdint.h>

	#define RCC_C 0X40021000U //DECLARA A LOS REGISTROS DEL RELOJ

	#define PORTA 0x50000000U //DECLARA EL PUERTO A

	#define PORTB 0X50000400U //DECLARA EL PUERTO B

	#define PORTC 0X50000800U //DECLARAR EL PUERTO C

	//CREAR ESTRUCTURA DEL GPIO

	typedef struct
	{
			uint32_t MODER;
			uint32_t OTYPER;
			uint32_t OSPEEDR;
			uint32_t PUPDR;
			uint32_t IDR;
			uint32_t ODR;
			uint32_t BSRR;
			uint32_t LCKR;
			uint32_t AFR[2];
			uint32_t BRR;

	}GPIO_RegDef_t;

	//CREAR ESTRUCTURA DEL RCC

	typedef struct{
			uint32_t CR;
			uint32_t ICSCR;
			uint32_t CRRCR;
			uint32_t CFGR;
			uint32_t CIER;
			uint32_t CIFR;
			uint32_t CICR;
			uint32_t IOPRSTR;
			uint32_t AHBRSTR;
			uint32_t APB2RSTR;
			uint32_t APB1RSTR;
			uint32_t IOPENR;
	}RCC_RegDef_t;

	// ESTRUCTURA PARA LLEVAR CONTROL DE VARIBALES DE TIEMPO
	struct Time_t {

		int8_t hora_decimal;
		int8_t hora_unidad ;

		int8_t minuto_decimal ;
		int8_t minuto_unidad  ;

		int8_t segundo_decimal ;
		int8_t segundo_unidad ;


	};
	struct alarm {

		uint8_t hora_decimal;
		uint8_t hora_unidad ;

		uint8_t minuto_decimal ;
		uint8_t minuto_unidad  ;

		uint8_t segundo_decimal ;
		uint8_t segundo_unidad ;


	};
	struct temporizador{
			uint8_t hora_decimal ;
			uint8_t hora_unidad ;

			uint8_t minuto_decimal ;
			uint8_t minuto_unidad ;

			uint8_t segundo_decimal ;
			uint8_t segundo_unidad ;
	};

	#define GPIOA ((GPIO_RegDef_t*)PORTA)
	#define GPIOB ((GPIO_RegDef_t*)PORTB)
	#define GPIOC ((GPIO_RegDef_t*)PORTC)
	#define RCC ((RCC_RegDef_t*)RCC_C)

	uint8_t myfsm = 0x00; //CREANDO VARIABLE QUE CONTROLA LA SECUENCIA DE ENCEDIDO DE LOS DISPLAY
	int cont =0; //CONTROL DE FORMATO 24 HORAS 12 HORAS Y CRONOMETRO
	uint32_t inc_segundos;
	void delay_ms(uint16_t n); //DEFINDIENDO EL METODO DE RETARDO

	uint8_t decoder(uint8_t value_to_decode); //DEFINIDENDO LA FUNCION SWITCH

	//DIGITOS EN CATODO COMUN

	#define digit0 0x3F
	#define digit1 0X06
	#define digit2 0X5B
	#define digit3 0X4F
	#define digit4 0X66
	#define digit5 0X6D
	#define digit6 0X7D
	#define digit7 0X07
	#define digit8 0X7F
	#define digit9 0X6F





	int main(void)
	{

		struct alarm alarma;

		alarma.minuto_unidad = 0x05;
		alarma.minuto_decimal = 0x05;
		alarma.hora_unidad = 0x02;
		alarma.hora_decimal = 0x01;

		struct temporizador temp;

		temp.segundo_unidad = 0x00;
		temp.segundo_decimal = 0x00;
		temp.minuto_unidad = 0x0;
		temp.minuto_decimal = 0x01;
		temp.hora_unidad = 0x00;
		temp.hora_decimal = 0x00;

		struct Time_t watch;

		watch.segundo_unidad = temp.segundo_unidad;
		watch.segundo_decimal = temp.segundo_decimal;
		watch.minuto_unidad = temp.minuto_unidad;
		watch.minuto_decimal = temp.minuto_decimal;
		watch.hora_unidad = temp.hora_unidad;
		watch.hora_decimal = temp.hora_decimal;

		int bandera =0;
		int i;



		//CONFIGURACION DE LOS PINES DEL PUERTO A

		RCC ->IOPENR |= 1 << 0; //HABILITAR PUERTO A RELOJ

		//CONFIGURACION DE BOTONES (INPUT) PUERTO A A0,A1

		GPIOA ->MODER &= ~(1 << 0); //PIN A0 CONTROL SEGUNDOS
		GPIOA ->MODER &= ~(1 << 1);
		GPIOA ->PUPDR &= ~(1 << 0); //RESISTENCIA DE PULLDOWN
		GPIOA ->PUPDR |= 1 << 1;

		GPIOA ->MODER &= ~(1 << 8); //PIN A4
		GPIOA ->MODER &= ~(1 << 9);
		GPIOA ->PUPDR &= ~(1 << 8); //RESISTENCIA DE PULLDOWN
		GPIOA ->PUPDR |= 1 << 9;

		//CONFIGURACION DE INDICADOR LUMINICO LEDS A8,A6,A10

		GPIOA ->MODER |= 1<<16;  //PIN A8
		GPIOA ->MODER &= ~(1 << 17);

		GPIOA ->MODER |= 1<<12;  //PIN A6
		GPIOA ->MODER &= ~(1 << 13);

		GPIOA ->MODER |= 1<<20;  //PIN A10
		GPIOA ->MODER &= ~(1 << 21);

		//CONFIGURACION DE PINES DEL PUERTO B
		RCC -> IOPENR |= 1 << 1; //HABILITAR  PUERTO B RELOJ

		// PINES CONFIGURADOS COMO SALIDA DEL PUERTO B, DISPLAY DE 7 SEGMENTOS MULTIPLEXADO

		GPIOB ->MODER |= 1<<0;  //PIN B0
		GPIOB ->MODER &= ~(1 << 1);

		GPIOB ->MODER |= 1<< 2;	//PIN B1
		GPIOB ->MODER &= ~(1 << 3);

		GPIOB ->MODER |= 1 << 4;	//PIN B2
		GPIOB ->MODER &= ~(1 << 5);

		GPIOB ->MODER |= 1 << 6;	//PIN B3
		GPIOB ->MODER &= ~(1 << 7);

		GPIOB ->MODER |= 1 << 8;	//PIN B4
		GPIOB ->MODER &= ~(1 << 9);

		GPIOB ->MODER |= 1 << 10;	//PIN B5
		GPIOB ->MODER &= ~(1 << 11);

		GPIOB ->MODER |= 1 << 12;	//PIN B6
		GPIOB ->MODER &= ~(1 << 13);

		GPIOB ->MODER |= 1 << 14;	//PIN B7
		GPIOB ->MODER &= ~(1 << 15);

	   //CONFIGURACIONES PARA EL PUERTO C, CONTROL DE DISPLAY MULTIPLEXADO

		RCC ->IOPENR |= 1 << 2; //HABILITAR PUERTO C RELOJ

		//PINES CONFIGURADOS COMO SALIDA DEL PUERTO C

		// PINES DE CONTROL DEL DISPLAY MULTIPLEXADO

		GPIOC ->MODER |= 1 << 10;  //PIN C5
		GPIOC ->MODER &= ~(1 << 11);

		GPIOC ->MODER |= 1 << 12;  //PIN C6
		GPIOC ->MODER &= ~(1 << 13);

		GPIOC ->MODER |= 1 << 16;  //PIN C8
		GPIOC ->MODER &= ~(1 << 17);

		GPIOC ->MODER |= 1 << 18;  //PIN C9
		GPIOC ->MODER &= ~(1 << 19);

		GPIOC ->MODER |= 1 <<20;//PIN C10
		GPIOC ->MODER &= ~(1 << 21);

		GPIOC ->MODER |= 1 <<22;//PIN C11
		GPIOC ->MODER &= ~(1 << 23);

		//PIN ALARMA C0
		GPIOC ->MODER |= 1 <<0;
		GPIOC ->MODER &= ~(1 << 1);

		while(1)
		{

			switch(myfsm)
			{
			case 0:
			{
				//PARA EL DISPLAY 1 UNIDAD SEGUNDOS
				myfsm++;
				GPIOB ->BSRR |= 0XFF << 16; //LIMPIAR DISPLAY
				GPIOC ->BSRR |= 0X01 << 27; // LIMPIAR D5/C11
				GPIOC ->BSRR |= 0X01 << 5; // HABILITA D0/C5
				GPIOB ->BSRR |= decoder(watch.segundo_unidad) << 0; // MANDA A PINTAR DISPLAY 1

				break;

			}
			case 1:{
				//PARA EL DISPLAY 2 DECENA SEGUNDOS
				myfsm++;
				GPIOB ->BSRR |= 0XFF << 16; //LIMPIAR DISPLAY
				GPIOC ->BSRR |= 0X01 << 21; // LIMPIAR D0/C5
				GPIOC ->BSRR |= 0X01 << 6 ; // HABILITA D1/C6
				GPIOB ->BSRR |= decoder(watch.segundo_decimal) << 0; //MANDA A PINTAR DISPLAY 2
				break;

			}
			case 2:{
				//PARA EL DISPLAY 3 UNIDAD MINUTOS
				myfsm++;
				GPIOB ->BSRR |= 0XFF << 16; //LIMPIAR DISPLAY
				GPIOC ->BSRR |= 0X01 << 22; // LIMPIAR D1/C6
				GPIOC ->BSRR |= 0X01 << 8; // HABILITA D2/C8
				GPIOB ->BSRR |= decoder(watch.minuto_unidad) << 0; //MANDA A PINTAR DISPLAY 3

				break;
			}
			case 3:{
				//PARA EL DISPLAY 4 DECENA MINUTOS
				myfsm ++;
				GPIOB ->BSRR |= 0XFF << 16; //LIMPIAR DISPLAY
				GPIOC ->BSRR |= 0X01 << 24; // LIMPIAR D2/C8
				GPIOC ->BSRR |= 0X01 << 9; // HABILITA D3/C9
				GPIOB ->BSRR |= decoder(watch.minuto_decimal) << 0; //MANDA A PINTAR DISPLAY 4
				break;

			}
			case 4:{
				//PARA EL DISPLAY 5 UNIDAD HORAS
				myfsm ++;
				GPIOB ->BSRR |= 0XFF << 16; //LIMPIAR DISPLAY
				GPIOC ->BSRR |= 0X01 << 25; // LIMPIAR D3/C9
				GPIOC ->BSRR |= 0X01 << 10; // HABILITA D4/C10
				GPIOB ->BSRR |= decoder(watch.hora_unidad) << 0; //MANDA A PINTAR DISPLAY 4
				break;

					}

			case 5:{
				//PARA EL DISPLAY 6 DECENA HORAS
				myfsm = 0;
				GPIOB ->BSRR |= 0XFF << 16; //LIMPIAR DISPLAY
				GPIOC ->BSRR |= 0X01 << 26 ; // LIMPIAR D4/C10
				GPIOC ->BSRR |= 0X01 << 11; // HABILITA D5/C11
				GPIOB ->BSRR |= decoder(watch.hora_decimal) << 0; //MANDA A PINTAR DISPLAY 4

				break;

							}

			default:
			{
				//CASO NO TOMA EN CUENTA NINGUNO myfsm POR SEGURIDAD
				myfsm =0 ;
				break;
			}

			}
			//TERMINA MAQUINA DE ESTADOS FINITOS
			if(bandera == 0){
				delay_ms(1);
				inc_segundos++;
			if (inc_segundos == 1000) {
				inc_segundos = 0;
				watch.segundo_unidad--;
				}
			if (watch.segundo_unidad < 0){
				watch.segundo_unidad = 9;
				watch.segundo_decimal--;
				}
			if (watch.segundo_decimal < 0){
				watch.segundo_decimal = 5;
				watch.minuto_unidad--;
				}
			if (watch.minuto_unidad < 0){
				watch.minuto_unidad = 9;
				watch.minuto_decimal--;
				}
			if (watch.minuto_decimal < 0){
				watch.minuto_decimal = 5;
				watch.hora_unidad--;
				}
			if (watch.hora_unidad  < 0){
				watch.hora_unidad = 9;
				watch.hora_decimal--;
				}
			if (watch.hora_decimal < 0){
				watch.hora_decimal = 0;
				}
			}


			if(bandera ==1 || bandera ==2){
				//INCREMENTO SEGUNDOS
				delay_ms(1);
				inc_segundos++;

				//PARA SEGUNDOS UNIDAD
				if (inc_segundos == 1000 || GPIOA->IDR  & (1<<0)) {
			    inc_segundos = 0;
			    watch.segundo_unidad++;
				}
				if (watch.segundo_unidad == 10){
				watch.segundo_unidad = 0;
				watch.segundo_decimal++;
				}
				if (watch.segundo_decimal ==6){
				watch.segundo_decimal = 0;
				watch.minuto_unidad++;
				}
				if (watch.minuto_unidad==10){
				watch.minuto_unidad =0;
				watch.minuto_decimal++;
				}
				if (watch.minuto_decimal==6){
				watch.minuto_decimal=0;
				watch.hora_unidad++;
				}
				if (watch.hora_unidad==10){
				watch.hora_unidad =0;
				watch.hora_decimal++;
				}
				if (bandera ==1){
					if (watch.hora_decimal==1){ //FORMATO 12 HORAS
						if(watch.hora_unidad==3){
							watch.hora_decimal=0;
							watch.hora_unidad=1;
						}
					}
				}
				if(bandera ==2){
					if(watch.hora_decimal==2){ //FORMATO 24 HORAS
						if(watch.hora_unidad==4){
						delay_ms(100);
						watch.hora_decimal=0;
						watch.hora_unidad=0;
						}
					}
				}

				if(watch.hora_decimal==alarma.hora_decimal && watch.hora_unidad==alarma.hora_unidad &&
				watch.minuto_decimal==alarma.minuto_decimal && watch.minuto_unidad ==alarma.minuto_unidad &&(bandera == 1 ||bandera == 2))
				{
				watch.minuto_unidad++;
				for(i=0; i<10;i++){
				delay_ms(500);
				GPIOC ->ODR ^= 1 << 0;

				}


				}
			}
			switch(cont)
				{
				case 0:{
					bandera =0;
					GPIOA->ODR |= 1 << 8;
					GPIOA->ODR  &= ~(1 << 6);
					GPIOA->ODR  &= ~(1 << 10);
					if(GPIOA ->IDR  & (1<<4)){
						watch.segundo_unidad = 0x00;
						watch.segundo_decimal = 0x00;
						watch.minuto_unidad = 0x00;
						watch.minuto_decimal = 0x00;
						watch.hora_unidad = 0x02;
						watch.hora_decimal = 0x01;
						delay_ms(500);
						cont++;
					}
					break;
				}
				case 1:{
					bandera = 1; //FORMATO 12 HORAS
					GPIOA->ODR |= 1 << 6;
					GPIOA->ODR  &= ~(1 << 8);
					GPIOA->ODR  &= ~(1 << 10);
					if(GPIOA ->IDR  & (1<<4)){
						watch.segundo_unidad = 0x00;
						watch.segundo_decimal = 0x00;
						watch.minuto_unidad = 0x00;
						watch.minuto_decimal = 0x00;
						watch.hora_unidad = 0x04;
						watch.hora_decimal = 0x02;

						delay_ms(500);
						cont++;
					}
					break;
				}
				case 2:{
					bandera =2; //FORMATO 24 HORAS
					GPIOA->ODR |= 1 << 10;
					GPIOA->ODR  &= ~(1 << 6);
					GPIOA->ODR  &= ~(1 << 8);
					if(GPIOA ->IDR  & (1<<4)){
						watch.segundo_unidad = temp.segundo_unidad;
						watch.segundo_decimal = temp.segundo_decimal;
						watch.minuto_unidad = temp.minuto_unidad;
						watch.minuto_decimal = temp.minuto_decimal;
						watch.hora_unidad = temp.hora_unidad;
						watch.hora_decimal = temp.hora_decimal;

						delay_ms(500);
						cont =0;
					}

					break;
				}



//TERMINA CONDICION DEL BOTON
				}




//TERMINA EL WHILE
}



}



	void delay_ms(uint16_t n){

		uint16_t i;
		for(; n>0; n--)
			for(i=0; i<140; i++);
	}
	uint8_t decoder(uint8_t value_to_decode)
	{
		switch(value_to_decode)
		{
		case 0:
		{
			return(digit0);

		}
		case 1:{

			return(digit1);
		}
		case 2:{

			return(digit2);
		}
		case 3:{

			return(digit3);
		}
		case 4:{

			return(digit4);
		}
		case 5:{

			return(digit5);
		}

		case 6:{

			return(digit6);
		}

		case 7:{

			return(digit7);
		}

		case 8:{

			return(digit8);
		}
		case 9:{

			return(digit9);
		}


		default:
		{

		}
		return 0x00;


		}
	}




