#include <stdio.h>
#include <stdlib.h>




#ifdef WIN32

#define DEFAULT_PORT "COM3"
#include <windows.h>

#else

#define DEFAULT_PORT "/dev/ttyACM0"
#include <strings.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <getopt.h>

#endif

#include <iostream>

#ifdef WIN32
bool readData(HANDLE fd, unsigned char *buff)
{
	const int data_num = 28;
	unsigned long len = 0;
	ReadFile(fd, buff, data_num, &len, 0);
	if (len == data_num) {
		if (buff[0] == 0xff && buff[1] == 0xff && buff[2] == 0x52 && buff[3] == 0x54)
		{
			PurgeComm(fd, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
#else
bool readData(int fd, unsigned char *buff)
{
	const int data_num = 28;
	int len = read(fd,buff,data_num);
	if(len <= 0)
	{
		return false;
	}
	tcflush(fd,TCIFLUSH);
	return true;
}
#endif


int main(int argc, char *argv[])
{
	std::string serial_port = DEFAULT_PORT;
	if(argc >= 2)serial_port = argv[1];
	//std::cout << serial_port.c_str() << std::endl;
	unsigned int baudrate = 115200;
	double Max_MagnRaw_x = -65536;
	double Max_MagnRaw_y = -65536;
	double Max_MagnRaw_z = -65536;
	double Min_MagnRaw_x = 65536;
	double Min_MagnRaw_y = 65536;
	double Min_MagnRaw_z = 65536;


#ifdef WIN32
	HANDLE fd;
	fd = CreateFile(serial_port.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (fd == INVALID_HANDLE_VALUE) return false;


	DCB dcb;

	GetCommState(fd, &dcb);
	dcb.BaudRate = baudrate;
	SetCommState(fd, &dcb);


	COMMTIMEOUTS cto;
	GetCommTimeouts(fd, &cto);
	cto.ReadIntervalTimeout = 1000;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 1000;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(fd, &cto);

#else
	int fd;
	struct termios newtio, oldtio;

	if(!(fd = open(serial_port.c_str(), O_RDWR ))) return false;
	ioctl(fd, TCGETS, &oldtio);
	bzero(&newtio, sizeof(newtio));

	newtio = oldtio;
	cfmakeraw(&newtio);
	cfsetispeed(&newtio, baudrate);
	cfsetospeed(&newtio, baudrate);
	ioctl(fd, TCSETS, &newtio);

#endif

	unsigned char buff[256];
	for(int i=0;i < 1000;i++)
	{
		if(i % 100 == 0)
		{
			std::cout << (1000-i)/100 << " seconds left" << std::endl;
		}
		readData(fd, buff);
		short MagnRaw_x = ((buff[23] << 8) | buff[22]);
		short MagnRaw_y = ((buff[25] << 8) | buff[24]);
		short MagnRaw_z = ((buff[27] << 8) | buff[26]);

		if (MagnRaw_x >= 32767)MagnRaw_x = MagnRaw_x - 65536;
		if (MagnRaw_y >= 32767)MagnRaw_y = MagnRaw_y - 65536;
		if (MagnRaw_z >= 32767)MagnRaw_z = MagnRaw_z - 65536;

		//std::cout << MagnRaw_x << "\t" << MagnRaw_y << "\t" << MagnRaw_z << std::endl;

		if(MagnRaw_x > Max_MagnRaw_x)Max_MagnRaw_x = MagnRaw_x;
		if(MagnRaw_y > Max_MagnRaw_y)Max_MagnRaw_y = MagnRaw_y;
		if(MagnRaw_z > Max_MagnRaw_z)Max_MagnRaw_z = MagnRaw_z;
		if(MagnRaw_x < Min_MagnRaw_x)Min_MagnRaw_x = MagnRaw_x;
		if(MagnRaw_y < Min_MagnRaw_y)Min_MagnRaw_y = MagnRaw_y;
		if(MagnRaw_z < Min_MagnRaw_z)Min_MagnRaw_z = MagnRaw_z;
#ifdef WIN32
		Sleep(10);
#else
		usleep(10000);
#endif
	}

#ifdef WIN32
	if (fd != INVALID_HANDLE_VALUE)
	{
		CloseHandle(fd);
	}
#else
	if(fd)
	{
		ioctl(fd, TCSETS, &oldtio);
		close(fd);
	}
#endif
	std::cout << "x=" << (Max_MagnRaw_x + Min_MagnRaw_x) / 2.0 << "\tmax:" << Max_MagnRaw_x << "\tmin:" << Min_MagnRaw_x << std::endl;
	std::cout << "y=" << (Max_MagnRaw_y + Min_MagnRaw_y) / 2.0 << "\tmax:" << Max_MagnRaw_y << "\tmin:" << Min_MagnRaw_y << std::endl;
	std::cout << "z=" << (Max_MagnRaw_z + Min_MagnRaw_z) / 2.0 << "\tmax:" << Max_MagnRaw_z << "\tmin:" << Min_MagnRaw_z << std::endl;
	double t;
	std::cin >> t;
	return 0;
}