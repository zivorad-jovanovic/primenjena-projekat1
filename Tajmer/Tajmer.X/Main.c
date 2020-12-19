#include <p30fxxxx.h>
#include "tajmeri.h"
#include "GLCD.h"

_FOSC(CSW_FSCM_OFF & XT_PLL4); //& XT_PLL4
_FWDT(WDT_OFF);


//---------------------------------
unsigned int brojac_ms,stoperica,ms,sekund;
unsigned char tempRX;
char open = 'o';
unsigned char close = 'c';
int pom1 = 1;
int pom2 = 1;
unsigned int stanje1=1,stanje2=1,stanje3=1,stanje4=1;
int brojac = 0;
unsigned int stanje5 = 1, stanje6 = 1, stanje7 = 1;
unsigned int flag11 = 0;
unsigned int array[10];
unsigned int i;
int counter = 0,a=1;


/*******************************************************************
//funkcija za kasnjenje u milisekundama
*********************************************************************/
void initUART1(void)    //inicijalizacija RS232 komunikacije sa 9600br
{
    U1BRG=0x0015;//baud rate 9600 
    U1MODEbits.ALTIO = 0;   //portovi RC13 i RC14 se ne koriste zbog TS
    IEC0bits.U1RXIE = 1;
    U1STA&=0xfffc;
    U1MODEbits.UARTEN=1;
    U1STAbits.UTXEN=1;
    
   // IPC2bits.U1TXIP=2;
    //IPC2bits.U1RXIP=7;    
}


void __attribute__((__interrupt__)) _U1RXInterrupt(void) //interrupt za UART
{
    IFS0bits.U1RXIF = 0;

    tempRX = U1RXREG;    
            
}

