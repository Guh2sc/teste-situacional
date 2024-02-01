/* ---------------------------------------------------------------------------------------------------------------------------------------------
                              TESTE SITUACIONAL � PROCESSO SELETIVO
Este � um projeto de avalia��o que busca identificar a compatibilidade da codifica��o com a equipe. Busque especificar e desenhar o circuito 
utilizando boas pr�ticas de eletr�nica, visando minimizar problemas com EMC, EMI, Loops de Terra, acoplamentos, entre outros problemas comuns 
em PCB.

Meta:
     * Crie um projeto, esquem�tico e placa, usando o software de sua prefer�ncia que atenda os seguintes requisitos:
     * Utilize um microcontrolador de sua prefer�ncia
     * Utilize um motor DC de 5V que dever� ser acionado pelo microcontrolador
     * Utilize um display 16x2, que dever� se comunicar com o microcontrolador utilizando uma das seguintes op��es de comunica��o: i2c ou por 
barramento.
     * Utilize um push-button conectado a uma entrada do microcontrolador 
     * A placa ser� alimentada por uma fonte externa de 5v.
     * Implemente um trecho de c�digo em linguagem C (n�o � necess�rio compilar, 
mas � importante seguir as boas pr�ticas), para tratar a leitura do sinal do Bot�o e a partir deste sinal, acionar a sa�da que ir� ligar o motor. 
O motor deve permanecer ligado por 10 segundos ap�s cada acionamento.
     * [OPCIONAL] Dimensionar uma fonte DC que receba at� 36v e alimente a sua placa com 5v.
-------------------------------------------------------------------------------------------------------------------------------------------- */

// Inclu��o das bibliotecas utilizadas
#include "lcd.h"
#include "io430.h"

// Defini��es de valores base
# define INI_TAR	64536		//Contagem de 1000 pulsos de clock (F=1kHZ)	| 1M/1k= 1K |65536-1000= 64536

// Declara��o de vari�veis globais
  int cont = 0, segundo = 0;


/*---------------------------------------------------------------------------------------------------------------------------------------------
Nome: Aciona Motor
Descri��o: Essa fun��o aciona um moto durante 10 segundos utilizando um recurso do Microcontrolador chamado "interrup��o", que permite que o programa 
principal seja parado para a CPU realizar outra tarefa.
Entradas:       -
Sa�das:         -
---------------------------------------------------------------------------------------------------------------------------------------------*/
void aciona_motor(void){
   __bis_SR_register(GIE);   	        // Habilita interrup��o geral no microcontrolador.
   
   TA0R=INI_TAR;                        //Carrega um valor no contador para que seja possivel gerar 1 interrup��o a cada 1ms.
   while(segundo!= 10){                 // Loop que ficar� ativado at� completar os 10 segundos de acionamento do motor.
         P2OUT |= BIT4;                 // Aciona o motor.
   }
   segundo = 0;                         // Zera a variavel dos segundos.
   
   __bic_SR_register(GIE);   	        // Desabilita interrup��o geral no microcontrolador.
}
/*
                          Programa Princiapal
*/
void main( void )
{
  //Configura o hardware
  WDTCTL = WDTPW+WDTHOLD;     	        // Desativa o WDT

  // Configura��o do m�dulo de clock do sistema.
  // Configura��o DCO para frequ�ncia ~734KHz - Configura��o default.
 
  BCSCTL1 &= ~XT2OFF;                   //Ligando o oscilador XT2CLK.
  BCSCTL2 |= SELS;	                //Define XT2 como fonte de clock do SMCLK.

  // Configura��o do Timer_A
  // N�cleo do Timer
  // Define valor inicial do TAR
  TA0R=INI_TAR;                     	//Carrega o valor para o contador. 
  TACTL = TASSEL1 |          	        // Fonte do clock: SMCLK ( 4MHz ).
	      ID1 +			// Divide SMCLK/4 = 1MHz.
                    MC_1 +              // Modo de contagem continua.
                            TAIE;       // Habilitei a interrup��o no n�cleo do timer_A.
  //Configura��o dos I/Os
  P2DIR &= ~BIT5;                       //Configura o bit 5 da porta 2  como entrada da chave bush-bottom.
  P2DIR |= BIT4;                        //Configura o bit 4 da porta 2 como a sa�da que controla o motor.
  //Configura��o do display
  configura_LCM();                      // Configura o display LCD.
  programa_LCM();                       // Inicializa o programa no display LCD.
  
  //programa
  envia_string("Sillion$");             // Escrevi o nome da Sillion no display, apenas para n�o deixa-lo em branco.
  while(1){//loop infinito 
        if(P2IN & BIT5){                // Verifica se a chave bush- bottom foi acionada.
            aciona_motor();             // Chama a fun��o que aciona o motor.       
        }          
  }
}


// TA0_A1 Interrupt vector
#pragma vector = TIMER0_A1_VECTOR        // Permite acionar a interrup��o relacionada ao timer A.
__interrupt void TIMER0_A1_ISR (void){
  cont++;                                // incrementa a quantide de interrup��es que foram geradas.
  if(cont == 1000){                      // Pergunta se o n�mero de interrup��es geradas � igual a 1000, pois assim significa que se passou 1s.
    segundo ++ ;                         // Incrementa a vari�vel dos segundos.
    cont = 0;                            // Zera o contador de interrup��es.
  }
  TA0R=INI_TAR;                          //Carrega um valor no contador para que seja possivel gerar 1 interrup��o a cada 1ms.
}