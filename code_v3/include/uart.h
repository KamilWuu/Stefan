#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
//=================================================================
#define TX_BUFFER_LEN 200
#define RX_BUFFER_LEN 200           //Dlugosc bufora odebranych danych
#define CLOCK_TYPE  0               //Rodzaj zegara
//=================================================================
typedef enum {
  USART_PORT_0 = 0,                         //Numer USART'a, ten mikrokontroler posiada jednen
}USART_PORT;
//=================================================================
typedef enum {
    Clock_16Mhz = 0,
    Clock_8Mhz = 1,
    Clock_1Mhz = 2
}CLOCK_TYPE_e;
//=================================================================
typedef enum {
  BaudRate_4800 = 0,
  BaudRate_9600 = 1,
  BaudRate_14400 = 2,
  BaudRate_19200 = 3,
  BaudRate_28800 = 4,
  BaudRate_38400 = 5,
  BaudRate_57600 = 6,
  BaudRate_115200 = 7,
  BaudRate_230400 = 8
}BAUDRATE;
//=================================================================
typedef enum {
  END_NOP_ASCII = 0,                //znak konczoncy pusty
  END_CR_ASCII = 1,                 //znak konczoncy 0x0D
  END_LF_ASCII = 2,                 //znak konczoncy 0x0A
  END_CRLF_ASCII = 3                //znak konczoncy 0x0D, 0x0A
}RX_END_ASCII;
//=================================================================
void USART_INIT(USART_PORT port, BAUDRATE baud);                    //Inicjalizacja
void USART_SEND(USART_PORT port, const char *ptr, RX_END_ASCII ende);   //wyslanie danych
void UART_SEND_MEASUREMENTS(int left, int right, int pwm_L, int pwm_R);
void UART_SEND_STRING_ENDL(char *text);
void UART_SEND_STRING(char *text);
void UART_SEND_INT(int number);

//=================================================================
//Funckje wewnetrzne
static void Stat_Send_Char(USART_PORT port, char wert);    
//=================================================================
#define CHAR_CR     0x0d   
#define CHAR_LF     0x0a   
#define ASCII_MIN   0x20        //Min spacja
#define ASCII_MAX   0x7e        //Max tylda
 
