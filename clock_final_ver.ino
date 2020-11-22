#include <TM1637Display.h>
#include <FlexiTimer2.h>
#include <TimerOne.h>

#define ButConf 3
#define ButIncrement 2
#define ButDecrement 4
#define ButColor 5
#define Red A0
#define Green A1
#define Blue A2
#define CLK_t 9
#define DIO_t 8
#define CLK2_t 11
#define DIO2_t 10
#define CLK3_t 7
#define DIO3_t 6



int minute, hour, day=1, month=1, year=2020, state_button, pressed, seconds, cycle_state=0, cycle=0, checked=0, blinki_state=0, state_increment=0, state_decrement=0, state_color_but=0, state_color=0, light=1, init_state=0, init_sm=0;
int prev_sec, prev_min, prev_hour, prev_day, prev_month, prev_year, prev_blink, conf_check=1, conf_var=0;
unsigned long curr_time, time_press, time_color;
bool AnoBissexto, ConfMode;
uint8_t second, blinking=0; 
TM1637Display display = TM1637Display(CLK_t, DIO_t);
TM1637Display display_2 = TM1637Display(CLK2_t, DIO2_t);
TM1637Display display_3 = TM1637Display(CLK3_t, DIO3_t);
const uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
const uint8_t init_1[] = {
  SEG_A,
  SEG_A
};
const uint8_t init_1_1[] = { //Para o display dos dias/ano
  SEG_A,
  SEG_A,
  SEG_A,
  SEG_A
};
const uint8_t init_2[] = {
  SEG_A | SEG_B 
};
const uint8_t init_2_2[] = { //Para o display do ano
  SEG_A,
  SEG_A | SEG_B | SEG_C 
};
const uint8_t init_3[] = {
  SEG_B | SEG_C
};
const uint8_t init_3_3[] = { //Para o display do ano
  SEG_A | SEG_B | SEG_C | SEG_D
};
const uint8_t init_4[] = {
  SEG_C | SEG_D 
};
const uint8_t init_4_4[] = { //Para o display do ano
  SEG_D,
  SEG_B | SEG_C | SEG_D
};
const uint8_t init_5[] = { 
  SEG_D,
  SEG_D
};
const uint8_t init_5_5[] = { //Para o display dos dias/ano
  SEG_D,
  SEG_D,
  SEG_D,
  SEG_D
};
const uint8_t init_6[] = {
  SEG_D | SEG_E
};
const uint8_t init_6_6[] = { //Para o display dos dias
  SEG_D | SEG_E | SEG_F,
  SEG_D
};
const uint8_t init_7[] = {
  SEG_E| SEG_F
};
const uint8_t init_7_7[] = { //Para o display dos dias
  SEG_D | SEG_E| SEG_F
};
const uint8_t init_8[] = {
  SEG_A | SEG_F
};
const uint8_t init_8_8[] = { //Para o display dos dias
  SEG_A | SEG_E | SEG_F,
  SEG_A
};
const uint8_t init_5_one[] = { 
  SEG_D
};
void setup() {
  // put your setup code here, to run once:
  FlexiTimer2::set(1000, timerInt);
  FlexiTimer2::start();
  Serial.begin(9600);
  pinMode(ButConf, INPUT_PULLUP);
  pinMode(ButIncrement, INPUT_PULLUP);
  pinMode(ButDecrement, INPUT_PULLUP);
  pinMode(ButColor, INPUT_PULLUP);
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
  display.clear();
  display_2.clear();
  display_3.clear();
  display.setBrightness(7);
  display_2.setBrightness(7);
  display_3.setBrightness(7);
  RGB_color(0, 0, 0);
  //declarar butões - 4
  //declarar ecrasinhos
  //declarar leds rgb
}

bool bi(){                                                                      //tells if current year has 365 or 366 days
  int D1, D2, D3; 
  D1 = year%4;
  D2 = year%100;
  D3 = year%400;
  if (D1==0 && D2!=0)
    return 1;
  else if (D1!=0 && D3!=0)
    return 0;
  else if (D1!=0 && D3==0)
    return 1;
}

void timerInt() {                                                               // increments seconds
  second++;           
}

