#include<LiquidCrystal.h>
#include<DHT11.h>
#include<EEPROM.h>
#include<stdlib.h>
#include<time.h>
#include <Keypad.h>
//  #include<IRremote.h>
#define IR_ A0
// int __heap_start;
// extern void*__brkval;
LiquidCrystal lcd(13,12,11,10,9,8);
short int pin1,pin2,pin3,iterator=0;
short iterator_remote = 0;
short int row[] = {7,6,5,4};
short int col[] = {3,2,1,0};
char symbol_1[4][4] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'D','0','M','='}//==------------------------------
}; 


bool handle = true;
bool handle_remote = true;
short int tor = A1;
short int buzzer =A2;
short int remote = A3;
char *templates[] = {
  "Whether","Calculator","Torch","Note pad","Timer","Stopwatch","Remote","Change Password","Ammeter","DINO GAME","Snake game","Voltmeter"
};
char *remotes[] = {"NEC","Sony","Raw one","BACK",""};

 
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
byte rowPins[ROWS] = {7,6,5,4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3,2,1,0}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
//   Keypad *customKeypad = new Keypad( makeKeymap(symbol_1), rowPins, colPins, ROWS, COLS); 
// Keypad *customKeypad_2 = new Keypad(makeKeymap(symbol_2),rowPins,colPins,ROWS,COLS);
// Keypad *customKeypad_3 = new Keypad(makeKeymap(kecaKeys),rowPins,colPins,ROWS,COLS);
// Keypad *customKeypad_4 = new Keypad(makeKeymap(mecaKeys),rowPins,colPins,ROWS,COLS);

bool kade_state = false;
bool error_occured = false;
bool  made_state = false;
bool  full_state = false;
bool radian_state = true;
double answer =0;
bool resultDisplayed = false;
bool end = false;
class owner{
  private:
  String own;
  int current ;
  int capacity ;
  public:
   
   owner()
   {
      own = "";
      current =0;
      capacity = 0;
   }
  void push(char data)
  {
    if (capacity>current)
    {
      own[current++] = data;
    }
    else
    {own += data;
    current++;
    capacity = current;}
  }
  void pop()
  {
    current--;
    if (current<0)
    {
        current = 0;
    }
  }
  void replace_at_index(int value,char data)
  {
    if(value>=0&&value<current)
    {
        own[value] = data;
    }
  }
  void push_at_index(int index,char data)
  {
    if (index>=0&& index<current)
    {
      push(" ");
      for(int i = size()-1;i>index;i--)
      {
        own[i] = own[i-1];
      }
      own[index] = data;
    }
  }
  void pop_at_index(int index)
  {
    if (index>=0&& index<current)
    {
      for(int i = index+1;i<current;i++)
      {
        own[i-1] = own[i];
      }
      pop();
    }

  }
  int size()
  {
    return current;
  }
  char back()
  {
    return own[current-1];
  }
  String return_at_index(int index_start,int index_end)
  {
    if(index_start<current){String temp = own.substring(index_start,index_end);
    return temp;}
  }
  void clear()
  {
    own = "";
    current = 0;
    capacity = current;
  }
  String all_return()
  {
    String temp = "";
    temp = own.substring(0,current);
    return temp;
  }
  void take_all(String giver)
  {
    own = giver;
  }
  int find(char data)
  {
    for(int i = 0;i<current;i++)
    {
      if (own[i] == data)
      {
        return i;
      }
    }
    return -1;
  }
 
};
String password = "";
owner expression;
void setpassword()
{
  for(int i =0;i<4;i++)
  {
    password += char(EEPROM.read(i));
  }
}
//void setup
void setup() {
  // put your setup code here, to run once:
  String hello = "WELCOME";
  String temp = "";
  setpassword();
  for(int i = 0;i < 4;i++)
  {
    pinMode(4+i,OUTPUT);
    digitalWrite(4+i,HIGH);
  }
  for(int i = 0;i < 3;i++)
  {
    pinMode(1+i,INPUT_PULLUP);
  }
  lcd.begin(20, 4);
  //begining phase
  for(int i = 0; i< hello.length();i++)
  {
      lcd.setCursor(6+i,1);
      lcd.print(hello[i]);
      delay(200);    
  }
  //any pin
  anypin(true);
  //password checking
 
  while(1)
  {
    if(passwordchecker(password))
    {
      break;
    }
    else
    {
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Try again");
    }
  }

}


void anypin(bool yes)
{
    char value = keypad(row,col,symbol_1,true,true);
    if(yes)return;
}

int generaterandomnumber(int min,int max)
{
  return (rand()%(max-min+1))+min;
}

char keypad(short int rows[4],short int cols[4],char symbolss[4][4],bool maths,bool game)
{
  char value=0;
  while(1)
  {    for(int i = 0; i < 4;i++)
      {
        //for making every rows to low
          digitalWrite(rows[i],LOW);
          delay(10);//small delay
          for(int j =0 ;j < 4;j++)
          {
            //checking
            if(digitalRead(cols[j])==LOW)
            {
              delay(50);//debouncing
                while(digitalRead(cols[j])==LOW&&game);
                //until break;
                value = symbolss[i][j];
                return value;
            }

          }

          digitalWrite(rows[i],HIGH);
          delay(10);
      }
      if(!maths)
      {
        break;
      }
  }
  return value;
}

