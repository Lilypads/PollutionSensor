//include guards
#ifndef STATEMACHINE
#define STATEMACHINE
#endif

enum states{
AcquiringGPSFix = 0,
Active = 1,
AcquiringData = 2,
CleanUp = 3,
};

class stateMachine {
    public:
	/**
	 * Constructor
	 **/
	stateMachine();

	/**
	 * Destructor 
	 **/
	~stateMachine();

	/**
	 * go from acquiring gps fix to active state
	 **/
    void fixedAcquired();

	/**
	 * go from active to acquiring gps fix state
	 **/
    void fixedLost();

	/**
	 * go from active to acquiring data state
	 **/
    void startAcquisition();

	/**
	 * go from acquiring data to active state
	 **/
    void stopAcquisition();

	/**
	 * go to cleanup state
	 **/
    void shutdown();

    int currentState;

    private:
};