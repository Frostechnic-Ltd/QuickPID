#ifndef QuickPID_h
#define QuickPID_h

class QuickPID
{

  public:

    //Constants used in some of the functions below
#define AUTOMATIC 1
#define MANUAL  0
#define DIRECT  0
#define REVERSE 1

#define FL_FX(a) (int32_t)(a*256.0)  // float to fixed point
#define FX_MUL(a,b) ((a*b)>>8)       // fixed point multiply

    // commonly used functions ************************************************************************************

    // Constructor. Links the PID to Input, Output, Setpoint and initial Tuning Parameters.
    QuickPID(int16_t*, int16_t*, int16_t*, float, float, float, float, uint8_t);

    // Overload constructor with proportional mode. Links the PID to Input, Output, Setpoint and Tuning Parameters.
    QuickPID(int16_t*, int16_t*, int16_t*, float, float, float, uint8_t);

    // Sets PID to either Manual (0) or Auto (non-0).
    void SetMode(uint8_t Mode);

    // Performs the PID calculation. It should be called every time loop() cycles. ON/OFF and calculation frequency
    // can be set using SetMode and SetSampleTime respectively.
    bool Compute();

    // Automatically determines and sets the tuning parameters Kp, Ki and Kd. Use this in the setup loop.
    void AutoTune(int, int, int, int, uint32_t);

    // Sets and clamps the output to a specific range (0-255 by default).
    void SetOutputLimits(int16_t, int16_t);

    // available but not commonly used functions ******************************************************************

    // While most users will set the tunings once in the constructor, this function gives the user the option of
    // changing tunings during runtime for Adaptive control.
    void SetTunings(float, float, float);

    // Overload for specifying proportional mode.
    void SetTunings(float, float, float, float);

    // Sets the Direction, or "Action" of control. DIRECT means the output will increase when error is positive.
    // REVERSE means the opposite. It's very unlikely that this will be needed once it is set in the constructor.
    void SetControllerDirection(uint8_t);

    // Sets the sample time in milliseconds with which each PID calculation is performed. Default is 100.
    void SetSampleTimeUs(uint32_t);

    //Display functions ******************************************************************************************
    float GetKp();         // These functions query the pid for interal values. They were created mainly for
    float GetKi();         // the pid front-end, where it's important to know what is actually inside the PID.
    float GetKd();
    float GetKu();         // Ultimate Gain
    float GetTu();         // Ultimate Period
    float GetTd();         // Dead Time
    uint8_t GetMode();
    uint8_t GetDirection();

    // Utility functions ******************************************************************************************
    int analogReadFast(int);
    float analogReadAvg(int);

  private:
    void Initialize();
    int16_t Saturate(int16_t);
    void CheckPeak(int);
    void StepUp(int, int, uint32_t);
    void StepDown(int, int, uint32_t);
    void Stabilize(int, int, uint32_t);

    float dispKp;          // We'll hold on to the tuning parameters for display purposes.
    float dispKi;
    float dispKd;
    float dispKu;
    float dispTu;
    float dispTd;

    float pOn;             // proportional mode (0-1) default = 1, 100% Proportional on Error
    float kp;              // (P)roportional Tuning Parameter
    float ki;              // (I)ntegral Tuning Parameter
    float kd;              // (D)erivative Tuning Parameter
    float kpi;             // proportional on error amount
    float kpd;             // proportional on measurement amount

    uint8_t controllerDirection;

    int16_t *myInput;      // Pointers to the Input, Output, and Setpoint variables. This creates a
    int16_t *myOutput;     // hard link between the variables and the PID, freeing the user from having
    int16_t *mySetpoint;   // to constantly tell us what these values are. With pointers we'll just know.

    uint32_t sampleTimeUs, lastTime;
    int16_t outMin, outMax, error;
    int16_t lastInput, outputSum;
    bool inAuto;

    // AutoTune
    float peakHigh, peakLow;
    const word readPeriod = 1667;
    const byte outputStep = 1;
    const byte hysteresis = 1;
    const int atSetpoint = 341;  // 1/3 of 10-bit ADC matches 8-bit PWM value of 85 for symetrical waveform
    const int atOutput = 85;

};

#endif
