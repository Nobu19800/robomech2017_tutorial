// -*- C++ -*-
/*!
 * @file  ControlEducatorVehicle.h
 * @brief Control Educator Vehicle
 * @date  $Date$
 *
 * @author 宮本　信彦　n-miyamoto@aist.go.jp
 * 産業技術総合研究所　ロボットイノベーション研究センター
 * ロボットソフトウエアプラットフォーム研究チーム
 *
 * LGPL
 *
 * $Id$
 */

#ifndef CONTROLEDUCATORVEHICLE_H
#define CONTROLEDUCATORVEHICLE_H

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="port_stub_h">
// </rtc-template>

using namespace RTC;

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

/*!
 * @class ControlEducatorVehicle
 * @brief Control Educator Vehicle
 *
 * EV3のEducator
 * Vehicleで障害物の回避、滑落の回避をするためのRTCです。
 *
 */
class ControlEducatorVehicle
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  ControlEducatorVehicle(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~ControlEducatorVehicle();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);

   void back_move();
   void rotate_move(bool dir);
   bool search_ground(double &a);
   //bool rotate_sensor(double spos, double epos, double &ret);
   void turn_move(double a);
   void stop_robot();
 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 走行できる地面があると判定する超音波センサの計測値
   * - Name: sensor_height sensor_height
   * - DefaultValue: 0.20
   * - Unit: m
   */
  double m_sensor_height;
  /*!
   * 後退運動をする速さ
   * - Name: back_speed back_speed
   * - DefaultValue: 0.1
   * - Unit: m/s
   */
  double m_back_speed;
  /*!
   * 後退運動する時間
   * - Name: back_time back_time
   * - DefaultValue: 0.5
   * - Unit: s
   */
  double m_back_time;
  /*!
   * 回転運動する速さ
   * - Name: rotate_speed rotate_speed
   * - DefaultValue: 1.6
   */
  double m_rotate_speed;
  /*!
   * 回転運動する時間
   * - Name: rotate_time rotate_time
   * - DefaultValue: 0.5
   * - Unit: s
   */
  double m_rotate_time;
  /*!
   * Mモーターの動作範囲
   * - Name: medium_motor_range medium_motor_range
   * - DefaultValue: 1.0
   * - Unit: rad
   * - Constraint: 0 < x < 3.14
   */
  double m_medium_motor_range;
  /*!
   * Mモーターの速度
   * - Name: medium_motor_speed medium_motor_speed
   * - DefaultValue: 0.6
   * - Unit: rad/s
   */
  double m_medium_motor_speed;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::TimedVelocity2D m_target_velocity_in;
  /*!
   * 目標速度
   * - Type: RTC::TimedVelocity2D
   * - Unit: m/s, rad/s
   */
  InPort<RTC::TimedVelocity2D> m_target_velocity_inIn;
  RTC::RangeData m_ultrasonic;
  /*!
   * 超音波センサをレンジセンサと仮定し、要素1の距離データを格納
   * - Type: RTC::RangeData
   * - Unit: m
   */
  InPort<RTC::RangeData> m_ultrasonicIn;
  RTC::TimedBooleanSeq m_touch;
  /*!
   * タッチセンサの値をBoolean[2] で出力
   * - Type: RTC::TimedBooleanSeq
   * - Number: 2
   */
  InPort<RTC::TimedBooleanSeq> m_touchIn;
  RTC::TimedPose2D m_current_pose;
  /*!
  * 現在の位置・姿勢（角度） (x, y, θ)
  * - Type: RTC::TimedPose2D
  * - Unit: m、rad
  */
  InPort<RTC::TimedPose2D> m_current_poseIn;
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::TimedVelocity2D m_target_velocity_out;
  /*!
   * 補正後の目標速度
   * - Type: RTC::TimedVelocity2D
   * - Unit: m/s, rad/s
   */
  OutPort<RTC::TimedVelocity2D> m_target_velocity_outOut;
  RTC::TimedDouble m_angle;
  /*!
   * モーターMの角度
   * - Type: RTC::TimedDouble
   * - Unit: rad
   */
  OutPort<RTC::TimedDouble> m_angleOut;
  RTC::TimedDouble m_light_reflect;
  /*!
   * 反射光の強さ
   * - Type: RTC::TimedDouble
   */
  InPort<RTC::TimedDouble> m_light_reflectIn;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
	 double vx, vy, va;
	 bool touch_r, touch_l;
	 double range;
	bool stop_flag;
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void ControlEducatorVehicleInit(RTC::Manager* manager);
};

#endif // CONTROLEDUCATORVEHICLE_H
