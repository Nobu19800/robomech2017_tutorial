// -*- C++ -*-
/*!
 * @file  RaspberryPiMouseController_Joystick.cpp
 * @brief Controll Raspberry Pi Mouse by Joystick
 * @date $Date$
 *
 * @author 宮本　信彦　n-miyamoto@aist.go.jp
 * 産業技術総合研究所　ロボットイノベーション研究センター
 * ロボットソフトウエアプラットフォーム研究チーム
 *
 * $Id$
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include "RaspberryPiMouseController_Joystick.h"

// Module specification
// <rtc-template block="module_spec">
static const char* raspberrypimousecontroller_joystick_spec[] =
  {
    "implementation_id", "RaspberryPiMouseController_Joystick",
    "type_name",         "RaspberryPiMouseController_Joystick",
    "description",       "Controll Raspberry Pi Mouse by Joystick",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Controller",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.forward_factor", "0.01",
    "conf.default.tangential_factor", "1.0",
	"conf.default.x_reverse", "0",
	"conf.default.y_reverse", "0",
    // Widget
    "conf.__widget__.forward_factor", "text",
    "conf.__widget__.tangential_factor", "text",
	"conf.__widget__.x_reverse", "radio",
	"conf.__widget__.y_reverse", "radio",
	// Constraints
	"conf.__constraints__.x_reverse", "(0,1)",
	"conf.__constraints__.y_reverse", "(0,1)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RaspberryPiMouseController_Joystick::RaspberryPiMouseController_Joystick(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
  m_joystick_floatIn("joystick_float", m_joystick_float),
  m_joystick_longIn("joystick_long", m_joystick_long),
	m_orientationIn("orientation", m_orientation),
    m_outOut("out", m_out)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RaspberryPiMouseController_Joystick::~RaspberryPiMouseController_Joystick()
{
}



RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
	addInPort("joystick_float", m_joystick_floatIn);
	addInPort("joystick_long", m_joystick_longIn);
  addInPort("orientation", m_orientationIn);
  
  // Set OutPort buffer
  addOutPort("out", m_outOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("forward_factor", m_forward_factor, "0.01");
  bindParameter("tangential_factor", m_tangential_factor, "1.0");
  bindParameter("x_reverse", m_x_reverse, "0");
  bindParameter("y_reverse", m_y_reverse, "0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onFinalize()
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onExecute(RTC::UniqueId ec_id)
{
	const double distance_of_wheels = 0.095;
	const double max_speed = 0.2;

	if (m_joystick_floatIn.isNew() || m_joystick_longIn.isNew())
	{
		double joy_posx = 0;
		double joy_posy = 0;
		if (m_joystick_floatIn.isNew())
		{
			m_joystick_floatIn.read();
			if (m_joystick_float.data.length() < 2)
			{
				return RTC::RTC_OK;
			}
			joy_posx = (double)m_joystick_float.data[0];
			joy_posy = (double)m_joystick_float.data[1];
		}
		else
		{
			m_joystick_longIn.read();
			if (m_joystick_long.data.length() < 2)
			{
				return RTC::RTC_OK;
			}
			joy_posx = (double)m_joystick_long.data[0];
			joy_posy = (double)m_joystick_long.data[1];
		}
		
		if (m_x_reverse == 1)joy_posx = -joy_posx;
		if (m_y_reverse == 1)joy_posy = -joy_posy;
		
		double target_value = sqrt(pow(joy_posx, 2) + pow(joy_posy, 2));
		if (target_value > 0.1)
		{
			double target_angle = atan2(joy_posy, joy_posx);
			if (m_orientationIn.isNew())
			{
				m_orientationIn.read();
				double rotate = m_orientation.data.y;
				if (rotate > M_PI)
				{
					rotate -= 2 * M_PI;
				}
				else if (rotate < -M_PI)
				{
					rotate += 2 * M_PI;
				}

				/*if (target_angle > 0 && rotate < -M_PI / 2.0)
				{
					rotate += 2 * M_PI;
				}
				else if (target_angle < 0 && rotate > M_PI / 2.0)
				{
					rotate -= 2 * M_PI;
				}*/
				//std::cout << target_angle << "\t" << rotate << std::endl;
				if(target_angle > 0 && rotate < 0)
				{
					if(target_angle - rotate > M_PI)
					{
						rotate += 2 * M_PI;
					}
				}
				else if(target_angle < 0 && rotate > 0)
				{
					if(target_angle - rotate < -M_PI)
					{
						rotate -= 2 * M_PI;
					}
				}

				m_out.data.va = m_tangential_factor*(target_angle - rotate);
				double va = sqrt(pow(m_out.data.va,2));

				double max_anguler_velocity = max_speed / (distance_of_wheels / 2.0);
				if (max_anguler_velocity < va)
				{
					if (m_out.data.va > 0)
					{
						m_out.data.va = max_anguler_velocity;
					}
					else
					{
						m_out.data.va = -max_anguler_velocity;
					}
				}

				double max_velocity = max_speed - (distance_of_wheels / 2.0)*va;
				if (max_velocity < 0)
				{
					m_out.data.vx = 0;
				}
				else
				{
					m_out.data.vx = m_forward_factor*target_value;
					if (m_out.data.vx > max_velocity)
					{
						m_out.data.vx = max_velocity;
					}
					else if (m_out.data.vx < -max_velocity)
					{
						m_out.data.vx = -max_velocity;
					}
						
				}
				setTimestamp(m_out);
				m_outOut.write();
			}
		}
		else
		{
			m_out.data.vx = 0;
			m_out.data.va = 0;
			setTimestamp(m_out);
			m_outOut.write();
		}
	}
	
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RaspberryPiMouseController_Joystick::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}



extern "C"
{
 
  void RaspberryPiMouseController_JoystickInit(RTC::Manager* manager)
  {
    coil::Properties profile(raspberrypimousecontroller_joystick_spec);
    manager->registerFactory(profile,
                             RTC::Create<RaspberryPiMouseController_Joystick>,
                             RTC::Delete<RaspberryPiMouseController_Joystick>);
  }
  
};