bool passwordchecker(String password)
{
  char value[4] = {-1};
   lcd.setCursor(0, 1);
  lcd.print("Enter your password");
  for(int i = 0;i< 4;i++)
  {
    lcd.setCursor(6+2*i, 2);
    lcd.print("_");
  }
  for(int i =0 ;i<4;i++)
  {
    value[i] = keypad(row,col,symbol_1,true,true);
    if(value[i] >= 48 && value[i] <= 57)
    {
      lcd.setCursor(6+2*i,2);
      lcd.print((char)value[i]);
    }
    else if(value[i]=='L')
    {
      if(i<=0)i=1;
      value[--i] = -1;
      lcd.setCursor(6+2*i,2);
      lcd.print("_");
    }
  }

  for(int i = 0;i<4;i++)
  {
    if(value[i]!=password[i]){delay(1000);return false;}
  }
  return true;
}

void dht()
{
  
  DHT11 mydht(A0);
  short int temperature;
  short int humidity;
  char key = '1';
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Whether station");
  delay(1000);
  while(1)
  {temperature = mydht.readTemperature();
   humidity = mydht.readHumidity();
  key = keypad(row,col,symbol_1,false,false);
  if(key == '3'){
    //pinMode(A0,INPUT);
    return;}
  delay(900);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Temperature = ");
  lcd.print(temperature);
  lcd.print(" *C");
  lcd.setCursor(0,2);
  lcd.print("Humidity = ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(15,3);
  lcd.print("EXIT");
  }
  
}

void Notepad()
{
  made_state = false;
  kade_state = false;
  lcd.clear();
  lcd.setCursor(6,1);
  lcd.print("NOTEPAD");
  lcd.setCursor(6,2);
 // lcd.print(freeMemory());
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.blink();
  char symbol_3[4][4] = {
  {'.','!','@','#'},
  {'$','%','^','&'},
  {'*','(',')','_'},
  {'`','~',' ',']'}//==------------------------------
};
    char symbol_4[4][4] = {
  {'A','B','C','D'},
  {'E','F','G','H'},//------------------------------------
  {'I','J','K','L'},
  {'M','~',' ',']'}
  };
    char symbol_5[4][4] = {
      {'Z','-','{','}'},
      {'+','\'','\"','?'},
      {';',' ','|',':'},
      {'`','~',' ',']'}
    };
  char keceKeys[4][4] = {
  {'N','O','P','Q'},
  {'R','S','T','U'},
  {'V','W','X','Y'},
  {'`','~',' ',']'}
};
  Keypad *customKeypad = new Keypad( makeKeymap(symbol_4), rowPins, colPins, ROWS, COLS); 
Keypad *customKeypad_2 = new Keypad(makeKeymap(keceKeys),rowPins,colPins,ROWS,COLS);
Keypad *customKeypad_3 = new Keypad(makeKeymap(symbol_3),rowPins,colPins,ROWS,COLS);
Keypad *customKeypad_4 = new Keypad(makeKeymap(symbol_5),rowPins,colPins,ROWS,COLS);
  while(1) {
  // put your main code here, to run repeatedly:
char key = (made_state == false && kade_state == false?customKeypad->getKey():(made_state == true && kade_state == false?customKeypad_2->getKey():(made_state == false && kade_state == true?customKeypad_3->getKey():customKeypad_4->getKey())));
  
  if (key != '\0') {
    KeyPress(key);
    if(end){
        delete customKeypad;
  delete customKeypad_2;
  delete customKeypad_3;

  customKeypad = nullptr;
  customKeypad_2 = nullptr;
  customKeypad_3 = nullptr;
  customKeypad_4 = nullptr;
  for (int i = 0; i < 4; i++) {
  pinMode(row[i], OUTPUT);
  digitalWrite(row[i], HIGH); // default state for rows
}

for (int j = 0; j < 4; j++) {
  pinMode(col[j], INPUT_PULLUP); // columns should be pulled up
}
  break;}
  }

  }
  expression.clear();
  end = false;
  lcd.noBlink();
}

void KeyPress(char key)
{
  if(key == '}')
  {
    expression.clear();
    lcd.setCursor(0, 0);
    return;
  }
  //for ending
  else if(key == '{')
  {
    end = true;
    return;
  }
  //chnging mode
  else if (key == '~')
  {//-----------------------------------------
    
    made_state = !made_state;
    lcd.setCursor(18,3);
    lcd.print((made_state == true?"M":" "));
    lcd.setCursor(17, 3);
    lcd.print((kade_state == true?"K":" "));
    lcd.noBlink();
    return;
  }
  //chnging key 
  else if(key == '`')
  {
    lcd.setCursor(18,3);
    lcd.print((made_state == true?"M":" "));
    kade_state = !kade_state;
    lcd.setCursor(17, 3);
    lcd.print((kade_state == true?"K":" "));
    lcd.noBlink();
    return;
  }
 
  //deleteing mode
  else if (key == ']') {//--------------------------------cutiing
    lcd.clear();
    if(made_state == true)
    {
      lcd.setCursor(18,3);
      lcd.print("M");
    }
    if(kade_state == true)
    {
      lcd.setCursor(17, 3);
    lcd.print("K");
    }
    if(expression.size()>40)
    {
      full_state = true;
    }
    if (full_state == true)
    {
        expression.pop();
         lcd.setCursor(0, 0);
    lcd.print(expression.return_at_index(0,20));
    lcd.setCursor(0,1);
    lcd.print(expression.return_at_index(20,40));
    lcd.setCursor(0,2);
    if (expression.size()>40)
      {//---------------------------------------------------------
        lcd.print(expression.return_at_index(40,expression.size()));}
    lcd.blink();
    full_state = false;
    }
    else
    {  expression.pop();
      lcd.setCursor(0,0);
      lcd.print(expression.all_return());
      lcd.blink();}
    return;
  }

  //writing format
  if (expression.size() <= 19*2) {
    lcd.clear();
    if(made_state == true)
    {
      lcd.setCursor(18,3);
      lcd.print("M");
    }
    if(kade_state == true)
    {
      lcd.setCursor(17, 3);
    lcd.print("K");
    }
    expression.push(key);
    lcd.setCursor(0, 0);
    lcd.print(expression.all_return());
    lcd.blink();
    
  } 

  else if(expression.size() >= 19*2+1 && expression.size()<=19*3)
  {
    lcd.clear();
    if(made_state == true)
    {
      lcd.setCursor(18,3);
      lcd.print("M");
    }
    if(kade_state == true)
    {
      lcd.setCursor(17, 3);
    lcd.print("K");
    }
    expression.push(key);
    lcd.setCursor(0, 0);
    lcd.print(expression.return_at_index(0,20));
    lcd.setCursor(0,1);
    lcd.print(expression.return_at_index(20,40));
    lcd.setCursor(0,2);
    if (expression.size()>40)
      {
        lcd.print(expression.return_at_index(40,expression.size()));}
    lcd.blink();
    
  } 

  else
  {
    lcd.setCursor(3,3);
    lcd.print("MAX LENGTH");
  }
}
void Remote(int pin)
{
  lcd.clear();
  lcd.setCursor(6,1);
  lcd.print("REMOTES");
  delay(3000);
  lcd.clear();
  //IrSender.begin(A0,false,13);
  while(1)
  {
       if(handle_remote)
  {
      int r = iterator_remote/4;
      lcd.clear();
      for(int  i = 0;i < 4;i++)
      {
        lcd.setCursor(0, i);
        lcd.print(remotes[4*r+i]);
      }
      lcd.setCursor(18, iterator_remote-4*r);
      lcd.print("<<");
      handle_remote = false;
  }
    char value = keypad(row,col,symbol_1,true,true);
    if(value == '2')
    {
      iterator_remote++;handle_remote=true;
      if(iterator_remote >= 4)iterator_remote = 3;
    }
    else if(value == '5')
    {
      iterator_remote--;handle_remote = true;
      if(iterator_remote < 0)iterator_remote = 0; 
    }
    else if(value == '1')
    {
    
      select_remote(iterator_remote);
      handle_remote = true;
      value = 0;
    }
    value = 0;
    if(end)
    {
      end = false;
      return;
    }
  }

}

void select_remote(int rem)
{
  switch(rem){
    case 0:
    break;
    case 1:
    break;
    case 2:
    break;
    case 3:
    end = true;
    break;
  }
}
void changePassword()
{
  int i=0;
   char change[5] = {'_','_','_','_','_'};
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Change Password");
  delay(3000);
  for(int i = 0;i < 6;i++)
  {
    if(i==5)return;
    if(passwordchecker(password))
    {
      break;
    }
    else
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Invalid Try again");
      if(i==4)
      {
        lcd.setCursor(2,3);
        lcd.print("Last chance");
      }
    }
  }
  while(1)
  {
    lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Enter new password");
  for(int j = 0;j< 4;j++)
  {
    lcd.setCursor(6+2*j, 2);
    lcd.print(change[j]);
  }
  lcd.setCursor(12,3);
  lcd.print("EXIT->D");
    char key = keypad(row,col,symbol_1,true,true);
  if(key>='0'&&key<='9')
  {
      change[i] = key;
      i++;
  }
  else if(key=='D')
  {
    return;
  }
  else if(key=='M')
  {
    if(i<=0)i=1;
   change[--i] = '_';
  }
    if(i==5)
    {
      bool sach=true;
        for(int k = 0;k<4;k++)
        {
          if(change[k]==-1){
            sach=false;
            break;
          }
        }
        if(!sach)
        {
            lcd.clear();
            for(int k =0 ;k<4;k++)change[k]=-1;
            i=0;
           
            delay(3000);
        }
        else if(sach)
        {
          lcd.setCursor(3,1);
          lcd.print("PASSWORD SAVED");
          lcd.setCursor(2,3);
          lcd.print("Saved to eeprom");
          for(int i = 0;i<4;i++)
          {
            EEPROM.update(i,change[i]);
            delay(30);
          }
          delay(1000);
          //remain
          return;
        }
    }

  }
}

