#include "gitWrapper.h"
#include <unistd.h>
#include <assert.h>
// this nonsense is needed because of incompatible c++ standard and old c functions
// requiring a non const char*
template <std::size_t N, std::size_t M>
int execve(const char* file, const char* const (&argv)[N], const char* const (&envp)[M])
{
  assert((N > 0) && (argv[N - 1] == nullptr));
  assert((M > 0) && (envp[M - 1] == nullptr));

  return execve(file, const_cast<char* const*>(argv),const_cast<char* const*>(envp));
}
GITWRAPPER::GITWRAPPER(gitwrapperSettings s){
  // these things MUST be set for anything to work
  assert(*s.path!=0);
  assert(*s.remote!=0);
  assert(*s.sshKeysFileAbs!=0);

  settings = s;

  // record the current directory so we can return to it later
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       // printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       exit(1);
   }
};

GITWRAPPER::~GITWRAPPER(){
  goHome();
};

// make sure we go home first
void GITWRAPPER::exit_gw(int status){
  goHome();
  exit(status);
};

int GITWRAPPER::goHome() {
  if (chdir(cwd) < 0) {
    perror("failed to cd");
    return -1;
  }
  return 0;
};

// does everything
int GITWRAPPER::saveJoureyFiles() {
  pid_t p;
  const char *git = "/bin/git";
  const char *touch = "/bin/touch";
  char sshCmd[FILE_PATH_MAX + 9];
  sprintf(sshCmd, "ssh -i %s", settings.sshKeysFileAbs);
  if (chdir(settings.path) < 0) {
    perror("failed to cd");
    return -1;
  }
  usleep(WAIT_TIME);

  // init
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git init\n");
    if (execl(git, git, "init", NULL) < 0) {
      perror("Failed to init git");
      exit_gw(1);
    }
  }
  usleep(WAIT_TIME);

  // branch
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git branch\n");
    if (execl(git, git, "branch", "-M", "main", NULL) < 0) {
      perror("Failed to branch git");
      exit_gw(1);
    }
  }
  usleep(WAIT_TIME);

  // remove origin
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git remove origin\n");
    if (execl(git, git, "remote", "remove", "origin", NULL) < 0) {
      perror("Git Failed to remove origin");
      exit_gw(1);
    };
  }
  usleep(WAIT_TIME);

  // add origin
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git adding origin\n");
    if (execl(git, git, "remote", "add", "origin", settings.remote, NULL) < 0) {
      perror("Git Failed to add origin");
      exit_gw(1);
    }
  }
  usleep(WAIT_TIME);

  // configure key to use
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git configuring key \n");
    if (execl(git, git, "config", "core.sshCommand", sshCmd, NULL) < 0) {
      perror("Git Failed to add configure key");
      exit_gw(1);
    }
  }
  usleep(WAIT_TIME);

  // pull
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git pull origin main\n");
    if (execl(git, git, "pull", "origin", "main", NULL) < 0) {
      perror("Git Failed to pull origin main");
      exit_gw(1);
    }
  }
  // git pull/push takes a bit longer
  usleep(WAIT_TIME * 40);

  // add
  usleep(WAIT_TIME);
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git add\n");
    if (execl(git, git, "add", "-A", NULL) < 0) {
      perror("Failed add to stage git");
      exit_gw(1);
    }
  }
  usleep(WAIT_TIME);

  // commit
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git commit\n");
    if (execl(git, git, "-c", "user.name='Pollution Sensor'", "-c", "user.email='my@email.org'", "commit", "-m", "\"AutoMesage\"", NULL) < 0) {
      perror("Failed to commit to git");
      exit_gw(1);
    };
  }
  usleep(WAIT_TIME);

  // push
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git push origin main\n");
    if (execl(git, git, "push", "origin", "main", NULL) < 0) {
      perror("Git Failed to push origin main");
      exit_gw(1);
    }
  }
  usleep(WAIT_TIME * 40);
  goHome();
  return 0;
}
