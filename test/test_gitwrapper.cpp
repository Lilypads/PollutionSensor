#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testGitWrapper
#include <boost/test/unit_test.hpp>
#include "gitWrapper.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <unistd.h>
#include <math.h>

bool nearly_equal(double a, double b)
{
    fprintf(stderr,"[%f,%f]",a,b);
  return std::nextafter(a, std::numeric_limits<double>::lowest()) <= b
    && std::nextafter(a, std::numeric_limits<double>::max()) >= b;
}
int recursive_delete(const char *dir)
{
    int ret = 0;
    FTS *ftsp = NULL;
    FTSENT *curr;

    // Cast needed (in C) because fts_open() takes a "char * const *", instead
    // of a "const char * const *", which is only allowed in C++. fts_open()
    // does not modify the argument.
    char *files[] = { (char *) dir, NULL };

    // FTS_NOCHDIR  - Avoid changing cwd, which could cause unexpected behavior
    //                in multithreaded programs
    // FTS_PHYSICAL - Don't follow symlinks. Prevents deletion of files outside
    //                of the specified directory
    // FTS_XDEV     - Don't cross filesystem boundaries
    ftsp = fts_open(files, FTS_NOCHDIR | FTS_PHYSICAL | FTS_XDEV, NULL);
    if (!ftsp) {
        fprintf(stderr, "%s: fts_open failed: %s\n", dir, strerror(errno));
        ret = -1;
        goto finish;
    }

    while ((curr = fts_read(ftsp))) {
        switch (curr->fts_info) {
        case FTS_NS:
        case FTS_DNR:
        case FTS_ERR:
            fprintf(stderr, "%s: fts_read error: %s\n",
                    curr->fts_accpath, strerror(curr->fts_errno));
            break;

        case FTS_DC:
        case FTS_DOT:
        case FTS_NSOK:
            // Not reached unless FTS_LOGICAL, FTS_SEEDOT, or FTS_NOSTAT were
            // passed to fts_open()
            break;

        case FTS_D:
            // Do nothing. Need depth-first search, so directories are deleted
            // in FTS_DP
            break;

        case FTS_DP:
        case FTS_F:
        case FTS_SL:
        case FTS_SLNONE:
        case FTS_DEFAULT:
            if (remove(curr->fts_accpath) < 0) {
                fprintf(stderr, "%s: Failed to remove: %s\n",
                        curr->fts_path, strerror(curr->fts_errno));
                ret = -1;
            }
            break;
        }
    }

finish:
    if (ftsp) {
        fts_close(ftsp);
    }

    return ret;
}

int gitInitRepo(char* path){
    // get cwd so we return home after operation
   char cwd[200];
    pid_t p;
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       fprintf(stderr,"Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       exit(1);
   }
  const char *git = "/bin/git";
  if (chdir(path) < 0) {
    perror("failed to cd");
    return -1;
  }
  usleep(10000);
  // init
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "Git init -b Master\n");
    if (execl(git, git, "init", NULL) < 0) {
      perror("Failed to init git");
      exit(1);
    }
  }
  // creat main branch
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "git branch\n");
    if (execl(git, git, "branch","main", NULL) < 0) {
      perror("Failed to init git");
      exit(1);
    }
  }
  usleep(10000);
  if (chdir(cwd) < 0) {
    perror("failed to cd home");
    exit(-1);
  }
  usleep(10000);
  return 0;
};

int gitCheckoutMain(char* path){
    // get cwd so we return home after operation
   char cwd[200];
    pid_t p;
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       fprintf(stderr,"Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       throw;
    }
  const char *git = "/bin/git";
  if (chdir(path) < 0) {
    perror("failed to cd");
    fprintf(stderr, "target: %s\n",path);
    throw;
  }
  usleep(10000);
  // checkout main
  p = fork();
  if (p == -1) {
    fprintf(stderr, "fork failed\n");
  }
  if (p == 0) {
    fprintf(stderr, "git checkout main \n");
    if (execl(git, git, "checkout","main", NULL) < 0) {
      perror("Git Failed to checkout main");
      throw;
    }
  }
  usleep(10000);
  if (chdir(cwd) < 0) {
    perror("failed to cd home");
    fprintf(stderr, "target: %s\n",path);
    exit(-1);
  }
  usleep(10000);
  return 0;
};

char remote[] = "./GitWrapperTestDir/remote";
char remoteGithub[] = "git@github.com:BodeanTheZealous/PollutionSensorData.git";
char remote_rel2Repo[] = "../remote";
char blankRepo[] = "./GitWrapperTestDir/blank";
char existingRepo[] = "./GitWrapperTestDir/existing";
char key[] = "../../../src/GitWrapper/pollSenseKey";
char testFileName_b[] = "./GitWrapperTestDir/blank/testfile_b";
char testRemoteFileName_b[] = "./GitWrapperTestDir/remote/testfile_b";
char testFileName_e[] = "./GitWrapperTestDir/existing/testfile_e";
char testRemoteFileName_e[] = "./GitWrapperTestDir/remote/testfile_e";

void ceateTestDirectories()   {
    FILE* file_ptr;
    fprintf(stderr,"creating fixture\n");
    // clear and recreate existing test repos
    int err[6];
    // remote
    err[0] = recursive_delete(remote);
    if (mkdir(remote, 0700)!=0){
        perror("failed to mkdir remote");
        throw;
    };
    gitInitRepo(remote);

    // blank test repo
    err[2] = recursive_delete(blankRepo);
    if (mkdir(blankRepo, 0700)!=0){
        perror("failed to mkdir blank");
        throw;
    };
    fclose(fopen(testFileName_b,"w"));

    // existing repo
    err[4] = recursive_delete(existingRepo);
    if (mkdir(existingRepo, 0700)!=0){
        perror("failed to mkdir exitsting:");
        throw;
    };
    fclose(fopen(testFileName_e,"w"));
    gitInitRepo(existingRepo);
    fprintf(stderr,"finished fixture\n");
}




BOOST_AUTO_TEST_SUITE(testGitCommitToRemote)

 BOOST_AUTO_TEST_CASE(testBlankRepo) {
  ceateTestDirectories();
  gitwrapperSettings s;
  strcpy(s.path, blankRepo);
  strcpy(s.remote, remote_rel2Repo); // git doesnt like the ./ bit
  strcpy(s.sshKeysFileAbs, key);

  // push to remote
  GITWRAPPER gw(s);
  gw.saveJourneyFiles();
  // we have to checkout main on the remote to see new file
  gitCheckoutMain(remote);
  // test the new file exists in the remote repo
  BOOST_CHECK_EQUAL(access(testRemoteFileName_b, F_OK ),0);
}

BOOST_AUTO_TEST_CASE(testExistingRepo) {
  ceateTestDirectories();
  gitwrapperSettings s;
    strcpy(s.path, existingRepo);
    strcpy(s.remote, remote_rel2Repo);
    strcpy(s.sshKeysFileAbs, key);

    // push to remote
    GITWRAPPER gw(s);
    gw.saveJourneyFiles();
    // we have to checkout main on the remote to see new file
    gitCheckoutMain(remote);
    // test the new file exists in the remote repo
  BOOST_CHECK_EQUAL(access(testRemoteFileName_e, F_OK ),0);
}


BOOST_AUTO_TEST_SUITE_END()