void timeCalc(){                                                                //calculates time and date
  seconds=second;
  
  if (prev_sec!=seconds){
    prev_sec=seconds;
  }
  if (prev_min!=minute){ 
    Print_clock(minute, hour, day, month, year);
    prev_min=minute;
  }
  if (prev_hour!=hour){ 
    Print_clock(minute, hour, day, month, year);
    prev_hour=hour;
  }
  if (prev_day!=day){ 
    Print_clock(minute, hour, day, month, year);
    prev_day=day;
  }
  if (prev_month!=month){ 
    Print_clock(minute, hour, day, month, year);
    prev_month=month;
  }
  if (prev_year!=year){ 
    Print_clock(minute, hour, day, month, year);
    prev_year=year;
  }
  
  if (seconds>=60){
    second=0;
    minute++;
  }
  if (minute>=60){
    minute=0;
    hour++;
  }
  if (hour>=24){
    hour=0;
    day++;
  }
  if (((month==4 || month==6 || month==9 || month==11) && day>=31) || ((month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12) && day>=32)){
    day=1;
    month++;
  }
  if (month>=13){
      month=1;
      year++;
    }
  if (month==2 && AnoBissexto && day>=30){
    day=1;
    month++;
  } else if (month==2 && !AnoBissexto && day>=29){
    day=1;
    month++;
  }
  Serial.println();
  Serial.print(hour/10);
  Serial.print(hour%10);
  Serial.print(":");
  Serial.print(minute/10);
  Serial.print(minute%10);
  Serial.print(":");
  Serial.print(seconds/10);
  Serial.print(seconds%10);
  //prints hh:mm:ss
  Serial.print(" ");
  Serial.print(day/10);
  Serial.print(day%10);
  Serial.print(" ");
  Serial.print(month/10);
  Serial.print(month%10);
  Serial.print(" ");
  Serial.print(year/10);
  Serial.print(year%10);
  //prints dd mm yy
  Serial.print(" ");
  Serial.print("Bi:");
  Serial.print(AnoBissexto);
  Serial.print(" ");
}

void ReadButs(){                                                                //reads buttons  
  if (state_button==0 && digitalRead(ButConf)==HIGH){ //not pressed
    ConfMode=0;
    conf_var=0;
    state_button=0;
    pressed=0;
  }else if (state_button==0 && digitalRead(ButConf)==LOW){ //pressed
    state_button=1; 
    if (pressed==0)time_press=millis(); 
    pressed=1;
  }else if (state_button==1 && digitalRead(ButConf)==HIGH){
    state_button=0;
  }else if (state_button==1 && millis()-time_press>=1500){
    ConfMode=1;
    if (conf_var==0){
      display.clear();
      display_2.clear();
      display_3.clear();
      display.setSegments(init_5, 2, 2);
      display.setSegments(init_5, 2, 0);
      display_2.setSegments(init_5, 2, 2);
      display_2.setSegments(init_5, 2, 0);
      display_3.setSegments(init_5, 2, 2);
      display_3.setSegments(init_5, 2, 0);
      conf_var=1;
    }
    state_button=4;
  }else if (state_button==4 && digitalRead(ButConf)==HIGH){
    state_button=2;
  }else if (state_button==2 && digitalRead(ButConf)==HIGH){
    ConfMode=1;
    if (conf_var==0){
      display.clear();
      display_2.clear();
      display_3.clear();
      display.setSegments(init_5, 2, 2);
      display.setSegments(init_5, 2, 0);
      display_2.setSegments(init_5, 2, 2);
      display_2.setSegments(init_5, 2, 0);
      display_3.setSegments(init_5, 2, 2);
      display_3.setSegments(init_5, 2, 0);
      conf_var=1;
    }
    state_button=2;
    pressed=0;
  }else if (state_button==2 && digitalRead(ButConf)==LOW){
    state_button=3;
    if (pressed==0)time_press=millis();
    pressed=1;
  }else if (state_button==3 && digitalRead(ButConf)==HIGH){
    state_button=2;
  }else if (state_button==3 && millis()-time_press>=1500){
    ConfMode=0;
    conf_var=0;
    state_button=5;
  }else if (state_button==5 && digitalRead(ButConf)==HIGH){
    state_button=0;
  }
  
  if (time_press > millis()) time_press=0;
    /*
    Serial.print("ConfMode:");
    Serial.print(ConfMode);*/
}

void blinki(){                                                                  //blinker used to make transition between "__" and "xx"                      
  if (checked==0 && blinki_state==0){
    curr_time=millis();
    checked=1;
    blinki_state=1;
  } else if (checked==1 && blinki_state==1 && millis()-curr_time>=500){
    blinking=1;
    curr_time=millis();    
    blinki_state=2;
  }else if (checked==1 && blinki_state==2 && millis()-curr_time>=500){
    blinking=0;
    curr_time=millis();    
    blinki_state=1;
  } else if (checked==0 && blinki_state!=0){
    blinki_state=0;
  } 
  if (curr_time > millis()) curr_time=0;

}

