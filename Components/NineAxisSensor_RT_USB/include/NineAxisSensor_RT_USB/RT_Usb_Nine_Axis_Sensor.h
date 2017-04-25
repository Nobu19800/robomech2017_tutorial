/*!
 * @file  RT_Usb_Nine_Axis_Sensor.h
 * @brief USB�o��9��IMU�Z���T���W���[���̒ʐM�֘A�̃N���X
 *
 */

#ifndef RT_Usb_Nine_Axis_Sensor_H
#define RT_Usb_Nine_Axis_Sensor_H

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



#include <string>


#define RT_Usb_Nine_Axis_Sensor_ACCEL_RANGE 0.0049
#define RT_Usb_Nine_Axis_Sensor_MAG_RANGE 300
#define RT_Usb_Nine_Axis_Sensor_GYRO_RANGE 0.001063




 

/**
 * @class LSM9DS0
*@brief 9���Z���TLSM9DS0��I2C�ʐM�֘A�̃N���X
*/
class RT_Usb_Nine_Axis_Sensor
{
public:
	/**
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
	RT_Usb_Nine_Axis_Sensor(bool &result, std::string serial_port = DEFAULT_PORT, unsigned int baudrate = 115200, int mx_offset = -65, int my_offset = -12, int mz_offset = -5,  double ar = 0.2, double mr = 0.2, double gr = 0.2);
	/**
	*@brief �f�X�g���N�^
	*/
	~RT_Usb_Nine_Axis_Sensor();
	
	/**
	*@brief �f�o�C�X�t�@�C���Đݒ�
	* @param serial_port �f�o�C�X�t�@�C����
	* @return ������True�A���s��False
	*/
	bool setSerialPort(std::string serial_port = DEFAULT_PORT, unsigned int baudrate = 115200);

	/**
	*@brief �f�o�C�X�t�@�C�������
	*/
	void closeSerialPort();
	
	/**
	*@brief �n���C�Z���T�̃I�t�Z�b�g�Đݒ�
	* @param mx_offset �I�t�Z�b�g(X)
	* @param my_offset �I�t�Z�b�g(Y)
	* @param mz_offset �I�t�Z�b�g(Z)
	*/
	void setOffset(int mx_offset = -65, int my_offset = -12, int mz_offset = -5);
	
	/**
	*@brief �v�����������x�擾
	* @param ax �����x(X)
	* @param ay �����x(Y)
	* @param az �����x(Z)
	*/
	void getAcc(double &ax, double &ay, double &az);
	/**
	*@brief �v�������n���C�擾
	* @param mx �n���C(X)
	* @param my �n���C(Y)
	* @param mz �n���C(Z)
	*/
	void getMagn(double &mx, double &my, double &mz);
	/**
	*@brief �v�������p���x�擾
	* @param avx �p���x(X)
	* @param avy �p���x(Y)
	* @param avz �p���x(Z)
	*/
	void getGyro(double &avx, double &avy, double &avz);
	/**
	*@brief �v���������x�擾(���݂̂Ƃ����getTempData�Ɠ���)
	* @return ���x
	*/
	double getTemp();
	/**
	*@brief �v�����������x�擾(�I�t�Z�b�g�L��)
	* @param ax �����x(X)
	* @param ay �����x(Y)
	* @param az �����x(Z)
	*/
	void getAccData(double &ax, double &ay, double &az);
	/**
	*@brief �v�������n���C�擾(�I�t�Z�b�g�L��)
	* @param mx �n���C(X)
	* @param my �n���C(Y)
	* @param mz �n���C(Z)
	*/
	void getMagnData(double &mx, double &my, double &mz);
	/**
	*@brief �v�������p���x�擾(�I�t�Z�b�g�L��)
	* @param avx �p���x(X)
	* @param avy �p���x(Y)
	* @param avz �p���x(Z)
	*/
	void getGyroData(double &avx, double &my, double &mz);
	/**
	*@brief �v���������x�擾
	* @return ���x
	*/
	double getTempData();
	/**
	*@brief �p�����v�Z
	* @param rx ���[���p
	* @param ry �s�b�`�p
	* @param rz ���[�p
	*/
	void getOrientation(double &rx, double &ry, double &rz);

	/**
	*@brief �f�[�^��ǂݍ���
	* @param buff �f�[�^
	*/
	bool readData();

	/**
	*@brief ������
	* @
	*/
	void reset();

protected:
	
	std::string _serial_port;
	double _ar, _mr, _gr;
	double lastAX, lastAY, lastAZ;
	double lastMX, lastMY, lastMZ;
	double lastGX, lastGY, lastGZ;
	
	double lastTemp;



	int _mx_offset, _my_offset, _mz_offset;
	double avx_offset, avy_offset, avz_offset;

	unsigned char _buff[256];
	unsigned int _baudrate;


#ifdef WIN32
	HANDLE _fd;
#else
	int _fd;
	struct termios _oldtio;
#endif

};





#endif