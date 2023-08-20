#include <mcs51reg.h>

#define DATA_BUS  (P0)
#define RS        (P2_0)
#define E         (P2_2)

#define READDATA	(P1)
#define ADC_READ        (P2_1)
#define INH	(P2_3)
#define A	(P2_6)
#define B	(P2_5)
#define C	(P2_4)
#define ADC_WRITE	(P2_7)

#define row1 P3_7
#define row2 P3_6
#define row3 P3_5
#define row4 P3_4
#define col1 P3_3
#define col2 P3_2
#define col3 P3_1
#define col4 P3_0

#define LCD_CMD_CLEAR_SCREEN (0X01)
#define LCD_CMD_RETURN_HOME (0x02)
#define LCD_CMD_MOVE_CURSER_LEFT (0x04)
#define LCD_CMD_MOVE_CURSOR_RIGHT (0x06)
#define LCD_CMD_SHIFT_DISPLAY_RIGHT (0x05)
#define LCD_CMD_SHIFT_DISPLAY_LEFT (0x07)
#define LCD_CMD_DISPLAY_OFF_CURSOR_OFF (0x08)
#define LCD_CMD_DISPLAY_OFF_CURSOR_ON (0x0A)
#define LCD_CMD_DISPLAY_ON_CURSOR_OFF (0x0C)
#define LCD_CMD_DISPLAY_ON_CURSOR_BLINING (0x0E)
#define LCD_CMD_SHIFT_CURSOR_POS_LEFT (0x10)
#define LCD_CMD_SHIFT_CURSOR_POS_RIGHT (0x14)
#define LCD_CMD_SHIFT_ENTIRE_DISPLAY_LEFT (0x18)
#define LCD_CMD_SHIFT_ENTIRE_DISPLAY_RIGHT (0x1C)
#define LCD_CMD_GOTO_HOME_FIRST_LINE (0x80)
#define LCD_CMD_GOTO_HOME_SECOND_LINE (0xC0)
#define LCD_CMD_2_LINES_5x7_MATRIX (0x38)


char pressedkey = 0;
unsigned int Number = 0;
unsigned int Number1 = 0;
unsigned int Number2 = 0;
float temp_read = 0;
int interrpt_state = 1;
float Adc_Value = 0;
int Temp[8];
int Temp_Average = 0;
int Temp_Minimum = 0;
int Temp_Maximum = 0;
int i = 0;
char selectedchannel = 1;
char tempchannel = 1;
char State = 0;


void Init(void);
void PutChar(char);
void StrOut(char, char, unsigned char *);
void WriteString(unsigned char *);
void ClearScreen(void);

char FindCol1(void);
char FindCol2(void);
char FindCol3(void);
char FindCol4(void);
char FindPressedKey(void);



void delay(unsigned int j)
{
   unsigned char i = 60;

   for(; j>0; j--)
   {
      while(--i);

      i = 59;
      while(--i);

      i = 60;
    }
}

void WriteCmd(unsigned char cmd)
{
   E = 0;
   RS = 0;
   DATA_BUS = cmd;
   E = 1;
   delay(1);
   E = 0;
   delay(1);
}

void WriteData(unsigned char wdata)
{
   E = 0;
   RS = 1;
   DATA_BUS = wdata;
   E = 1;
   delay(1);
   E = 0;
   delay(1);
}

void ClearScreen(void)
{
   WriteCmd(LCD_CMD_CLEAR_SCREEN);
   delay(1);
}

void Init(void)
{
   WriteCmd(LCD_CMD_2_LINES_5x7_MATRIX); // 8-bits, 2 lines, 7x5 dots
   WriteCmd(LCD_CMD_DISPLAY_ON_CURSOR_BLINING); // no cursor, no blink, enable display
   WriteCmd(LCD_CMD_MOVE_CURSOR_RIGHT); // auto-increment on
   WriteCmd(LCD_CMD_CLEAR_SCREEN);
   delay(150);
}

void WriteString(unsigned char *s)
{
   while(*s>0)
   {
      WriteData(*s++);
      delay(100);
   }
}