void setting()
{

}
void dinogame()
{
        byte dino[8] = {
      B01111,
        B01101,
        B01111,
        B01100,
        B11101,
          B11111,
        B10010,
        B10010
      };
      byte cactus[8]={
      B00100,
        B00100,
        B00100,
        B10101,
        B01110,
        B01110,
        B00100,
        B01110
      };
lcd.createChar(0, dino);
lcd.createChar(1, cactus);
lcd.createChar(2,cactus);
lcd.createChar(3,cactus);
dlask://dlask................................
// lcd.createChar(4,cloud_1);
// lcd.createChar(5,cloud_2);
short int first=0,second=0,third=0,/*fourth=0*/position[4]={0},airtime=0,score=0,low=3,tim=500;
    char key='0';
  while(1)
  {
    
    if(20+position[0]-first<=0)
    {
      position[0]= generaterandomnumber(1,16);
      first=0;
    }
    if(20+position[1]-second<=0)
    {
      position[1]= generaterandomnumber(1,16);
      second=0;
    }
    if(20+position[2]-third<=0)
    {
      position[2]= generaterandomnumber(1,16);
      third=0;
    }
    
    lcd.clear();
    key=keypad(row,col,symbol_1,false,false);
    if(key=='1')
    {
      if(++airtime>=5){lcd.setCursor(4,3);low=3;}
      else{lcd.setCursor(4,1);low=1;}
      lcd.write(byte(0));
    }
    else
    {
      lcd.setCursor(4, 3);
      lcd.write(byte(0));
      airtime=0;
      low = 3;
    }
    if(key=='D')
    {
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("Do you want to exit");
      lcd.setCursor(1,3);
      lcd.print("YES");
      lcd.setCursor(16,3);
      lcd.print("NO");
      while(1)
      {
        key=keypad(row,col,symbol_1,true,true);
        if(key=='3')break;
        else if(key=='1')return;
      }
    }
    lcd.setCursor(20+position[0]-first,3);
    lcd.write(byte(1));
    lcd.setCursor(20+position[1]-second,3);
    lcd.write(byte(2));
    lcd.setCursor(20+position[2]-third,3);
    lcd.write(byte(3));
    // lcd.setCursor(21+position[3]-fourth,1);
    // lcd.write(byte(4));
    // lcd.setCursor(21+position[3]-fourth+1,1);
    // lcd.write(byte(5));
    lcd.setCursor(0,0);//removing glitch
    lcd.print(" ");
    lcd.setCursor(14,0);
    lcd.print("S:");
    lcd.setCursor(16,0);
    lcd.print(score);
    first++;
    second++;
    third++;
    // fourth++;
    key = '0';
    score++;
    //collision case 

    if((low==3)&&(20+position[0]-first==2||20+position[1]-second==2||20+position[2]-third==2))
    {
          lcd.clear();
        lcd.setCursor(8,1);
        if(score >= 100 && score <= 0)
        {lcd.print("GOOD");
        lcd.setCursor(5,2);
        lcd.print("SCORE:" + String(score));
        }
        else if(score <= 500 && score >= 100)
        {lcd.print("BETTER");
        lcd.setCursor(5,2);
        lcd.print("SCORE:" + String(score));
        }
        else if(score <= 1000 && score >= 500)
        {lcd.print("BEST");
        lcd.setCursor(5,2);
        lcd.print("SCORE:" + String(score));
        }
        else if(score>1000)
        {
          lcd.print("You are legend");
          lcd.setCursor(5, 2);
          lcd.print("SCORE:"+String(score));
        }
        lcd.setCursor(1,3);
        lcd.print("RESTART");
        tone(buzzer,4500,2000);
        delay(2000);
        noTone(buzzer);
        lcd.setCursor(15,3);
        lcd.print("END");
        while(1)
        {
          key = keypad(row,col,symbol_1,true,true);
          if(key=='1')goto dlask;
          else if(key=='3')
          {
            return;
          }
        } 
    }
    if(score%100==0)
    {
      tim = tim-50*(score/100);
    }
    delay(tim);
  }
}
void torch(int tor)
{
  char key='1';
  pinMode(tor,OUTPUT);
  while(1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ON");
    lcd.setCursor(0,1);
    lcd.print("OFF");
    lcd.setCursor(15,3);
    lcd.print("EXIT");
    if(key=='1')
    {
      digitalWrite(tor,HIGH);
      lcd.setCursor(18,0);
      lcd.print("<<");
      delay(500);
    }
    else if(key=='2')
    {
      digitalWrite(tor,LOW);
      lcd.setCursor(18, 1);
      lcd.print("<<");
    }
    else if(key=='3')
    {
      return;
    }
    key = keypad(row,col,symbol_1,true,true);
  }
}

