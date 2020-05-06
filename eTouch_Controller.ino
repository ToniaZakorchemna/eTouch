//function prototypes
void move_motor(int pulses);
bool set_pin(int row, int col, int state);
bool set_pins(int row, int *col, int pin_count, int *states);

// defines pins numbers
const int step_pin = 12; 
const int dir_pin = 13; 
const int sol_pins[] = {9, 10, 11, 12}; // Important: must be in order by column
const int sol_pin_count = 4;

//layout of eReader pins
const int rows = 1;
const int cols = 4;
int **pin_states;

//track positioning
int cur_row = 0;
int cur_col = 0;

const int pulses_per_row = 200; // # of pulses to move motor 1 row up/down

void setup() {
  // Sets the pins as Outputs
  pinMode(step_pin,OUTPUT); 
  pinMode(dir_pin,OUTPUT);
  for(int i=0; i < sol_pin_count; i++)
    pinMode(solPins[i], OUTPUT);
  
  // build pin state array
  pin_states = (int **) calloc(rows, sizeof(int *));
  for(int i = 0; i < rows; i++){
    int *row = (int *) calloc(cols, sizeof(int));
    pin_states[i] = row;
  }
}


void loop() {
  //move the motor up and down
  move_motor(1);
  move_motor(-1);
  delay(1000); 
  //should activate 3rd pin for 1 second
  set_pin(0, 2, 1);
  delay(5000); //wait 5 secs before repeating
  
}

//moves motor given number of rows
void move_motor(int rows){
  if(rows >= 0) digitalWrite(dir_pin,HIGH); // Set motor to move forward
  else digitalWrite(dir_pin,LOW); //Set moto to move backward
  
  // moves motor by sending it specified number of pulses (200 pulses = 1 rotation)
  for(int x = 0; x < abs(rows * pulses_per_row); x++) {
    digitalWrite(step_pin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(step_pin,LOW); 
    delayMicroseconds(500); 
  }
  
  cur_row += rows;
}

//sets a single pin to given state
bool set_pin(int row, int col, int state){
  if(cur_row != row) return false; //in wrong row
  if(pin_states[row][col] = state) return true; //pin already correct state
  
  digitalWrite(sol_pins[col], HIGH); //activate corresponding solenoid
  delay(1000);                          //Wait 1 Second
  digitalWrite(sol_pins[col], LOW); //retract corresponding solenoid
  pin_states[row][col] = state; //update pin states array
  return true;
}

//sets mutiple pins to given states
bool set_pins(int row, int *cols, int pin_count, int *states){
  if(cur_row != row) return false;
  for(int i=0; i < pin_count; i++){ //activate solenoids
    if(pin_states[row][cols[i]] != states[i]){
      digitalWrite(sol_pins[cols[i]], HIGH);
    }
  }
  delay(1000); //wait 1 second
  for(int i=0; i < pin_count; i++){ //retract solenoids
    if(pin_states[row][cols[i]] != states[i]){
      digitalWrite(sol_pins[cols[i]], LOW);
      pin_states[row][cols[i]] = states[i];
    }
  }
  return true;
}


