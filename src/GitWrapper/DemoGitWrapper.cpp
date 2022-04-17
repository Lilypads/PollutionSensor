#include "gitWrapper.h"
int main(){
    gitwrapperSettings s;
    strcpy(s.path, "/home/benjaminf/Repos/PollutionSensorDataDir");
    strcpy(s.remote, "git@github.com:BodeanTheZealous/PollutionSensorData.git");
    strcpy(s.sshKeysFileAbs, "~/Repos/PollutionSensor/src/GitWrapper/pollSenseKey");
    GITWRAPPER gw(s);
    gw.saveJoureyFiles();
}