void timer()
{
  lcd.clear();
  lcd.setCursor(7,1);
  lcd.print("TIMER");
  delay(2000);
  flask://!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!>>>>>>>>>>>>>>>>
  lcd.clear();
  lcd.setCursor(6,2);
  lcd.print("00:00:00");
  lcd.setCursor(13, 3);
  lcd.print("-");
  short int val[6] = {0},i=0;
  bool pos = false;
  //decision
  while(1)
  {
    char value = keypad(row,col,symbol_1,true,true);
    if(value>='0' && value <= '9')
    {
      val[i] = value-'0';
      if(val[1]>=6){val[1]=5;}
        else if(val[3]>=6){val[3]=5;}
    }
    else if(value == 'D')
    {
      i++;
      if(i>=6)i=0;
    }
    else if(value == 'M')
    {
      lcd.clear();
      char key='2';
          while(1)
          {
            lcd.clear();
            lcd.setCursor(1,1);
            lcd.print("START");
            lcd.setCursor(16,1);
            lcd.print("END");
            key = keypad(row,col,symbol_1,true,true);
            if(key=='1')
            {
              pos = true;
              break;
            }
            else if(key=='3')
            {

              return;
            }
          }
      
    }
    if(pos==1)break;
    lcd.clear();
    if(i<2)lcd.setCursor(13-i,3);
    else if(i>=2&&i<4)lcd.setCursor(12-i,3);
    else if(i>=4&&i<6)lcd.setCursor(11-i,3);
    lcd.print("-");
    
    //for printing
    for(int j=0,k=5;j<8;j++)
    {
    lcd.setCursor(6+j, 2);
      if((j+1)%3==0)lcd.print(":");
      else
      {
        lcd.print(val[k]);k--;
      }
    }
  }
  //starts
  while(1)
  {
    char value;
    //for checking !!!...
    val[0] = (val[0]==0?(val[1]==0&&val[2]==0&&val[3]==0&&val[4]==0&&val[5]==0?0:9):val[0]-1);
    val[1] = (val[0]==9?(val[1]==0?(val[2]==0&&val[3]==0&&val[4]==0&&val[5]==0?0:5):val[1]-1):val[1]);
    val[2] = (val[1]==5 && val[0]==9?(val[2]==0?(val[3]==0&&val[4]==0&&val[5]==0?0:9):val[2]-1):val[2]);
    val[3] = (val[2]==9 && val[1]==5 && val[0]==9?(val[3]==0?(val[4]==0&&val[5]==0?0:5):val[3]-1):val[3]);
    val[4] = (val[3]==5 && val[2]==9 && val[1]==5 && val[0]==9?(val[4]==0?(val[5]==0?0:9):val[4]-1):val[4]);
    val[5] = (val[4]==9 && val[3]==5 && val[2]==9 && val[1]==5 && val[0]==9?(val[5]==0?0:val[5]-1):val[5]);
    if(val[5]==0&&val[4]==0&&val[3]==0&&val[2]==0&&val[1]==0&&val[0]==0)
    {
      lcd.clear();
      lcd.setCursor(6,1);
      lcd.print("TIME UP");
      lcd.setCursor(1,3);
      lcd.print("START");
      lcd.setCursor(16,3);
      lcd.print("END");
      while(1)
      {
        char key = keypad(row,col,symbol_1,false,false);
        if(key=='3'){
          noTone(buzzer);
          return;}
        else if(key=='1')goto flask;
        tone(buzzer,4500,500);
        delay(200);
        noTone(buzzer);
        tone(buzzer,4500,500);
        delay(200);
        noTone(buzzer);
      }
      return;
    }
    for(int i=0;i<13;i++)
      {value = keypad(row,col,symbol_1,false,true);//for checking
      if(value == 'D')
      {
        lcd.setCursor(19,3);
        lcd.print("S");
        anypin(true);
        lcd.print(" ");
      }
      if(value == 'M')
      {
        lcd.clear();
        lcd.setCursor(7, 1);
        lcd.print("EXIT");
        delay(3000);
        return;
      }
    }
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("START:");
    
    for(int j=0,k=5;j<8;j++)
    {
    lcd.setCursor(6+j, 2);
      if((j+1)%3==0)lcd.print(":");
      else
      {
        lcd.print(val[k]);k--;
      }
    }
  }

}