void PutChar(char key_pressed)
{
   char key_array[] = "0123456789ABCDEF";

   WriteData(key_array[key_pressed]);
}

void StrOut(char x, char y, unsigned char *s)
{
   char k = 0;

   if(y == 0 )
         WriteCmd(LCD_CMD_GOTO_HOME_FIRST_LINE);
   else if(y == 1 )
      WriteCmd(LCD_CMD_GOTO_HOME_SECOND_LINE);

   delay(1);

   for( k = 0; k < x; k ++ )
   {
      WriteCmd(LCD_CMD_SHIFT_CURSOR_POS_RIGHT);
   }

   while(*s>0)
   {
      WriteData(*s++);
      delay(1);
   }
}


char FindCol1(void)
{
   char retkey = 100;

   row1=row2=row3=row4=1;

   row1=0;
   if(col1==0) { while(col1==0); return 7; }

   row1=1; row2=0;
   if(col1==0) { while(col1==0); return 4; }

   row2=1; row3=0;
   if(col1==0) { while(col1==0); return 1; }

   row3=1; row4=0;
   if(col1==0) { while(col1==0); return 15; }

   row4=1;

   return retkey;
}

char FindCol2(void)
{
   char retkey = 100;

   row1=row2=row3=row4=1;

   row1=0;
   if(col2==0) { while(col2==0); return 8; }

   row1=1; row2=0;
   if(col2==0) { while(col2==0); return 5; }

   row2=1; row3=0;
   if(col2==0) { while(col2==0); return 2; }

   row3=1; row4=0;
   if(col2==0) { while(col2==0); return 0; }

   row4=1;

   return retkey;
}

char FindCol3(void)
{
   char retkey = 100;

   row1=row2=row3=row4=1;

   row1=0;
   if(col3==0) { while(col3==0); return 9; }

   row1=1; row2=0;
   if(col3==0) { while(col3==0); return 6; }

   row2=1; row3=0;
   if(col3==0) { while(col3==0); return 3; }

   row3=1; row4=0;
   if(col3==0) { while(col3==0); return 14; }

   row4=1;

   return retkey;
}

char FindCol4(void)
{
   char retkey = 100;

   row1=row2=row3=row4=1;

   row1=0;
   if(col4==0) { while(col4==0); return 10; }

   row1=1; row2=0;
   if(col4==0) { while(col4==0); return 11; }

   row2=1; row3=0;
   if(col4==0) { while(col4==0); return 12; }

   row3=1; row4=0;
   if(col4==0) { while(col4==0); return 13; }

   row4=1;

   return retkey;
}

char FindPressedKey(void)
{
   char retval = 100;

   while(retval == 100)
   {
      row1=row2=row3=row4=0;
      col1=col2=col3=col4=1;

      if(col1 == 0) retval = FindCol1();
      else if(col2 == 0) retval = FindCol2();
      else if(col3 == 0) retval = FindCol3();
      else if(col4 == 0) retval = FindCol4();
   }

   return retval;
}


int ReadTemp(int ndx)
{

   INH = 1;

   if( ndx == 1)
   {
      C = 0;
      B = 0;
      A = 0;
   }
   else if( ndx == 2)
   {
      C = 0;
      B = 0;
      A = 1;
   }
   else if( ndx == 3)
   {
      C = 0;
      B = 1;
      A = 0;
   }
   else if( ndx == 4)
   {
      C = 0;
      B = 1;
      A = 1;
   }
   else if( ndx == 5)
   {
      C = 1;
      B = 0;
      A = 0;
   }
   else if( ndx == 6)
   {
      C = 1;
      B = 0;
      A = 1;
   }
   else if( ndx == 7)
   {
      C = 1;
      B = 1;
      A = 0;
   }
   else if( ndx == 8)
   {
      C = 1;
      B = 1;
      A = 1;
   }

   INH = 0;
   delay(100);

   ADC_READ = 1;
   delay(10);
   ADC_WRITE = 0;
   delay(10);
   ADC_WRITE = 1;
   delay(1);
   ADC_READ = 0;
   delay(1);
   Adc_Value = READDATA;
   ADC_READ = 1;



   temp_read = (5 * Adc_Value) / 255.0;
   temp_read *= 100;
   temp_read -= 273;

   temp_read -= 93;
   temp_read /= 6;
   temp_read *= 10;
   temp_read += 10;

   return (int)temp_read;
}

