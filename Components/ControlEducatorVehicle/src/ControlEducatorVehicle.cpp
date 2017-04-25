// -*- C++ -*-
/*!
 * @file  ControlEducatorVehicle.cpp
 * @brief Control Educator Vehicle
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


#define _USE_MATH_DEFINES
#include <math.h>
#include "ControlEducatorVehicle.h"

// Module specification
// <rtc-template block="module_spec">
static const char* controleducatorvehicle_spec[] =
  {
    "implementation_id", "ControlEducatorVehicle",
    "type_name",         "ControlEducatorVehicle",
    "description",       "Control Educator Vehicle",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Controller",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.sensor_height", "0.20",
    "conf.default.back_speed", "0.1",
    "conf.default.back_time", "1.0",
    "conf.default.rotate_speed", "0.8",
    "conf.default.rotate_time", "2.0",
    "conf.default.medium_motor_range", "1.6",
    // Widget
    "conf.__widget__.sensor_height", "text",
    "conf.__widget__.back_speed", "text",
    "conf.__widget__.back_time", "text",
    "conf.__widget__.rotate_speed", "text",
    "conf.__widget__.rotate_time", "text",
    "conf.__widget__.medium_motor_range", "text",
    // Constraints
    "conf.__constraints__.medium_motor_range", "0 < x < 3.14",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ControlEducatorVehicle::ControlEducatorVehicle(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_target_velocity_inIn("target_velocity_in", m_target_velocity_in),
    m_ultrasonicIn("ultrasonic", m_ultrasonic),
    m_touchIn("touch", m_touch),
	m_current_poseIn("current_pose", m_current_pose),
    m_light_reflectIn("light_reflect", m_light_reflect),
    m_target_velocity_outOut("target_velocity_out", m_target_velocity_out),
    m_angleOut("angle", m_angle)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ControlEducatorVehicle::~ControlEducatorVehicle()
{
}



RTC::ReturnCode_t ControlEducatorVehicle::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("target_velocity_in", m_target_velocity_inIn);
  addInPort("current_pose", m_current_poseIn);
  addInPort("ultrasonic", m_ultrasonicIn);
  addInPort("touch", m_touchIn);
  addInPort("light_reflect", m_light_reflectIn);
  
  
  // Set OutPort buffer
  addOutPort("target_velocity_out", m_target_velocity_outOut);
  addOutPort("angle", m_angleOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("sensor_height", m_sensor_height, "0.20");
  bindParameter("back_speed", m_back_speed, "0.1");
  bindParameter("back_time", m_back_time, "1.0");
  bindParameter("rotate_speed", m_rotate_speed, "0.8");
  bindParameter("rotate_time", m_rotate_time, "2.0");
  bindParameter("medium_motor_range", m_medium_motor_range, "1.6");
  // </rtc-template>
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ControlEducatorVehicle::onFinalize()
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ControlEducatorVehicle::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ControlEducatorVehicle::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ControlEducatorVehicle::onActivated(RTC::UniqueId ec_id)
{
	vx = 0;
	vy = 0;
	va = 0;

	touch_r = false;
	touch_l = false;
	range = 0;

	m_angle.data = 0;
	setTimestamp(m_angle);
	m_angleOut.write();

	stop_flag = false;


  return RTC::RTC_OK;
}



RTC::ReturnCode_t ControlEducatorVehicle::onDeactivated(RTC::UniqueId ec_id)
{
	stop_robot();

	

  return RTC::RTC_OK;
}

void ControlEducatorVehicle::stop_robot()
{
	m_target_velocity_out.data.vx = 0;
	m_target_velocity_out.data.vy = 0;
	m_target_velocity_out.data.va = 0;
	setTimestamp(m_target_velocity_out);
	m_target_velocity_outOut.write();
}

void ControlEducatorVehicle::back_move()
{
	m_target_velocity_out.data.vx = -m_back_speed;
	m_target_velocity_out.data.vy = 0;
	m_target_velocity_out.data.va = 0;
	setTimestamp(m_target_velocity_out);
	m_target_velocity_outOut.write();
	
	double sec, usec;
	usec = modf(m_back_time, &sec);
	
	coil::TimeValue ts((int)sec, (int)(usec*1000000.0));
	coil::sleep(ts);

	stop_robot();
}

void ControlEducatorVehicle::rotate_move(bool dir)
{
	m_target_velocity_out.data.vx = 0;
	m_target_velocity_out.data.vy = 0;
	if (dir)m_target_velocity_out.data.va = m_rotate_speed;
	else m_target_velocity_out.data.va = -m_rotate_speed;
	setTimestamp(m_target_velocity_out);
	m_target_velocity_outOut.write();
	
	double sec, usec;
	usec = modf(m_rotate_time, &sec);
	coil::TimeValue ts((int)sec, (int)(usec*1000000.0));
	coil::sleep(ts);

	stop_robot();
}

bool ControlEducatorVehicle::search_ground(double &a)
{
	m_angle.data = (-1)*m_medium_motor_range;
	setTimestamp(m_angle);
	m_angleOut.write();
	coil::TimeValue ts(2, 0);
	coil::sleep(ts);
	




	if (m_ultrasonicIn.isNew())
	{
		while(m_ultrasonicIn.isNew())m_ultrasonicIn.read();
		if (m_ultrasonic.ranges.length() >= 1)
		{
			//std::cout << m_ultrasonic.ranges[0] << "\t"  << m_medium_motor_range << std::endl;
			if (m_ultrasonic.ranges[0] < m_sensor_height)
			{
				a = m_medium_motor_range;
				return true;
			}
		}
	}
	

	m_angle.data = (-1)*-m_medium_motor_range;
	setTimestamp(m_angle);
	m_angleOut.write();
	ts = coil::TimeValue(4, 0);
	coil::sleep(ts);


	if (m_ultrasonicIn.isNew())
	{
		while(m_ultrasonicIn.isNew())m_ultrasonicIn.read();
		if (m_ultrasonic.ranges.length() >= 1)
			{
			//std::cout << m_ultrasonic.ranges[0] << "\t"  << -m_medium_motor_range << std::endl;
			if (m_ultrasonic.ranges[0] < m_sensor_height)
			{
				a = -m_medium_motor_range;
				return true;
			}
				
		}
	}
	

	m_angle.data = 0;
	setTimestamp(m_angle);
	m_angleOut.write();
	
	/*if (rotate_sensor(0, m_medium_motor_range, a))
	{
		return true;
	}
	
	if (rotate_sensor(m_medium_motor_range, -m_medium_motor_range, a))
	{
		return true;
	}
	
	if (rotate_sensor(-m_medium_motor_range, 0, a))
	{
		return true;
	}*/
	
	return false;
}

