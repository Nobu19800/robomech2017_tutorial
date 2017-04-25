// -*- C++ -*-
/*!
 * @file  EV3Simulator.h
 * @brief LRGO MINDSTORMS EV3 Simulator
 * @date  $Date$
 *
 * @author Nobuhiko Miyamoto <n-miyamoto@aist.go.jp>
 *
 * LGPL
 *
 * $Id$
 */

#ifndef EV3SIMULATOR_H
#define EV3SIMULATOR_H

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

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

#include "DrawThread_EV3.h"
#include "EV3SimulatorObj.h"

using namespace RTC;

/*!
 * @class EV3Simulator
 * @brief LRGO MINDSTORMS EV3 Simulator
 *
 * 宮本　信彦　n-miyamoto@aist.go.jp
 * 産業技術総合研究所　ロボットイノベーション研究センター
 * ロボットソフトウエアプラットフォーム研究チーム
 *
 */
class EV3Simulator
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  EV3Simulator(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~EV3Simulator();

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
  // virtual RTC::ReturnCode_t onFinalize();

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
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

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
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

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
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

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
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

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
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
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
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

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
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * モーターMの速度
   * - Name: medium_motor_speed medium_motor_speed
   * - DefaultValue: 1.6
   * - Unit: m/s
   */
  double m_medium_motor_speed;
  /*!
   * 障害物の配置設定ファイルの名前
   * - Name: blocksConfigFile blocksConfigFile
   * - DefaultValue: None
   */
  std::string m_blocksConfigFile;
  /*!
   * タッチセンサをオンと判定する押し込んだ距離
   * - Name: touchSensorOnLength touchSensorOnLength
   * - DefaultValue: 0.003
   * - Unit: m
   */
  double m_touchSensorOnLength;
  /*!
   * カラーセンサから物体までの距離がこの値以下になると255を出力
   * - Name: lightReflectThreshold lightReflectThreshold
   * - DefaultValue: 0.1
   * - Unit: m
   */
  double m_lightReflectThreshold;
  /*!
   * 1の時は新たに地面作成
   * - Name: plane_exist plane_exist
   * - DefaultValue: 0
   * - Constraint: (0,1)
   */
  int m_plane_exist;
  /*!
   * 地面の位置(X)
   * - Name: plane_x plane_x
   * - DefaultValue: 0
   */
  double m_plane_x;
  /*!
   * 地面の位置(Y)
   * - Name: plane_y plane_y
   * - DefaultValue: 0
   * - Unit: m
   */
  double m_plane_y;
  /*!
   * 地面の長さ
   * - Name: plane_lx plane_lx
   * - DefaultValue: 1.0
   * - Unit: m
   */
  double m_plane_lx;
  /*!
   * 地面の幅
   * - Name: plane_ly plane_ly
   * - DefaultValue: 1.0
   * - Unit: m
   */
  double m_plane_ly;
  /*!
   * 地面の高さ
   * - Name: plane_lz plane_lz
   * - DefaultValue: 1.0
   * - Unit: m
   */
  double m_plane_lz;
  /*!
  * 描画の速さ
  * - Name: fps draw_time
  * - DefaultValue: 0.01
  * - Unit: s
  */
  double m_draw_time;
  /*!
  * シミュレーションのステップ時間
  * 0以下の場合は実行周期
  * - Name: sampling_time sampling_time
  * - DefaultValue: -1
  */
  double m_sampling_time;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::TimedVelocity2D m_velocity2D;
  /*!
   * 目標速度
   * - Type: RTC::TimedVelocity2D
   * - Unit: m/s, rad/s
   */
  InPort<RTC::TimedVelocity2D> m_velocity2DIn;
  RTC::TimedDouble m_angle;
  /*!
   * モーターMの角度
   * - Type: RTC::TimedDouble
   * - Unit: rad
   */
  InPort<RTC::TimedDouble> m_angleIn;
  RTC::TimedPose2D m_pos_update;
  /*!
  * モーターMの角度
  * - Type: RTC::TimedDouble
  * - Unit: rad
  */
  InPort<RTC::TimedPose2D> m_pos_updateIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::TimedPose2D m_odometry;
  /*!
   * 現在の位置・姿勢
   * - Type: RTC::TimedPose2D
   * - Unit: m, rad
   */
  OutPort<RTC::TimedPose2D> m_odometryOut;
  RTC::RangeData m_ultrasonic;
  /*!
   * 超音波センサで計測した距離
   * - Type: RTC::RangeData
   * - Unit: m
   */
  OutPort<RTC::RangeData> m_ultrasonicOut;
  RTC::TimedDouble m_gyro;
  /*!
   * ジャイロセンサで計測した角度
   * - Type: RTC::TimedDouble
   * - Unit: rad
   */
  OutPort<RTC::TimedDouble> m_gyroOut;
  RTC::TimedDouble m_light_reflect;
  /*!
   * カラーセンサで計測した反射光の強さ
   * - Type: RTC::TimedDouble
   */
  OutPort<RTC::TimedDouble> m_light_reflectOut;
  RTC::TimedBooleanSeq m_touch;
  /*!
   * タッチセンサのオンオフ。右側が0番目の要素、左側が1番目の要素
   * - Type: RTC::TimedBooleanSeq
   */
  OutPort<RTC::TimedBooleanSeq> m_touchOut;
  
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
	 EV3SimulatorObj *m_so;
	 DrawThread_EV3 *m_dt;
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void EV3SimulatorInit(RTC::Manager* manager);
};

#endif // EV3SIMULATOR_H
