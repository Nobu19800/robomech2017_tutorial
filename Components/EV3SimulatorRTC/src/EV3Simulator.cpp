// -*- C++ -*-
/*!
 * @file  EV3Simulator.cpp
 * @brief LRGO MINDSTORMS EV3 Simulator
 * @date $Date$
 *
 * @author Nobuhiko Miyamoto <n-miyamoto@aist.go.jp>
 *
 * LGPL
 *
 * $Id$
 */

#include "EV3Simulator.h"

// Module specification
// <rtc-template block="module_spec">
static const char* ev3simulator_spec[] =
  {
    "implementation_id", "EV3Simulator",
    "type_name",         "EV3Simulator",
    "description",       "LRGO MINDSTORMS EV3 Simulator",
    "version",           "1.0.0",
    "vendor",            "Miyamoto Nobuhiko",
    "category",          "Simulator",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.medium_motor_speed", "1.6",
    "conf.default.blocksConfigFile", "None",
    "conf.default.touchSensorOnLength", "0.003",
    "conf.default.lightReflectThreshold", "0.1",
    "conf.default.plane_exist", "0",
    "conf.default.plane_x", "0",
    "conf.default.plane_y", "0",
    "conf.default.plane_lx", "1.0",
    "conf.default.plane_ly", "1.0",
    "conf.default.plane_lz", "1.0",
	"conf.default.draw_time", "0.01",
	"conf.default.sampling_time", "-1",

    // Widget
	"conf.__widget__.draw_time", "text",
	"conf.__widget__.sampling_time", "text",
    "conf.__widget__.medium_motor_speed", "text",
    "conf.__widget__.blocksConfigFile", "text",
    "conf.__widget__.touchSensorOnLength", "text",
    "conf.__widget__.lightReflectThreshold", "text",
    "conf.__widget__.plane_exist", "radio",
    "conf.__widget__.plane_x", "text",
    "conf.__widget__.plane_y", "text",
    "conf.__widget__.plane_lx", "text",
    "conf.__widget__.plane_ly", "text",
    "conf.__widget__.plane_lz", "text",
    // Constraints
    "conf.__constraints__.plane_exist", "(0,1)",

	"conf.__type__.draw_time", "double",
	"conf.__type__.sampling_time", "double",
    "conf.__type__.medium_motor_speed", "double",
    "conf.__type__.blocksConfigFile", "string",
    "conf.__type__.touchSensorOnLength", "double",
    "conf.__type__.lightReflectThreshold", "double",
    "conf.__type__.plane_exist", "int",
    "conf.__type__.plane_x", "double",
    "conf.__type__.plane_y", "double",
    "conf.__type__.plane_lx", "double",
    "conf.__type__.plane_ly", "double",
    "conf.__type__.plane_lz", "double",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
EV3Simulator::EV3Simulator(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_velocity2DIn("velocity2D", m_velocity2D),
    m_angleIn("angle", m_angle),
	m_pos_updateIn("pos_update", m_pos_update),
    m_odometryOut("odometry", m_odometry),
    m_ultrasonicOut("ultrasonic", m_ultrasonic),
    m_gyroOut("gyro", m_gyro),
    m_light_reflectOut("light_reflect", m_light_reflect),
    m_touchOut("touch", m_touch)

    // </rtc-template>
{
	m_so = new EV3SimulatorObj();
	m_dt = NULL;
}

/*!
 * @brief destructor
 */
EV3Simulator::~EV3Simulator()
{
	delete m_so;
	if (m_dt)
	{
		m_dt->stop();
		delete m_dt;
	}
}



RTC::ReturnCode_t EV3Simulator::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("velocity2D", m_velocity2DIn);
  addInPort("angle", m_angleIn);
  addInPort("pos_update", m_pos_updateIn);
  
  // Set OutPort buffer
  addOutPort("odometry", m_odometryOut);
  addOutPort("ultrasonic", m_ultrasonicOut);
  addOutPort("gyro", m_gyroOut);
  addOutPort("light_reflect", m_light_reflectOut);
  addOutPort("touch", m_touchOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("draw_time", m_draw_time, "0.01");
  bindParameter("sampling_time", m_sampling_time, "-1");
  bindParameter("medium_motor_speed", m_medium_motor_speed, "1.6");
  bindParameter("blocksConfigFile", m_blocksConfigFile, "None");
  bindParameter("touchSensorOnLength", m_touchSensorOnLength, "0.003");
  bindParameter("lightReflectThreshold", m_lightReflectThreshold, "0.1");
  bindParameter("plane_exist", m_plane_exist, "0");
  bindParameter("plane_x", m_plane_x, "0");
  bindParameter("plane_y", m_plane_y, "0");
  bindParameter("plane_lx", m_plane_lx, "1.0");
  bindParameter("plane_ly", m_plane_ly, "1.0");
  bindParameter("plane_lz", m_plane_lz, "1.0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t EV3Simulator::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EV3Simulator::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EV3Simulator::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t EV3Simulator::onActivated(RTC::UniqueId ec_id)
{
	m_so->destroyRobot();

	if (m_blocksConfigFile != "None")
	{
		m_so->loadBlocksData(m_blocksConfigFile);
	}
	m_so->ev3.velocity_mmotor = m_medium_motor_speed;
	if (m_plane_exist == 1)
	{
		m_so->makePlane(m_plane_x, m_plane_y, m_plane_lx, m_plane_ly, m_plane_lz);
		m_so->makeParam(m_plane_lz);
	}
	else
	{
		m_so->makeParam(0);
	}

	
	m_so->makeRobot();

	if (m_sampling_time > 0)
	{
		m_so->setSamplingTime(m_sampling_time);
	}
	else
	{
		m_so->setSamplingTime(1.0 / this->getExecutionContext(0)->get_rate());
	}

	if (m_dt == NULL)
	{
		m_dt = new DrawThread_EV3(m_so, m_draw_time);

		m_dt->activate();
	}
	if (m_dt)
	{
		m_dt->fps = 1.0 / m_draw_time;
		m_dt->setRCPFlag();
	}

	m_so->ev3.setCurrentPosition(0, 0, 0);

  return RTC::RTC_OK;
}


RTC::ReturnCode_t EV3Simulator::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t EV3Simulator::onExecute(RTC::UniqueId ec_id)
{
	if (m_velocity2DIn.isNew())
	{
		m_velocity2DIn.read();
		m_so->ev3.setTargetVelocity(m_velocity2D.data.vx, m_velocity2D.data.va);
	}
	if (m_angleIn.isNew())
	{
		m_angleIn.read();
		m_so->ev3.target_mangle = m_angle.data;
	}
	if (m_pos_updateIn.isNew())
	{
		m_pos_updateIn.read();
		
		m_so->ev3.setCurrentPosition(m_pos_update.data.position.x, m_pos_update.data.position.y, m_pos_update.data.heading);
	}
	
	m_so->update();
	
	setTimestamp(m_odometry);
	m_odometry.data.position.x = m_so->ev3.current_px;
	m_odometry.data.position.y = m_so->ev3.current_py;
	m_odometry.data.heading = m_so->ev3.current_pa;
	m_odometryOut.write();

	setTimestamp(m_ultrasonic);
	m_ultrasonic.ranges.length(1);
	m_ultrasonic.ranges[0] = m_so->ev3.current_ultrasonicSensorData;
	m_ultrasonicOut.write();

	setTimestamp(m_gyro);
	m_gyro.data = m_so->ev3.gyro_data;
	m_gyroOut.write();

	setTimestamp(m_light_reflect);
	if (m_so->ev3.current_colourSensorData > m_lightReflectThreshold)
	{
		m_light_reflect.data = 0;
	}
	else
	{
		m_light_reflect.data = 255;
	}
	m_light_reflectOut.write();
	
	setTimestamp(m_touch);
	m_touch.data.length(2);
	m_touch.data[0] = m_so->ev3.getRightTouch(m_touchSensorOnLength);
	m_touch.data[1] = m_so->ev3.getLeftTouch(m_touchSensorOnLength);
	m_touchOut.write();


  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t EV3Simulator::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EV3Simulator::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EV3Simulator::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EV3Simulator::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t EV3Simulator::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void EV3SimulatorInit(RTC::Manager* manager)
  {
    coil::Properties profile(ev3simulator_spec);
    manager->registerFactory(profile,
                             RTC::Create<EV3Simulator>,
                             RTC::Delete<EV3Simulator>);
  }
  
};


