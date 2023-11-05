#include "signal_handler.hpp"

#include <QWindow>
#include <system/logs/logger_setup.hpp>

namespace btc2 {

SignalHandler *SignalHandler::m_instance = nullptr;

SignalHandler::SignalHandler(std::shared_ptr<QApplication> app,
                             std::string log_path, std::string crash_report_dir)
    : m_app(std::move(app)), m_log_path(std::move(log_path)),
      m_crash_report_dir(std::move(crash_report_dir)) {
  m_instance = this;
}

void SignalHandler::AppendToBuffer(const char *ptr) {
  size_t towrite = MAX_BUFFER_SIZE - m_buf_current_size;
  size_t ptr_length = std::string(ptr).length();
  towrite = towrite > ptr_length ? ptr_length : towrite;

  char *buf_ptr = m_buf;
  buf_ptr += m_buf_current_size;
  m_buf_current_size += towrite;
  std::memcpy(buf_ptr, ptr, towrite);
}

void SignalHandler::SetDatestring() {
  time_t rawtime{};
  struct tm *timeinfo{};
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  m_date_string_size = strftime(m_date_string, sizeof(m_date_string),
                                "%Y-%m-%d-%H-%M-%S", timeinfo);
}

void SignalHandler::StaticInterruptSignalHandler(int signum) {
  if (m_instance) {
    m_instance->InterruptSignalHandler(signum);
  }
}

void SignalHandler::InterruptSignalHandler(int signum) {
  static bool once = true;
  SPDLOG_TRACE("Interrupt signal ({}) received.", m_signames[signum]);
  if (once) {
    once = false;

    if (m_app != nullptr) {
      SPDLOG_INFO("Trying to stop gracefully. Press CTRL+C to force stop...");
      m_app->quit();
      m_successfully_quit = true;
    } else {
      SPDLOG_CRITICAL("App pointer is not valid. Forcing quit!");
      exit(signum);
    }
  } else {
    SPDLOG_CRITICAL("Stop forced!");
    exit(signum);
  }
}

void SignalHandler::ReportCrash(const std::string &exception_message) {
  SetDatestring();

  std::string file_name(m_date_string, m_date_string_size);
  file_name += "-crash.log";

  // Print the fault's backtrace with debug symbols in a file
  int fd = open((m_crash_report_dir + "/" + file_name).c_str(),
                O_CREAT | O_WRONLY, 0644);                            // NOLINT
  errno = 0;                                                          // NOLINT
  if (fd >= 0) {                                                      // NOLINT
    write(fd, exception_message.c_str(), exception_message.length()); // NOLINT
    write(fd, "\nDate-time: ", 12);                                   // NOLINT
    write(fd, m_date_string, m_date_string_size);                     // NOLINT
    write(fd, "\n", 1);                                               // NOLINT
    write(fd, "\nBacktrace:", 11);                                    // NOLINT
    write(fd, "\n", 1);                                               // NOLINT

#ifdef UNIX
    backtrace_symbols_fd(array, size, fd); // NOLINT
#else
    write(fd, m_buf, m_buf_current_size); // NOLINT
#endif

    write(fd, "\nLogs:", 6); // NOLINT
    write(fd, "\n", 1);      // NOLINT

    StopLogger();
    int fd_log = open(m_log_path.c_str(), O_RDONLY); // NOLINT
    bool abort_copy = false;
    if (fd_log >= 0) {
      int64_t nread = read(fd_log, m_buf, sizeof(m_buf)); // NOLINT

      while (nread > 0) {
        char *out_ptr = m_buf;
        int64_t nwritten{};

        do {
          nwritten = write(fd, out_ptr, nread); // NOLINT

          if (nwritten >= 0) {
            nread -= nwritten;
            out_ptr += nwritten;
          } else if (errno != EINTR) { // NOLINT
            close(fd_log);             // NOLINT
            abort_copy = true;
            break;
          }
        } while (nread > 0);

        if (abort_copy) {
          break;
        }

        nread = read(fd_log, m_buf, sizeof(m_buf)); // NOLINT
      }

      if (nread == 0 && !abort_copy) {
        close(fd_log); // NOLINT
      }
    }

    close(fd); // NOLINT
  }
}

void SignalHandler::StaticErrorSignalHandler(int signum) {
  if (m_instance) {
    m_instance->ErrorSignalHandler(signum);
  }
}

void SignalHandler::ErrorSignalHandler(int signum) {
  // No malloc (or function calling maloc) is authorized in a signal handler.
  // We might have a few, but we try to avoid them. Anyways, in case of a
  // SegFault or other, there is not much worse we can do here considering what
  // already happened.

  static bool once = true;
  if (once && !m_successfully_quit) {
    once = false;

    if (signum == SIGABRT) {
      std::exception_ptr eptr = std::current_exception();
      try {
        if (eptr) {
          std::rethrow_exception(eptr);
        }
        ReportCrash("Aborted: no active exception");
      } catch (const std::exception &e) {
        ReportCrash("Aborted: " + std::string(e.what()));
      } catch (const std::string &e) {
        ReportCrash("Aborted: " + e);
      } catch (char const *e) {
        ReportCrash("Aborted: " + std::string(e));
      } catch (...) {
        ReportCrash("Aborted: unhandled exception");
      }
    } else if (signum == SIGSEGV) {
      ReportCrash("SegmentationFault");
    } else {
      ReportCrash("Unhandled signal: " + m_signames[signum]);
    }
  }
  exit(signum);
}

void SignalHandler::SetupSignals() {
  if (signal(SIGINT, &SignalHandler::StaticInterruptSignalHandler) ==
      SIG_ERR) { // NOLINT
    SPDLOG_WARN("Cannot catch SIGINT");
  }
  if (signal(SIGTERM, &SignalHandler::StaticInterruptSignalHandler) ==
      SIG_ERR) { // NOLINT
    SPDLOG_WARN("Cannot catch SIGTERM");
  }

  if (signal(SIGSEGV, &SignalHandler::StaticErrorSignalHandler) ==
      SIG_ERR) { // NOLINT
    SPDLOG_WARN("Cannot catch SIGSEGV");
  }
  if (signal(SIGILL, &SignalHandler::StaticErrorSignalHandler) ==
      SIG_ERR) { // NOLINT
    SPDLOG_WARN("Cannot catch SIGILL");
  }
  if (signal(SIGFPE, &SignalHandler::StaticErrorSignalHandler) ==
      SIG_ERR) { // NOLINT
    SPDLOG_WARN("Cannot catch SIGFPE");
  }
  if (signal(SIGABRT, &SignalHandler::StaticErrorSignalHandler) ==
      SIG_ERR) { // NOLINT
    SPDLOG_WARN("Cannot catch SIGABRT");
  }

// Ignore SIGUP that happens when you launch the app through ssh and
// disconnect after
#ifdef UNIX
  signal(SIGHUP, SIG_IGN); // NOLINT
#endif
}

void SignalHandler::SetSuccessfullyQuit(int res) {
  m_successfully_quit = res == 0;
}

} // namespace btc2
