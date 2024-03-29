#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

string _ltrim(const std::string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
  return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
  FUNC_ENTRY()
  int i = 0;
  std::istringstream iss(_trim(string(cmd_line)).c_str());
  for(std::string s; iss >> s; ) {
    args[i] = (char*)malloc(s.length()+1);
    memset(args[i], 0, s.length()+1);
    strcpy(args[i], s.c_str());
    args[++i] = NULL;
  }
  return i;

  FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
  const string str(cmd_line);
  return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
  const string str(cmd_line);
  // find last character other than spaces
  unsigned int idx = str.find_last_not_of(WHITESPACE);
  // if all characters are spaces then return
  if (idx == string::npos) {
    return;
  }
  // if the command line does not end with & then return
  if (cmd_line[idx] != '&') {
    return;
  }
  // replace the & (background sign) with space and then remove all tailing spaces.
  cmd_line[idx] = ' ';
  // truncate the command line string up to the last non-space character
  cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

// TODO: Add your implementation for classes in Commands.h 



SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
	// For example:

  string cmd_s = _trim(string(cmd_line));
  string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

  if (firstWord.compare("pwd") == 0) {
    return new GetCurrDirCommand(cmd_line,path);
  }
  else if (firstWord.compare("showpid") == 0) {
    return new ShowPidCommand(cmd_line,pid);
  }
  else if (firstWord.compare("chprompt") == 0 ) {
      return new ChpromptCommand(cmd_line,&prompt);
  }
  else if (firstWord.compare("cd") == 0) {
      return new ChangeDirCommand(cmd_line,previous_path,path);
  }
  else {
    return new ExternalCommand(cmd_line);
  }

  return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
  // TODO: Add your implementation here
  Command* cmd = CreateCommand(cmd_line);
   cmd->execute();

}


//////////////////////////////////////////////////////////////////////
// START OF MY CODE

Command::Command(const char *cmd_line)
{

    std::string string1(cmd_line);
     if (string1.length() > COMMAND_LINE_MAX_LEN)
     {
         printf("to many characters");
     }
    if (_isBackgroundComamnd(cmd_line) == true)
    {
        state = BACKGROUND;
    }
    else {
        state = FOREGROUND;
    }
    num_of_arguments = _parseCommandLine(cmd_line, args);

}

void ChpromptCommand::execute() {

    if (num_of_arguments == 1)
        *prompt_name = " smash> ";
    else {
        std::string temp(args[1]);
        * prompt_name = temp + " ";
    }

}
void ShowPidCommand::execute() {

    cout << "smash pid is " << pid;
}

void GetCurrDirCommand::execute() {

    cout << path;
}

void ChangeDirCommand::execute() {

    if (num_of_arguments > 2) {
      printf("smash error: cd: too many arguments") ;
        return;
    }
    if (num_of_arguments == 1) {
        return;
    }
    std::string temp = args[1];

    if (temp == ("-") ) {

        if ( previous == nullptr) {

            printf("smash error: cd: OLDPWD not set");
            return;

        }
        std::string* temp = previous;
        previous = current;                        // update the last pwd
        const char* new_dir = temp->c_str();
        int return_value = chdir(new_dir);                                // go to previous
        if (return_value == -1){
            printf("smash error: cd: change direction failed");
        }
        return;
    }
    else {                                 // argument different from -
        previous = current;
        const char* new_dir = args[1];
        chdir(new_dir);
        //replace if syscall macro
        return;

    }

}