uint8_t buffer_0[RX_BUFFER_LEN];
uint8_t position_0 = 0;
volatile bool stat_ok = false;
//=================================================================
void USART_INIT(USART_PORT port, BAUDRATE baud)
{
  uint8_t presc=0;
 
  if(CLOCK_TYPE == 0)       // 16MHz
  {
    if(baud==BaudRate_4800)         {   presc=207;  }   //0.2%
    else if(baud==BaudRate_9600)    {   presc=103;  }   //0.2%
    else if(baud==BaudRate_14400)   {   presc=68;   }   //0.6%
    else if(baud==BaudRate_19200)   {   presc=51;   }   //0.2%
    else if(baud==BaudRate_28800)   {   presc=34;   }   //-0.8%
    else if(baud==BaudRate_38400)   {   presc=25;   }   //0.2%
    else if(baud==BaudRate_57600)   {   presc=16;   }   //2.1%
    else if(baud==BaudRate_115200)  {   presc=8;    }   //-3.5%
    else if(baud==BaudRate_230400)  {   presc=3;    }   //8.5%
  }
  else if(CLOCK_TYPE == 1)  //8MHz
  {
    if(baud==BaudRate_4800)         {   presc=103;  }   //0.2%
    else if(baud==BaudRate_9600)    {   presc=51;   }   //0.2%
    else if(baud==BaudRate_14400)   {   presc=34;   }   //-0.8%
    else if(baud==BaudRate_19200)   {   presc=25;   }   //0.2%
    else if(baud==BaudRate_28800)   {   presc=16;   }   //2.1%
    else if(baud==BaudRate_38400)   {   presc=12;   }   //0.2%
    else if(baud==BaudRate_57600)   {   presc=8;    }   //-7%
    else if(baud==BaudRate_115200)  {   presc=3;    }   //8.5%
    else if(baud==BaudRate_230400)  {   presc=1;    }   //0%
  }
  else if(CLOCK_TYPE == 2) //1MHz
  {
    if(baud==BaudRate_4800)         {   presc=12;   }   //0.2%
    else if(baud==BaudRate_9600)    {   presc=6;    }   //-7%
    else if(baud==BaudRate_14400)   {   presc=3;    }   //8.5%
    else if(baud==BaudRate_19200)   {   presc=2;    }   //8.5%
  }
 
  if(port==USART_PORT_0)
  {
    //Ustawienie baudrate
    UBRR0H=0x00;
    UBRR0L=presc;
    //---------------------------
    UCSR0A=0x00;
    //---------------------------
    UCSR0B=0x00;
    UCSR0B|=0x80 | 0x10 | 0x08;     //Rx przerwania on, rx enable, tx enalbe
    //---------------------------
    UCSR0C=0x00;                    // tryb asynchroniczny, bez przystosci, 1 bit stopu
    UCSR0C|=0x04 | 0x02;            // 8 date bits
    //---------------------------
  }
  sei();
}
//=================================================================
void USART_SEND(USART_PORT port_number, const char *buf, RX_END_ASCII end_sign)
{
  uint8_t n;
  uint8_t c;
 
  for(n=0;n<=TX_BUFFER_LEN;n++)
  {
    c=buf[n];
   
    if(c==0)                                //Jesli natrafi na koniec bufora
    {
      if(end_sign==END_CR_ASCII)
      {
        Stat_Send_Char(port_number, CHAR_CR);
      }
      else if(end_sign==END_LF_ASCII)
      {
        Stat_Send_Char(port_number, CHAR_LF);  
      }
      else if(end_sign==END_CRLF_ASCII)
      {
        Stat_Send_Char(port_number, CHAR_CR);    
        Stat_Send_Char(port_number, CHAR_LF);      
      }
      break;
    }
    else
    {
      Stat_Send_Char(port_number, c);
    }  
  }
}
//=================================================================
void Stat_Send_Char(USART_PORT port, char data)
{
    if(port==USART_PORT_0)
    {
        while(!(UCSR0A & 0x20));
        UDR0=data;
    }
}
//=================================================================
ISR(USART_RX_vect)
{    
  uint8_t recive_char;
 
  recive_char=UDR0;
 
  if(recive_char==CHAR_CR) { stat_ok=true; }
  else
  {
    if(stat_ok==false)
     {
      if(position_0<RX_BUFFER_LEN)
      {
        if((recive_char>=ASCII_MIN) && (recive_char<=ASCII_MAX))
        {
          buffer_0[position_0]=recive_char;
          buffer_0[position_0+1]=0x00;
          position_0++;
        }
      }
    }
  }  
  Stat_Send_Char(USART_PORT_0, recive_char);
}

void UART_SEND_MEASUREMENTS(int left, int right, int pwm_L, int pwm_R){
  char buffL[60]; 
  char sign[30] = " <===L P===> ";
  char sign2[30] = " \t\t\t ";
  char buffR[60]; 
  char buffPWML[10];
  char buffPWMR[10];
  char endL[5] = "\n";

  sprintf(buffL,"%d",left);
  sprintf(buffR,"%d",right);
  sprintf(buffPWML,"%d",pwm_L);
  sprintf(buffPWMR,"%d",pwm_R);
  sprintf(buffR,"%d",right);


  USART_SEND(USART_PORT_0,buffL,END_CR_ASCII);
  USART_SEND(USART_PORT_0,sign,END_CR_ASCII);
  USART_SEND(USART_PORT_0,buffR,END_CR_ASCII);

  USART_SEND(USART_PORT_0,sign2,END_CR_ASCII);

  USART_SEND(USART_PORT_0,buffPWML,END_CR_ASCII);
  USART_SEND(USART_PORT_0,sign,END_CR_ASCII);
  USART_SEND(USART_PORT_0,buffPWMR,END_CR_ASCII);


  USART_SEND(USART_PORT_0,endL,END_CR_ASCII);
    
}

void UART_SEND_STRING_ENDL(char *text){
  char endL[5] = "\n";
  USART_SEND(USART_PORT_0,text,END_CR_ASCII);
  USART_SEND(USART_PORT_0,endL,END_CR_ASCII);
}

void UART_SEND_STRING(char *text){
  
  USART_SEND(USART_PORT_0,text,END_CR_ASCII);
  
}

void UART_SEND_INT(int number){
  char number_buff[100];
  
  sprintf(number_buff,"%d",number);
  USART_SEND(USART_PORT_0,number_buff,END_CR_ASCII);
  
}