void WriteUART1(unsigned int data)  //ispis jednog karaktera za UART
{
	while (U1STAbits.TRMT==0);
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void WriteUART1dec2string(unsigned int data) //saljemo 4-cifrene brojeve za UART
{
	unsigned char temp;

	temp=data/1000;
	WriteUART1(temp+'0');
	data=data-temp*1000;
	temp=data/100;
	WriteUART1(temp+'0');
	data=data-temp*100;
	temp=data/10;
	WriteUART1(temp+'0');
	data=data-temp*10;
	WriteUART1(data+'0');
}
void RS232_putst(register const char *str){
    while((*str)!=0){
        WriteUART1(*str);
        if (*str==13) WriteUART1(10);
        if (*str==10) WriteUART1(13);
        str++;
    }
}
void Write_GLCD(unsigned int data)  //ispiuje 4-cifrene karakter na GLCD
{
    unsigned char temp;

    temp=data/1000;
    Glcd_PutChar(temp+'0');
    data=data-temp*1000;
    temp=data/100;
    Glcd_PutChar(temp+'0');
    data=data-temp*100;
    temp=data/10;
    Glcd_PutChar(temp+'0');
    data=data-temp*10;
    Glcd_PutChar(data+'0');
}


void Delay_ms (int vreme)//funkcija za kasnjenje u milisekundama
	{
		stoperica = 0;
		while(stoperica < vreme);
	}



void __attribute__ ((__interrupt__)) _T2Interrupt(void) // svakih 1ms
{

		TMR2 =0;
        
	brojac_ms++;//brojac milisekundi
    stoperica++;//brojac za funkciju Delay_ms

   
	IFS0bits.T2IF = 0; 
       
}
void tastatura()
{
    //GoToXY(75,6);
		GLCD_Rectangle(2,1,24,20);
        
	    GoToXY(11,1);
        Glcd_PutChar ('1');
        //GoToXY(9,6);
        //Write_GLCD(X);
        GLCD_Rectangle(26,1,48,20);
        GoToXY(34,1);
        Glcd_PutChar ('2');
        
        GLCD_Rectangle(50,1,72,20);
        GoToXY(57,1);
        Glcd_PutChar ('3');
        
        GLCD_Rectangle(74,1,95,20);
        GoToXY(80,1);
        Glcd_PutChar ('4');
        
        GLCD_Rectangle(100,1,122,20);
        GoToXY(108,1);
        Glcd_PutChar ('E');
        
        GLCD_Rectangle(2,22,24,41);
        GoToXY(11,3);
        Glcd_PutChar ('5');
        
        GLCD_Rectangle(26,22,48,41);
        GoToXY(34,3);
        Glcd_PutChar ('6');
        
        GLCD_Rectangle(50,22,72,41);
        GoToXY(57,3);
        Glcd_PutChar ('7');
        
        GLCD_Rectangle(74,22,95,41);
        GoToXY(80,3);
        Glcd_PutChar ('8');
        
         GLCD_Rectangle(100,22,122,41);
        GoToXY(108,3);
        Glcd_PutChar ('R');
         
        GLCD_Rectangle(2,43,24,62);
        GoToXY(11,6);
        Glcd_PutChar ('9');
        
        GLCD_Rectangle(26,43,48,62);
        GoToXY(34,6);
        Glcd_PutChar ('0');
        
        
        GoToXY(70,7);
        GLCD_Printf ("__");
        
        GoToXY(85,7);
        GLCD_Printf ("__");
        
        GoToXY(100,7);
        GLCD_Printf ("__");
        
        GoToXY(115,7);
        GLCD_Printf ("__");
        
        	
}
	


void main (void)
{
    ADPCFGbits.PCFG4=1;//defini?emo da pin B4 nije analogni vec digitalni
	ADPCFGbits.PCFG5=1;//defini?emo da pin B5 nije analogni vec digitalni

	TRISBbits.TRISB4=0;//konfigurisemo kao izlaz
	TRISBbits.TRISB6=0;
    
    TRISDbits.TRISD0=1;//konfigurisemo kao ulaz
    TRISDbits.TRISD1=1;//konfigurisemo kao ulaz
    TRISDbits.TRISD2=1;
    TRISDbits.TRISD8=1;//konfigurisemo kao ulaz
    TRISDbits.TRISD9=1;
    TRISFbits.TRISF6=1;//konfigurisemo kao ulaz(ENTER))
    
   // TRISDbits.TRISF6 = 0;
    initUART1();
    ConfigureLCDPins();
    GLCD_LcdInit();
	GLCD_ClrScr();
    
    
Init_T2();
//	Init_T1();
//	Init();

 //RS232_putst("Otvorite vrata\n");
 int kon1 = 0,kon2=0;
 //tastatura();
	while(1)
	{
        
       

    if(PORTDbits.RD8 && (stanje5 == 1))
    {
        stanje5 = 0;
        array[brojac++] = 8; 
        GoToXY(0, brojac);
        Write_GLCD(brojac);
    }
    
    if(!PORTDbits.RD8)
        stanje5 = 1;
     
   if(PORTDbits.RD9 && (stanje6 == 1))
   {
      array[brojac++] = 9; 
      GoToXY(0, brojac);
      Write_GLCD(brojac);
      stanje6 = 0;
   }
    
   if(!PORTDbits.RD9)
        stanje6 = 1;
   
 
    if(PORTFbits.RF6 && (stanje7 == 1))
    {
        stanje7 = 0;
        flag11 = 1;
    }
    
    if(!PORTFbits.RF6)
        stanje7 = 1;
    
    if(1 == flag11)
    {
        GLCD_ClrScr();
        
        if(brojac != 4)
        {
            GoToXY(0, 0);
            GLCD_Printf("Wrong1 password");
            counter++;
        }
        else
        {
            if((array[0] == 8) && (array[1] == 9) && (array[2] == 8) && (array[3] == 9))
            {
                GoToXY(0,0);
               GLCD_Printf("Correct password");
               RS232_putst("Otvorite vrata");
               
               
               while(!(tempRX == open)); 
                   LATBbits.LATB6 = 1;
                   
               while(!(tempRX == close));
                    LATBbits.LATB6 = 0;
               
               
            }
            else
            {
                GoToXY(0,0);
               GLCD_Printf("Wrong2 password");
               counter++;
               
            }
        }
        
        flag11 = 0;
        brojac = 0;
    }
      
    if(counter == 3 && a == 1){
        GLCD_ClrScr();
        GLCD_Printf("JU PROVALNIK!!");
    a = 0;
    
     }
    }
    
}
