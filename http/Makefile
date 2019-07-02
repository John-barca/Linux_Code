FLAG=-std=c++11 -lpthread -ljsoncpp

.PHONY:all
all:calc_server calc_client

calc_server:calc_server.cc tcp_thread.hpp tcp_socket.hpp
	g++ calc_server.cc -o calc_server $(FLAG)

calc_client:calc_client.cc tcp_thread.hpp tcp_socket.hpp tcp_client.hpp
	g++ calc_client.cc -o calc_client $(FLAG)

.PHONY:clean
clean:
	rm calc_client calc_server
