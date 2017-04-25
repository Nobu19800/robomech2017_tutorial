#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -*- Python -*-

"""
 @file TestRasPiMousePy.py
 @brief Test Raspberry Pi Mouse
 @date $Date$

 @author 宮本　信彦　n-miyamoto@aist.go.jp
 産業技術総合研究所　ロボットイノベーション研究センター
 ロボットソフトウエアプラットフォーム研究チーム

 LGPL

"""
import sys
import time
sys.path.append(".")

# Import RTM module
import RTC
import OpenRTM_aist


# Import Service implementation class
# <rtc-template block="service_impl">

# </rtc-template>

# Import Service stub modules
# <rtc-template block="consumer_import">
# </rtc-template>


# This module's spesification
# <rtc-template block="module_spec">
testraspimousepy_spec = ["implementation_id", "TestRasPiMousePy", 
		 "type_name",         "TestRasPiMousePy", 
		 "description",       "Test Raspberry Pi Mouse", 
		 "version",           "1.0.0", 
		 "vendor",            "AIST", 
		 "category",          "Controller", 
		 "activity_type",     "STATIC", 
		 "max_instance",      "1", 
		 "language",          "Python", 
		 "lang_type",         "SCRIPT",
		 "conf.default.stop_distance", "300",

		 "conf.__widget__.stop_distance", "text",

         "conf.__type__.stop_distance", "short",

		 ""]
# </rtc-template>