void Conf(int s, int m, int h, int D, int M, int Y){                            //configuration mode
  
  if (cycle_state==0 && digitalRead(ButConf)==HIGH){
    cycle=2;                                                        //minute
    cycle_state=1;
  } else if (cycle_state==1 && digitalRead(ButConf)==LOW){
    cycle_state=2; 
  } else if (cycle_state==2 && digitalRead(ButConf)==HIGH){
    cycle=3;                                                        //hour 
    cycle_state=3; 
  } else if (cycle_state==3 && digitalRead(ButConf)==LOW){
    cycle_state=4; 
  } else if (cycle_state==4 && digitalRead(ButConf)==HIGH){
    cycle=4;                                                        //year1
    cycle_state=5; 
  } else if (cycle_state==5 && digitalRead(ButConf)==LOW){
    cycle_state=6; 
  } else if (cycle_state==6 && digitalRead(ButConf)==HIGH){
    cycle=5;                                                        //year2(decade)
    cycle_state=7; 
  } else if (cycle_state==7 && digitalRead(ButConf)==LOW){
    cycle_state=8; 
  } else if (cycle_state==8 && digitalRead(ButConf)==HIGH){
    cycle=6;                                                        //month
    cycle_state=9; 
  } else if (cycle_state==9 && digitalRead(ButConf)==LOW){
    cycle_state=10; 
  } else if (cycle_state==10 && digitalRead(ButConf)==HIGH){
    cycle=7;                                                        //day
    cycle_state=11; 
  } else if (cycle_state==11 && digitalRead(ButConf)==LOW){
    cycle_state=12; 
  } else if (cycle_state==12 && digitalRead(ButConf)==HIGH){
    cycle=2;                                                        //END
    cycle_state=0; 
  }

  if (state_increment==0 && digitalRead(ButIncrement)==LOW){
    state_increment=1;
  } else if (state_increment==1 && cycle==2 && digitalRead(ButIncrement)==HIGH){
    m++;
    if (m>=60)m=0;
    state_increment=0;
  } else if (state_increment==1 && cycle==3 && digitalRead(ButIncrement)==HIGH){
    h++;
    if (h>=24)h=0;
    state_increment=0;
  } else if (state_increment==1 && cycle==4 && digitalRead(ButIncrement)==HIGH){
    Y++;
    state_increment=0;
  } else if (state_increment==1 && cycle==5 && digitalRead(ButIncrement)==HIGH){
    Y+=10;
    state_increment=0;
  } else if (state_increment==1 && cycle==6 && digitalRead(ButIncrement)==HIGH){
    M++;
    if (M>=13)M=1;
    state_increment=0;
  } else if (state_increment==1 && cycle==7 && digitalRead(ButIncrement)==HIGH){
    D++;
    if (((M==4 || M==6 || M==9 || M==11) && D>=31) || ((M==1 || M==3 || M==5 || M==7 || M==8 || M==10 || M==12) && D>=32)){
      D=1;
    }
    if (M==2 && AnoBissexto && D>=30){
      D=1;
    } else if (M==2 && !AnoBissexto && D>=29){
      D=1;
    }
      state_increment=0;
  }

  if (state_decrement==0 && digitalRead(ButDecrement)==LOW){
    state_decrement=1;
  } else if (state_decrement==1 && cycle==2 && digitalRead(ButDecrement)==HIGH){
    m--;
    if (m<=-1)m=59;
    state_decrement=0;
  } else if (state_decrement==1 && cycle==3 && digitalRead(ButDecrement)==HIGH){
    h--;
    if (h<=-1)h=23;
    state_decrement=0;
  } else if (state_decrement==1 && cycle==4 && digitalRead(ButDecrement)==HIGH){
    Y--;
    state_decrement=0;
  } else if (state_decrement==1 && cycle==5 && digitalRead(ButDecrement)==HIGH){
    Y-=10;
    state_decrement=0;
  } else if (state_decrement==1 && cycle==6 && digitalRead(ButDecrement)==HIGH){
    M--;
    if (M<=0)M=12;
    state_decrement=0;
  } else if (state_decrement==1 && cycle==7 && digitalRead(ButDecrement)==HIGH){
    D--;
    if (((M==4 || M==6 || M==9 || M==11) && D<=0)){
      D=30;
    } else if ((M==1 || M==3 || M==5 || M==7 || M==8 || M==10 || M==12) && D<=0){
      D=31;
    }
    if (M==2 && AnoBissexto && D<=0){
      D=29;
    } else if (M==2 && !AnoBissexto && D<=0){
      D=28;
    }
      state_decrement=0;
  }
   
    
    Serial.println();
    Serial.print("C: ");
  if (blinking==0 && cycle==3 && prev_hour==h){                                 //Set hours animations
    if (prev_blink!=blinking){
      prev_blink=blinking;
      Print_clock_blink(m, h, D, M, Y, blinking, cycle);
    }
    Serial.print("_");
    Serial.print("_");
  } else if ((blinking==1 && cycle==3) && prev_hour==h){
    if (prev_blink!=blinking){
      prev_blink=blinking;
      Print_clock_blink(m, h, D, M, Y, blinking, cycle);
    } 
    Serial.print(h/10);
    Serial.print(h%10);
  } else if (prev_hour!=h){
     Print_clock_blink(m, h, D, M, Y, 1, cycle);
     prev_hour=h;
     prev_blink=blinking;
  }
    Serial.print(":");

    
  if (blinking==0 && cycle==2 && prev_min==m){                                  //Set minutes animation
    if (prev_blink!=blinking){
      prev_blink=blinking;
      Print_clock_blink(m, h, D, M, Y, blinking, cycle);
    }
    Serial.print("_");
    Serial.print("_");
  } else if ((blinking==1 && cycle==2 && prev_min==m)){
    if (prev_blink!=blinking){
      prev_blink=blinking;
      Print_clock_blink(m, h, D, M, Y, blinking, cycle);
    }
    Serial.print(m/10);
    Serial.print(m%10);
  } else if (prev_min!=m){
     Print_clock_blink(m, h, D, M, Y, 1, cycle);
     prev_min=m;
     prev_blink=blinking;
  }
    Serial.print(":");
  
  
  //if (blinking==0 && cycle==1){
  //Serial.print("_");
  //Serial.print("_");
  //} else if ((blinking==1 && cycle==1) || cycle!=1){
    Serial.print(s/10);
    Serial.print(s%10);
  //}
  //prints hh:mm:ss
    Serial.print(" ");

    
  if (blinking==0 && cycle==7 && prev_day==D){               //Set day animation 
    if (prev_blink!=blinking){
      prev_blink=blinking;
      Print_clock_blink(m, h, D, M, Y, blinking, cycle);
    }
    Serial.print("_");
    Serial.print("_");  
  } else if (blinking==1 && cycle==7 && prev_day==D){
      if (prev_blink!=blinking){
        prev_blink=blinking;
        Print_clock_blink(m, h, D, M, Y, blinking, cycle);
    }
    Serial.print(D/10);
    Serial.print(D%10);
  } else if (prev_day!=D){
     Print_clock_blink(m, h, D, M, Y, 1, cycle);
     prev_day=D;
     prev_blink=blinking;
  }
    Serial.print(" ");


  if (blinking==0 && cycle==6 && prev_month==M){               //Set month animation 
    if (prev_blink!=blinking){
      prev_blink=blinking;
      Print_clock_blink(m, h, D, M, Y, blinking, cycle);
    }
    Serial.print("_");
    Serial.print("_");  
  } else if (blinking==1 && cycle==6 && prev_month==M){
      if (prev_blink!=blinking){
        prev_blink=blinking;
        Print_clock_blink(m, h, D, M, Y, blinking, cycle);
      }
    Serial.print(M/10);
    Serial.print(M%10);
  } else if (prev_month!=M){
      Print_clock_blink(m, h, D, M, Y, 1, cycle);
      prev_month=M;
      prev_blink=blinking;
  }
    Serial.print(" ");

    
  if (blinking==0 && cycle==4 && prev_year==Y){               //Set year animation (fazer)
    if (prev_blink!=blinking){
      prev_blink=blinking;
      Print_clock_blink(m, h, D, M, Y, blinking, cycle);
    }
    Serial.print(Y/10);
    Serial.print("_");  
  } else if (blinking==0 && cycle==5 && prev_year==Y){
      if (prev_blink!=blinking){
        prev_blink=blinking;
        Print_clock_blink(m, h, D, M, Y, blinking, cycle);
      }
    Serial.print(Y/100);
    Serial.print("_");
    Serial.print(Y%10);  
  } else if (((blinking==1 && cycle==4) || (blinking==1 && cycle==5)) && prev_year==Y){
      if (prev_blink!=blinking){
        prev_blink=blinking;
        Print_clock_blink(m, h, D, M, Y, blinking, cycle);
      }
    Serial.print(Y/10);
    Serial.print(Y%10);
  } else if (prev_year!=Y){
      Print_clock_blink(m, h, D, M, Y, 1, cycle);
      prev_year=Y;
      prev_blink=blinking;
  }
  
  //prints dd mm yy  
    Serial.print(" Bi:");
    Serial.print(AnoBissexto);
    Serial.print(" ");

  second=s;
  minute=m;
  hour=h;
  day=D;
  month=M;
  year=Y;
  
}

