#include <TLC7524CN.h>
#include <MCP3XXX.h>
// pins for SPI ADC
uint8_t miso = 14;
uint8_t mosi = 12;
uint8_t cs = 13;
uint8_t clk = 27;
// pins for DAC
const int freq = 10000;
const int resolution = 8;
const int pwm_channel = 0;
//const int adc_input_pin = 13;
const int dac_wr = 18;
const int dac_cs = 15;
const int dac_d0 = 19;
const int dac_d1 = 21;
const int dac_d2 = 22;
const int dac_d3 = 23;
const int dac_d4 = 4;
const int dac_d5 = 5;
const int dac_d6 = 16;
const int dac_d7 = 17;
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
0.000811, 
0.003570, 
0.008920, 
0.017198, 
0.027937, 
0.039756, 
0.050594, 
0.058241, 
0.061002, 
0.058241, 
0.050594, 
0.039756, 
0.027937, 
0.017198, 
0.008920, 
0.003570, 
0.000811
};
// variable to store output
float output_val = 0;
// DAC object
TLC7524CN dac(dac_wr, dac_cs, dac_d0, dac_d1, dac_d2, dac_d3, dac_d4, dac_d5, dac_d6, dac_d7);
// ADC object
MCP3004 adc;

void setup() {
  // put your setup code here, to run once:
  analogSetWidth(9);
  // set array of samples to zero initially
  for(int i = 0; i < taps; i++){
    input_samples[i] = 0;
  }
  Serial.begin(9600);
  adc.begin(cs,mosi,miso,clk);
}

//variables to measure execution time
unsigned long t1, t2;

void loop() {
  // put your main code here, to run repeatedly:
  // store latest sampled value from adc
  //t1 = micros();
  newest_val = adc.analogRead(0);
  // DAC is 8 bits so limit adc input to 255
  if(newest_val > 255){
    newest_val = 255; 
  }
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
