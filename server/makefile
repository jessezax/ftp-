server: main.c child.c send_fd.c send_file.c send_n.c md5.c mysql_op.c checkin.c
	g++  $^ -o $@  -I /usr/include/mysql/ -lmysqlclient -std=c++11
clean:
	rm server
