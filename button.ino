// ------------------------------------------------------------------------
//this routine sees if a button was pushed, returns the button value

int get_button (void){
  
int read_button;
int button_pushed;

read_button =0;
button_pushed = NOBUTTON;

if (digitalRead (PUSH01_PIN) == 0) {  //see if the Enter button was pushed
   button_pushed = BUTTON_1;
   Serial.println("button 1");
   }
if (digitalRead (PUSH02_PIN) == 0) {  //see if the Enter button was pushed
   button_pushed = BUTTON_2;
   }

if (digitalRead (PUSH03_PIN) == 0) {  //see if the Enter button was pushed
   button_pushed = BUTTON_3;
   }
if (digitalRead (PUSH04_PIN) == 0) {  //see if the Enter button was pushed
   button_pushed = BUTTON_4;
   }

return button_pushed;  
}
