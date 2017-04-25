// -*- C++ -*-
/*!
 * @file  TestRasPiMouseCPP.cpp
 * @brief Test Raspberry Pi Mouse
 * @date $Date$
 *
 * @author 宮本　信彦　n-miyamoto@aist.go.jp
 * 産業技術総合研究所　ロボットイノベーション研究センター
 * ロボットソフトウエアプラットフォーム研究チーム
 *
 * LGPL
 *
 * $Id$
 */

#include "TestRasPiMouseCPP.h"

// Module specification
// <rtc-template block="module_spec">
static const char* testraspimousecpp_spec[] =
  {
    "implementation_id", "TestRasPiMouseCPP",
    "type_name",         "TestRasPiMouseCPP",
    "description",       "Test Raspberry Pi Mouse",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Controller",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.stop_distance", "300",

    // Widget
    "conf.__widget__.stop_distance", "text",
    // Constraints

    "conf.__type__.stop_distance", "short",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
TestRasPiMouseCPP::TestRasPiMouseCPP(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_velocity_inIn("velocity_in", m_velocity_in),
    m_distance_sensorIn("distance_sensor", m_distance_sensor),
    m_velocity_outOut("velocity_out", m_velocity_out),
    m_buzzerOut("buzzer", m_buzzer)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
TestRasPiMouseCPP::~TestRasPiMouseCPP()
{
}



RTC::ReturnCode_t TestRasPiMouseCPP::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("velocity_in", m_velocity_inIn);
  addInPort("distance_sensor", m_distance_sensorIn);
  
  // Set OutPort buffer
  addOutPort("velocity_out", m_velocity_outOut);
  addOutPort("buzzer", m_buzzerOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("stop_distance", m_stop_distance, "300");
  // </rtc-template>
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onFinalize()
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onActivated(RTC::UniqueId ec_id)
{
	for (int i = 0; i < 4; i++)
	{
		m_last_sensor_data[i] = 0;
	}
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onDeactivated(RTC::UniqueId ec_id)
{
	//停止する
	m_velocity_out.data.vx = 0;
	m_velocity_out.data.vy = 0;
	m_velocity_out.data.va = 0;
	setTimestamp(m_velocity_out);
	m_velocity_outOut.write();

	//ブザーを止める
	m_buzzer.data = 0;
	setTimestamp(m_buzzer);
	m_buzzerOut.write();

  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onExecute(RTC::UniqueId ec_id)
{
	//データを新規に受信した場合に、データをm_last_sensor_dataを格納する
	if (m_distance_sensorIn.isNew())
	{
		m_distance_sensorIn.read();
		if (m_distance_sensor.data.length() == 4)
		{
			for (int i = 0; i < 4; i++)
			{
				m_last_sensor_data[i] = m_distance_sensor.data[i];
			}
		}
	}

	if (m_velocity_inIn.isNew())
	{
		m_velocity_inIn.read();
		
		//vxが0以上(前進)のときのみ停止するか判定する
		if (m_velocity_in.data.vx > 0)
		{
			for (int i = 0; i < 4; i++)
			{
				//センサの計測値がstop_distance以上の時に前進しないようにする
				if (m_last_sensor_data[i] > m_stop_distance)
				{
					//停止する
					m_velocity_out.data.vx = 0;
					m_velocity_out.data.vy = 0;
					m_velocity_out.data.va = 0;
					setTimestamp(m_velocity_out);
					m_velocity_outOut.write();
					
					//ブザーを鳴らす
					m_buzzer.data = 50;
					setTimestamp(m_buzzer);
					m_buzzerOut.write();

					return RTC::RTC_OK;
				}
			}
		}
		//入力速度をそのまま出力
		m_velocity_out.data.vx = m_velocity_in.data.vx;
		m_velocity_out.data.vy = m_velocity_in.data.vy;
		m_velocity_out.data.va = m_velocity_in.data.va;
		setTimestamp(m_velocity_out);
		m_velocity_outOut.write();

		//ブザーを止める
		m_buzzer.data = 0;
		setTimestamp(m_buzzer);
		m_buzzerOut.write();
	}
	
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestRasPiMouseCPP::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}



extern "C"
{
 
  void TestRasPiMouseCPPInit(RTC::Manager* manager)
  {
    coil::Properties profile(testraspimousecpp_spec);
    manager->registerFactory(profile,
                             RTC::Create<TestRasPiMouseCPP>,
                             RTC::Delete<TestRasPiMouseCPP>);
  }
  
};


