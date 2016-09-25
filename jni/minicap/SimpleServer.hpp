#ifndef MINICAP_SIMPLE_SERVER_HPP
#define MINICAP_SIMPLE_SERVER_HPP

class SimpleServer {
public:
  SimpleServer();
  ~SimpleServer();

  int
  start(const char* sockname);

  int
  start(int port);

  int accept();

private:
  int port = 0;
  int mFd;
};

#endif
