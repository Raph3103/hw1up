#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <map>
#include <iterator>
using namespace std;

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define COMMAND_LINE_MAX_LEN (80)
enum Job_state {FOREGROUND =0, BACKGROUND, STOPPED};

class Command {
// TODO: Add your data members
protected:
    int num_of_arguments;
    char* args[COMMAND_MAX_ARGS];
    std::string* command_name;
    int Job_id;
    bool finished;
 public:
  Command(const char* cmd_line);
  virtual ~Command();
  virtual void execute() = 0;
  int getId() const{
      return this->Job_id;
  }
  bool operator > ( const Command* cmd2) const {

     return this->getId() > cmd2->getId();

  }

  //virtual void prepare();
  //virtual void cleanup();
  // TODO: Add your extra methods if needed


};



class BuiltInCommand : public Command {
 public:
  BuiltInCommand(const char* cmd_line);
  virtual ~BuiltInCommand() {}
};

class ExternalCommand : public Command {
 public:
  ExternalCommand(const char* cmd_line);
  virtual ~ExternalCommand() {}
  void execute() override;
};

class PipeCommand : public Command {
  // TODO: Add your data members
 public:
  PipeCommand(const char* cmd_line);
  virtual ~PipeCommand() {}
  void execute() override;
};

class RedirectionCommand : public Command {
 // TODO: Add your data members
 public:
  explicit RedirectionCommand(const char* cmd_line);
  virtual ~RedirectionCommand() {}
  void execute() override;
  //void prepare() override;
  //void cleanup() override;
};

class ChpromptCommand : public BuiltInCommand {
public:
    std::string* prompt_name;
    ChpromptCommand(const char* cmd_line, std::string* name) : BuiltInCommand(cmd_line),prompt_name(name) {}
    virtual ~ChpromptCommand() {}
    void execute() override;
};

class ChangeDirCommand : public BuiltInCommand {
// TODO: Add your data members public:
public:
    std::string* previous;
    std::string* current;

  ChangeDirCommand(const char* cmd_line,std::string* previous_past,std::string* path)
  :BuiltInCommand(cmd_line), previous(previous_past),current(path) {}
  virtual ~ChangeDirCommand() {}
  void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand {
    std::string* path;
 public:
  GetCurrDirCommand(const char* cmd_line, std::string* path_to_print) : BuiltInCommand(cmd_line), path(path_to_print){}
  virtual ~GetCurrDirCommand() {}
  void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
 public:
    pid_t pid;
  ShowPidCommand(const char* cmd_line, pid_t pid) : pid(pid) {}
  virtual ~ShowPidCommand() {}
  void execute() override;
};

class JobsList;
class QuitCommand : public BuiltInCommand {
// TODO: Add your data members
public:
  QuitCommand(const char* cmd_line, JobsList* jobs);
  virtual ~QuitCommand() {}
  void execute() override;
};
class compare_jobs {

public:

    bool operator()(Command* cmd1, Command* cmd2) {

        return cmd2->getId() > cmd1->getId();
    }
};

class JobsList {
 public:
  class JobEntry {
   // TODO: Add your data members
  public:
      int Job_id;
      Command* command
      bool finished;
      bool IsFinished()const {
          return finished;
      }
      int blocked;

  };
 // TODO: Add your data members
 std::map<int, JobEntry*> my_list;  /// int = id

 public:

  JobsList();
  ~JobsList();
  void addJob(Command* cmd, bool isStopped = false);
  void printJobsList();
  void killAllJobs();
  void removeFinishedJobs();
  JobEntry * getJobById(int jobId);
  void removeJobById(int jobId);
  JobEntry * getLastJob(int* lastJobId);
  JobEntry *getLastStoppedJob(int *jobId);
  // TODO: Add extra methods or modify exisitng ones as needed
};

class JobsCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  JobsCommand(const char* cmd_line, JobsList* jobs);
  virtual ~JobsCommand() {}
  void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  ForegroundCommand(const char* cmd_line, JobsList* jobs);
  virtual ~ForegroundCommand() {}
  void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  BackgroundCommand(const char* cmd_line, JobsList* jobs);
  virtual ~BackgroundCommand() {}
  void execute() override;
};

class TimeoutCommand : public BuiltInCommand {
/* Optional */
// TODO: Add your data members
 public:
  explicit TimeoutCommand(const char* cmd_line);
  virtual ~TimeoutCommand() {}
  void execute() override;
};

class FareCommand : public BuiltInCommand {
  /* Optional */
  // TODO: Add your data members
 public:
  FareCommand(const char* cmd_line);
  virtual ~FareCommand() {}
  void execute() override;
};

class SetcoreCommand : public BuiltInCommand {
  /* Optional */
  // TODO: Add your data members
 public:
  SetcoreCommand(const char* cmd_line);
  virtual ~SetcoreCommand() {}
  void execute() override;
};

class KillCommand : public BuiltInCommand {
  /* Bonus */
 // TODO: Add your data members
 public:
  KillCommand(const char* cmd_line, JobsList* jobs);
  virtual ~KillCommand() {}
  void execute() override;
};

class SmallShell {

public:

  // TODO: Add your data members

  std::string prompt;
  pid_t pid;
  std::string * path;
  std::string* previous_path;

  JobsList job_list;
  SmallShell() {
      pid  = getInstance().getpid();
      path = getInstance().getcwd();
      previous_path = nullptr;
  }

  Command *CreateCommand(const char* cmd_line);
  SmallShell(SmallShell const&)      = delete; // disable copy ctor
  void operator=(SmallShell const&)  = delete; // disable = operator
  static SmallShell& getInstance() // make SmallShell singleton
  {
    static SmallShell instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
  }
  ~SmallShell();
  void executeCommand(const char* cmd_line);
  // TODO: add extra methods as needed
};


/////////////////////////// jobs functions /////////////////////////////////////////


void JobsList::removeFinishedJobs () {

    std::map<int, JobEntry *>::iterator it;
    for (it = my_list.begin(); it != my_list.end(); ++it) {
        if (it->second->IsFinished() == true) {
            my_list.erase(it);
        }
    }
}


void JobsList::printJobsList() {

    this->removeFinishedJobs();
    std::map<int, JobEntry *>::iterator it;
    for (it = my_list.begin(); it != my_list.end(); ++it) {
        if (it->second->blocked == true) {


            cout << it->second->Job_id << it->second->command->
        }
    }

}





#endif //SMASH_COMMAND_H_
