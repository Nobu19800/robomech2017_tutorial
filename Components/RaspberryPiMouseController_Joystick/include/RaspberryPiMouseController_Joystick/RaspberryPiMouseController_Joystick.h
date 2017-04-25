// -*- C++ -*-
/*!
 * @file  RaspberryPiMouseController_Joystick.h
 * @brief Controll Raspberry Pi Mouse by Joystick
 * @date  $Date$
 *
 * @author 宮本　信彦　n-miyamoto@aist.go.jp
 * 産業技術総合研究所　ロボットイノベーション研究センター
 * ロボットソフトウエアプラットフォーム研究チーム
 *
 * $Id$
 */

#ifndef RASPBERRYPIMOUSECONTROLLER_JOYSTICK_H
#define RASPBERRYPIMOUSECONTROLLER_JOYSTICK_H

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
 * @class RaspberryPiMouseController_Joystick
 * @brief Controll Raspberry Pi Mouse by Joystick
 *
 * ラズパイマウスをジョイスティックで指定した方角に移動させる
 *
 */
class RaspberryPiMouseController_Joystick
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  RaspberryPiMouseController_Joystick(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~RaspberryPiMouseController_Joystick();

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


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 入力に対する直進速度の大きさ
   * - Name: forward_factor forward_factor
   * - DefaultValue: 0.01
   */
  double m_forward_factor;
  /*!
   * 入力に対する回転速度の大きさ
   * - Name: tangential_factor tangential_factor
   * - DefaultValue: 1.0
   */
  double m_tangential_factor;
  /*!
  * この値が1の時はジョイスティックのX座標を反転する
  * - Name: x_reverse x_reverse
  * - DefaultValue: 0
  * - Constraint: (0,1)
  */
  int m_x_reverse;
  /*!
  * この値が1の時はジョイスティックのY座標を反転する
  * - Name: y_reverse y_reverse
  * - DefaultValue: 0
  * - Constraint: (0,1)
  */
  int m_y_reverse;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::TimedFloatSeq m_joystick_float;
  /*!
  * 変換前のデータ
  * - Type: RTC::TimedFloatSeq
  */
  InPort<RTC::TimedFloatSeq> m_joystick_floatIn;
  RTC::TimedLongSeq m_joystick_long;
  /*!
  * 変換前のデータ
  * - Type: RTC::TimedLongSeq
  */
  InPort<RTC::TimedLongSeq> m_joystick_longIn;
  RTC::TimedOrientation3D m_orientation;
  /*!
   * ラズパイマウスの現在の姿勢
   * - Type: RTC::TimedOrientation3D
   * - Unit: m,rad
   */
  InPort<RTC::TimedOrientation3D> m_orientationIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::TimedVelocity2D m_out;
  /*!
   * 変換後のデータ
   * - Type: RTC::TimedVelocity2D
   * - Unit: m/s,rad/s
   */
  OutPort<RTC::TimedVelocity2D> m_outOut;
  
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
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void RaspberryPiMouseController_JoystickInit(RTC::Manager* manager);
};

#endif // RASPBERRYPIMOUSECONTROLLER_JOYSTICK_H
