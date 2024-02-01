/* ---------------------------------------------------------------------------------------------------------------------------------------------
                              TESTE SITUACIONAL – PROCESSO SELETIVO
Este é um projeto de avaliação que busca identificar a compatibilidade da codificação com a equipe. Busque especificar e desenhar o circuito 
utilizando boas práticas de eletrônica, visando minimizar problemas com EMC, EMI, Loops de Terra, acoplamentos, entre outros problemas comuns 
em PCB.

Meta:
     * Crie um projeto, esquemático e placa, usando o software de sua preferência que atenda os seguintes requisitos:
     * Utilize um microcontrolador de sua preferência
     * Utilize um motor DC de 5V que deverá ser acionado pelo microcontrolador
     * Utilize um display 16x2, que deverá se comunicar com o microcontrolador utilizando uma das seguintes opções de comunicação: i2c ou por 
barramento.
     * Utilize um push-button conectado a uma entrada do microcontrolador 
     * A placa será alimentada por uma fonte externa de 5v.
     * Implemente um trecho de código em linguagem C (não é necessário compilar, 
mas é importante seguir as boas práticas), para tratar a leitura do sinal do Botão e a partir deste sinal, acionar a saída que irá ligar o motor. 
O motor deve permanecer ligado por 10 segundos após cada acionamento.
     * [OPCIONAL] Dimensionar uma fonte DC que receba até 36v e alimente a sua placa com 5v.
-------------------------------------------------------------------------------------------------------------------------------------------- */

// Inclução das bibliotecas utilizadas
#include "lcd.h"
#include "io430.h"

// Definições de valores base
# define INI_TAR	64536		//Contagem de 1000 pulsos de clock (F=1kHZ)	| 1M/1k= 1K |65536-1000= 64536

// Declaração de variáveis globais
  int cont = 0, segundo = 0;


/*---------------------------------------------------------------------------------------------------------------------------------------------
Nome: Aciona Motor
Descrição: Essa função aciona um moto durante 10 segundos utilizando um recurso do Microcontrolador chamado "interrupção", que permite que o programa 
principal seja parado para a CPU realizar outra tarefa.
Entradas:       -
Saídas:         -
---------------------------------------------------------------------------------------------------------------------------------------------*/
void aciona_motor(void){
   __bis_SR_register(GIE);   	        // Habilita interrupção geral no microcontrolador.
   
   TA0R=INI_TAR;                        //Carrega um valor no contador para que seja possivel gerar 1 interrupção a cada 1ms.
   while(segundo!= 10){                 // Loop que ficará ativado até completar os 10 segundos de acionamento do motor.
         P2OUT |= BIT4;                 // Aciona o motor.
   }
   segundo = 0;                         // Zera a variavel dos segundos.
   
   __bic_SR_register(GIE);   	        // Desabilita interrupção geral no microcontrolador.
}
/*
                          Programa Princiapal
*/
void main( void )
{
  //Configura o hardware
  WDTCTL = WDTPW+WDTHOLD;     	        // Desativa o WDT

  // Configuração do módulo de clock do sistema.
  // Configuração DCO para frequência ~734KHz - Configuração default.
 
  BCSCTL1 &= ~XT2OFF;                   //Ligando o oscilador XT2CLK.
  BCSCTL2 |= SELS;	                //Define XT2 como fonte de clock do SMCLK.

  // Configuração do Timer_A
  // Núcleo do Timer
  // Define valor inicial do TAR
  TA0R=INI_TAR;                     	//Carrega o valor para o contador. 
  TACTL = TASSEL1 |          	        // Fonte do clock: SMCLK ( 4MHz ).
	      ID1 +			// Divide SMCLK/4 = 1MHz.
                    MC_1 +              // Modo de contagem continua.
                            TAIE;       // Habilitei a interrupção no núcleo do timer_A.
  //Configuração dos I/Os
  P2DIR &= ~BIT5;                       //Configura o bit 5 da porta 2  como entrada da chave bush-bottom.
  P2DIR |= BIT4;                        //Configura o bit 4 da porta 2 como a saída que controla o motor.
  //Configuração do display
  configura_LCM();                      // Configura o display LCD.
  programa_LCM();                       // Inicializa o programa no display LCD.
  
  //programa
  envia_string("Sillion$");             // Escrevi o nome da Sillion no display, apenas para não deixa-lo em branco.
  while(1){//loop infinito 
        if(P2IN & BIT5){                // Verifica se a chave bush- bottom foi acionada.
            aciona_motor();             // Chama a função que aciona o motor.       
        }          
  }
}


// TA0_A1 Interrupt vector
#pragma vector = TIMER0_A1_VECTOR        // Permite acionar a interrupção relacionada ao timer A.
__interrupt void TIMER0_A1_ISR (void){
  cont++;                                // incrementa a quantide de interrupções que foram geradas.
  if(cont == 1000){                      // Pergunta se o número de interrupções geradas é igual a 1000, pois assim significa que se passou 1s.
    segundo ++ ;                         // Incrementa a variável dos segundos.
    cont = 0;                            // Zera o contador de interrupções.
  }
  TA0R=INI_TAR;                          //Carrega um valor no contador para que seja possivel gerar 1 interrupção a cada 1ms.
}