void stopwatch()
{
   lcd.clear();
  lcd.setCursor(4,1);
  lcd.print("STOP WATCH");
  delay(2000);
  lcd.clear();
  lcd.setCursor(6,2);
  lcd.print("00:00:00");
  short int val[6] = {0};
  char yek = keypad(row,col,symbol_1,true,true);
  if(yek=='D')return;
  while(1)
  {
    char key;
    val[0] = (val[0]==9?0:val[0]+1);
    val[1] = (val[0]==0?(val[1]==5?0:val[1]+1):val[1]);
    val[2] = (val[1]==0&&val[0]==0?(val[2]==9?0:val[2]+1):val[2]);
    val[3] = (val[2]==0&&val[1]==0&&val[0]==0?(val[3]==5?0:val[3]+1):val[3]);
    val[4] = (val[3]==0&&val[2]==0&&val[1]==0&&val[0]==0?(val[4]==9?0:val[4]+1):val[4]);
    val[5] = (val[4]==0&&val[3]==0&&val[2]==0&&val[1]==0&&val[0]==0?val[5]+1:val[5]);
    if(val[5]==10)return;//for returning
    for(int i =0;i<13;i++)
    {
      key = keypad(row,col,symbol_1,false,true);
      if(key == 'M')
    {
      lcd.setCursor(19,3);
      lcd.print("S");
      anypin(true);
      lcd.print(" ");
    }
    if(key == 'D')
    {
      lcd.clear();
      lcd.setCursor(7,0);
      lcd.print("EXIT");
      delay(3000);
      return;
    }
    }

    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("START:");
    
    for(int j=0,k=5;j<8;j++)
    {
    lcd.setCursor(6+j, 2);
      if((j+1)%3==0)lcd.print(":");
      else
      {
        lcd.print(val[k]);k--;
      }
    }

  }
  
}
void select(short int detect)
{
  switch (detect){
    case 0:
    dht();
    break;
    case 1:
    calculator();
    iterator++;
    break;
    case 2:
    torch(tor);
    break;
    case 3:
    Notepad();
    break;
    case 4:
    timer();
    break;
    case 5:
    stopwatch();
    break;
    case 6:
    Remote(remote);
    break;
    case 7:
    changePassword();
    break;
    case 8:
    setting();
    break;
    case 9:
    dinogame();
    break;
    case 10:
    break;
    case 11:
    voltmeter('V');
    break;
    case 12:
    voltmeter('A');
    break;
    default:
    break;
  }
}

void number()
{
    if(handle)
  {
      int r = iterator/4;
      lcd.clear();
      for(int  i = 0;i < 4;i++)
      {
        lcd.setCursor(0, i);
        lcd.print(templates[4*r+i]);
      }
      lcd.setCursor(18, iterator-4*r);
      lcd.print("<<");
      handle = false;
  }
    char value = keypad(row,col,symbol_1,true,true);
    if(value == '2')
    {
      iterator++;handle=true;
      if(iterator >= 12)iterator = 11;
    }
    else if(value == '5')
    {
      iterator--;handle = true;
      if(iterator < 0)iterator = 0; 
    }
    else if(value == '1')
    {
    
      select(iterator);
      handle = true;
      value = 0;
    }
    value = 0;
  
}
void loop() {
  // put your main code here, to run repeatedly:
number();
}
void calculator()
{
  made_state = false;
  kade_state = false;
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("CALCULATOR");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,3);
  lcd.print((radian_state==true?"Rad":"Deg"));
  lcd.setCursor(0,0);
  lcd.blink();
  char symbol_2[4][4] = {
  {'/','^','!','S'},
  {'C','T',')','('},//------------------------------------
  {'Y','L','l','R'},
  {'D','K','M','.'}
  };
  char kecaKeys[4][4] = {
  {'A','s','o','t'},
  {'c','p','e','P'},
  {'>','X','n','y'},
  {'D','K','M','r'}
};
  Keypad *customKeypad = new Keypad( makeKeymap(symbol_1), rowPins, colPins, ROWS, COLS); 
