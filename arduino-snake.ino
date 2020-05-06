#include <avr/io.h>
#include <util/delay.h>
#include <time.h>
#include <stdlib.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int tabGame[4][16] = {},
    headXY[] = {0, 3},
    appleXY[] = {rand()%4, rand()%16},
    size = 4,
    timeGame = 0;

void show(){
  for (int x=0; x<4; x+=2){
    for (int y=0; y<16; y++){
      if(tabGame[x][y]==0 && tabGame[x+1][y]==0 ){
        lcd.setCursor(y,x); 
        lcd.write(" ");
      }else if(tabGame[x][y]>0 && tabGame[x][y]<64 && tabGame[x+1][y]>0 && tabGame[x+1][y]<64){
        lcd.setCursor(y,x); 
        lcd.write(uint8_t(2));
      }else if(tabGame[x][y]>0 && tabGame[x][y]<64 && tabGame[x+1][y]==0){
        lcd.setCursor(y,x); 
        lcd.write(uint8_t(1));
      }else if(tabGame[x+1][y]>0 && tabGame[x+1][y]<64 && tabGame[x][y]==0){
        lcd.setCursor(y,x); 
        lcd.write(uint8_t(0));
      }else if(tabGame[x][y]==666 && tabGame[x+1][y]==0){
        lcd.setCursor(y,x); 
        lcd.write(uint8_t(5));
      }else if(tabGame[x+1][y]==666 && tabGame[x][y]==0){
        lcd.setCursor(y,x); 
        lcd.write(uint8_t(6));
      }else if(tabGame[x][y]>0 && tabGame[x][y]<64 && tabGame[x+1][y]==666){
        lcd.setCursor(y,x); 
        lcd.write(uint8_t(3));
      }else if(tabGame[x+1][y]>0 && tabGame[x+1][y]<64 && tabGame[x][y]==666){
        lcd.setCursor(y,x); 
        lcd.write(uint8_t(4));
      }
    }
  }
}

void move(int poz[2], int headXY[2]){
  headXY[0]+=poz[0];
  headXY[1]+=poz[1];
  //eat
  if(tabGame[headXY[0]][headXY[1]] == 666){
    size = size + 1;
    tabGame[headXY[0]][headXY[1]]=size; 
    appleXY[0] = rand()%4;
    appleXY[1] = rand()%16;
    tabGame[appleXY[0]][appleXY[1]] = 666;
  }else{
    for (int x=0; x<4; x++){
    for (int y=0; y<16; y++){
      if(tabGame[x][y]>0 && tabGame[x][y]<64){
        tabGame[x][y]--; 
      }
     }
    }
    tabGame[headXY[0]][headXY[1]]=size; 
  }
}

void createCustomChar(){

   uint8_t sUP[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x0E,
        0x0E,
        0x0E
      };

      uint8_t sDN[] = {
        0x0E,
        0x0E,
        0x0E,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
      };

      uint8_t sAL[] = {
        0x0E,
        0x0E,
        0x0E,
        0x00,
        0x00,
        0x0E,
        0x0E,
        0x0E
      };

      uint8_t asUP[] = {
        0x0E,
        0x0E,
        0x0E,
        0x00,
        0x00,
        0x04,
        0x0A,
        0x04
      };

      uint8_t asDN[] = {
        0x04,
        0x0A,
        0x04,
        0x00,
        0x00,
        0x0E,
        0x0E,
        0x0E
      };

      uint8_t aUP[] = {
        0x04,
        0x0A,
        0x04,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
      };

      uint8_t aDN[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x04,
        0x0A,
        0x04
      };

        lcd.createChar(0, sUP);
        lcd.createChar(1, sDN);
        lcd.createChar(2, sAL);
        lcd.createChar(3, asUP);
        lcd.createChar(4, asDN);
        lcd.createChar(5, aUP);
        lcd.createChar(6, aDN);
}

int main (void){
  lcd.begin(16, 2);
  createCustomChar();

   // BUTTON CONTROLLER
    DDRD &= ~(1<<DDD2) | ~(1<<DDD3);       // direction - input
    PORTD |= (1<<PD2) | (1<<PD3);         // pin to VCC (internal pullup)

  tabGame[0][0] = 1;
  tabGame[0][1] = 2;
  tabGame[0][2] = 3;
  tabGame[0][3] = 4;
  tabGame[appleXY[0]][appleXY[1]] = 666;
  int poz[2] = {0,1}; // {up/down, left/right}
  
  int click = 0;

  while(1){

      if(!(PIND & 1<<PIND2) && click == 0){
        if(poz[0]==0 && poz[1]==1){poz[0]=1; poz[1]=0;}
        else if(poz[0]==1 && poz[1]==0){poz[0]=0; poz[1]=-1;}
        else if(poz[0]==0 && poz[1]==-1){poz[0]=-1; poz[1]=0;}
        else if(poz[0]==-1 && poz[1]==0){poz[0]=0; poz[1]=1;}
        click = 1;
        _delay_ms(80);
     }else if(!(PIND & 1<<PIND3) && !click){
        if(poz[0]==0 && poz[1]==1){poz[0]=-1; poz[1]=0;}
        else if(poz[0]==-1 && poz[1]==0){poz[0]=0; poz[1]=-1;}
        else if(poz[0]==0 && poz[1]==-1){poz[0]=1; poz[1]=0;}
        else if(poz[0]==1 && poz[1]==0){poz[0]=0; poz[1]=1;}
        click = 1;
        _delay_ms(80);
     }
    
    if(timeGame == 500){
      move(poz, headXY);
      show();
      lcd.setCursor(14,0);
      lcd.print(size);
      timeGame = 0;
      click = 0;
    }
    timeGame+= 100;
    _delay_ms(100);
  }
}
