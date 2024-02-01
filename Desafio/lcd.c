#include "lcd.h"

/*------------------------------------------------------------------------------
Nome:      Configura_LCM
Descri��o:  Configura��o das portas conectadas ao LCM
Entrada:         -
Sa�da:           -
------------------------------------------------------------------------------*/
void configura_LCM(void)
{
 //Configura��o das portas
 LCM_DADO_DIR = 0XFF;   // Todos os bits da porta de dados como sa�da
 LCM_DADO_OUT  = 0X00;   // Limpa a sa�da
 LCM_CTL_DIR |= RS+EN;  // Bits da porta de controle como sa�da
 LCM_CTL_OUT &= ~(RS+EN); // Limpa pinos
}

/*------------------------------------------------------------------------------
Nome:      Envia_comando
Descri��o: Envia os comando para o display para que ele possa ser utilizado
Entrada:      char comando
Sa�da:            -
------------------------------------------------------------------------------*/

void envia_comando(char comando)
{
 // Envio dos comandos
 LCM_DADO_OUT = comando;    // Passa o comando para os pinos do LCM
 LCM_CTL_OUT &= ~(RS+EN);   // joga nivel l�gico baixo no E e RS
 __delay_cycles(TEMPO_CONFIG);
 LCM_CTL_OUT |= EN;         // Joga nivel l�gico alto no E
 __delay_cycles(TEMPO_CONFIG);
 LCM_CTL_OUT &= ~(EN);      // joga n�vel l�gico baixo no E
}

/*------------------------------------------------------------------------------
Nome:      Envia_dado
Descri��o: Envia os dados para o display para que ele possa ser utilizado
Entrada:       char dados
Sa�da:            -
------------------------------------------------------------------------------*/
void envia_dado(char dados)
{
 // Envio dos dados
 LCM_DADO_OUT = dados;      // Passa o dado para os pinos do LCM
 LCM_CTL_OUT |= RS;         // joga n�vel l�gico alto no RS
 LCM_CTL_OUT &= ~(EN);      // joga n�vel l�gico baixo no E
 __delay_cycles(TEMPO_CONFIG);
 LCM_CTL_OUT |= EN;         // Joga nivel l�gico alto no E
 __delay_cycles(TEMPO_CONFIG);
 LCM_CTL_OUT &= ~(EN);      // joga n�vel l�gico baixo no E
}

/*------------------------------------------------------------------------------
Nome:      Envia_string
Descri��o: Envia uma menssagem para o display LCM (letra-a-letra).
Entrada:     const char *apt 
Sa�da:            -
------------------------------------------------------------------------------*/
void envia_string(const char *apt)
{
 // Envio dos string
 unsigned char letra = *apt;    // Carrega o caracter na varivel letra
 while(letra != '$')            // Pergunta se a letra � igual a '$'
 {
   envia_dado(letra);           // envia a letra para o display
   apt += 1;                    // incrementa o apontador de memoria
   letra = *apt;                // carrega o novo caracter em 'letra'
 }
}
/*------------------------------------------------------------------------------
Nome:      Tamanho_string
Descri��o: Essa fun��o varre a menssagem interira at� o caracter de que inidca o final
e retorna o tamanho da menssagem.
Entrada:    const char * apt ------- o apontador de menssagem
Sa�da:        char tamanho   ------- o tamanho da menssagem
------------------------------------------------------------------------------*/
char tamanho_string(const char * apt)
{
 char letra = *apt;     //Carrega a letra inicial da menssagem
 char tamanho = 0;      //Cria e inicia a varivel que quatifica o tamanho da string.
 while (letra != '$')   //Pergunta se a variavel letra � igual a '$'
   {
     apt++;             //Incrementa o indice da letra
     letra = *apt;      //Carrega a nova letra na variavel lista.
     tamanho++;         //incrementa o tamanho da lista
   } 
 return(tamanho);       // Retorna o tamanho da lista
}
/*------------------------------------------------------------------------------
Nome:     Rotaciona_string
Descri��o:Essa fun��o atr�vez do tamanho da string consegue rotacionar a menssagem
at� retornar a menssagem inicial.
Entrada:   const char *apt ------- o apontador de menssagem
Sa�da:             -
------------------------------------------------------------------------------*/
/*
void rotaciona_string( const char *apt)
{
  char final = tamanho_string(menssagem1);  // Carrega o tamannho da string na variavel final
  const char *inicio;           // Cria a variavel inicio
  inicio = apt;                 // Carrega o apontador de menssagem navariavel inicio
  char contador,letra;          // Cria a vriavel contador e letra
  char rotacao = 0;             // Cria a variavel que ira contar quantas rota��es a palavra fez.
  while(rotacao != (final+1))   // Pergunta se a menssagem j� fez todas as rota��es necess�rias para completar a menssagem inteira.
  {
    envia_comando(0x01);        // Apaga o que est� escrito no display.
    contador =0;                // Carrega zero no contador.
    apt = inicio;               // Carrega a varivel que est� com o inidice inicial no apontador de messagem
    apt += rotacao;             // Soma o apontador de menssagem com o n�mero de roa��es
    letra = *apt;               // Carrega o novo caracter na variavel letra        
    while(contador != final)    // Pergunta se o contador � igual ao tamanho da lista para verificar se a menssagem inteira j� fi enviada.
    {
      if(letra == '$')          // Pergunta se a letra � igual ao caracter que inica o final da menssagem
      {
        apt = inicio;           // Carrega a varivel que est� com o inidice inicial no apontador de messagem
        letra = *apt;           // Carrega o novo caracter na variavel letra 
      }
      else
      {
        envia_dado(letra);     // Chama a fun��o que carrega as letras no display
        __delay_cycles(TEMPO); // Chama a fun��o de tempo para que n�o fique muito rapido.
        apt++;                 // Incrementa o apontador de menssagem
        letra = *apt;          // Carrega o novo caracter na variavel letra
        contador++;            // Incrementa o apontador de messagem
      }  
    }
    rotacao++;                 // Incrementa o n�mero de rota��es que a palavra fez
    __delay_cycles(TEMPO);     // Chama a fun��o de tempo para que n�o fique muito rapido.
  }
}*/
/*------------------------------------------------------------------------------
Nome:      Programa_LCM
Descri��o: Deixa o LCM preparado para receber informa��es 
Entrada:         -
Sa�da:           -
------------------------------------------------------------------------------*/
void programa_LCM(void)
{
 envia_comando(0x38);
 envia_comando(0x0E);
 envia_comando(0x06);
 envia_comando(0x01);
}
