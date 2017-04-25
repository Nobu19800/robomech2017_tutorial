/*!
 * @file  RT_Usb_Nine_Axis_Sensor.cpp
 * @brief USB�o��9��IMU�Z���T���W���[���̒ʐM�֘A�̃N���X
 *
 */

#define _USE_MATH_DEFINES

#include <iostream>

#include <math.h>

#include "RT_Usb_Nine_Axis_Sensor.h"



/**
*@brief USB�o��9��IMU�Z���T���W���[���̒ʐM�֘A�̃N���X�̃R���X�g���N�^
*@brief �R���X�g���N�^
* @param result ������������True�A���s��False
* @param serial_port �f�o�C�X�t�@�C����
* @param mx_offset �n���C�Z���T�̃I�t�Z�b�g(X)
* @param my_offset �n���C�Z���T�̃I�t�Z�b�g(Y)
* @param mz_offset �n���C�Z���T�̃I�t�Z�b�g(Z)
* @param ar �����x�ւ�IIR�t�B���^�̌W��
* @param mr �n���C�ւ�IIR�t�B���^�̌W��
* @param gr �p���x�ւ�IIR�t�B���^�̌W��
*/
RT_Usb_Nine_Axis_Sensor::RT_Usb_Nine_Axis_Sensor(bool &result, std::string serial_port, unsigned int baudrate, int mx_offset, int my_offset, int mz_offset,  double ar, double mr, double gr) {
	
#ifdef WIN32
	_fd = INVALID_HANDLE_VALUE;
#else
	_fd = -1;
#endif
	
	_ar = ar;
	_mr = mr;
	_gr = gr;
	
	


	_mx_offset = mx_offset;
	_my_offset = my_offset;
	_mz_offset = mz_offset;

	result = setSerialPort(serial_port, baudrate);
	//if(!result)return;

	//reset();

}

/**
*@brief USB�o��9��IMU�Z���T���W���[���̒ʐM�֘A�̃N���X�̃f�X�g���N�^
*/
RT_Usb_Nine_Axis_Sensor::~RT_Usb_Nine_Axis_Sensor() {
	closeSerialPort();
}


/**
*@brief �f�o�C�X�t�@�C���Đݒ�
* @param serial_port �f�o�C�X�t�@�C����
* @return ������True�A���s��False
*/
bool RT_Usb_Nine_Axis_Sensor::setSerialPort(std::string serial_port, unsigned int baudrate)
{
	if(_serial_port == serial_port && _baudrate == baudrate)return true;
	closeSerialPort();


#ifdef WIN32
	_fd = CreateFile(serial_port.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (_fd == INVALID_HANDLE_VALUE) return false;


	DCB dcb;

	GetCommState(_fd, &dcb);
	dcb.BaudRate = baudrate;
	SetCommState(_fd, &dcb);


	COMMTIMEOUTS cto;
	GetCommTimeouts(_fd, &cto);
	cto.ReadIntervalTimeout = 1000;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 1000;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(_fd, &cto);

#else
	struct termios newtio;

	if(!(_fd = open(serial_port.c_str(), O_RDWR ))) return false;
	ioctl(_fd, TCGETS, &_oldtio);
	bzero(&newtio, sizeof(newtio));

	newtio = _oldtio;
	cfmakeraw(&newtio);
	cfsetispeed(&newtio, baudrate);
	cfsetospeed(&newtio, baudrate);
	ioctl(_fd, TCSETS, &newtio);

	

#endif
	_serial_port = serial_port;
	_baudrate = baudrate;

	reset();
	return true;

}

/**
*@brief �f�o�C�X�t�@�C�������
*/
void RT_Usb_Nine_Axis_Sensor::closeSerialPort()
{
#ifdef WIN32
	if (_fd != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_fd);
	}
#else
	if(_fd)
	{
		ioctl(_fd, TCSETS, &_oldtio);
		close(_fd);
	}
#endif
}

/**
*@brief �n���C�Z���T�̃I�t�Z�b�g�Đݒ�
* @param mx_offset �I�t�Z�b�g(X)
* @param my_offset �I�t�Z�b�g(Y)
* @param mz_offset �I�t�Z�b�g(Z)
*/
void RT_Usb_Nine_Axis_Sensor::setOffset(int mx_offset, int my_offset, int mz_offset)
{
	_mx_offset = mx_offset;
	_my_offset = my_offset;
	_mz_offset = mz_offset;
}

