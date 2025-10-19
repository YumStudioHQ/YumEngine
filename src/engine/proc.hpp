#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <sstream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#endif

namespace YumEngine {  
  class SubProcessBase {
  public:
    inline SubProcessBase() : running(false)
  #ifdef _WIN32
      , hProcess(nullptr), hStdOutRead(nullptr), hStdErrRead(nullptr)
  #endif
    {}

    inline ~SubProcessBase() {
      stop();
    }

    inline void start(const std::string& command, const std::vector<std::string>& args = {}) {
      if (running) stop();

  #ifdef _WIN32
      SECURITY_ATTRIBUTES sa{};
      sa.nLength = sizeof(sa);
      sa.bInheritHandle = TRUE;
      sa.lpSecurityDescriptor = nullptr;

      HANDLE hStdOutWrite = nullptr;
      HANDLE hStdErrWrite = nullptr;

      if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0) ||
        !CreatePipe(&hStdErrRead, &hStdErrWrite, &sa, 0)) {
        throw std::runtime_error("Failed to create pipes");
      }

      SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0);
      SetHandleInformation(hStdErrRead, HANDLE_FLAG_INHERIT, 0);

      std::string cmdline = command;
      for (const auto& arg : args) cmdline += " " + arg;

      STARTUPINFOA si{};
      si.cb = sizeof(si);
      si.dwFlags |= STARTF_USESTDHANDLES;
      si.hStdOutput = hStdOutWrite;
      si.hStdError = hStdErrWrite;
      si.hStdInput = nullptr;

      PROCESS_INFORMATION pi{};
      if (!CreateProcessA(nullptr, cmdline.data(), nullptr, nullptr, TRUE,
        0, nullptr, nullptr, &si, &pi)) {
        throw std::runtime_error("Failed to start process");
      }

      CloseHandle(hStdOutWrite);
      CloseHandle(hStdErrWrite);

      hProcess = pi.hProcess;
      CloseHandle(pi.hThread);

  #else
      int outPipe[2];
      int errPipe[2];
      if (pipe(outPipe) < 0 || pipe(errPipe) < 0) throw std::runtime_error("pipe failed");

      pid = fork();
      if (pid < 0) throw std::runtime_error("fork failed");

      if (pid == 0) { // Child
        dup2(outPipe[1], STDOUT_FILENO);
        dup2(errPipe[1], STDERR_FILENO);
        close(outPipe[0]); close(outPipe[1]);
        close(errPipe[0]); close(errPipe[1]);

        std::vector<char*> argv;
        argv.push_back(const_cast<char*>(command.c_str()));
        for (const auto& arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
        argv.push_back(nullptr);

        execvp(command.c_str(), argv.data());
        _exit(1);
      }

      // Parent
      close(outPipe[1]);
      close(errPipe[1]);
      stdoutFd = outPipe[0];
      stderrFd = errPipe[0];
      fcntl(stdoutFd, F_SETFL, O_NONBLOCK);
      fcntl(stderrFd, F_SETFL, O_NONBLOCK);
  #endif
      running = true;
    }

    inline void stop() {
      if (!running) return;

  #ifdef _WIN32
      if (hProcess) {
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
        hProcess = nullptr;
      }
      if (hStdOutRead) { CloseHandle(hStdOutRead); hStdOutRead = nullptr; }
      if (hStdErrRead) { CloseHandle(hStdErrRead); hStdErrRead = nullptr; }
  #else
      kill(pid, SIGTERM);
      int status;
      waitpid(pid, &status, 0);
      if (stdoutFd >= 0) { close(stdoutFd); stdoutFd = -1; }
      if (stderrFd >= 0) { close(stderrFd); stderrFd = -1; }
  #endif
      running = false;
    }

    inline bool isRunning() {
  #ifdef _WIN32
      if (!hProcess) return false;
      DWORD code;
      if (GetExitCodeProcess(hProcess, &code)) return code == STILL_ACTIVE;
      return false;
  #else
      if (!running) return false;
      int status;
      pid_t result = waitpid(pid, &status, WNOHANG); // Non-blocking wait
      if (result == 0) return true; // Still running
      running = false; // Process finished
      return false;
  #endif
    }

    std::string readStdOut() {
  #ifdef _WIN32
      return readPipe(hStdOutRead);
  #else
      return readFd(stdoutFd);
  #endif
    }

    std::string readStdErr() {
  #ifdef _WIN32
      return readPipe(hStdErrRead);
  #else
      return readFd(stderrFd);
  #endif
    }

  private:
    bool running;
  #ifdef _WIN32
    HANDLE hProcess;
    HANDLE hStdOutRead;
    HANDLE hStdErrRead;

    inline std::string readPipe(HANDLE pipe) {
      if (!pipe) return "";
      char buffer[4096];
      DWORD bytesRead = 0;
      std::string out;
      while (ReadFile(pipe, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0) {
        out.append(buffer, bytesRead);
      }
      return out;
    }

  #else
    pid_t pid;
    int stdoutFd = -1;
    int stderrFd = -1;

    inline std::string readFd(int fd) {
      if (fd < 0) return "";
      char buffer[4096];
      ssize_t count = read(fd, buffer, sizeof(buffer));
      if (count <= 0) return "";
      return std::string(buffer, count);
    }
  #endif
  };
}