Keypad *customKeypad_2 = new Keypad(makeKeymap(symbol_2),rowPins,colPins,ROWS,COLS);
Keypad *customKeypad_3 = new Keypad(makeKeymap(kecaKeys),rowPins,colPins,ROWS,COLS);

  while(1) {
  // put your main code here, to run repeatedly:
char key = (made_state == false && kade_state == false?customKeypad->getKey():(made_state == true && kade_state == false?customKeypad_2->getKey():(made_state == false && kade_state == true?customKeypad_3->getKey():customKeypad_3->getKey())));
  
  if (key != '\0') {
    handleKeyPress(key);
    if(end){
        delete customKeypad;
  delete customKeypad_2;
  delete customKeypad_3;

  customKeypad = nullptr;
  customKeypad_2 = nullptr;
  customKeypad_3 = nullptr;

  for (int i = 0; i < 4; i++) {
  pinMode(row[i], OUTPUT);
  digitalWrite(row[i], HIGH); // default state for rows
}

for (int j = 0; j < 4; j++) {
  pinMode(col[j], INPUT_PULLUP); // columns should be pulled up
}
  break;}
  }
  // delete customKeypad;
  // delete customKeypad_2;
  // delete customKeypad_3;
  // delete customKeypad_4;
  // customKeypad = NULL;
  // customKeypad_2 = NULL;
  // customKeypad_3 = NULL;
  // customKeypad_4 = NULL;
}
expression.clear();
  end = false;
  
}
void handleKeyPress(char key) {
  lcd.noBlink();
  if (resultDisplayed && key != 'D') {//---------------------
    lcd.clear();
    expression.clear();
    resultDisplayed = false;
    lcd.setCursor(0,3);
    lcd.print((radian_state==true?"Rad":"Deg"));
  }

  if (key == 'D') {//--------------------------------
    lcd.clear();
    if(made_state == true)
    {
      lcd.setCursor(16,3);
      lcd.print("Mode");
    }
    if(kade_state == true)
    {
      lcd.setCursor(12, 3);
    lcd.print("key");
    }
    lcd.setCursor(0,3);
    lcd.print((radian_state==true?"Rad":"Deg"));
    if(resultDisplayed==true)resultDisplayed = false;
    if(expression.size()>40)
    {
      full_state = true;
    }
    if (full_state == true)
    {
        expression.pop();
         lcd.setCursor(0, 0);
    lcd.print(expression.return_at_index(0,20));
    lcd.setCursor(0,1);
    lcd.print(expression.return_at_index(20,40));
    lcd.setCursor(0,2);
    if (expression.size()>40)
      {//---------------------------------------------------------
        lcd.print(expression.return_at_index(40,expression.size()));}
    lcd.blink();
    full_state = false;
    }
    else
    {  expression.pop();
      lcd.setCursor(0,0);
      lcd.print(expression.all_return());
      lcd.blink();}
    return;
  }

  if(key == 'X')
  {
    end = true;
    return;
  }

  if (key == '=') {
    
    double result = evaluateExpression(expression.all_return());
    lcd.clear();
    if(error_occured)
    {
      lcd.setCursor(0,0);
      lcd.print("Error NAN");
      error_occured = false;
      return;
    }
    lcd.print("Result:");
    lcd.setCursor(0, 1);
    lcd.print(result, 4);
    resultDisplayed = true;
    return;
  }

  if(key == 'Y')
  {
    lcd.clear();
    if(made_state == true)
    {
      lcd.setCursor(16,3);
      lcd.print("Mode");
    }
    lcd.setCursor(0,3);
    lcd.print((radian_state==true?"Rad":"Deg"));
    expression.clear();
    lcd.setCursor(0,0);
    lcd.blink();
    return;//----------------------------------
  }

  if(key == 'R')
  {
    radian_state = !radian_state;
    lcd.setCursor(0,3);
    lcd.print((radian_state==true?"Rad":"Deg"));
    return;
  }  

  if (key == 'M')
  {//-----------------------------------------
    lcd.setCursor(0,3);
    lcd.print((radian_state==true?"Rad":"Deg"));
    made_state = !made_state;
    lcd.setCursor(16,3);
    lcd.print((made_state == true?"Mode":"    "));
    lcd.setCursor(12, 3);
    lcd.print((kade_state == true?"Key":"   "));
    return;
  }

  if(key == 'K')
  {
    lcd.setCursor(0,3);
    lcd.print((radian_state==true?"Rad":"Deg"));
    lcd.setCursor(16,3);
    lcd.print((made_state == true?"Mode":"    "));
    kade_state = !kade_state;
    lcd.setCursor(12, 3);
    lcd.print((kade_state == true?"Key":"   "));
    return;
  }
  if (expression.size() <= 19*2) {
    lcd.clear();
    lcd.setCursor(0,3);
    lcd.print((radian_state==true?"Rad":"Deg"));
    if(made_state == true)
    {
      lcd.setCursor(16,3);
      lcd.print("Mode");
    }
    if(kade_state == true)
    {
      lcd.setCursor(12, 3);
    lcd.print("key");
    }
    expression.push(key);
    lcd.setCursor(0, 0);
    lcd.print(expression.all_return());
    lcd.blink();
    
  } 

  else if(expression.size() >= 19*2+1 && expression.size()<=19*3)
  {
    lcd.clear();
    if(made_state == true)
    {
      lcd.setCursor(16,3);
      lcd.print("Mode");
    }
    if(kade_state == true)
    {
      lcd.setCursor(12, 3);
    lcd.print("key");
    }
    lcd.setCursor(0,3);
    lcd.print((radian_state==true?"Rad":"Deg"));
    expression.push(key);
    lcd.setCursor(0, 0);
    lcd.print(expression.return_at_index(0,20));
    lcd.setCursor(0,1);
    lcd.print(expression.return_at_index(20,40));
    lcd.setCursor(0,2);
    if (expression.size()>40)
      {
        lcd.print(expression.return_at_index(40,expression.size()));}
    lcd.blink();
    
  } 

  else
  {
    lcd.setCursor(0,3);
    lcd.print((radian_state==true?"Rad":"Deg"));
    lcd.setCursor(3,3);
    lcd.print("MAX LENGTH");
  }
  
}

