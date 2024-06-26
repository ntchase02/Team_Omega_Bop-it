#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <string.h>
#include <stdio.h>


#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
//#define OLED_MOSI   9
//#define OLED_CLK   10
//#define OLED_DC    11
//#define OLED_CS    12
//#define OLED_RESET 13
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
//  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

//Comment out above, uncomment this block to use hardware SPI
#define OLED_DC 8
#define OLED_CS 10
#define OLED_RESET 9
#define OLED_D0 13
#define OLED_D1 11
#define TONE_GEN 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

int in_pin[] = { 7, 6, 5 };
int start_pin = 3;
int num_in = 3;

bool in_read[3];
bool start_read = false;
bool err = false;

char *cmd_string[] = { "Solder the component", "Press in the LED", "Test the leads" };
char *err_string[] = { "YOU DIDN'T SOLDER THE COMPONENT", "YOU DIDN'T PRESS IN THE LED", "YOU DIDN'T TEST THE LEADS" };
double tone_freq[] = { 440, 550, 660 };
double tone_err = 311.127;

int cnt = 0;
int max_limit = 99;
int mv_start, mv_fin;
int time_limit = 4;
int r_num; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  int time_out = 60;
  int cur_time = millis();
  while (!display.begin(SSD1306_SWITCHCAPVCC) && cur_time < time_out*1000) {
    Serial.println(F("SSD1306 allocation failed"));
    //for(;;); // Don't proceed, loop forever
    Serial.println("Can not connect to display");
        //exit with error
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC)){
    Serial.print("Can not connect to display");
    exit(-1);
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  pinMode(OLED_DC, OUTPUT);
  pinMode(OLED_CS, OUTPUT);
  pinMode(OLED_RESET, OUTPUT);
  pinMode(OLED_D0, OUTPUT);
  pinMode(OLED_D1, OUTPUT);
  pinMode(TONE_GEN, OUTPUT);

  for (int i = 0; i < num_in; i++) {
    pinMode(in_pin[i], INPUT);
  }
  pinMode(start_pin, INPUT);

  randomSeed(analogRead(0));

  
}


void loop() {


  // put your main code here, to run repeatedly:
  start_read = digitalRead(start_pin);
  cnt = 0;
  print_text("WELCOME TO CIRC-IT");
  while (start_read || (cnt < max_limit && cnt > 0) ) {

    display.clearDisplay();
    char num_cur_in[32];
    sprintf(num_cur_in,"Successful Moves: %d",cnt);
    print_text(num_cur_in);
    delay(1000);
    display.clearDisplay();
    //Serial.print("Move :" + cnt);

    //set up the initial read values
    in_read[0] = false;
    in_read[1] = false;
    in_read[2] = false;

    //give command
    r_num = random(3);
    print_text(cmd_string[r_num]);
    tone(TONE_GEN, tone_freq[r_num], 500);
    
    delay(1000);
    display.clearDisplay();
    //Serial.print(cmd_string[r_num]+'\n');

    //set up the time
    char time_char[32];
    
    
    if(cnt == 33 || cnt == 66 || cnt == 90){
      time_limit = time_limit - 1;
      //sprintf(time_char,"You have %d seconds left",time_limit);
      //print_text(time_char);
      //delay(500);
      //display.clearDisplay();
    }
    
    mv_start = millis();
    mv_fin = millis();
    //wait for input or time-out
    while (mv_fin <= mv_start + time_limit * 1000 && !in_read[0] && !in_read[1] && !in_read[2]) {
      //print_text(cmd_string[r_num]);
      in_read[0] = digitalRead(in_pin[0]);
      delay(2);
      in_read[1] = digitalRead(in_pin[1]);
      delay(2);
      in_read[2] = digitalRead(in_pin[2]);
      delay(2);
      mv_fin = millis();
      
    }

    //were too slow
    if (mv_fin > mv_start + time_limit * 1000) {
      print_text("YOU WERE TOO SLOW, TRY AGAIN");
      delay(1000);
      display.clearDisplay();
      //Serial.print("YOU WERE TOO SLOW, TRY AGAIN\n");
      
      tone(TONE_GEN, tone_err, 500);
      delay(1000);
      //goto begin_bopit;  //exit the while loop
      break;
    } 
    else {
      err = false;

      //if you did the wrong input or no input at all
      for (int i = 0; i < num_in; i++) {
        if (i == r_num) {
          //goto begin_bopit;
          continue;
        } else {
          if (in_read[i] == true || in_read[r_num] == false) {
            print_text(err_string[r_num]);
            //Serial.print(err_string[r_num]+'\n');
            //display.display();
            tone(TONE_GEN, tone_err, 500);
            delay(1000);
            display.clearDisplay();
            err = true;
            //goto begin_bopit;
            break;
          }
        }
      }

      if (err) {
        //goto begin_bopit;  // exit while loop
        break;
      }
      //right input
      else {
        print_text("CORRECT");
        //Serial.print("CORRECT\n");
        
        tone(TONE_GEN, 880, 500);
        delay(1000);
        display.clearDisplay();
        cnt++;
      }
    }
  }

  // if you win
  if (cnt > max_limit) {
    print_text("YOU HAVE WON CIRC-IT");
    //Serial.println("YOU HAVE WON CIRC-IT");
    tone(TONE_GEN, tone_freq[0], 250);
    delay(250);
    tone(TONE_GEN, tone_freq[1], 250);
    delay(250);
    tone(TONE_GEN, tone_freq[2], 250);
    delay(250);
    tone(TONE_GEN, 880, 250);
    display.display();
    delay(250);
    exit(0);
  }
}

void print_text(char words[]){
  //display.clearDisplay();
  display.setTextSize(1.5); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(words);
  display.display();
  delay(100);
}
