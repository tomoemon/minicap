#include "SimpleServer.hpp"

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>

SimpleServer::SimpleServer(): mFd(0) {
}

SimpleServer::~SimpleServer() {
  if (mFd > 0) {
    ::close(mFd);
  }
}

int
SimpleServer::start(const char* sockname) {
  int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if (sfd < 0) {
    return sfd;
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(&addr.sun_path[1], sockname, strlen(sockname));

  if (::bind(sfd, (struct sockaddr*) &addr,
      sizeof(sa_family_t) + strlen(sockname) + 1) < 0) {
    ::close(sfd);
    return -1;
  }

  ::listen(sfd, 1);

  mFd = sfd;

  return mFd;
}

int
SimpleServer::start(int port) {
  int sfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sfd < 0) {
    return sfd;
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(struct sockaddr_in));

  addr.sin_family        = AF_INET;
  addr.sin_addr.s_addr   = htonl(INADDR_LOOPBACK);
  addr.sin_port          = htons(port);
  if (::bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0) {
    ::close(sfd);
    return -1;
  }

  ::listen(sfd, 1);

  this->port = port;
  this->mFd = sfd;
  return this->mFd;
}

int
SimpleServer::accept() {
  if (this->port > 0) {
    struct sockaddr_in iaddr;
    socklen_t iaddr_len = sizeof(iaddr);
    return ::accept(this->mFd, (struct sockaddr *)&iaddr, &iaddr_len);
  }
  else {
    struct sockaddr_un uaddr;
    socklen_t uaddr_len = sizeof(uaddr);
    return ::accept(this->mFd, (struct sockaddr *) &uaddr, &uaddr_len);
  }
}
