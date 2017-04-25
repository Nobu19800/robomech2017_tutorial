#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -*- Python -*-

"""
 @file TestEV3Py.py
 @brief Test EV3
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
testev3py_spec = ["implementation_id", "TestEV3Py", 
		 "type_name",         "TestEV3Py", 
		 "description",       "Test EV3", 
		 "version",           "1.0.0", 
		 "vendor",            "AIST", 
		 "category",          "Controller", 
		 "activity_type",     "STATIC", 
		 "max_instance",      "1", 
		 "language",          "Python", 
		 "lang_type",         "SCRIPT",
		 "conf.default.sound_output", "beep",

		 "conf.__widget__.sound_output", "text",

         "conf.__type__.sound_output", "string",

		 ""]
# </rtc-template>

##
# @class TestEV3Py
# @brief Test EV3
# 
# EV3制御RTC
# 
# 
class TestEV3Py(OpenRTM_aist.DataFlowComponentBase):
	
	##
	# @brief constructor
	# @param manager Maneger Object
	# 
	def __init__(self, manager):
		OpenRTM_aist.DataFlowComponentBase.__init__(self, manager)

		self._last_sensor_data = [False, False]

		velocity_in_arg = [None] * ((len(RTC._d_TimedVelocity2D) - 4) / 2)
		#self._d_velocity_in = RTC.TimedVelocity2D(*velocity_in_arg)
		self._d_velocity_in = RTC.TimedVelocity2D(RTC.Time(0,0),RTC.Velocity2D(0,0,0))
		"""
		補正前の目標速度
		 - Type: RTC::TimedVelocity2D
		 - Unit: m/s, rad/s
		"""
		self._velocity_inIn = OpenRTM_aist.InPort("velocity_in", self._d_velocity_in)
		touch_arg = [None] * ((len(RTC._d_TimedBooleanSeq) - 4) / 2)
		#self._d_touch = RTC.TimedBooleanSeq(*touch_arg)
		self._d_touch = RTC.TimedBooleanSeq(RTC.Time(0,0),[])
		"""
		タッチセンサのオンオフ
		 - Type: RTC::TimedBooleanSeq
		 - Number: 2
		"""
		self._touchIn = OpenRTM_aist.InPort("touch", self._d_touch)
		velocity_out_arg = [None] * ((len(RTC._d_TimedVelocity2D) - 4) / 2)
		#self._d_velocity_out = RTC.TimedVelocity2D(*velocity_out_arg)
		self._d_velocity_out = RTC.TimedVelocity2D(RTC.Time(0,0),RTC.Velocity2D(0,0,0))
		"""
		補正後の目標速度
		 - Type: RTC::TimedVelocity2D
		 - Unit: m/s, rad/s
		"""
		self._velocity_outOut = OpenRTM_aist.OutPort("velocity_out", self._d_velocity_out)
		sound_arg = [None] * ((len(RTC._d_TimedString) - 4) / 2)
		#self._d_sound = RTC.TimedString(*sound_arg)
		self._d_sound = RTC.TimedString(RTC.Time(0,0),"")
		"""
		音声出力
		 - Type: RTC::TimedString
		"""
		self._soundOut = OpenRTM_aist.OutPort("sound", self._d_sound)


		


		# initialize of configuration-data.
		# <rtc-template block="init_conf_param">
		"""
		停止した時に鳴らす音
		 - Name: sound_output sound_output
		 - DefaultValue: beep
		"""
		self._sound_output = ['beep']
		
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
		self.bindParameter("sound_output", self._sound_output, "beep")
		
		# Set InPort buffers
		self.addInPort("velocity_in",self._velocity_inIn)
		self.addInPort("touch",self._touchIn)
		
		# Set OutPort buffers
		self.addOutPort("velocity_out",self._velocity_outOut)
		self.addOutPort("sound",self._soundOut)
		
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
		self._last_sensor_data = [False, False]
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
		self._d_target_velocity.data.vx = 0
		self._d_target_velocity.data.vy = 0
		self._d_target_velocity.data.va = 0
		OpenRTM_aist.setTimestamp(self._d_target_velocity)
		self._target_velocityOut.write()
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
		if self._touchIn.isNew():
			data = self._touchIn.read()
			if len(data.data) == 2:
				for i in range(2):
					#タッチセンサがOFFからONになった時に音を鳴らす
					if not self._last_sensor_data[i] and data.data[i]:
						self._d_sound.data = self._sound_output[0]
						OpenRTM_aist.setTimestamp(self._d_sound)
						self._soundOut.write()
				self._last_sensor_data = data.data[:]

		if self._velocity_inIn.isNew():
			data = self._velocity_inIn.read()
			#vxが0以上(前進)のときのみ停止するか判定する
			if data.data.vx > 0:
				for d in self._last_sensor_data:
					#タッチセンサがONの時に停止する
					if d:
						#停止する
						self._d_velocity_out.data.vx = 0
						self._d_velocity_out.data.vy = 0
						self._d_velocity_out.data.va = 0
						OpenRTM_aist.setTimestamp(self._d_velocity_out)
						self._velocity_outOut.write()
						
						return RTC.RTC_OK

			#入力速度をそのまま出力する
			self._d_velocity_out.data.vx = data.data.vx
			self._d_velocity_out.data.vy = data.data.vy
			self._d_velocity_out.data.va = data.data.va
			OpenRTM_aist.setTimestamp(self._d_velocity_out)
			self._velocity_outOut.write()

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
	



def TestEV3PyInit(manager):
    profile = OpenRTM_aist.Properties(defaults_str=testev3py_spec)
    manager.registerFactory(profile,
                            TestEV3Py,
                            OpenRTM_aist.Delete)

def MyModuleInit(manager):
    TestEV3PyInit(manager)

    # Create a component
    comp = manager.createComponent("TestEV3Py")

def main():
	mgr = OpenRTM_aist.Manager.init(sys.argv)
	mgr.setModuleInitProc(MyModuleInit)
	mgr.activateManager()
	mgr.runManager()

if __name__ == "__main__":
	main()

