#include <TLC7524CN.h>
#include <MCP3XXX.h>
//control pins for amplifier
const int A = 14;
const int B = 13;
// pins for SPI ADC
uint8_t miso = 21;
uint8_t mosi = 47;
uint8_t cs = 48;
uint8_t clk = 45;
const int freq = 10000;
const int resolution = 8;
const int pwm_channel = 0;
//const int adc_input_pin = 13;
// pins for DAC
const int dac_wr = 46;
const int dac_cs = 3;
const int dac_d0 = 18;
const int dac_d1 = 17;
const int dac_d2 = 16;
const int dac_d3 = 4;
const int dac_d4 = 5;
const int dac_d5 = 6;
const int dac_d6 = 7;
const int dac_d7 = 15;
// number of coefficients
const int taps = 17;
// array for stored signal samples initially 0 
float input_samples[taps];
// array for result of multiplying each sample by it's respective coefficient
float input_x_coefficients[taps];
// variable to store newest sampled value, initially 0
int newest_val = 0;
// array for FIR Filter Coefficients
float filter_coefficients[taps] = {
0.000696, 
0.003527, 
0.009773, 
0.020360, 
0.035047, 
0.052023, 
0.068138, 
0.079762, 
0.084004, 
0.079762, 
0.068138, 
0.052023, 
0.035047, 
0.020360, 
0.009773, 
0.003527, 
0.000696
};
// variable to store output
float output_val = 0;
// DAC object
TLC7524CN dac(dac_wr, dac_cs, dac_d0, dac_d1, dac_d2, dac_d3, dac_d4, dac_d5, dac_d6, dac_d7);
// ADC object
MCP3004 adc;

void setup() {
  // put your setup code here, to run once:
  pinMode(8,OUTPUT);
  digitalWrite(8, HIGH);
  // setup amplifier control pins
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  // set array of samples to zero initially
  for(int i = 0; i < taps; i++){
    input_samples[i] = 0;
  }
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  adc.begin(cs,mosi,miso,clk);
}

//variables to measure execution time
unsigned long t1, t2;

void loop() {
  // put your main code here, to run repeatedly:
  // store latest sampled value from adc
  //t1 = micros();
  newest_val = adc.analogRead(2);
  // DAC is 8 bits so limit adc input to 255
  if(newest_val > 255){
    newest_val = 255; 
  }
  //newest_val = newest_val & 0011111111;
  // reset output value to be 0
  output_val = 0;
  // shift values right in input buffer
  for(int i = taps-1; i > 0; i--){
    input_samples[i] = input_samples[i-1];
  }
  // add newest value to buffer
  input_samples[0] = newest_val;
  // multiply samples by coefficients
  for(int j = 0; j < taps; j++){
    input_x_coefficients[j] = input_samples[j] * filter_coefficients[j];
  }
  // calculate output value of signal
  for(int k = 0; k < taps; k++){
    output_val = output_val + input_x_coefficients[k];
  }
  // write output value to dac
  dac.Write((unsigned int)output_val);
  //t2 = micros();
  //Serial.println(t2-t1);
}
