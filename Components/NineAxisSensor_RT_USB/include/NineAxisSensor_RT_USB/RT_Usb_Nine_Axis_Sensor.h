/*!
 * @file  RT_Usb_Nine_Axis_Sensor.h
 * @brief USB出力9軸IMUセンサモジュールの通信関連のクラス
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
*@brief 9軸センサLSM9DS0のI2C通信関連のクラス
*/
class RT_Usb_Nine_Axis_Sensor
{
public:
	/**
	*@brief コンストラクタ
	* @param result 初期化成功でTrue、失敗でFalse
	* @param serial_port デバイスファイル名
	* @param mx_offset 地磁気センサのオフセット(X)
	* @param my_offset 地磁気センサのオフセット(Y)
	* @param mz_offset 地磁気センサのオフセット(Z)
	* @param ar 加速度へのIIRフィルタの係数
	* @param mr 地磁気へのIIRフィルタの係数
	* @param gr 角速度へのIIRフィルタの係数
	*/
	RT_Usb_Nine_Axis_Sensor(bool &result, std::string serial_port = DEFAULT_PORT, unsigned int baudrate = 115200, int mx_offset = -65, int my_offset = -12, int mz_offset = -5,  double ar = 0.2, double mr = 0.2, double gr = 0.2);
	/**
	*@brief デストラクタ
	*/
	~RT_Usb_Nine_Axis_Sensor();
	
	/**
	*@brief デバイスファイル再設定
	* @param serial_port デバイスファイル名
	* @return 成功でTrue、失敗でFalse
	*/
	bool setSerialPort(std::string serial_port = DEFAULT_PORT, unsigned int baudrate = 115200);

	/**
	*@brief デバイスファイルを閉じる
	*/
	void closeSerialPort();
	
	/**
	*@brief 地磁気センサのオフセット再設定
	* @param mx_offset オフセット(X)
	* @param my_offset オフセット(Y)
	* @param mz_offset オフセット(Z)
	*/
	void setOffset(int mx_offset = -65, int my_offset = -12, int mz_offset = -5);
	
	/**
	*@brief 計測した加速度取得
	* @param ax 加速度(X)
	* @param ay 加速度(Y)
	* @param az 加速度(Z)
	*/
	void getAcc(double &ax, double &ay, double &az);
	/**
	*@brief 計測した地磁気取得
	* @param mx 地磁気(X)
	* @param my 地磁気(Y)
	* @param mz 地磁気(Z)
	*/
	void getMagn(double &mx, double &my, double &mz);
	/**
	*@brief 計測した角速度取得
	* @param avx 角速度(X)
	* @param avy 角速度(Y)
	* @param avz 角速度(Z)
	*/
	void getGyro(double &avx, double &avy, double &avz);
	/**
	*@brief 計測した温度取得(現在のところはgetTempDataと同じ)
	* @return 温度
	*/
	double getTemp();
	/**
	*@brief 計測した加速度取得(オフセット有り)
	* @param ax 加速度(X)
	* @param ay 加速度(Y)
	* @param az 加速度(Z)
	*/
	void getAccData(double &ax, double &ay, double &az);
	/**
	*@brief 計測した地磁気取得(オフセット有り)
	* @param mx 地磁気(X)
	* @param my 地磁気(Y)
	* @param mz 地磁気(Z)
	*/
	void getMagnData(double &mx, double &my, double &mz);
	/**
	*@brief 計測した角速度取得(オフセット有り)
	* @param avx 角速度(X)
	* @param avy 角速度(Y)
	* @param avz 角速度(Z)
	*/
	void getGyroData(double &avx, double &my, double &mz);
	/**
	*@brief 計測した温度取得
	* @return 温度
	*/
	double getTempData();
	/**
	*@brief 姿勢を計算
	* @param rx ロール角
	* @param ry ピッチ角
	* @param rz ヨー角
	*/
	void getOrientation(double &rx, double &ry, double &rz);

	/**
	*@brief データを読み込む
	* @param buff データ
	*/
	bool readData();

	/**
	*@brief 初期化
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