void Set_color(){                                                               //Switch between colors
  if (state_color_but==0 && digitalRead(ButColor)==LOW){
    state_color_but=1;
    time_color=millis(); 
  } else if (state_color_but==1 && digitalRead(ButColor)==HIGH){
    if (state_color >= 5) state_color=0;
    else state_color++;
    state_color_but=0;
  } else if (state_color_but==1 && digitalRead(ButColor)==LOW && millis()-time_color>=1500){
    state_color_but=2;
    light=0;
  } else if (state_color_but==2 && digitalRead(ButColor)==HIGH){
    state_color_but=3;
  } else if (state_color_but==3 && digitalRead(ButColor)==LOW){    
    state_color_but=4;
    time_color=millis();     
  } else if (state_color_but==4 && digitalRead(ButColor)==HIGH){
    state_color_but=3;
  } else if (state_color_but==4 && digitalRead(ButColor)==LOW && millis()-time_color>=1500){
    state_color_but=5;
    light=1;
  } else if (state_color_but==5 && digitalRead(ButColor)==HIGH){
    state_color_but=0;
  }

  if (light==0) RGB_color(0, 0, 0); //OFF
  else if (light!=0){
    if (state_color==0 && (day!=5 || month!=10))
      RGB_color(255, 255, 0);   //YELLOW (DEFAULT)
    else if (state_color==0 && day==5 && month==10)
      RGB_color(255, 0, 255);   //PINK <3 (DEFAULT)
    else if (state_color==1 && (day!=5 || month!=10))
      RGB_color(255, 0, 0);     //RED
    else if (state_color==1 && day==5 && month==10)
      RGB_color(255, 255, 0);   //YELLOW
    else if (state_color==2)
      RGB_color(0, 255, 0);     //GREEN
    else if (state_color==3)
      RGB_color(0, 0, 255);     //BLUE
    else if (state_color==4)
      RGB_color(0, 255, 255);   //CIAN BLUE
    else if (state_color==5)
      RGB_color(255, 255, 255); //WHITE
  }
  // RGB_color(255, 0, 255); //PINK
}

