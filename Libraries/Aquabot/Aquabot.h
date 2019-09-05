
#ifndef Aquabot_h
#define Aquabot_h

#include "Arduino.h"

//{----------STEPPER---------------------------------
class Stepper
{
  public:
    Stepper(int PUL, int DIR, int ENA, int delayTime, int pulseWidth);
    void stepMotor();
	void disable();
	void enable();
	void up();
	void down();
	bool held();
	void holdOn();
	void holdOff();
	
  private:
	int _ENA;
	int _DIR;
	int _PUL;
	
	int _delayTime;
	int _pulseWidth;
	
	bool _held; //true if should be held, false if should be free
	bool _moving; //true -> moving. false -> either held or free
};
//}


//{----------LIGHT---------------------------------

class Light
{
	public:
		Light(int lightRelay);
		void lightsOn();
		void lightsOff();
	private:
		int _lightRelay;
};
//}

//{----------TIME---------------------------------

class Time
{
	public:
		Time(
		int startingHour,
		int seconds,
		int minutes,
		int days,
		int dailyErrorFast,
		int dailyErrorBehind,
		int correctedToday,
		int hourLightsOn,
		int minuteLightsOn,
		int hourLightsOff,
		int minuteLightsOff
		);
		
		bool lightStatus; //for keeping track of status of the lights (on or off)
		void updateTime();
		void checkLights();
		void printTime();
		
		int startingHour;
		int seconds;
		int minutes;
		int hours;
		int days;
		
	private:
		//variables for holding time
		
		unsigned long _timeNow;
		unsigned long _timeLast;

		
		//variable for time corrections
		int _dailyErrorFast;
		int _dailyErrorBehind;
		int _correctedToday;
	
		//variables for when lights go on or off
		int _hourLightsOn;
		int _minuteLightsOn;
		int _hourLightsOff;
		int _minuteLightsOff;
		
		
};

//}

//{----------CAMERA---------------------------------

class Camera
{
	public:
		Camera(int);
		void takePicture();
	private:
		int _camTriggerPin;
};
//}

#endif