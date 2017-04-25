// -*- C++ -*-
/*!
 * @file  TestEV3CPP.cpp
 * @brief Test EV3
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

#include "TestEV3CPP.h"

// Module specification
// <rtc-template block="module_spec">
static const char* testev3cpp_spec[] =
  {
    "implementation_id", "TestEV3CPP",
    "type_name",         "TestEV3CPP",
    "description",       "Test EV3",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Controller",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.sound_output", "beep",

    // Widget
    "conf.__widget__.sound_output", "text",
    // Constraints

    "conf.__type__.sound_output", "string",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
TestEV3CPP::TestEV3CPP(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_velocity_inIn("velocity_in", m_velocity_in),
    m_touchIn("touch", m_touch),
    m_velocity_outOut("velocity_out", m_velocity_out),
    m_soundOut("sound", m_sound)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
TestEV3CPP::~TestEV3CPP()
{
}



RTC::ReturnCode_t TestEV3CPP::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("velocity_in", m_velocity_inIn);
  addInPort("touch", m_touchIn);
  
  // Set OutPort buffer
  addOutPort("velocity_out", m_velocity_outOut);
  addOutPort("sound", m_soundOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("sound_output", m_sound_output, "beep");
  // </rtc-template>
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onFinalize()
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onActivated(RTC::UniqueId ec_id)
{
	for (int i = 0; i < 2; i++)
	{
		m_last_sensor_data[i] = false;
	}
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onDeactivated(RTC::UniqueId ec_id)
{
	//停止する
	m_velocity_out.data.vx = 0;
	m_velocity_out.data.vy = 0;
	m_velocity_out.data.va = 0;
	setTimestamp(m_velocity_out);
	m_velocity_outOut.write();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onExecute(RTC::UniqueId ec_id)
{
	//データを新規に受信した場合に、データをm_last_sensor_dataを格納する
	if (m_touchIn.isNew())
	{
		m_touchIn.read();
		if (m_touch.data.length() == 2)
		{
			for (int i = 0; i < 2; i++)
			{
				//タッチセンサがOFFからONになった時に音を鳴らす
				if (!m_last_sensor_data[i] && m_touch.data[i])
				{
					m_sound.data = m_sound_output.c_str();
					setTimestamp(m_sound);
					m_soundOut.write();


				}
				m_last_sensor_data[i] = m_touch.data[i];
			}
		}
	}
	if (m_velocity_inIn.isNew())
	{
		m_velocity_inIn.read();
		//vxが0以上(前進)のときのみ停止するか判定する
		if (m_velocity_in.data.vx > 0)
		{
			for (int i = 0; i < 2; i++)
			{
				//タッチセンサがONの時に停止する
				if (m_last_sensor_data[i])
				{
					//停止する
					m_velocity_out.data.vx = 0;
					m_velocity_out.data.vy = 0;
					m_velocity_out.data.va = 0;
					setTimestamp(m_velocity_out);
					m_velocity_outOut.write();


					return RTC::RTC_OK;
				}
			}
		}
		//入力速度をそのまま出力する
		m_velocity_out.data.vx = m_velocity_in.data.vx;
		m_velocity_out.data.vy = m_velocity_in.data.vy;
		m_velocity_out.data.va = m_velocity_in.data.va;
		setTimestamp(m_velocity_out);
		m_velocity_outOut.write();


	}

  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TestEV3CPP::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}



extern "C"
{
 
  void TestEV3CPPInit(RTC::Manager* manager)
  {
    coil::Properties profile(testev3cpp_spec);
    manager->registerFactory(profile,
                             RTC::Create<TestEV3CPP>,
                             RTC::Delete<TestEV3CPP>);
  }
  
};