double evaluateExpression(String exp) {
  exp = remove_constant(exp);//----------------------
  exp = removeBrackets(exp);
  exp = remove_extra_bracket(exp);
  if(error_occured == true){//--------------------------
    return 0.0;
  }
  char ops[16] = {'p','c','s','o','t','S','C','T','L','l','^','!','/', '*', '-', '+'};//-------------------------
  double nums[30] = {0};
  char symbols[30];
  

  int numCount = 0, symCount = 0;
  String num = "";

  // Parse the expression
  for (int i = 0; i < exp.length(); i++) {
    char ch = exp[i];
    if (isDigit(ch) || ch == '.') {//-------------
      num += ch;
    } else {
      nums[numCount++] = num.toDouble();
      num = "";
      symbols[symCount++] = ch;
    }
  }
  // Push the last number if any
  if (num.length() > 0) {
    nums[numCount++] = num.toDouble();
  }

  // Perform operations in order of precedence: /, *, +, -
  for (int opIndex = 0; opIndex < 16; opIndex++) {
    int i = 0;
    while (i < symCount) {
      if (symbols[i] == ops[opIndex]) {
        double result = 0;
        switch (ops[opIndex]) {
          case '/': result = nums[i] / nums[i + 1]; break;
          case '*': result = nums[i] * nums[i + 1]; break;
          case '+': result = nums[i] + nums[i + 1]; break;
          case '-': result = nums[i] - nums[i + 1]; break;
          case '!': result = factorial(nums[i]) + nums[i+1];break;
          case '^': result = pow(nums[i],nums[i+1]);break;//--------------------
          case 'S': result = (nums[i]==0?sin((radian_state==true?nums[i+1]:radians(nums[i+1]))):nums[i]*sin((radian_state==true?nums[i+1]:radians(nums[i+1]))));break;
          // if(nums[i]==0)result = sin(nums[i+1]);
          // else result = nums[i]*sin(nums[i+1]);
          case 'C': result = (nums[i]==0?cos((radian_state==true?nums[i+1]:radians(nums[i+1]))):nums[i]*cos((radian_state==true?nums[i+1]:radians(nums[i+1]))));break;
          case 'T': result = (nums[i]==0?tan((radian_state==true?nums[i+1]:radians(nums[i+1]))):nums[i]*tan((radian_state==true?nums[i+1]:radians(nums[i+1]))));break;
          case 'l': result = (nums[i]==0?log(nums[i+1]):nums[i]*log(nums[i+1]));break;
          case 'L': result = (nums[i]==0?log10(nums[i+1]):nums[i]*log10(nums[i+1]));break;
          case 'p': result = permutation(nums[i],nums[i+1]);break;
          case 'c': result = combinations(nums[i],nums[i+1]);break;
          case 's': result = (nums[i]==0?asin((radian_state==true?nums[i+1]:radians(nums[i+1]))):nums[i]*asin((radian_state==true?nums[i+1]:radians(nums[i+1]))));break;
          case 'o': result = (nums[i]==0?acos((radian_state==true?nums[i+1]:radians(nums[i+1]))):nums[i]*acos((radian_state==true?nums[i+1]:radians(nums[i+1]))));break;
          case 't': result = (nums[i]==0?atan((radian_state==true?nums[i+1]:radians(nums[i+1]))):nums[i]*atan((radian_state==true?nums[i+1]:radians(nums[i+1]))));break;
        }

        // Replace nums[i] with result and shift others
        nums[i] = result;
        for (int j = i + 1; j < numCount - 1; j++) {
          nums[j] = nums[j + 1];
        }
        numCount--;

        // Remove the used operator
        for (int j = i; j < symCount - 1; j++) {
          symbols[j] = symbols[j + 1];
        }
        symCount--;
        // Stay at same index to recheck current position
      } else {
        i++;
      }
    }
  }
  if(isnan(nums[0]))error_occured = true;//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
  answer = nums[0];//----------------------
  return nums[0];
}

double factorial(double number)
{
  double result = 1;
  if(number <= 0)return 1.0;
  for(int i = 1;i<=number;i++)
  {
      result *= i;
  }
  return result;
}