/*
bool ControlEducatorVehicle::rotate_sensor(double spos, double epos, double &ret)
{
	double last_angle = spos;
	double dir = 1;
	if (spos > epos)dir = -1;
	
	coil::TimeValue t0(coil::gettimeofday());
	int max_count = 1000000;
	coil::TimeValue ts(0, 10000);


	for (int i = 0; i < max_count; i++)
	{
		coil::TimeValue t1(coil::gettimeofday());
		double diff_time = (double)(t1 - t0);
		last_angle = spos + dir * m_medium_motor_speed * diff_time;
		m_angle.data = last_angle;
		setTimestamp(m_angle);
		m_angleOut.write();
		coil::sleep(ts);
		if (m_ultrasonicIn.isNew())
		{
			m_ultrasonicIn.read();
			if (m_ultrasonic.ranges.length() >= 1)
			{
				//std::cout << m_ultrasonic.ranges[0] << "\t"  << last_angle << std::endl;
				
				if (m_ultrasonic.ranges[0] < m_sensor_height)
				{
				//std::cout << "test" << std::endl;
				range = m_ultrasonic.ranges[0];
					ret = m_ultrasonic.config.minAngle;
					return true;
				}

			}

		}
		
		if (dir > 0 && last_angle > epos)
		{
			
			return false;
		}
		else if (dir < 0 && last_angle < epos)
		{
			
			return false;
		}

	}

	return false;
}*/