/**
*@brief �v�����������x�擾
* @param ax �����x(X)
* @param ay �����x(Y)
* @param az �����x(Z)
*/
void RT_Usb_Nine_Axis_Sensor::getAcc(double &ax, double &ay, double &az)
{
	getAccData(ax,ay,az);

	lastAX = _ar*ax + (1-_ar)*lastAX;
	lastAY = _ar*ay + (1-_ar)*lastAY;
	lastAZ = _ar*az + (1-_ar)*lastAZ;
}
/**
*@brief �v�������n���C�擾
* @param mx �n���C(X)
* @param my �n���C(Y)
* @param mz �n���C(Z)
*/
void RT_Usb_Nine_Axis_Sensor::getMagn(double &mx, double &my, double &mz)
{
	getMagnData(mx,my,mz);

	lastMX = _mr*mx + (1-_mr)*lastMX;
	lastMY = _mr*my + (1-_mr)*lastMY;
	lastMZ = _mr*mz + (1-_mr)*lastMZ;
	//std::cout << lastMX << "\t" << lastMY << "\t" << lastMZ << std::endl;
}
/**
*@brief �v�������p���x�擾
* @param avx �p���x(X)
* @param avy �p���x(Y)
* @param avz �p���x(Z)
*/
void RT_Usb_Nine_Axis_Sensor::getGyro(double &avx, double &avy, double &avz)
{
	getGyroData(avx,avy,avz);
	avx -= avx_offset;
	avy -= avy_offset;
	avz -= avz_offset;

	

	lastGX = _gr*avx + (1-_gr)*lastGX;
	lastGY = _gr*avy + (1-_gr)*lastGY;
	lastGZ = _gr*avz + (1-_gr)*lastGZ;
}
/**
*@brief �v���������x�擾(���݂̂Ƃ����getTempData�Ɠ���)
* @return ���x
*/
double RT_Usb_Nine_Axis_Sensor::getTemp()
{
	lastTemp = getTempData();
	return lastTemp;
}
/**
*@brief �v�����������x�擾(�I�t�Z�b�g�L��)
* @param ax �����x(X)
* @param ay �����x(Y)
* @param az �����x(Z)
*/
void RT_Usb_Nine_Axis_Sensor::getAccData(double &ax, double &ay, double &az)
{
	short AccelRaw_x = ((_buff[9] << 8) | _buff[8]);
	short AccelRaw_y = ((_buff[11] << 8) | _buff[10]);
	short AccelRaw_z = ((_buff[13] << 8) | _buff[12]);

	if (AccelRaw_x >= 32767)AccelRaw_x = AccelRaw_x - 65536;
	if (AccelRaw_y >= 32767)AccelRaw_y = AccelRaw_y - 65536;
	if (AccelRaw_z >= 32767)AccelRaw_z = AccelRaw_z - 65536;


	ax = (double)AccelRaw_x * RT_Usb_Nine_Axis_Sensor_ACCEL_RANGE;
	ay = (double)AccelRaw_y * RT_Usb_Nine_Axis_Sensor_ACCEL_RANGE;
	az = (double)AccelRaw_z * RT_Usb_Nine_Axis_Sensor_ACCEL_RANGE;
}
/**
*@brief �v�������n���C�擾(�I�t�Z�b�g�L��)
* @param mx �n���C(X)
* @param my �n���C(Y)
* @param mz �n���C(Z)
*/
void RT_Usb_Nine_Axis_Sensor::getMagnData(double &mx, double &my, double &mz)
{
	short MagnRaw_x = ((_buff[23] << 8) | _buff[22]);
	short MagnRaw_y = ((_buff[25] << 8) | _buff[24]);
	short MagnRaw_z = ((_buff[27] << 8) | _buff[26]);

	if (MagnRaw_x >= 32767)MagnRaw_x = MagnRaw_x - 65536;
	if (MagnRaw_y >= 32767)MagnRaw_y = MagnRaw_y - 65536;
	if (MagnRaw_z >= 32767)MagnRaw_z = MagnRaw_z - 65536;
	
	
	
	MagnRaw_x += _mx_offset;
	MagnRaw_y += _my_offset;
	MagnRaw_z += _mz_offset;
	//std::cout << MagnRaw_x << "\t" << MagnRaw_y << "\t" << MagnRaw_z << std::endl;

	
	

	mx = (double)MagnRaw_x * RT_Usb_Nine_Axis_Sensor_MAG_RANGE;
	my = (double)MagnRaw_y * RT_Usb_Nine_Axis_Sensor_MAG_RANGE;
	mz = (double)MagnRaw_z * RT_Usb_Nine_Axis_Sensor_MAG_RANGE;
}
/**
*@brief �v�������p���x�擾(�I�t�Z�b�g�L��)
* @param avx �p���x(X)
* @param avy �p���x(Y)
* @param avz �p���x(Z)
*/
void RT_Usb_Nine_Axis_Sensor::getGyroData(double &avx, double &avy, double &avz)
{
	short GyroRaw_x = ((_buff[17] << 8) | _buff[16]);
	short GyroRaw_y = ((_buff[19] << 8) | _buff[18]);
	short GyroRaw_z = ((_buff[21] << 8) | _buff[20]);

	if (GyroRaw_x >= 32767)GyroRaw_x = GyroRaw_x - 65536;
	if (GyroRaw_y >= 32767)GyroRaw_y = GyroRaw_y - 65536;
	if (GyroRaw_z >= 32767)GyroRaw_z = GyroRaw_z - 65536;
	

	


	avx = (double)GyroRaw_x * RT_Usb_Nine_Axis_Sensor_GYRO_RANGE;
	avy = (double)GyroRaw_y * RT_Usb_Nine_Axis_Sensor_GYRO_RANGE;
	avz = (double)GyroRaw_z * RT_Usb_Nine_Axis_Sensor_GYRO_RANGE;


	
}
/**
*@brief �v���������x�擾
* @return ���x
*/
double RT_Usb_Nine_Axis_Sensor::getTempData()
{
	short Temp = (_buff[15] << 8) | _buff[14];

	if (Temp >= 32767)Temp = Temp - 65536;

	
	return (double)Temp/340 + 35;
}
/**
*@brief �p�����v�Z
* @param rx ���[���p
* @param ry �s�b�`�p
* @param rz ���[�p
*/
void RT_Usb_Nine_Axis_Sensor::getOrientation(double &rx, double &ry, double &rz)
{
	rx = atan2(lastAY,lastAZ)-M_PI;
	ry = atan2(-lastAX,sqrt(lastAY*lastAY+lastAZ*lastAZ));

	//std::cout << lastMX << "\t" << lastMX << "\t" << lastMZ << std::endl;

	double mxi = cos(ry)*lastMX + sin(rx)*sin(ry)*lastMX + cos(rx)*sin(ry)*lastMZ;
	double myi = cos(rx)*lastMY - sin(rx)*lastMZ;

	

	rz = atan2(-myi,mxi) - M_PI/2;
}