String removeBrackets(String exp) {
  while (exp.indexOf('(') != -1) {
    int close = exp.indexOf(')');
    int open = close;
    while (open >= 0 && exp[open] != '(') open--;
    if (open < 0) break; // Invalid bracket

    String inner = exp.substring(open + 1, close);
    double innerResult = evaluateExpression(inner); // Recursive call
    
    bool leftIsDigit = (open > 0) && isDigit(exp[open - 1]);
    bool rightIsDigit = (close + 1 < exp.length()) && isDigit(exp[close + 1]);
    bool IsTrig = (open > 0) && isSinCosTanLog(exp[open - 1]);//-------------------------
    if(IsTrig && innerResult<0)
    {
      //----------
      double result = 0;
      switch(exp[open - 1]){
        case 'S':result = sin(radian_state==true?innerResult:radians(innerResult));break;
        case 'C':result = cos(radian_state==true?innerResult:radians(innerResult));break;
        case 'T':result = tan(radian_state==true?innerResult:radians(innerResult));break;
        case 'L':result = log10(innerResult);break;
        case 'l':result = log(innerResult);break;
        case 's':result = asin(radian_state==true?innerResult:radians(innerResult));break;
        case 'o':result = acos(radian_state==true?innerResult:radians(innerResult));break;
        case 't':result = atan(radian_state==true?innerResult:radians(innerResult));break;
      }
      if(isnan(result))
      {
        error_occured = true;//eeeeeeeeeeeeeeeeeeeeeeeeeeeeee
        return exp;
      }
      open = open-1;
      leftIsDigit = (open > 0) && isDigit(exp[open - 1]);
      rightIsDigit = (close + 1 < exp.length()) && isDigit(exp[close + 1]);
      innerResult = result;
      
    }//------------------------------------------------
if (leftIsDigit && rightIsDigit) {
  exp = exp.substring(0, open) + "*" + String(innerResult) + "*" + exp.substring(close + 1);
} else if (leftIsDigit) {
  exp = exp.substring(0, open) + "*" + String(innerResult) + exp.substring(close + 1);
} else if (rightIsDigit) {
  exp = exp.substring(0, open) + String(innerResult) + "*" + exp.substring(close + 1);
} else {
  exp = exp.substring(0, open) + String(innerResult) + exp.substring(close + 1);
}
  }
  
  return exp;}
 
String remove_extra_bracket(String exp) {
  char brackets[3] = {'[','{','('};
  char ends[3] = {']','}',')'};
  for(int a = 0;a < 3;a++)
  {  
    for (int i = 1; i < exp.length(); i++) {
      if (isDigit(exp[i - 1]) && exp[i] == brackets[a]) {
        exp = exp.substring(0, i) + "*" + exp.substring(i+1);
        i++; // Skip the '*' we just inserted
      } else if (exp[i - 1] == ends[a] && isDigit(exp[i])) {
        exp = exp.substring(0, i-1) + "*" + exp.substring(i);
        i++; // Skip the '*' we just inserted
      }
    }
  }
    return exp;
}

bool isSinCosTanLog(char ch)
{
  if(ch == 'S'||ch == 'C'||ch == 'T'||ch == 'L'||ch == 'l'||ch == 's'||ch == 'o'||ch == 't')return true;
}


String remove_constant(String exp)
{
  char symbol[4] = {'y','A','P','e'};
  for(int j = 0;j < 4;j++)
  {
      if(exp.indexOf(symbol[j])!=-1)
    {
      double result=0;
      int i = exp.indexOf(symbol[j]);
      bool isleftDigit = (i > 0) && (isDigit(exp[i-1]));
      bool isRightDigit = (i+1 < exp.length()) && (isDigit(exp[i+1]));
      if(symbol[j]=='A')result = answer;
      else if(symbol[j]=='P')result = PI;
      else if(symbol[j]=='e')result = 2.71828;
      else if(symbol[j]=='y')result = random(1000);
      if(isleftDigit && isRightDigit)
      {
        exp = exp.substring(0,i) + "*" + String(result) + "*" + exp.substring(i+1);
      }
      else if(isleftDigit)
      {
        exp = exp.substring(0,i) + "*" + String(result) + exp.substring(i+1);
      }
      else if(isRightDigit)
      {
        exp = exp.substring(0,i) + String(result) + "*" + exp.substring(i+1);
      }
      else
      {
        exp = exp.substring(0,i) + String(result) + exp.substring(i+1);
      }
    }
  }
 
  return exp;
}

double permutation(int n,int r)
{
  double result = (factorial(n))/(factorial(n-r));
  return result;
}
double combinations(int n,int r)
{
  double result = permutation(n,r);
  result = result/factorial(r);
  return result;
}
// int freememory()
// {
//   int gree;
//   if((int)__brkval == 0)
//   {
//     gree = ((int)&gree) - ((int)&__heap_start);
//   }
//   else
//   {
//     gree = ((int)&gree) - ((int)__brkval);
//   }
//   return gree;
// }
void voltmeter(char value)
{
  lcd.setCursor(5,1);
  lcd.print(value == 'V'?"Voltmeter":(value == 'A'?"Ammeter":"Rheostat"));
  if(value == 'V')
  {
    pinMode(A2,INPUT);
  }
  else if(value == 'A')
  {
    pinMode(A2,INPUT);
    pinMode(A3,OUTPUT);
    digitalWrite(A3,HIGH);
  }
  else
  {
    pinMode(A3,INPUT);
    pinMode(A2,INPUT);
  }
  
  delay(3000);

  while(1)
  {
    double volt = analogRead(A2);
    double answer = volt/1.023;
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(value == 'V'?"Voltage = ":(value == 'A'?"Ampere = ":" "));
    lcd.print(answer);
    delay(100);
  }
}