void RGB_color(int R, int G, int B){                                            //Prints the color in the RGB LED
  R=255-R; if(R<0)R=0; if(R>255)R=255;
  G=255-G; if(G<0)G=0; if(G>255)G=255;
  B=255-B; if(B<0)B=0; if(B>255)B=255;
  analogWrite(Red, R);    //blue
  analogWrite(Green, G);   //red
  analogWrite(Blue, B);   //green
}

int initialization(){                                                           //Initialization animation
    if (init_sm==0 && millis()>=0 && millis()<250){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_1, 2, 0);
        display_2.setSegments(init_1_1, 4, 0);
        init_sm=1;
        return 0;
    } else if (init_sm==1 && millis()>=250 && millis()<500){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_1, 2, 1);
        display_2.setSegments(init_1, 2, 2);
        display_3.setSegments(init_1, 2, 0);
        init_sm=2;
        return 0;
    } else if (init_sm==2 && millis()>=500 && millis()<750){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_1, 2, 2);
        display_3.setSegments(init_1_1, 4, 0);
        init_sm=3;
        return 0;
    } else if (init_sm==3 && millis()>=750 && millis()<1000){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_2, 1, 3);
        display_3.setSegments(init_2_2, 2, 2);
        init_sm=4;
        return 0;
    } else if (init_sm==4 && millis()>=1000 && millis()<1250){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_3, 1, 3);
        display_3.setSegments(init_3_3, 1, 3);
        init_sm=5;
        return 0;
    } else if (init_sm==5 && millis()>=1250 && millis()<1500){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_4, 1, 3);
        display_3.setSegments(init_4_4, 2, 2);
        init_sm=6;
        return 0;
    } else if (init_sm==6 && millis()>=1500 && millis()<1750){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_5, 2, 2);
        display_3.setSegments(init_5_5, 4, 0);
        init_sm=7;
        return 0;
    } else if (init_sm==7 && millis()>=1750 && millis()<2000){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_5, 2, 1);
        display_2.setSegments(init_5, 2, 2);
        display_3.setSegments(init_5, 2, 0);
        init_sm=8;
        return 0;
    } else if (init_sm==8 && millis()>=2000 && millis()<2250){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_5, 2, 0);
        display_2.setSegments(init_5_5, 4, 0);
        init_sm=9;
        return 0;
    } else if (init_sm==9 && millis()>=2250 && millis()<2500){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_6, 1, 0);
        display_2.setSegments(init_6_6, 2, 0);
        init_sm=10;
        return 0;
    } else if (init_sm==10 && millis()>=2500 && millis()<2750){
        display.clear();
        display_2.clear();
        display_3.clear();
        display.setSegments(init_7, 1, 0);
        display_2.setSegments(init_7_7, 1, 0);
        init_sm=11;
        return 0;
    /*} else if (init_sm==11 && millis()>=2750 && millis()<3000){
        display.clear();
        display.setSegments(init_8, 1, 0);
        init_sm=12;
        return 0;*/
    } else if (millis()>=2750){
        display.clear();
        display_2.clear();
        display_3.clear();
        return 1;
    }
}