##
# @class TestRasPiMousePy
# @brief Test Raspberry Pi Mouse
# 
# テスト用ラズパイマウス制御RTC
# 
# 
class TestRasPiMousePy(OpenRTM_aist.DataFlowComponentBase):
	
	##
	# @brief constructor
	# @param manager Maneger Object
	# 
	def __init__(self, manager):
		OpenRTM_aist.DataFlowComponentBase.__init__(self, manager)

		self._last_sensor_data = [0, 0, 0, 0]

		velocity_in_arg = [None] * ((len(RTC._d_TimedVelocity2D) - 4) / 2)
		#self._d_velocity_in = RTC.TimedVelocity2D(*velocity_in_arg)
		self._d_velocity_in = RTC.TimedVelocity2D(RTC.Time(0,0),RTC.Velocity2D(0,0,0))
		"""
		補正前の目標速度
		 - Type: RTC::TimedVelocity2D
		 - Unit: m/s, rad/s
		"""
		self._velocity_inIn = OpenRTM_aist.InPort("velocity_in", self._d_velocity_in)
		distance_sensor_arg = [None] * ((len(RTC._d_TimedShortSeq) - 4) / 2)
		#self._d_distance_sensor = RTC.TimedShortSeq(*distance_sensor_arg)
		self._d_distance_sensor = RTC.TimedShortSeq(RTC.Time(0,0),[])
		"""
		距離センサの計測値
		 - Type: RTC::TimedShortSeq
		 - Number: 4
		"""
		self._distance_sensorIn = OpenRTM_aist.InPort("distance_sensor", self._d_distance_sensor)
		velocity_out_arg = [None] * ((len(RTC._d_TimedVelocity2D) - 4) / 2)
		#self._d_velocity_out = RTC.TimedVelocity2D(*velocity_out_arg)
		self._d_velocity_out = RTC.TimedVelocity2D(RTC.Time(0,0),RTC.Velocity2D(0,0,0))
		"""
		補正後の目標速度
		 - Type: RTC::TimedVelocity2D
		 - Unit: m/s, rad/s
		"""
		self._velocity_outOut = OpenRTM_aist.OutPort("velocity_out", self._d_velocity_out)
		buzzer_arg = [None] * ((len(RTC._d_TimedShort) - 4) / 2)
		#self._d_buzzer = RTC.TimedShort(*buzzer_arg)
		self._d_buzzer = RTC.TimedShort(RTC.Time(0,0),0)
		"""
		ブザーの周波数
		 - Type: RTC::TimedShort
		"""
		self._buzzerOut = OpenRTM_aist.OutPort("buzzer", self._d_buzzer)


		


		# initialize of configuration-data.
		# <rtc-template block="init_conf_param">
		"""
		距離センサの計測値がこの値以下になった場合に停止する
		 - Name: stop_distance stop_distance
		 - DefaultValue: 50
		"""
		self._stop_distance = [50]
		
		# </rtc-template>


		 
	##
	#
	# The initialize action (on CREATED->ALIVE transition)
	# formaer rtc_init_entry() 
	# 
	# @return RTC::ReturnCode_t
	# 
	#
	def onInitialize(self):
		# Bind variables and configuration variable
		self.bindParameter("stop_distance", self._stop_distance, "300")
		
		# Set InPort buffers
		self.addInPort("velocity_in",self._velocity_inIn)
		self.addInPort("distance_sensor",self._distance_sensorIn)
		
		# Set OutPort buffers
		self.addOutPort("velocity_out",self._velocity_outOut)
		self.addOutPort("buzzer",self._buzzerOut)
		
		# Set service provider to Ports
		
		# Set service consumers to Ports
		
		# Set CORBA Service Ports
		
		return RTC.RTC_OK
	
		##
		# 
		# The finalize action (on ALIVE->END transition)
		# formaer rtc_exiting_entry()
		# 
		# @return RTC::ReturnCode_t
	
		# 
	def onFinalize(self):
	
		return RTC.RTC_OK
	
		##
		#
		# The startup action when ExecutionContext startup
		# former rtc_starting_entry()
		# 
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onStartup(self, ec_id):
	
		return RTC.RTC_OK
	
		##
		#
		# The shutdown action when ExecutionContext stop
		# former rtc_stopping_entry()
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onShutdown(self, ec_id):
	
		return RTC.RTC_OK
	
		##
		#
		# The activated action (Active state entry action)
		# former rtc_active_entry()
		#
		# @param ec_id target ExecutionContext Id
		# 
		# @return RTC::ReturnCode_t
		#
		#
	def onActivated(self, ec_id):
		self._last_sensor_data = [0, 0, 0, 0]
		return RTC.RTC_OK
	
		##
		#
		# The deactivated action (Active state exit action)
		# former rtc_active_exit()
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onDeactivated(self, ec_id):
		#停止する
		self._d_velocity_out.data.vx = 0
		self._d_velocity_out.data.vy = 0
		self._d_velocity_out.data.va = 0
		OpenRTM_aist.setTimestamp(self._d_velocity_out)
		self._velocity_outOut.write()

		#ブザーを止める
		self._d_buzzer.data = 0
		OpenRTM_aist.setTimestamp(self._d_buzzer)
		self._buzzerOut.write()
		
		return RTC.RTC_OK
	
		##
		#
		# The execution action that is invoked periodically
		# former rtc_active_do()
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onExecute(self, ec_id):
		#データを新規に受信した場合に、データをm_last_sensor_dataを格納する
		if self._distance_sensorIn.isNew():
			data = self._distance_sensorIn.read()
			if len(data.data) == 4:
				self._last_sensor_data = data.data[:]

		
		if self._velocity_inIn.isNew():
			data = self._velocity_inIn.read()
			#vxが0以上(前進)のときのみ停止するか判定する
			if data.data.vx > 0:
				for d in self._last_sensor_data:
					#センサの計測値がstop_distance以上の時に前進しないようにする
					if d > self._stop_distance[0]:
						#停止する
						self._d_velocity_out.data.vx = 0
						self._d_velocity_out.data.vy = 0
						self._d_velocity_out.data.va = 0
						OpenRTM_aist.setTimestamp(self._d_velocity_out)
						self._velocity_outOut.write()

						#ブザーを鳴らす
						self._d_buzzer.data = 50
						OpenRTM_aist.setTimestamp(self._d_buzzer)
						self._buzzerOut.write()
						
						return RTC.RTC_OK

			#入力速度をそのまま出力する
			self._d_velocity_out.data.vx = data.data.vx
			self._d_velocity_out.data.vy = data.data.vy
			self._d_velocity_out.data.va = data.data.va
			OpenRTM_aist.setTimestamp(self._d_velocity_out)
			self._velocity_outOut.write()

			#ブザーを止める
			self._d_buzzer.data = 0
			OpenRTM_aist.setTimestamp(self._d_buzzer)
			self._buzzerOut.write()

		return RTC.RTC_OK
	
		##
		#
		# The aborting action when main logic error occurred.
		# former rtc_aborting_entry()
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onAborting(self, ec_id):
	
		return RTC.RTC_OK
	
		##
		#
		# The error action in ERROR state
		# former rtc_error_do()
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onError(self, ec_id):
	
		return RTC.RTC_OK
	
		##
		#
		# The reset action that is invoked resetting
		# This is same but different the former rtc_init_entry()
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onReset(self, ec_id):
	
		return RTC.RTC_OK
	
		##
		#
		# The state update action that is invoked after onExecute() action
		# no corresponding operation exists in OpenRTm-aist-0.2.0
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#

		#
	def onStateUpdate(self, ec_id):
	
		return RTC.RTC_OK
	
		##
		#
		# The action that is invoked when execution context's rate is changed
		# no corresponding operation exists in OpenRTm-aist-0.2.0
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onRateChanged(self, ec_id):
	
		return RTC.RTC_OK
	



def TestRasPiMousePyInit(manager):
    profile = OpenRTM_aist.Properties(defaults_str=testraspimousepy_spec)
    manager.registerFactory(profile,
                            TestRasPiMousePy,
                            OpenRTM_aist.Delete)

def MyModuleInit(manager):
    TestRasPiMousePyInit(manager)

    # Create a component
    comp = manager.createComponent("TestRasPiMousePy")

def main():
	mgr = OpenRTM_aist.Manager.init(sys.argv)
	mgr.setModuleInitProc(MyModuleInit)
	mgr.activateManager()
	mgr.runManager()

if __name__ == "__main__":
	main()

