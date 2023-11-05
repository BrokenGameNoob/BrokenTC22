#pragma once

#include <QApplication>
#include <csignal>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <string>

#define MAX_BUFFER_SIZE 40960

namespace btc2 {

class SignalHandler {
public:
  SignalHandler(std::shared_ptr<QApplication> app, std::string log_path,
                std::string crash_report_dir);
  void SetupSignals();
  void SetSuccessfullyQuit(int res);
  void AppendToBuffer(const char *ptr);

private:
  void SetDatestring();
  void InterruptSignalHandler(int signum);
  void ErrorSignalHandler(int signum);
  void ReportCrash(const std::string &exception_message);

  static void StaticInterruptSignalHandler(int signum);
  static void StaticErrorSignalHandler(int signum);

  std::string m_signames[32] = {
      "INVALID", "SIGHUP",  "SIGINT",    "SIGQUIT", "SIGILL",    "SIGTRAP",
      "SIGABRT", "SIGBUS",  "SIGFPE",    "SIGKILL", "SIGUSR1",   "SIGSEGV",
      "SIGUSR2", "SIGPIPE", "SIGALRM",   "SIGTERM", "SIGSTKFLT", "SIGCHLD",
      "SIGCONT", "SIGSTOP", "SIGTSTP",   "SIGTTIN", "SIGTTOU",   "SIGURG",
      "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH",  "SIGPOLL",
      "SIGPWR",  "SIGSYS"};

  char m_date_string[100] = {0};
  char m_buf[MAX_BUFFER_SIZE] = {0};
  size_t m_buf_current_size = 0;
  size_t m_date_string_size = 0;
  bool m_successfully_quit = false;

  std::shared_ptr<QApplication> m_app;
  std::string m_log_path;
  std::string m_crash_report_dir;

  static SignalHandler *m_instance;
};

} // namespace btc2