void ControlEducatorVehicle::turn_move(double a)
{
	
	

	
	int max_count = 100;
	coil::TimeValue ts(0,10000);


	double spos = 0;
	for (int i = 0; i < max_count; i++)
	{
		if (m_current_poseIn.isNew())
		{
			m_current_poseIn.read();
			spos = m_current_pose.data.heading;
			break;
		}
		coil::sleep(ts);
		if (i == max_count - 1)return;
	}
	max_count = 1000;

	double data_new_count = 0;
	double max_data_new_count = 100;
	for (int i = 0; i < max_count; i++)
	{
		if (m_current_poseIn.isNew())
		{
			while(m_current_poseIn.isNew())m_current_poseIn.read();

			double pos = m_current_pose.data.heading - spos;
			double k = 1;
			m_target_velocity_out.data.vx = 0;
			m_target_velocity_out.data.vy = 0;
			/*if (a > 0)m_target_velocity_out.data.va = m_rotate_speed;
			else m_target_velocity_out.data.va = -m_rotate_speed;*/
			double diff = (a - pos);
			double vela = k * diff;
			if(vela > m_rotate_speed)vela = m_rotate_speed;
			if(vela < -m_rotate_speed)vela = -m_rotate_speed;
			m_target_velocity_out.data.va = vela;

			setTimestamp(m_target_velocity_out);
			m_target_velocity_outOut.write();
			
		
			
			
			//std::cout << m_current_pose.data.heading << "\t" << spos << "\t" << a << "\t" << pos << std::endl;
			//std::cout << max_count << "\t" << i << std::endl;
			if(sqrt(pow(diff,2)) < 0.03)
			{
				stop_robot();
				return;
			}
			/*if (a > 0)
			{
				if (pos > a)
				{
					//std::cout << pos << "\t" << a << std::endl;
					stop_robot();
					return;
				}
			}
			else
			{
				if (pos < a)
				{
					//std::cout << pos << "\t" << a << std::endl;
					stop_robot();
					return;
				}
			}*/
			data_new_count = 0;
		}
		else
		{
			data_new_count += 1;
			if(data_new_count > max_data_new_count)
			{
				stop_robot();
				return;
			}
			coil::sleep(ts);
		}


	}
	stop_robot();
	
}



RTC::ReturnCode_t ControlEducatorVehicle::onExecute(RTC::UniqueId ec_id)
{
	if (m_target_velocity_inIn.isNew())
	{
		m_target_velocity_inIn.read();
		vx = m_target_velocity_in.data.vx;
		vy = m_target_velocity_in.data.vy;
		va = m_target_velocity_in.data.va;
	}
	if (m_touchIn.isNew())
	{
		if(m_touchIn.isNew())m_touchIn.read();
		if (m_touch.data.length() >= 2)
		{
			touch_r = m_touch.data[0];
			touch_l = m_touch.data[1];
			if (vx > 0)
			{
				if (m_touch.data[0])
				{
					stop_robot();
					back_move();
					rotate_move(true);

				}
				else if (m_touch.data[1])
				{
					stop_robot();
					back_move();
					rotate_move(false);

				}

			}
		}

	}
	if (m_ultrasonicIn.isNew())
	{
		while(m_ultrasonicIn.isNew())m_ultrasonicIn.read();
		if (m_ultrasonic.ranges.length() >= 1)
		{
			range = m_ultrasonic.ranges[0];
			if (vx > 0)
			{
				if (m_ultrasonic.ranges[0] > m_sensor_height)
				{
					stop_flag = true;
					//std::cout << m_ultrasonic.ranges[0] << "\t"  << 0 << std::endl;
					stop_robot();
					m_angle.data = 0;
					setTimestamp(m_angle);
					m_angleOut.write();

					double a = 0;
					bool ret = search_ground(a);
					m_angle.data = 0;
					setTimestamp(m_angle);
					m_angleOut.write();
					//std::cout << "ret = " << ret << std::endl;

					
					
					if (ret)
					{
						turn_move(a);
					}
					coil::TimeValue ts(2, 0);
					coil::sleep(ts);
					/*else
					{
						vx = 0;
						vy = 0;
						va = 0;
					}*/
				}
				else
				{
					stop_flag = false;
				}
			}
		}
	}

	if(m_light_reflectIn.isNew())
	{
		m_light_reflectIn.read();
		if(m_light_reflect.data < 1)
		{
			stop_flag = true;
		};

	}

	if (vx <= 0)
	{
		stop_flag = false;
	
	}
	if (stop_flag)
	{
		vx = 0;
		vy = 0;
		va = 0;
	}
	m_target_velocity_out.data.vx = vx;
	m_target_velocity_out.data.vy = vy;
	m_target_velocity_out.data.va = va;
	setTimestamp(m_target_velocity_out);
	m_target_velocity_outOut.write();
	
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ControlEducatorVehicle::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ControlEducatorVehicle::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ControlEducatorVehicle::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ControlEducatorVehicle::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ControlEducatorVehicle::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}



extern "C"
{
 
  void ControlEducatorVehicleInit(RTC::Manager* manager)
  {
    coil::Properties profile(controleducatorvehicle_spec);
    manager->registerFactory(profile,
                             RTC::Create<ControlEducatorVehicle>,
                             RTC::Delete<ControlEducatorVehicle>);
  }
  
};


