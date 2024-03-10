#include <IRremote.h>

byte redLed   = 6;
byte greenLed = 10;
byte blueLed  = 9;
byte buzzer = 12;
byte buzzer2 = 7;
byte touchPin = 2;
byte joyX = A4;
byte joyY = A5;
//byte knob = A0;
byte joy2X = A0;
byte joy2Y = A1;
byte button = 13;
byte button2 = 8;
byte irPin = 4;

int freq;
int vrX;
int vrY;
int k;
float RGB[3];

byte mode = 1;
byte voiceNum = 1;
byte type = 1;
int key = 5;
byte style = 1;
byte acmpStyle = 1;
byte songNum = 1;
byte tempo = 120;
int shift = 0;
byte chordType = 1;
int drumsON = 1;
int intro = 1;
int ending = 1;
byte color = 1;
byte bright = 1;

float transpose[] = {0.75,0.79,0.84,0.89,0.94,1.0,1.06,1.12,1.19,1.26,1.33,1.41};
byte scaleSize[] = {7,5,12,9};
int major[] = {262,294,330,349,392,440,494};
int china[] = {262,294,330,392,440};
int semi[] = {262,277,294,311,330,349,370,392,415,440,466,494};
byte R[] = {255,255,255,0,0,0,255,255,255,255,128,128};
byte G[] = {0,85,170,225,225,0,0,0,85,170,225,225};
byte B[] = {0,0,0,0,255,255,255,85,85,85,85,225};

IRrecv irrecv(irPin);
decode_results results;

