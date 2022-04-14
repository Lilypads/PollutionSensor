#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
// #include <git2.h>
// #include <git2/sys/repository.h>

#define WAIT_TIME 100000

#ifndef GITWRAPPER_H_
#define GITWRAPPER_H_

#define FILE_PATH_MAX 200
#define REMOTE_MAX 200

struct gitwrapperSettings{
    char path[FILE_PATH_MAX] = {};
    char remote[REMOTE_MAX] = {};
    char sshKeysFileAbs[FILE_PATH_MAX] = {};
    char name[30] = "PollutionSensor";
    char email[50] = "PollutionSensor@notAnEmail.com";
    // char journeyFileExtention[4]="pol";
};

class GITWRAPPER{
    //constructor
public:
    GITWRAPPER(gitwrapperSettings settings);
    ~GITWRAPPER();
    int saveJoureyFiles();
private:
    gitwrapperSettings settings;
    int goHome();
    char cwd[FILE_PATH_MAX];
    void exit_gw(int status);// a wrapper around exit that takes us back to the home directory
        // git_repository *repo = NULL;
    // int initialiseExistingRepo();
    // int createRepo();
    // int openRepo();
    // int creatCommit();
    // int createCommitMsg(char*);
};

#endif // GITWRAPPER_H_