void WriteTemp(int tmp)
{
   int new_tmp = tmp;
   char digit_tens = 0;
   char digit_ones = 0;

   if( new_tmp < 0 )
   {
      new_tmp *= -1;
      WriteString("-");
   }

   digit_tens = (char)(new_tmp / 10 );
   digit_ones = (char)(new_tmp - ((int)digit_tens) * 10 );

   PutChar(digit_tens);
   PutChar(digit_ones);
}

void WriteLine1(int t_A, int t_max, int t_min)
{
   StrOut(0,1,"Ag=");
   WriteTemp(t_A);
   WriteString(" M=");
   WriteTemp(t_max);
   WriteString(",");
   WriteTemp(t_min);
}

void WriteLine2(char ch, int Tmps[], char st)
{
   StrOut(0,1,"CH");
   PutChar(ch);
   WriteString("=");
   WriteTemp(Tmps[ch-1]);
   WriteString(" State=");
   PutChar(st);
}

int GetState3(int tmps[] )
{
   int i;
   int cnt = 0;

   for( i = 0; i <8; i ++)
   {
      if(tmps[i] < 0) cnt ++;
   }

   return cnt;
}


int FindAverageValue(int tmps[] )
{
   int i;
   int total = 0;

   for( i = 0; i <8; i ++)
   {
      total += tmps[i];
   }

   total = (int)(total / 8);

   return total;
}

int FindMinimumValue(int tmps[] )
{
   int i;
   int min = tmps[0];

   for( i = 0; i <8; i ++)
   {
      if(tmps[i] < min )
	 min = tmps[i];
   }

   return min;
}

int FindMaximumValue(int tmps[] )
{
   int i;
   int max = tmps[0];

   for( i = 0; i <8; i ++)
   {
      if(tmps[i] > max )
	 max = tmps[i];
   }

   return max;
}


void main()
{

   P0 = 0x00;
   P1 = 0xff;
   P2 = 0x00;
   P3 = 0x0f;

   INH = 1;
   A = 0;
   B = 0;
   C = 0;
   ADC_WRITE = 1;
   ADC_READ = 1;

   Init();

   for(i=0; i <8;i ++)
   {
      Temp[i] = 0;
   }

   selectedchannel = 1;


   while(1)
   {
      Temp[0] = ReadTemp(1);
      Temp[1] = ReadTemp(2);
      Temp[2] = ReadTemp(3);
      Temp[3] = ReadTemp(4);
      Temp[4] = ReadTemp(5);
      Temp[5] = ReadTemp(6);
      Temp[6] = ReadTemp(7);
      Temp[7] = ReadTemp(8);

      Temp_Average = FindAverageValue(Temp);
      Temp_Minimum = FindMinimumValue(Temp);
      Temp_Maximum = FindMaximumValue(Temp);

      if( (Temp_Average < 0 ) && ( Temp_Maximum < 0 ) ) State = 1;
      else if( (Temp_Average < 0 ) && ( Temp_Maximum > 0 ) ) State = 2;
      else if( (Temp_Average > 0 ) && ( GetState3(Temp) >= 2) ) State = 3;
      else if( (Temp[0] < 0 ) && (Temp[1] > 0 ) && (Temp[2] < 0 ) && (Temp[3] > 0 ) && (Temp[4] < 0 ) && (Temp[5] > 0 ) && (Temp[6] < 0 ) && (Temp[7] > 0 ) ) State = 4;

      ClearScreen();
      WriteLine1(Temp_Average, Temp_Maximum, Temp_Minimum);
      WriteLine2(selectedchannel, Temp, State);


      pressedkey = FindPressedKey();
      if( (pressedkey >= 1) && (pressedkey <= 8) )
      {
	 tempchannel = pressedkey;
	 pressedkey = FindPressedKey();
	 if( pressedkey == 14 ) selectedchannel = tempchannel;
      }
   }

}
