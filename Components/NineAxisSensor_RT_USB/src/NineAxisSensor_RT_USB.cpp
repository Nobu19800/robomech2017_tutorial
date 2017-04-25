// -*- C++ -*-
/*!
 * @file  NineAxisSensor_RT_USB.cpp
 * @brief RT-USB-9AXIS-00 Component
 * @date $Date$
 *
 * @author 宮本信彦
 * n-miyamoto@aist.go.jp
 * 産業技術総合研究所　
 * ロボットイノベーション研究センター　
 * ロボットソフトウェアプラットフォーム研究チーム
 *
 * LGPL
 *
 * $Id$
 */

#include "NineAxisSensor_RT_USB.h"

// Module specification
// <rtc-template block="module_spec">
static const char* nineaxissensor_rt_usb_spec[] =
  {
    "implementation_id", "NineAxisSensor_RT_USB",
    "type_name",         "NineAxisSensor_RT_USB",
    "description",       "RT-USB-9AXIS-00 Component",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Sensor",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.rotOffset", "0",
    "conf.default.magnOffsetX", "-65",
    "conf.default.magnOffsetY", "-60",
    "conf.default.magnOffsetZ", "-5",
	"conf.default.serial_port", DEFAULT_PORT,
    // Widget
    "conf.__widget__.rotOffset", "text",
    "conf.__widget__.magnOffsetX", "text",
    "conf.__widget__.magnOffsetY", "text",
    "conf.__widget__.magnOffsetZ", "text",
    "conf.__widget__.serial_port", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
NineAxisSensor_RT_USB::NineAxisSensor_RT_USB(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_accOut("acc", m_acc),
    m_magnOut("magn", m_magn),
    m_gyroOut("gyro", m_gyro),
    m_tempOut("temp", m_temp),
    m_rotOut("rot", m_rot)

    // </rtc-template>
{
	rt_usb_sensor = NULL;
}

/*!
 * @brief destructor
 */
NineAxisSensor_RT_USB::~NineAxisSensor_RT_USB()
{
}



RTC::ReturnCode_t NineAxisSensor_RT_USB::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("acc", m_accOut);
  addOutPort("magn", m_magnOut);
  addOutPort("gyro", m_gyroOut);
  addOutPort("temp", m_tempOut);
  addOutPort("rot", m_rotOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("rotOffset", m_rotOffset, "0");
  bindParameter("magnOffsetX", m_magnOffsetX, "-65");
  bindParameter("magnOffsetY", m_magnOffsetY, "-60");
  bindParameter("magnOffsetZ", m_magnOffsetZ, "-5");
  bindParameter("serial_port", m_serial_port, DEFAULT_PORT);
  // </rtc-template>
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onFinalize()
{
	if(rt_usb_sensor)
	{
		delete rt_usb_sensor;
	}
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onActivated(RTC::UniqueId ec_id)
{
	if(rt_usb_sensor == NULL)
	{
		bool result = true;
		rt_usb_sensor = new RT_Usb_Nine_Axis_Sensor(result, m_serial_port, 115200, m_magnOffsetX, m_magnOffsetY, m_magnOffsetZ);

		if(result != true)
		{
			delete rt_usb_sensor;
			rt_usb_sensor = NULL;
			return RTC::RTC_ERROR;
		}

	}
	else
	{
		rt_usb_sensor->setSerialPort(m_serial_port);
		rt_usb_sensor->setOffset(m_magnOffsetX, m_magnOffsetY, m_magnOffsetZ);
		
	}
	
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onExecute(RTC::UniqueId ec_id)
{
	double ax = 0;
	double ay = 0;
	double az = 0;

	double mx = 0;
	double my = 0;
	double mz = 0;

	double avx = 0;
	double avy = 0;
	double avz = 0;

	double rx = 0;
	double ry = 0;
	double rz = 0;

	if(rt_usb_sensor)
	{

		if(rt_usb_sensor->readData())
		{

			rt_usb_sensor->getAcc(ax,ay,az);
			m_acc.data.ax = ax;
			m_acc.data.ay = ay;
			m_acc.data.az = az;
			setTimestamp(m_acc);
			m_accOut.write();


			rt_usb_sensor->getMagn(mx,my,mz);
			m_magn.data.length(3);
			m_magn.data[0] = mx;
			m_magn.data[1] = my;
			m_magn.data[2] = mz;
			setTimestamp(m_magn);
			m_magnOut.write();


			rt_usb_sensor->getGyro(avx,avy,avz);
			m_gyro.data.avx = avx;
			m_gyro.data.avy = avy;
			m_gyro.data.avz = avz;
			
			setTimestamp(m_gyro);
			m_gyroOut.write();

			m_temp.data = rt_usb_sensor->getTemp();
			setTimestamp(m_temp);
			m_tempOut.write();

			rt_usb_sensor->getOrientation(rx,ry,rz);
			m_rot.data.r = rx;
			m_rot.data.p = ry;
			m_rot.data.y = rz - m_rotOffset;

			setTimestamp(m_rot);
			m_rotOut.write();
		}
	}
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t NineAxisSensor_RT_USB::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}



extern "C"
{
 
  void NineAxisSensor_RT_USBInit(RTC::Manager* manager)
  {
    coil::Properties profile(nineaxissensor_rt_usb_spec);
    manager->registerFactory(profile,
                             RTC::Create<NineAxisSensor_RT_USB>,
                             RTC::Delete<NineAxisSensor_RT_USB>);
  }
  
};