void setup() {
  pinMode(redLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
  pinMode(blueLed,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(buzzer2,OUTPUT);
  pinMode(touchPin,INPUT);
  digitalWrite(buzzer2,HIGH);
  pinMode(button,INPUT);
  digitalWrite(button,HIGH);
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  for(float x=0;x<PI;x=x+0.00001){
    if(digitalRead(button)==LOW){
      tone(buzzer,523,50);
      mode++;
      if(mode > 5){
        mode = 1;
        tone(buzzer,262,50);
      }
      delay(200);
    }
    vrX = analogRead(joyX);
    vrY = analogRead(joyY);

    // mode switch
    switch(mode){
    case 1:
    if(irrecv.decode(&results)){
      if(voiceNum != 5){
        voiceRemote(results.value);
      }
      else{
        drumRemote(results.value);
      }
      Serial.println(results.value,HEX);
      irrecv.resume();
    }
    if(vrX<256){  // shake down
      key--;
      if(key < 0){
        key = 0;
      }
      timingVoice(523,voiceNum,50);
      delay(200);
    }
    if(vrX>768){  // shake up
      key++;
      if(key > 11){
        key = 11;
      }
      timingVoice(523,voiceNum,50);
      delay(200);
    }
    if(vrY<256){  // shake left
      type++;
      tone(buzzer,1046,50);
      if(type > 3){
        type = 1;
        tone(buzzer,523,50);
      }
      delay(200);
    }
    if(vrY>768){  // shake right
      voiceNum++;
      if(voiceNum > 5){
        voiceNum = 1;
      }
      if(voiceNum == 5){
        drum(1);
      }
      else{
        timingVoice(523,voiceNum,50);
      }
      delay(200);
    }
    if(voiceNum == 5){
      light(4);
      if(digitalRead(touchPin) == HIGH){
        handDrum();
        while(digitalRead(touchPin) == HIGH){
        }
        digitalWrite(buzzer2,HIGH);
      }
    }
    else{
    freq = scale(type);
    light(type);
    while(digitalRead(touchPin) == HIGH){
      voice(freq,voiceNum);
      vrX = analogRead(joyX);
      vrY = analogRead(joyY);
      if(vrY<256){  // shake left
      shift = -1;
      }
      else if(vrY>768){  // shake right
      shift = 1;
      }
      else if(vrX<256){  // shake dowm
      shift = -2;
      }
      else if(vrX>768){  // shake up
      shift = 2;
      }
      else{
      shift = 0;
      }
    }
    noTone(buzzer);
    }
    break;
    
    case 2:
    if(irrecv.decode(&results)){
      rhythmRemote(results.value);
      Serial.println(results.value,HEX);
      irrecv.resume();
    }
    if(vrX<256){  // shake down
      tone(buzzer,523,50);
      tempo += -10;
      if(tempo < 40){
        tempo = 40;
        tone(buzzer,262,100);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(200);
    }
    if(vrX>768){  // shake up
      tone(buzzer,1046,50);
      tempo += 10;
      if(tempo > 240){
        tempo = 240;
        tone(buzzer,262,100);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(200);
    }
    if(vrY<256){  // shake left
      tone(buzzer,523,50);
      style--;
      if(style < 1){
        style = 15;
        tone(buzzer,1046,50);
      }
      delay(200);
    }
    if(vrY>768){  // shake right
      tone(buzzer,1046,50);
      style++;
      if(style > 15){
        style = 1;
        tone(buzzer,523,50);
      }
      delay(200);
    }
    if(digitalRead(touchPin) == HIGH){
      delay(200);
      rhythm(style);
    }
    break;
    
    case 3:
    if(irrecv.decode(&results)){
      acmpRemote(results.value);
      Serial.println(results.value,HEX);
      irrecv.resume();
    }
    if(voiceNum == 5){
      voiceNum = 1;
    }
    if(vrX<256){  // shake down
      tone(buzzer,523,50);
      tempo += -10;
      if(tempo < 40){
        tempo = 40;
        tone(buzzer,262,100);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(200);
    }
    if(vrX>768){  // shake up
      tone(buzzer,1046,50);
      tempo += 10;
      if(tempo > 240){
        tempo = 240;
        tone(buzzer,262,100);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(200);
    }
    if(vrY<256){  // shake left
      tone(buzzer,1046,50);
      drumsON++;
      if(drumsON > 1){
        drumsON = 0;
        tone(buzzer,523,50);
      }
      delay(200);
    }
    if(vrY>768){  // shake right
      tone(buzzer,1046,50);
      acmpStyle++;
      if(acmpStyle > 6){
        acmpStyle = 1;
        tone(buzzer,523,50);
      }
      delay(200);
    }
    if(digitalRead(touchPin) == HIGH){
      delay(200);
      acmp(acmpStyle);
    }
    break;
    
    case 4:
    if(irrecv.decode(&results)){
      songRemote(results.value);
      Serial.println(results.value,HEX);
      irrecv.resume();
    }
    if(voiceNum == 5){
      voiceNum = 1;
    }
    if(vrX<256){  // shake down
      tone(buzzer,523,50);
      tempo += -10;
      if(tempo < 40){
        tempo = 40;
        tone(buzzer,262,100);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(200);
    }
    if(vrX>768){  // shake up
      tone(buzzer,1046,50);
      tempo += 10;
      if(tempo > 240){
        tempo = 240;
        tone(buzzer,262,100);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(200);
    }
    if(vrY<256){  // shake left
      tone(buzzer,1046,50);
      songNum++;
      if(songNum > 3){
        songNum = 1;
        tone(buzzer,523,50);
      }
      delay(200);
    }
    if(vrY>768){  // shake right
      voiceNum++;
      if(voiceNum > 4){
        voiceNum = 1;
      }
      timingVoice(523,voiceNum,50);
      delay(200);
    }
    if(digitalRead(touchPin) == HIGH){
      music(songNum);
    }
    break;
    
    case 5:
    if(irrecv.decode(&results)){
      lightRemote(results.value);
      Serial.println(results.value,HEX);
      irrecv.resume();
    }
    if(vrX<256){  // shake down the joystick to decrease brightness
      tone(buzzer,523,50);
      bright++;
      if(bright > 4){
        bright = 1;
        tone(buzzer,1046,50);
      }
      delay(200);
    }
    if(vrX>768){  // shake up the joystick to increase brightness
      tone(buzzer,1046,50);
      bright--;
      if(bright < 1){
        bright = 4;
        tone(buzzer,523,50);
      }
      delay(200);
    }
    if(vrY<256){  // shake left the joystick to select the previous color
      tone(buzzer,523,50);
      color--;
      if(color < 1){
        color = 9;
        tone(buzzer,1046,50);
      }
      delay(200);
    }
    if(vrY>768){  // shake right the joystick to select the next color
      tone(buzzer,1046,50);
      color++;
      if(color > 9){
        color = 1;
        tone(buzzer,523,50);
      }
      delay(200);
    }
    if(digitalRead(touchPin) == HIGH){
      color = random(1,10);
      tone(buzzer,523,50);
      delay(200);
    }
    break;
    }
    
    // color switch
    switch(color){
    case 1:  // cyclic discoloration
      RGB[0]=255*abs(sin(x*(180/PI)));
      RGB[1]=255*abs(sin((x+PI/3)*(180/PI)));
      RGB[2]=255*abs(sin((x+(2*PI)/3)*(180/PI)));
      break;
    case 2:  // white
      RGB[0]=255;
      RGB[1]=255;
      RGB[2]=255;
      break;
    case 3:  // red
      RGB[0]=255;
      RGB[1]=0;
      RGB[2]=0;
      break;
    case 4:  // orange
      RGB[0]=255;
      RGB[1]=85;
      RGB[2]=0;
      break;
    case 5:  // yellow
      RGB[0]=255;
      RGB[1]=170;
      RGB[2]=0;
      break;
    case 6:  // green
      RGB[0]=0;
      RGB[1]=225;
      RGB[2]=0;
      break;
    case 7:  // sky blue
      RGB[0]=0;
      RGB[1]=255;
      RGB[2]=255;
      break;
    case 8:  // dark blue
      RGB[0]=0;
      RGB[1]=0;
      RGB[2]=255;
      break;
    case 9:  // purple
      RGB[0]=255;
      RGB[1]=0;
      RGB[2]=255;
      break;
    }

    // brightness switch
    switch(bright){
    case 1:  // high
      break;
    case 2:  // medium
      RGB[0]=RGB[0]*(2.0/3.0);
      RGB[1]=RGB[1]*(2.0/3.0);
      RGB[2]=RGB[2]*(2.0/3.0);
      break;
    case 3:  // low
      RGB[0]=RGB[0]*(1.0/3.0);
      RGB[1]=RGB[1]*(1.0/3.0);
      RGB[2]=RGB[2]*(1.0/3.0);
      break;
    case 4:  // breathing
      RGB[0]=RGB[0]*abs(sin(x*(180/PI)));
      RGB[1]=RGB[1]*abs(sin(x*(180/PI)));
      RGB[2]=RGB[2]*abs(sin(x*(180/PI)));
      break;
    }
    if(mode != 1){
      analogWrite(redLed,RGB[0]);
      analogWrite(greenLed,RGB[1]);
      analogWrite(blueLed,RGB[2]);
    }
  }
}

void voice(int freq,byte voiceNum){
  freq = freq*transpose[key]*pow(1.06,shift);
  switch(voiceNum){
    default:
    tone(buzzer,freq);
    delay(30);
    break;
    case 2:
    tone(buzzer,freq);
    delay(20);
    tone(buzzer,freq*2);
    delay(10);
    break;
    case 3:
    tone(buzzer,freq);
    delay(20);
    tone(buzzer,freq*3);
    delay(10);
    break;
    case 4:
    tone(buzzer,freq);
    delay(10);
    tone(buzzer,freq*2);
    delay(10);
    tone(buzzer,freq*3);
    delay(10);
    break;
  }
}

void timingVoice(int freq,byte voiceNum,int timing){
  if(freq > 0){
  freq = freq*transpose[key]*pow(1.06,shift);
  switch(voiceNum){
    default:
    tone(buzzer,freq,timing);
    delay(timing);
    break;
    case 2:
    while(timing>0){
    tone(buzzer,freq);
    delay(20);
    tone(buzzer,freq*2);
    delay(10);
    timing -= 30;
    }
    noTone(buzzer);
    break;
    case 3:
    while(timing>0){
    tone(buzzer,freq);
    delay(20);
    tone(buzzer,freq*3);
    delay(10);
    timing -= 30;
    }
    noTone(buzzer);
    break;
    case 4:
    while(timing>0){
    tone(buzzer,freq);
    delay(10);
    tone(buzzer,freq*2);
    delay(10);
    tone(buzzer,freq*3);
    delay(10);
    timing -= 30;
    }
    noTone(buzzer);
    break;
  }
  }
  else{
    delay(timing);
  }
}

int scale(byte type){
  int m = 0;
  int x = 0;
  int octave = 0;
//  k = analogRead(knob);
//  m = (k-150)/50;
//  x = m%scaleSize[type-1];
//  octave = m/scaleSize[type-1];
  k = analogRead(joy2Y);
  m = (k-180)/(760/(scaleSize[type-1]));
  x = m;
  if(x >= scaleSize[type-1]){
    x = scaleSize[type-1] - 1;
  }
  octave = 1;
  vrX = analogRead(joy2X);
  if(vrX<400){  // shake down
    octave = 0;
  }
  else if(vrX>624){  // shake up
    octave = 2;
  }
  switch(type){
    case 1:
    freq = major[x]*pow(2,octave);
    break;
    case 2:
    freq = china[x]*pow(2,octave);
    break;
    case 3:
    freq = semi[x]*pow(2,octave);
    break;
  }
  return freq;
}

void handDrum(){
  int m = 0;
  int x = 0;
//  k = analogRead(knob);
//  m = (k-150)/50;
//  x = m%9+1;
  k = analogRead(joy2Y);
  m = (k-180)/(760/9);
  x = m+1;
  if(x > 9){
    x = 9;
  }
  drum(x);
  if(x == 4){
    delay(60);
  }
}

void light(byte type){
  int m = 0;
  int x = 0;
//  k = analogRead(knob);
//  m = (k-150)/50;
//  x = m%scaleSize[type-1];
  k = analogRead(joy2Y);
  m = (k-180)/(760/(scaleSize[type-1]));
  x = m;
  if(x >= scaleSize[type-1]){
    x = scaleSize[type-1] - 1;
  }
  RGB[0] = R[x];
  RGB[1] = G[x];
  RGB[2] = B[x];
  analogWrite(redLed,RGB[0]);
  analogWrite(greenLed,RGB[1]);
  analogWrite(blueLed,RGB[2]);
}

void rhythm(byte style){
  const static PROGMEM byte cymbals1[32]={3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4};
  const static PROGMEM byte cymbals2[16]={3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4};
  const static PROGMEM byte fillDrums1[8]={1,2,2,2,2,2,2,2};
  const static PROGMEM byte fillCymbals1[8]={3,3,3,4,5,5,6,7};
  const static PROGMEM byte fillDrums2[12]={1,0,0,2,0,2,1,2,2,5,6,7};
  
  if(style==1){  // 8 Beat
    const static PROGMEM byte drums[16]={1,0,2,1,1,0,2,0,1,0,2,1,1,0,2,1};
    rhythmPlay(drums,cymbals1,fillDrums1,fillCymbals1,16,8);
  }
  
  if(style==2){  // 16 Beat
    const static PROGMEM byte drums[32]={1,0,0,0,2,0,0,1,1,0,0,0,2,0,0,1,1,0,0,0,2,0,0,1,1,0,1,0,2,1,0,0};
    const static PROGMEM byte fillDrums[16]={1,0,0,0,2,0,2,2,1,2,0,2,5,5,6,7};
    const static PROGMEM byte fillCymbals[16]={3,3,3,3,3,3,3,3,4,0,0,0,0,0,0,0};
    rhythmPlay(drums,cymbals1,fillDrums,fillCymbals,32,16);
  }

  if(style==3){  // Slow Rock
    const static PROGMEM byte drums[12]={1,0,0,2,0,1,1,0,0,2,0,0};
    const static PROGMEM byte fillCymbals[12]={3,3,3,3,3,3,4,0,0,0,0,0};
    rhythmPlay(drums,cymbals1,fillDrums2,fillCymbals,12,12);
  }

  if(style==4){  // Rock & Roll
    const static PROGMEM byte drums[16]={1,0,2,2,1,1,2,0,1,0,2,2,1,1,2,1};
    rhythmPlay(drums,cymbals2,fillDrums1,fillCymbals1,16,8);
  }
  
  if(style==5){  // Hard Rock
    const static PROGMEM byte drums[16]={1,0,0,1,2,0,0,1,1,0,0,1,2,0,0,1};
    const static PROGMEM byte cymbals[16]={4,0,3,3,4,0,3,3,4,0,3,3,4,0,3,3};
    const static PROGMEM byte fillDrums[16]={1,0,0,1,2,0,0,1,2,2,5,5,6,6,7,7};
    const static PROGMEM byte fillCymbals[16]={4,0,3,3,4,0,3,3,4,0,0,0,0,0,0,0};
    rhythmPlay(drums,cymbals,fillDrums,fillCymbals,16,16);
  }
  
  if(style==6){  // Swing
    const static PROGMEM byte drums[12]={1,0,0,2,0,0,1,0,0,2,0,0};
    const static PROGMEM byte cymbals[12]={4,0,0,3,0,4,4,0,0,3,0,4};
    const static PROGMEM byte fillCymbals[12]={4,0,0,3,0,4,4,0,0,0,0,0};
    rhythmPlay(drums,cymbals,fillDrums2,fillCymbals,12,12);
  }
  
  if(style==7){  // Bossanova
    const static PROGMEM byte drums[16]={2,0,2,1,1,2,0,1,2,0,0,2,1,0,2,1};
    const static PROGMEM byte cymbals[16]={3,4,3,3,3,4,3,3,3,4,3,3,3,4,3,3};
    const static PROGMEM byte fillDrums[8]={2,6,6,2,1,2,5,7};
    const static PROGMEM byte fillCymbals[8]={3,4,3,3,4,0,0,0};
    rhythmPlay(drums,cymbals,fillDrums,fillCymbals,16,8);
  }
  
  if(style==8){  // Samba
    const static PROGMEM byte drums[16]={1,0,0,1,1,0,0,0,1,0,0,1,1,0,1,0};
    const static PROGMEM byte cymbals[16]={8,0,8,0,9,0,9,8,0,8,0,8,9,0,9,0};
    const static PROGMEM byte fillDrums[16]={1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0};
    const static PROGMEM byte fillCymbals[16]={8,8,0,9,8,8,0,9,8,8,0,9,9,0,9,8};
    rhythmPlay(drums,cymbals,fillDrums,fillCymbals,16,16);
  }
  
  if(style==9){  // Rumba
    const static PROGMEM byte drums[32]={1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0};
    const static PROGMEM byte cymbals[32]={3,0,6,6,6,0,3,0,6,0,6,0,3,0,6,0,6,0,6,6,3,0,6,0,3,0,6,0,7,0,7,0};
    const static PROGMEM byte fillDrums[16]={1,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0};
    const static PROGMEM byte fillCymbals[16]={6,0,0,6,3,0,6,0,6,0,0,5,5,0,7,0};
    rhythmPlay(drums,cymbals,fillDrums,fillCymbals,32,16);
  }

  if(style==10){  // Tango
    const static PROGMEM byte drums[8]={2,0,2,0,2,0,2,2};
    const static PROGMEM byte cymbals[8]={3,0,3,0,3,0,3,4};
    const static PROGMEM byte fillDrums[8]={2,2,0,2,2,0,2,2};
    const static PROGMEM byte fillCymbals[8]={3,3,0,3,3,0,4,0};
    rhythmPlay(drums,cymbals,fillDrums,fillCymbals,8,8);
  }

  if(style==11){  // Beguine
    const static PROGMEM byte drums[16]={1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0};
    const static PROGMEM byte cymbals[16]={5,6,5,6,5,6,6,5,6,5,3,6,3,7,7,7};
    const static PROGMEM byte fillDrums[8]={1,0,0,1,1,0,1,1};
    const static PROGMEM byte fillCymbals[8]={6,5,3,6,6,0,7,7};
    rhythmPlay(drums,cymbals,fillDrums,fillCymbals,16,8);
  }

  if(style==12){  // Reggae
    const static PROGMEM byte drums[12]={1,0,0,0,0,0,2,0,0,0,0,0};
    const static PROGMEM byte cymbals[12]={3,0,0,3,0,3,4,0,0,3,0,0};
    const static PROGMEM byte fillDrums[12]={1,2,0,2,1,2,2,5,5,6,6,7};
    const static PROGMEM byte fillCymbals[12]={3,0,0,3,0,3,4,0,0,0,0,0};
    rhythmPlay(drums,cymbals,fillDrums,fillCymbals,12,12);
  }

  if(style==13){  // Afrobeat
    const static PROGMEM byte drums[16]={1,0,0,2,1,0,2,0,1,0,0,2,1,0,2,0};
    const static PROGMEM byte fillDrums[16]={1,0,0,2,1,0,2,0,1,2,2,2,5,5,6,7};
    const static PROGMEM byte fillCymbals[16]={3,3,3,3,3,3,3,3,4,0,4,0,4,0,4,0};
    rhythmPlay(drums,cymbals2,fillDrums,fillCymbals,16,16);
  }
  
  if(style==14){  // March
    const static PROGMEM byte drums[32]={1,0,2,0,1,0,2,0,1,0,2,0,1,0,2,0,1,0,2,0,1,0,2,0,1,0,2,2,2,0,2,0};
    const static PROGMEM byte cymbals[32]={3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,4,0};
    const static PROGMEM byte fillDrums[16]={1,0,2,2,1,0,1,0,1,2,2,2,2,0,2,2};
    const static PROGMEM byte fillCymbals[16]={4,0,3,0,4,0,4,0,4,0,3,0,3,0,4,0};
    rhythmPlay(drums,cymbals,fillDrums,fillCymbals,32,16);
  }
  
  if(style==15){  // Waltz
    const static PROGMEM byte drums[6]={1,0,2,0,2,0};
    const static PROGMEM byte fillDrums[6]={1,2,2,0,2,2};
    const static PROGMEM byte fillCymbals[6]={3,3,3,3,4,0};
    rhythmPlay(drums,cymbals1,fillDrums,fillCymbals,6,6);
  }
}

void rhythmPlay(byte drums[],byte cymbals[],byte fillDrums[],byte fillCymbals[],byte number,byte beat){
  float j;
  int n;
  while(true){
  for(int i=0;i<number;i++){
    if(digitalRead(button) == LOW){
      n = i%beat;
      for(int j=n;j<beat;j++){
      analogWrite(redLed,255);
      analogWrite(greenLed,0);
      analogWrite(blueLed,0);
      drum(pgm_read_byte(&fillDrums[j]));
      drum(pgm_read_byte(&fillCymbals[j]));
      switch(beat){
      case 6:
      delay(30000/tempo-60);
      break;
      case 8:
      delay(30000/tempo-60);
      break;
      case 16:
      delay(30000/(tempo*2)-60);
      break;
      case 12:
      delay(20000/tempo-60);
      break;
      }
      digitalWrite(buzzer2,HIGH);
      vrX = analogRead(joyX);
      if(vrX<256){  // shake down
        tempo += -10;
        if(tempo < 40){
          tempo = 40;
        }
      }
      if(vrX>768){  // shake up
        tempo += 10;
        if(tempo > 240){
          tempo = 240;
        }
      }
      if(digitalRead(touchPin) == HIGH){
        break;
      }
      }
      break;
    }
    j = 0.01*i;
    RGB[0]=255*abs(sin(j*(180/PI)));
    RGB[1]=255*abs(sin((j+PI/3)*(180/PI)));
    RGB[2]=255*abs(sin((j+(2*PI)/3)*(180/PI)));
    analogWrite(redLed,RGB[0]);
    analogWrite(greenLed,RGB[1]);
    analogWrite(blueLed,RGB[2]);
    drum(pgm_read_byte(&drums[i]));
    drum(pgm_read_byte(&cymbals[i]));
    switch(beat){
    case 6:
    delay(30000/tempo-60);
    break;
    case 8:
    delay(30000/tempo-60);
    break;
    case 16:
    delay(30000/(tempo*2)-60);
    break;
    case 12:
    delay(20000/tempo-60);
    break;
    }
    digitalWrite(buzzer2,HIGH);
    vrX = analogRead(joyX);
    if(vrX<256){  // shake down
      tempo += -10;
      if(tempo < 40){
        tempo = 40;
      }
    }
    if(vrX>768){  // shake up
      tempo += 10;
      if(tempo > 240){
        tempo = 240;
      }
    }
    if(digitalRead(touchPin) == HIGH){
      break;
    }
    }
  if(digitalRead(touchPin) == HIGH){
    delay(500);
    break;
  }
  }
}

void drum(byte type){
  switch(type){
    case 0://rest
    delay(30);
    break;
    case 1://kick
    tone(buzzer,100,30);
    delay(30);
    break;
    case 2://snare
    for(int i=0;i<15;i++){
    tone(buzzer,700);
    delay(1);
    tone(buzzer,1400);
    delay(1);
    }
    noTone(buzzer);
    break;
    case 3://hi-hat close
    digitalWrite(buzzer2,LOW);
    delay(30);
    digitalWrite(buzzer2,HIGH);
    break;
    case 4://hi-hat open/cymbal
    digitalWrite(buzzer2,LOW);
    delay(30);
    break;
    case 5://tom H
    for(int i=900;i>870;i--){
    tone(buzzer,i);
    delay(1);
    }
    noTone(buzzer);
    break;
    case 6://tom M
    for(int i=600;i>570;i--){
    tone(buzzer,i);
    delay(1);
    }
    noTone(buzzer);
    break;
    case 7://tom L
    for(int i=300;i>270;i--){
    tone(buzzer,i);
    delay(1);
    }
    noTone(buzzer);
    break;
    case 8://agogo H
    tone(buzzer,900,60);
    delay(30);
    break;
    case 9://agogo L
    tone(buzzer,600,60);
    delay(30);
    break;
  }
}

void songPlay(byte musicTune[],byte musicDelay[],byte musicSize){
  int i = 0;
  byte x;
  byte octave;
  while(i < musicSize){
    x = (pgm_read_byte(&musicTune[i])-1)%7;
    octave = (pgm_read_byte(&musicTune[i])-1)/7;
    RGB[0]=R[x];
    RGB[1]=G[x];
    RGB[2]=B[x];
    analogWrite(redLed,RGB[0]);
    analogWrite(greenLed,RGB[1]);
    analogWrite(blueLed,RGB[2]);
    timingVoice(major[x]*pow(2,octave),voiceNum,pgm_read_byte(&musicDelay[i])*250*0.8*120/tempo);
    delay(pgm_read_byte(&musicDelay[i])*250*0.2*120/tempo);
    i++;
    vrX = analogRead(joyX);
    vrY = analogRead(joyY);
    if(vrX<256){  // shake down
      tempo += -10;
      if(tempo < 40){
        tempo = 40;
      }
    }
    if(vrX>768){  // shake up
      tempo += 10;
      if(tempo > 240){
        tempo = 240;
      }
    }
    if(vrY<256){  // shake left
      voiceNum--;
      if(voiceNum < 1){
        voiceNum = 4;
      }
    }
    if(vrY>768){  // shake right
      voiceNum++;
      if(voiceNum > 4){
        voiceNum = 1;
      }
    }
    if(digitalRead(touchPin) == HIGH){
      break;
    }
  }
}

void music(byte number){
  if(number == 1){
  const static PROGMEM byte musicTune1[] = {6,6,10,9,8,7,7,7,9,8,7};
  const static PROGMEM byte musicDelay1[] = {2,1,1,2,2,2,1,1,2,1,1};
  const static PROGMEM byte musicTune2[] = {6,6,15,14,15,14,15};
  const static PROGMEM byte musicDelay2[] = {2,1,1,1,1,1,1};
  const static PROGMEM byte musicTune3[] = {8};
  const static PROGMEM byte musicTune4[] = {10};
  const static PROGMEM byte musicTune5[] = {9};
  const static PROGMEM byte musicTune6[] = {12};
  const static PROGMEM byte musicTune7[] = {13};
  const static PROGMEM byte musicDelay3[] = {1};
  const static PROGMEM byte musicTune8[] = {9,8,7,5};
  const static PROGMEM byte musicDelay8[] = {1,1,1,1};
  while(true){
    for(int i=0;i<3;i++){
      songPlay(musicTune1,musicDelay1,11);
      if(digitalRead(touchPin) == HIGH){
        break;
      }
      for(int i=0;i<2;i++){
        songPlay(musicTune2,musicDelay2,7);
        if(digitalRead(touchPin) == HIGH){
          break;
        }
      }
      if(digitalRead(touchPin) == HIGH){
        break;
      }
    }
    if(digitalRead(touchPin) == HIGH){
      delay(500);
      break;
    }
    for(int i=0;i<4;i++){
      songPlay(musicTune3,musicDelay3,1);
      if(digitalRead(touchPin) == HIGH){
        break;
      }
    }
    if(digitalRead(touchPin) == HIGH){
      delay(500);
      break;
    }
    for(int i=0;i<4;i++){
      songPlay(musicTune4,musicDelay3,1);
      if(digitalRead(touchPin) == HIGH){
        break;
      }
    }
    if(digitalRead(touchPin) == HIGH){
      delay(500);
      break;
    }
    for(int i=0;i<4;i++){
      songPlay(musicTune5,musicDelay3,1);
      if(digitalRead(touchPin) == HIGH){
        break;
      }
    }
    if(digitalRead(touchPin) == HIGH){
      delay(500);
      break;
    }
    for(int i=0;i<4;i++){
      songPlay(musicTune6,musicDelay3,1);
      if(digitalRead(touchPin) == HIGH){
        break;
      }
    }
    if(digitalRead(touchPin) == HIGH){
      delay(500);
      break;
    }
    for(int j=0;j<12;j++){
      songPlay(musicTune7,musicDelay3,1);
      if(digitalRead(touchPin) == HIGH){
        break;
      }
    }
    if(digitalRead(touchPin) == HIGH){
      delay(500);
      break;
    }
    songPlay(musicTune8,musicDelay8,4);
    if(digitalRead(touchPin) == HIGH){
      delay(500);
      break;
    }
  }
  }
  
  if(number == 2){
    const static PROGMEM byte musicTune1[] = {8,9,10,10,12,13,13,8,8,10,9,8};
    const static PROGMEM byte musicDelay1[] = {2,2,2,2,2,1,3,2,2,2,4,2};
    const static PROGMEM byte musicTune2[] = {9,10};
    const static PROGMEM byte musicDelay2[] = {2,4};
    const static PROGMEM byte musicTune3[] = {7,8};
    while(true){
      songPlay(musicTune1,musicDelay1,12);
      if(digitalRead(touchPin) == HIGH){
        delay(500);
        break;
      }
      songPlay(musicTune2,musicDelay2,2);
      if(digitalRead(touchPin) == HIGH){
        delay(500);
        break;
      }
      songPlay(musicTune1,musicDelay1,12);
      if(digitalRead(touchPin) == HIGH){
        delay(500);
        break;
      }
      songPlay(musicTune3,musicDelay2,2);
      if(digitalRead(touchPin) == HIGH){
        delay(500);
        break;
      }
    }
  }
  
  if(number == 3){
    const static PROGMEM byte musicTune1[] = {6,7,8,9,7,8,8,8,7,8,9,7,8,8,8,9,10,9,10,9,10,10,9,10,12,10};
    const static PROGMEM byte musicDelay1[] = {1,1,1,1,1,1,2,1,1,1,1,1,1,2,1,1,1,1,1,1,2,1,1,2,2,2};
    const static PROGMEM byte musicTune2[] = {12,10,12,10,12,10,12,13,10,12};
    const static PROGMEM byte musicDelay2[] = {2,3,1,3,1,1,1,1,1,2};
    const static PROGMEM byte musicTune3[] = {12,12,10,9,9,8,10,9,9,8,6};
    const static PROGMEM byte musicDelay3[] = {1,1,2,2,2,1,3,2,2,1,15};
    while(true){
      for(int i=0;i<2;i++){
        songPlay(musicTune1,musicDelay1,26);
        if(digitalRead(touchPin) == HIGH){
          break;
        }
      }
      if(digitalRead(touchPin) == HIGH){
        delay(500);
        break;
      }
      for(int i=0;i<2;i++){
        songPlay(musicTune2,musicDelay2,10);
        if(digitalRead(touchPin) == HIGH){
          break;
        }
      }
      if(digitalRead(touchPin) == HIGH){
        delay(500);
        break;
      }
      for(int i=0;i<2;i++){
        songPlay(musicTune3,musicDelay3,11);
        if(digitalRead(touchPin) == HIGH){
          break;
        }
      }
      if(digitalRead(touchPin) == HIGH){
        delay(500);
        break;
      }
    }
  }
}

int chord(byte number,byte octave){
  if(number == 0){
    return 0;
  }
  int major[] = {262,330,392};
  int minor[] = {262,311,392};
  int seventh[] = {262,330,392,466};
  number = number - 1;
  switch(chordType){
    case 1:
    freq = major[number%3]*pow(2,number/3)*pow(2,octave);
    break;
    case 2:
    freq = minor[number%3]*pow(2,number/3)*transpose[7]*pow(2,octave);
    break;
    case 3:
    freq = minor[number%3]*pow(2,number/3)*transpose[9]*pow(2,octave);
    break;
    case 37:
    freq = seventh[number%4]*pow(2,number/4)*transpose[9]*pow(2,octave);
    break;
    case 4:
    freq = major[number%3]*pow(2,number/3)*transpose[10]*pow(2,octave);
    break;
    case 5:
    freq = major[number%3]*pow(2,number/3)*transpose[0]*pow(2,octave);
    break;
    case 57:
    freq = seventh[number%4]*pow(2,number/4)*transpose[0]*pow(2,octave);
    break;
    case 6:
    freq = minor[number%3]*pow(2,number/3)*transpose[2]*pow(2,octave);
    break;
  }
  return freq;
}

void acmp(byte style){
  const static PROGMEM byte arp1[32] = {1,2,3,4,5,4,3,2,1,2,3,4,5,4,3,2,1,2,3,4,5,4,3,2,1,2,3,4,5,4,3,2};
  const static PROGMEM byte arp2[12] = {1,2,3,4,3,2,1,2,3,4,3,2};
  const static PROGMEM byte drums1[16]={1,0,2,1,1,0,2,0,1,0,2,1,1,0,2,1};
  const static PROGMEM byte fillDrums1[8]={1,2,2,2,2,2,2,2};
  const static PROGMEM byte fillCymbals1[8]={3,3,3,4,5,5,6,7};
  const static PROGMEM byte drums2[32]={1,0,0,0,2,0,0,1,1,0,0,0,2,0,0,1,1,0,0,0,2,0,0,1,1,0,1,0,2,1,0,0};
  const static PROGMEM byte fillDrums2[16]={1,0,0,0,2,0,2,2,1,2,0,2,5,5,6,7};
  const static PROGMEM byte fillCymbals2[16]={3,3,3,3,3,3,3,3,4,0,0,0,0,0,0,0};
  const static PROGMEM byte introDrums1[8]={0,0,0,0,1,2,2,6};
  const static PROGMEM byte introCymbals1[8]={3,0,3,0,4,0,0,0};
  const static PROGMEM byte introDrums2[16]={0,0,0,0,0,0,0,0,1,0,2,2,5,5,6,7};
  const static PROGMEM byte introCymbals2[16]={3,0,0,0,3,0,0,0,4,0,0,0,0,0,0,0};
  
  if(style == 1){  // 8 Beat 1
    introPlay(introDrums1,introCymbals1,8);
    acmpPlay(arp1,drums1,fillDrums1,fillCymbals1,16,8,0);
    endingPlay(0);
  }

  if(style == 2){  // 8 Beat 2
    const static PROGMEM byte arp[16] = {1,2,3,2,4,3,2,3,1,2,3,2,4,2,3,2};
    introPlay(introDrums1,introCymbals1,8);
    acmpPlay(arp,drums1,fillDrums1,fillCymbals1,16,8,0);
    endingPlay(0);
  }
  
  if(style == 3){  // 16 Beat 1
    introPlay(introDrums2,introCymbals2,16);
    acmpPlay(arp1,drums2,fillDrums2,fillCymbals2,32,16,0);
    endingPlay(0);
  }

  if(style == 4){  // 16 Beat 2
    const static PROGMEM byte arp[32] = {1,2,3,4,1,2,3,4,1,2,3,4,5,4,3,2,1,2,3,4,1,2,3,4,1,2,3,4,5,4,3,2};
    introPlay(introDrums2,introCymbals2,16);
    acmpPlay(arp,drums2,fillDrums2,fillCymbals2,32,16,0);
    endingPlay(0);
  }
  
  if(style == 5){  // Slow Rock
    const static PROGMEM byte drums[12]={1,0,0,2,0,1,1,0,0,2,0,0};
    const static PROGMEM byte fillDrums[12]={1,0,0,2,0,2,1,2,2,5,6,7};
    const static PROGMEM byte fillCymbals[12]={3,3,3,3,3,3,4,0,0,0,0,0};
    const static PROGMEM byte introDrums[12]={0,0,0,0,0,0,1,2,2,5,6,7};
    const static PROGMEM byte introCymbals[12]={3,0,0,3,0,0,4,0,0,0,0,0};
    introPlay(introDrums,introCymbals,12);
    acmpPlay(arp2,drums,fillDrums,fillCymbals,12,12,0);
    endingPlay(0);
  }
  
  if(style == 6){  // Waltz
    const static PROGMEM byte drums[6] = {1,0,2,0,2,0};
    const static PROGMEM byte fillDrums[6]={1,2,2,0,2,2};
    const static PROGMEM byte fillCymbals[6]={3,3,3,3,4,0};
    const static PROGMEM byte introDrums[6]={0,0,0,1,2,6};
    const static PROGMEM byte introCymbals[6]={3,0,3,0,4,0};
    introPlay(introDrums,introCymbals,6);
    acmpPlay(arp2,drums,fillDrums,fillCymbals,6,6,0);
    endingPlay(0);
  }
}

void acmpPlay(byte arp[],byte drums[],byte fillDrums[],byte fillCymbals[],byte number,byte beat,byte octave){
  float j;
  int n;
  while(true){
  for(int i=0;i<number;i++){
    vrX = analogRead(joyX);
    vrY = analogRead(joyY);
      if(vrX<256&&vrY<256){  // shake left down
        chordType = 2;
      }
      else if(vrX>768&&vrY<256){  // shake left up
        chordType = 37;
      }
      else if(vrX<256&&vrY>768){  // shake right down
        chordType = 6;
      }
      else if(vrX>768&&vrY>768){  // shake right up
        chordType = 5;
      }
      else if(vrX<256){  // shake down
        chordType = 1;
      }
      else if(vrX>768){  // shake up
        chordType = 4;
      }
      else if(vrY<256){  // shake left
        chordType = 3;
      }
      else if(vrY>768){  // shake right
        chordType = 57;
      }
    if(digitalRead(button) == LOW){
      n = i%beat;
      for(int j=n;j<beat;j++){
      analogWrite(redLed,255);
      analogWrite(greenLed,0);
      analogWrite(blueLed,0);
      if(drumsON == 1){
        drum(pgm_read_byte(&fillDrums[j]));
        drum(pgm_read_byte(&fillCymbals[j]));
      }
      else{
        delay(60);
      }
      switch(beat){
      case 6:
      delay(30000/tempo-60);
      break;
      case 8:
      delay(30000/tempo-60);
      break;
      case 16:
      delay(30000/(tempo*2)-60);
      break;
      case 12:
      delay(20000/tempo-60);
      break;
      }
      digitalWrite(buzzer2,HIGH);
      if(digitalRead(touchPin) == HIGH){
        break;
      }
      }
      break;
    }
    j = 0.01*i;
    RGB[0]=255*abs(sin(j*(180/PI)));
    RGB[1]=255*abs(sin((j+PI/3)*(180/PI)));
    RGB[2]=255*abs(sin((j+(2*PI)/3)*(180/PI)));
    analogWrite(redLed,RGB[0]);
    analogWrite(greenLed,RGB[1]);
    analogWrite(blueLed,RGB[2]);
    if(drumsON == 1){
      drum(pgm_read_byte(&drums[i]));
    }
    else{
      delay(30);
    }
    switch(beat){
    case 6:
    timingVoice(chord(pgm_read_byte(&arp[i]),octave),voiceNum,30000/tempo-30);
    break;
    case 8:
    timingVoice(chord(pgm_read_byte(&arp[i]),octave),voiceNum,30000/tempo-30);
    break;
    case 16:
    timingVoice(chord(pgm_read_byte(&arp[i]),octave),voiceNum,30000/(tempo*2)-30);
    break;
    case 12:
    timingVoice(chord(pgm_read_byte(&arp[i]),octave),voiceNum,20000/tempo-30);
    break;
    }
    digitalWrite(buzzer2,HIGH);
    if(digitalRead(touchPin) == HIGH){
      break;
    }
    }
  if(digitalRead(touchPin) == HIGH){
    break;
  }
  }
}

void introPlay(byte introDrums[],byte introCymbals[],byte beat){
  if(intro == 1){
  byte c;
  for(int j=0;j<beat;j++){
  analogWrite(redLed,0);
  analogWrite(greenLed,255);
  analogWrite(blueLed,0);
  if(drumsON == 1){
    drum(pgm_read_byte(&introDrums[j]));
    drum(pgm_read_byte(&introCymbals[j]));
  }
  else{
    switch(beat){
    case 6:
    c = j%2;
    break;
    case 8:
    c = j%2;
    break;
    case 16:
    c = j%4;
    break;
    case 12:
    c = j%3;
    break;
    }
    if(c == 0){
      drum(3);
      delay(30);
    }
    else{
      delay(60);
    }
  }
  switch(beat){
  case 6:
  delay(30000/tempo-60);
  break;
  case 8:
  delay(30000/tempo-60);
  break;
  case 16:
  delay(30000/(tempo*2)-60);
  break;
  case 12:
  delay(20000/tempo-60);
  break;
  }
  digitalWrite(buzzer2,HIGH);
  }
  }
}

void endingPlay(byte octave){
  if(ending == 1){
  analogWrite(redLed,0);
  analogWrite(greenLed,0);
  analogWrite(blueLed,255);
  vrX = analogRead(joyX);
  vrY = analogRead(joyY);
  if(vrX<256&&vrY<256){  // shake left down
    chordType = 2;
  }
  else if(vrX>768&&vrY<256){  // shake left up
    chordType = 37;
  }
  else if(vrX<256&&vrY>768){  // shake right down
    chordType = 6;
  }
  else if(vrX>768&&vrY>768){  // shake right up
    chordType = 5;
  }
  else if(vrX<256){  // shake down
    chordType = 1;
  }
  else if(vrX>768){  // shake up
    chordType = 4;
  }
  else if(vrY<256){  // shake left
    chordType = 3;
  }
  else if(vrY>768){  // shake right
    chordType = 57;
  }
  if(drumsON == 1){
    drum(1);
    drum(4);
  }
  timingVoice(chord(1,octave),voiceNum,60);
  timingVoice(chord(2,octave),voiceNum,60);
  timingVoice(chord(3,octave),voiceNum,60);
  timingVoice(chord(4,octave),voiceNum,60);
  digitalWrite(buzzer2,HIGH);
  }
  delay(500);
}

void voiceRemote(unsigned long value){
  switch(value){
    case 0xFFA25D:  // CH-
      key--;
      if(key < 0){
        key = 0;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF629D:  // CH
      voiceNum++;
      if(voiceNum > 5){
        voiceNum = 1;
      }
      if(voiceNum == 5){
        drum(1);
      }
      else{
        timingVoice(523,voiceNum,50);
      }
      break;
    case 0xFFE21D:  // CH+
      key++;
      if(key > 11){
        key = 11;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF22DD:  // prev
      shift = -1;
      break;
    case 0xFF02FD:  // next
      shift = 1;
      break;
    case 0xFFC23D:  // play
      timingVoice(262,voiceNum,50);
      shift = 0;
      break;
    case 0xFFE01F:  // -
      timingVoice(294,voiceNum,50);
      shift = 0;
      break;
    case 0xFFA857:  // +
      timingVoice(330,voiceNum,50);
      shift = 0;
      break;
    case 0xFF906F:  // EQ
      timingVoice(349,voiceNum,50);
      shift = 0;
      break;
    case 0xFF6897:  // 0
      timingVoice(392,voiceNum,50);
      shift = 0;
      break;
    case 0xFF9867:  // 100+
      timingVoice(440,voiceNum,50);
      shift = 0;
      break;
    case 0xFFB04F:  // 200+
      timingVoice(494,voiceNum,50);
      shift = 0;
      break;
    case 0xFF30CF:  // 1
      timingVoice(523,voiceNum,50);
      shift = 0;
      break;
    case 0xFF18E7:  // 2
      timingVoice(587,voiceNum,50);
      shift = 0;
      break;
    case 0xFF7A85:  // 3
      timingVoice(659,voiceNum,50);
      shift = 0;
      break;
    case 0xFF10EF:  // 4
      timingVoice(698,voiceNum,50);
      shift = 0;
      break;
    case 0xFF38C7:  // 5
      timingVoice(784,voiceNum,50);
      shift = 0;
      break;
    case 0xFF5AA5:  // 6
      timingVoice(880,voiceNum,50);
      shift = 0;
      break;
    case 0xFF42BD:  // 7
      timingVoice(988,voiceNum,50);
      shift = 0;
      break;
    case 0xFF4AB5:  // 8
      timingVoice(1046,voiceNum,50);
      shift = 0;
      break;
    case 0xFF52AD:  // 9
      timingVoice(1175,voiceNum,50);
      shift = 0;
      break;
    default:
      break;
  }
  irrecv.enableIRIn();
}

void drumRemote(unsigned long value){
  switch(value){
    case 0xFF629D:  // CH
      voiceNum++;
      if(voiceNum > 5){
        voiceNum = 1;
      }
      if(voiceNum == 5){
        drum(1);
      }
      else{
        timingVoice(523,voiceNum,50);
      }
      break;
    case 0xFF30CF:  // 1
      drum(1);
      break;
    case 0xFF18E7:  // 2
      drum(2);
      break;
    case 0xFF7A85:  // 3
      drum(3);
      break;
    case 0xFF10EF:  // 4
      drum(4);
      delay(60);
      digitalWrite(buzzer2,HIGH);
      break;
    case 0xFF38C7:  // 5
      drum(5);
      break;
    case 0xFF5AA5:  // 6
      drum(6);
      break;
    case 0xFF42BD:  // 7
      drum(7);
      break;
    case 0xFF4AB5:  // 8
      drum(8);
      delay(30);
      break;
    case 0xFF52AD:  // 9
      drum(9);
      delay(30);
      break;
    default:
      break;
  }
  irrecv.enableIRIn();
}

void rhythmRemote(unsigned long value){
  switch(value){
    case 0xFFA25D:  // CH-
      tone(buzzer,523,50);
      tempo += -10;
      if(tempo < 40){
        tempo = 40;
        tone(buzzer,262,100);
        delay(50);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(50);
      break;
    case 0xFF629D:  // CH
      tempo = 120;
      tone(buzzer,784,50);
      delay(50);
      break;
    case 0xFFE21D:  // CH+
      tone(buzzer,1046,50);
      tempo += 10;
      if(tempo > 240){
        tempo = 240;
        tone(buzzer,262,100);
        delay(50);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(50);
      break;
    case 0xFF22DD:  // prev
      tone(buzzer,523,50);
      style--;
      if(style < 1){
        style = 15;
        tone(buzzer,1046,50);
      }
      delay(50);
      break;
    case 0xFF02FD:  // next
      tone(buzzer,1046,50);
      style++;
      if(style > 15){
        style = 1;
        tone(buzzer,523,50);
      }
      delay(50);
      break;
    case 0xFFC23D:  // play
      rhythm(style);
      break;
    case 0xFFE01F:  // -
      style = 10;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFFA857:  // +
      timingVoice(330,voiceNum,50);
      style = 11;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF906F:  // EQ
      style = 12;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF6897:  // 0
      style = 13;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF9867:  // 100+
      style = 14;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFFB04F:  // 200+
      style = 15;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF30CF:  // 1
      style = 1;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF18E7:  // 2
      style = 2;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF7A85:  // 3
      style = 3;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF10EF:  // 4
      style = 4;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF38C7:  // 5
      style = 5;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF5AA5:  // 6
      style = 6;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF42BD:  // 7
      style = 7;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF4AB5:  // 8
      style = 8;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF52AD:  // 9
      style = 9;
      tone(buzzer,523,50);
      delay(50);
      break;
    default:
      break;
  }
  irrecv.enableIRIn();
}

void acmpRemote(unsigned long value){
  switch(value){
    case 0xFFA25D:  // CH-
      tone(buzzer,523,50);
      tempo += -10;
      if(tempo < 40){
        tempo = 40;
        tone(buzzer,262,100);
        delay(50);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(50);
      break;
    case 0xFF629D:  // CH
      tempo = 120;
      tone(buzzer,784,50);
      delay(50);
      break;
    case 0xFFE21D:  // CH+
      tone(buzzer,1046,50);
      tempo += 10;
      if(tempo > 240){
        tempo = 240;
        tone(buzzer,262,100);
        delay(50);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(50);
      break;
    case 0xFF22DD:  // prev
      tone(buzzer,523,50);
      acmpStyle--;
      if(acmpStyle < 1){
        acmpStyle = 6;
        tone(buzzer,1046,50);
      }
      delay(50);
      break;
    case 0xFF02FD:  // next
      tone(buzzer,1046,50);
      acmpStyle++;
      if(acmpStyle > 6){
        acmpStyle = 1;
        tone(buzzer,523,50);
      }
      delay(50);
      break;
    case 0xFFC23D:  // play
      acmp(acmpStyle);
      break;
    case 0xFFE01F:  // -
      voiceNum--;
      if(voiceNum < 1){
        voiceNum = 4;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFFA857:  // +
      voiceNum++;
      if(voiceNum > 4){
        voiceNum = 1;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF906F:  // EQ
      tone(buzzer,1046,50);
      drumsON++;
      if(drumsON > 1){
        drumsON = 0;
        tone(buzzer,523,50);
      }
      delay(50);
      break;
    case 0xFF6897:  // 0
      key--;
      if(key < 0){
        key = 0;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF9867:  // 100+
      key = 5;
      timingVoice(523,voiceNum,50);
      break;
    case 0xFFB04F:  // 200+
      key++;
      if(key > 11){
        key = 11;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF30CF:  // 1
      acmpStyle = 1;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF18E7:  // 2
      acmpStyle = 2;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF7A85:  // 3
      acmpStyle = 3;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF10EF:  // 4
      acmpStyle = 4;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF38C7:  // 5
      acmpStyle = 5;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF5AA5:  // 6
      acmpStyle = 6;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF4AB5:  // 8
      tone(buzzer,1046,50);
      intro++;
      if(intro > 1){
        intro = 0;
        tone(buzzer,523,50);
      }
      delay(50);
      break;
    case 0xFF52AD:  // 9
      tone(buzzer,1046,50);
      ending++;
      if(ending > 1){
        ending = 0;
        tone(buzzer,523,50);
      }
      delay(50);
      break;
    default:
      break;
  }
  irrecv.enableIRIn();
}

void songRemote(unsigned long value){
  switch(value){
    case 0xFFA25D:  // CH-
      tone(buzzer,523,50);
      tempo += -10;
      if(tempo < 40){
        tempo = 40;
        tone(buzzer,262,100);
        delay(50);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(50);
      break;
    case 0xFF629D:  // CH
      tempo = 120;
      tone(buzzer,784,50);
      delay(50);
      break;
    case 0xFFE21D:  // CH+
      tone(buzzer,1046,50);
      tempo += 10;
      if(tempo > 240){
        tempo = 240;
        tone(buzzer,262,100);
        delay(50);
      }
      if(tempo == 120){
        tone(buzzer,784,50);
      }
      delay(50);
      break;
    case 0xFF22DD:  // prev
      tone(buzzer,523,50);
      songNum--;
      if(songNum < 1){
        songNum = 3;
        tone(buzzer,1046,50);
      }
      delay(50);
      break;
    case 0xFF02FD:  // next
      tone(buzzer,1046,50);
      songNum++;
      if(songNum > 3){
        songNum = 1;
        tone(buzzer,523,50);
      }
      delay(50);
      break;
    case 0xFFC23D:  // play
      music(songNum);
      break;
    case 0xFFE01F:  // -
      voiceNum--;
      if(voiceNum < 1){
        voiceNum = 4;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFFA857:  // +
      voiceNum++;
      if(voiceNum > 4){
        voiceNum = 1;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF6897:  // 0
      key--;
      if(key < 0){
        key = 0;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF9867:  // 100+
      key = 5;
      timingVoice(523,voiceNum,50);
      break;
    case 0xFFB04F:  // 200+
      key++;
      if(key > 11){
        key = 11;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF30CF:  // 1
      songNum = 1;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF18E7:  // 2
      songNum = 2;
      tone(buzzer,523,50);
      delay(50);
      break;
    case 0xFF7A85:  // 3
      songNum = 3;
      tone(buzzer,523,50);
      delay(50);
      break;
    default:
      break;
  }
  irrecv.enableIRIn();
}

void lightRemote(unsigned long value){
  switch(value){
    case 0xFFA25D:  // CH-
      key--;
      if(key < 0){
        key = 0;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF629D:  // CH
      voiceNum++;
      if(voiceNum > 4){
        voiceNum = 1;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFFE21D:  // CH+
      key++;
      if(key > 11){
        key = 11;
      }
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF22DD:  // prev
      tone(buzzer,523,50);
      color--;
      if(color < 1){
        color = 9;
        tone(buzzer,1046,50);
      }
      delay(50);
      break;
    case 0xFF02FD:  // next
      tone(buzzer,1046,50);
      color++;
      if(color > 9){
        color = 1;
        tone(buzzer,523,50);
      }
      delay(50);
      break;
    case 0xFFC23D:  // play
      color = 1;
      timingVoice(330,voiceNum,50);
      break;
    case 0xFFE01F:  // -
      tone(buzzer,523,50);
      bright++;
      if(bright > 4){
        bright = 1;
        tone(buzzer,1046,50);
      }
      delay(50);
      break;
    case 0xFFA857:  // +
      tone(buzzer,1046,50);
      bright--;
      if(bright < 1){
        bright = 4;
        tone(buzzer,523,50);
      }
      delay(50);
      break;
    case 0xFF906F:  // EQ
      bright = 4;
      timingVoice(349,voiceNum,50);
      break;
    case 0xFF6897:  // 0
      bright = 3;
      timingVoice(392,voiceNum,50);
      break;
    case 0xFF9867:  // 100+
      bright = 2;
      timingVoice(440,voiceNum,50);
      break;
    case 0xFFB04F:  // 200+
      bright = 1;
      timingVoice(494,voiceNum,50);
      break;
    case 0xFF30CF:  // 1
      color = 3;
      timingVoice(523,voiceNum,50);
      break;
    case 0xFF18E7:  // 2
      color = 4;
      timingVoice(587,voiceNum,50);
      break;
    case 0xFF7A85:  // 3
      color = 5;
      timingVoice(659,voiceNum,50);
      break;
    case 0xFF10EF:  // 4
      color = 6;
      timingVoice(698,voiceNum,50);
      break;
    case 0xFF38C7:  // 5
      color = 7;
      timingVoice(784,voiceNum,50);
      break;
    case 0xFF5AA5:  // 6
      color = 8;
      timingVoice(880,voiceNum,50);
      break;
    case 0xFF42BD:  // 7
      color = 9;
      timingVoice(988,voiceNum,50);
      break;
    case 0xFF4AB5:  // 8
      color = 2;
      timingVoice(1046,voiceNum,50);
      break;
    case 0xFF52AD:  // 9
      color = random(1,10);
      timingVoice(1175,voiceNum,50);
      break;
    default:
      break;
  }
  irrecv.enableIRIn();
}