void Print_clock(int minute, int hour, int day, int month, int year){
    display.clear();
    display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
    display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
    //display_2.showNumberDec(day, false, 2, 0);
    //display_2.showNumberDec(month, false, 2, 2);
    display_3.showNumberDec(year, false, 4, 0);
}

void Print_clock_blink(int minute, int hour, int day, int month, int year, int B, int cyc){
    display.clear();
    if (B==0 && cyc==2){
      display.setSegments(init_5, 2, 2);
      display.showNumberDec(hour, true, 2, 0);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.showNumberDec(year, false, 4, 0);
    } else if (B==1 && cyc==2){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.showNumberDec(year, false, 4, 0);
    } else if (B==0 && cyc==3){
      display.setSegments(init_5, 2, 0);
      display.showNumberDec(minute%10, false, 1, 3);
      display.showNumberDec(minute/10, false, 1, 2);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.showNumberDec(year, false, 4, 0);
    } else if (B==1 && cyc==3){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.showNumberDec(year, false, 4, 0);
    } else if (B==0 && cyc==6){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.setSegments(init_5, 2, 2);
      display_2.showNumberDec(day, false, 2, 0);
      display_3.showNumberDec(year, false, 4, 0);
    } else if (B==1 && cyc==6){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.showNumberDec(year, false, 4, 0);
    } else if (B==0 && cyc==7){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.setSegments(init_5, 2, 0);
      display_2.showNumberDec(month%10, false, 1, 3);
      display_2.showNumberDec(month/10, false, 1, 2);
      display_3.showNumberDec(year, false, 4, 0);
    } else if (B==1 && cyc==7){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.showNumberDec(year, false, 4, 0);
    } else if (B==0 && cyc==4){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.setSegments(init_5_one, 1, 3);
      display_3.showNumberDec(year/10, false, 3, 0);
    } else if (B==1 && cyc==4){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.showNumberDec(year, false, 4, 0);
    }else if (B==0 && cyc==5){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.setSegments(init_5_one, 1, 2);
      display_3.showNumberDec(year/100, false, 2, 0);
      display_3.showNumberDec(year%10, false, 1, 3);
    } else if (B==1 && cyc==5){
      display.showNumberDecEx(hour*100+minute, 0b01000000, true, 4, 0);
      display_2.showNumberDecEx(day*100+month, 0b01000000, false, 4, 0);
      display_3.showNumberDec(year, false, 4, 0);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //display_2.setSegments(data);
  //display_3.setSegments(data);
  if (init_state==0) init_state = initialization();
  else if (init_state!=0){

    Set_color();
    AnoBissexto=bi();
    ReadButs();
    if (!ConfMode){
      if (conf_check==1){
        Print_clock(minute, hour, day, month, year);
        conf_check=0;  
      } 
      timeCalc();
      cycle=0;
      cycle_state=0;
      state_increment=0;
    }
    if (ConfMode){
      Conf(seconds,minute,hour,day,month,year);
      conf_check=1;
    }
    blinki();
  }
}
