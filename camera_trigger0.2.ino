  // this is intended to be a time lapse remote control for 3 Canon EOS 1200D cameras.

  // non changing parameters
  const int pic1pin=7; //pin for camera 1
  const int pic2pin=2; //pin for camera 2
  const int pic3pin=4; //pin for camera 3
  const String stopstring="go"; //this strores the string user needs to input for start
  const String dtstring="dt"; //this strores the string user needs to input for changing dt
  const String helpstring="help"; //this strores the string user needs to input for changing dt
  const int sizedt=2; //lenght of previous string
  const String stopdtstring="sec"; //this strores the string user needs to input for start
  const int valuedtsize=10; //max length of a caracter read in serial
  const String messstart="time interval was changed to "; //start of a massage
  const String messend=" s "; //end of a massage
  const int delaytps=1000; //duration of simulated button push in msec
    
  // these caracters can be modified by user
  int start=0; //this variable will store if user started
  int dt=600; //time step, seconds between pictures

  // to read orders send by serial
  char valuedt [valuedtsize]; //a string with given size to read numbers
  char c; // will contain the reads from serial, one caracter at a time
  String inString=""; //will contain the converted string sent by user
  char ind2 = 0;      // index to go through chains of character
  int indcmd = -1;              // used to read input strings
  String mess=""; // a string for a message
  char lengthstr = 0;           // will contain length of chain containing numbers for interval commands

  

void setup() {
  Serial.begin(9600);
  pinMode(pic1pin, OUTPUT);
  pinMode(pic2pin, OUTPUT);
  pinMode(pic3pin, OUTPUT);
  
}

void loop() {

// reading the user input from serial
while (Serial.available()) {
    delay(10);  //small delay to allow input buffer to fill
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      if (c == ';') {
        break;
      }  //breaks out of capture loop to print readstring
      inString += c; //makes the string inString
    }
  }

// what does user want?
if (inString.length() > 0) { //user sent something
  Serial.println(inString);
  indcmd = inString.indexOf(stopstring); //comparing string to stopstring
  if (indcmd >= 0) { //there is a match
      start = (start-1)*(start-1); //this will initiate (1) or stop (0) the picture taking process
      indcmd = -1; //reinitialise the incmd
      if (start==1) {Serial.println ("started");}
      if (start==0) {Serial.println ("stopped");}
    }

  indcmd = inString.indexOf(helpstring); //comparing string to startstring
  if (indcmd >= 0) { //there is a match
    Serial.println ("To change variables:");
    Serial.println ("- send 'dt+TIME+sec' to set the time delay in seconds");
    Serial.println ("- send 'go' to start or stop an experiment");
    Serial.println ("*****************************************************");
    Serial.println ("Experiment state:");
    Serial.print ("dt is currently ");
    mess=dt+messend;
    Serial.println (mess);
    if (start==1) {Serial.println ("timelapse is currently running");}
    if (start==0) {Serial.println ("timelapse is not running");}
      indcmd=-1; //reinitialise the incmd
    }

  indcmd = inString.indexOf(dtstring); //comparing to dtstring
  if (indcmd >= 0) { //there is a match
    for (ind2 = 0; ind2 < valuedtsize; ind2++) { //looking at the 10 characters after signal
        if (isDigit(inString[indcmd + sizedt + ind2])) { //extracting numbers
          valuedt[ind2] = inString[indcmd + ind2 + sizedt]; 
          lengthstr = ind2; // stocking index of last number found = length of chain
        }
      }
      valuedt[lengthstr + 1] = '\0'; //ending chain properly
      dt = atoi(valuedt); //converting it to integer
      indcmd = inString.indexOf(stopdtstring); //looking if message is complete
      if (indcmd >= 0 ) { //if complete
        mess=messstart+dt+messend;
        Serial.println(mess);
        lengthstr = 0; //reset ind2
        for (ind2 = 0; ind2 < valuedtsize; ind2++) { //reinitialize strValue
          valuedt[ind2] = '0';
        }
      }
      else { //string not complete
        Serial.println("to change dt, enter dt+NEWVAL+sec (no spaces)"); //ask for command again to user
      }
      indcmd = -1; //reinitialise the incmd
      }
      inString = ""; //reinitialise the inString
}

// taking the pictures (this is the easy coding part!)
if (start==1) {
  
  digitalWrite(pic1pin, HIGH);
  digitalWrite(pic2pin, HIGH);
  digitalWrite(pic3pin, HIGH);
delay(delaytps); //giving some time for camera to read input
  digitalWrite(pic1pin, LOW);
  digitalWrite(pic2pin, LOW);
  digitalWrite(pic3pin, LOW);
  Serial.println("picture taken");
  Serial.print("waiting:");
  int wait=0;
  while (wait<dt-1) {
  delay(1000); //pause between pictures
  wait=wait+1;
  if ((dt-wait)/10==round((dt-wait)/10))
  Serial.print(dt-wait);
  Serial.print(messend);
  }
}


}