/**
*@brief �f�[�^��ǂݍ���
* @return ������true�A���s��false
*/
bool RT_Usb_Nine_Axis_Sensor::readData()
{
	const int data_num = 28;
#ifdef WIN32
	unsigned long len = 0;
	ReadFile(_fd, _buff, data_num, &len, 0);
	if (len == data_num) {
		if (_buff[0] == 0xff && _buff[1] == 0xff && _buff[2] == 0x52 && _buff[3] == 0x54)
		{
			PurgeComm(_fd, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
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
	
#else
	int len = read(_fd,_buff,data_num);
	if(len <= 0)
	{
		return false;
	}
	tcflush(_fd,TCIOFLUSH);
	return true;
#endif
}

/**
*@brief ������
*/
void RT_Usb_Nine_Axis_Sensor::reset()
{
	const double count = 10;
	double ax,ay,az;
	lastAX = 0;
	lastAY = 0;
	lastAZ = 0;

	double mx,my,mz;
	
	lastMX = 0;
	lastMY = 0;
	lastMZ = 0;

	double avx,avy,avz;
	
	lastGX = 0;
	lastGY = 0;
	lastGZ = 0;
	
	for(int i=0;i < count;i++)
	{

		if(readData())
		{

			getAccData(ax,ay,az);

			lastAX += ax/count;
			lastAY += ay/count;
			lastAZ += az/count;

			getMagnData(mx,my,mz);
			lastMX += mx/count;
			lastMY += my/count;
			lastMZ += mz/count;

			getGyroData(avx,avy,avz);
			lastGX += avx/count;
			lastGY += avy/count;
			lastGZ += avz/count;

#ifdef WIN32
			Sleep(10);
#else
			usleep(10000);
#endif
		}
	}

	avx_offset = lastGX;
	avy_offset = lastGY;
	avz_offset = lastGZ;

	//std::cout << avx_offset << "\t" << avy_offset << "\t" << avz_offset << std::endl;
}