from machine import Pin, ADC, PWM

#pwm input
pwm = PWM(28, freq=5000, duty_u16=32768)
# adc object
adc = ADC(26)
# initialize array of sampled values, x
x = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
# initialize array of coefficients
h = [0.02365176519964525,
0.05672797585059333,
0.05349705100161639,
0.005167930303005795,
-0.041724505338303436,
-0.01733900748261218,
0.04349651031327105,
0.04170342813265582,
-0.04978534689379814,
-0.09095477853845331,
0.05276334115936858,
0.31354724355941566,
0.44482581706065605,
0.31354724355941566,
0.05276334115936858,
-0.09095477853845331,
-0.04978534689379814,
0.04170342813265582,
0.04349651031327105,
-0.01733900748261218,
-0.041724505338303436,
0.005167930303005795,
0.05349705100161639,
0.05672797585059333,
0.02365176519964525]
# initialize array of inputs * coefficients
x_h = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
# output value
output = 0
def main():
    try:
        while True:
            # shift the values in the inputs array right by 1
            for i in range(len(x) - 1, 0, -1):
                x[i] = x[i - 1]
            # read newest adc sample and store in array
            x[0] = adc.read_u16()
            # multiply input values by coefficients
            for i in range(len(x_h)):
                x_h[i] = x[i] * h[i]
            # calculate filter output by summing x_h
            output = sum(x_h)
            print(output)
    except KeyboardInterrupt:
        pass
    except Exception as e:
        print(e)

if __name__ == "__main__":
    main()