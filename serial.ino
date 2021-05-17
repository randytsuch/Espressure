/* ------------------------------------------------------------------------
Routine for reading serial port
Added for debugging, to look for a special char from the serial port, and then do something special
when that char is received.

*/

int char_cnt = 0;     //counter for number of bytes or characters in the serial data
int k = 0;
//int prof_no = 0;
//int prof_ind = 0;
int val = 0;
int digit = 0;
boolean done_serial = false;

// ------------------------------------------------------------------------

//

void receive_serial ()
{
byte in_byte[60];  //input array, set to 60 chars
byte *ptr;
char aChar;
//char tempchar;

ptr = &in_byte[0];      //set to point to array
byte counter;
val = 0;
digit = 0;
boolean done_serial = false;

while (Serial.available() > 0) {
    aChar = Serial.read();
    if (aChar == '@') {  //
      Serial.println("@ command");
     // done_serial = true;
      }
    else if (aChar == '!') {
      Serial.println("! command");
     // done_serial = true;
      }
    else if (aChar == ',') {
    //the following two lines convert integer val to two bytes to store in in_byte
      in_byte[k++] = (val & 0xFF);         //read in the lsb
      in_byte[k++] = ((val >> 8) & 0xFF);  //read in the msb
      val = 0;
      digit = 0;
      char_cnt++;
      }
 }               
}
