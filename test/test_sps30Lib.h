#define NO_HARDWARE
#define NDEBUG
     
//checks if the difference exceed threshold instead of just checking equal >> it causes precision error with floatings
bool isExceedThreshold(float input1, float input2, float threshold){
    float diff;
    diff = input1-input2;
    if(-threshold<diff&&diff<threshold){
        return false;
    }
    else{
        return true;
    }
}
