#include<util/delay.h>
 
const int ledpin = 9; //led출력핀
 
const int inpin = 16; //버튼 입력핀, A2
 
const byte interruptPin = 2;
 
 
 
volatile int count=0; // 버튼 눌린 횟수
 
 
 
volatile int state = LOW;  //stores if the switch was high before at all
 
volatile int state_short = LOW;  //storing the button state for short press mode
 
volatile int state_long = LOW;  //storing the button state for long press mode
 
 
 
volatile unsigned long current_high;
 
volatile unsigned long current_low;
 
 
 
static unsigned long startTime = 0; //스위치 누를때의 시간 
 
int i=0, f;
 
int bright=0;
 
int longtime=0;
 
 
 
void read_button() //버튼과 버튼사이의 간격이 짧다, 길다를 구분
 
{ 
 
  _delay_ms(100);
 
  if(digitalRead(inpin) == HIGH && state == LOW)      //버튼 안눌린 상태
 
  { 
 
    current_high = millis(); //버튼을 에서 손을 뗀시간
 
      state = HIGH;
 
    //return 0;
 
 
 
    if(current_low==0)
 
      state_long=HIGH;
 
  }
 
  
 
   if(digitalRead(inpin) == LOW && state == HIGH)  //버튼 눌린 상태    
 
  { 
 
    current_low = millis();  //버튼을 누른시간
 
    count += 1; //버튼 눌린횟수를 count가 가짐
 
    if((current_low - current_high) > 1 && (current_low - current_high) < 300) //버튼을 두번 눌렀을 때 버튼간의 간격이 짧다
 
    {
 
      state_short = HIGH;
 
       
 
      state = LOW;
 
    }
 
    else if((current_low - current_high) >= 300) //버튼을 두번 눌렀을 때 버튼간의 간격이 길다
 
    {
 
      state_long = HIGH;
 
      state = LOW;
 
    }
 
  }
 
}
 
 
 
long switchTime() //스위치가 눌렸으면 눌린 시간을 리턴함, 눌리지 않았으면 0을 리턴함
 
{
 
  static boolean state_2;                
 
 
 
  if (digitalRead(inpin) != state_2) 
 
  {
 
    state_2 = ! state_2;       
 
     startTime = millis();  
 
  }
 
  if (state_2 == LOW)
 
    return millis() - startTime;   
 
                                   
 
  else
 
    return 0; 
 
}
 
 
 
void setup() 
 
{
 
  pinMode(ledpin, OUTPUT); 
 
  digitalWrite(ledpin, LOW);
 
  pinMode(inpin, INPUT);
 
  attachInterrupt(digitalPinToInterrupt(interruptPin), read_button, CHANGE);
 
  Serial.begin(9600);
 
}
 
 
 
void loop()
 
{ 
 
  int stime = switchTime(); //스위치 눌린시간
 
  
 
//////////////////////////////////////////on, off//////////////////////////////////////////////////////////
 
  if(current_high > 0 && stime<450) //짧게 누른 후 버튼을 뗀 순간
 
  { 
 
    if(digitalRead(ledpin)==1) //led가 켜져있으면 끄고
 
      digitalWrite(ledpin, LOW);
 
      
 
    else if(digitalRead(ledpin)==0) //led가 꺼져 있으면 킨다
 
        digitalWrite(ledpin,HIGH);
 
  }
 
      
 
//////////////////////////////////on,off 끝/////////////////////////////////////////////////////////////////      
 
 
 
//////////////////////////////////밝기변화////////////////////////////////////////////////////////////////////   
 
 
 
  else if(stime >= 450) //길게 눌렸을 때 
 
  { 
 
    if( digitalRead(ledpin)==0 )  //불이 꺼져있으면 끈상태유지
 
    {  Serial.println("!!");
 
        i=bright;
 
        if(f!=1) //밝기변화가 '최고까지증가~최소까지감소'의 동작이 한번도 없었으면, 불이 꺼져있을때 끈상태를 유지한다
 
          digitalWrite(ledpin, LOW);
 
          
 
 
 
        else if(f==1) //밝기변화가 '최고까지증가~최소까지감소'의 동작이 한번이라도 있었으면,led에 불을켜서 아래의 else if문에 해당되게하여 밝기변화를 반복한다
 
          digitalWrite(ledpin, HIGH);
 
    }
 
   
 
    else if(i<=255)  // 길게눌렀는데 i가 255보다 작을 때, 밝기 증가
 
    { 
 
      i++;
 
      analogWrite(ledpin,i);
 
      delay(10);
 
      bright=i; 
 
    }
 
    
 
    else if(i>255 && bright>0) // 길게 누르고 밝기증가 끝났을 때, 밝기 감소
 
    { 
 
      bright--;
 
      analogWrite(ledpin,bright);
 
      delay(10);
 
      if(bright==1) //밝기가 최소로되면 flag표시를 한다.
 
        f=1;
 
    }
 
 
 
  }
 
  current_high = 0;  //인터럽터가 끝나고나면 버튼 뗀순간의 값초기화
 
  current_low = 0;  //인터럽터가 끝나고나면 버튼 누른순간의 값초기화
 
}
