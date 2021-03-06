#ifndef MIDAS_RAW_SOCKET_H
#define MIDAS_RAW_SOCKET_H

#include <sys/types.h>
#include <cstdint>
#include <cstdlib>

namespace midas {
int make_tcp_socket_server(char const *addrListen, uint16_t portListen);

int make_tcp_socket_client(char const *addrLocal, uint16_t portLocal, char const *addrRemote, uint16_t portRemote,
                           int timeout);

int accept_tcp_socket_client(int fdServerSocket, uint16_t *portClient, char **ipClient, bool setSocketNonblock = false);

int close_socket(int fdSocket);

ssize_t write_socket(int fdSocket, uint8_t const *buf, std::size_t numBytes);

ssize_t read_socket(int fdSocket, uint8_t *buf, std::size_t numBytes, bool readExactAmount = true);

int make_udp_socket(char const *addrLocal, uint16_t portLocal);

ssize_t send_udp(int fdSocket, uint8_t const *buf, std::size_t numBytes, char const *addrRemote, uint16_t portRemote);

ssize_t read_udp(int fdSocket, uint8_t *buf, std::size_t numBytes);
}

#endif
