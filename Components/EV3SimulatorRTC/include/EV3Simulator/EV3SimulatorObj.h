/*!
* @file  SimulatorObj.h
* @brief �V�~�����[�V�����֘A�̃N���X
*
*/

#ifndef EV3SIMULATOROBJ_H
#define EV3SIMULATOROBJ_H

#define dDOUBLE

#include <coil/Mutex.h>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <ode/ode.h>
#include <vector>


#define _USE_MATH_DEFINES
#include <math.h>


#define DEFAULT_STICK_WIDTH 0.005
#define DEFAULT_STICK_RADIUS (M_PI/4.0)


#define DEFAULT_BODY_LENGTH 0.1
#define DEFAULT_BODY_WIDTH 0.05
#define DEFAULT_BODY_HEIGHT 0.05


#define DEFAULT_BODY_X 0
#define DEFAULT_BODY_Y 0
#define DEFAULT_BODY_Z (0.1)


#define DEFAULT_BODY_MASS 2.0

#define DEFAULT_WHEEL_RADIUS 0.02
#define DEFAULT_WHEEL_WIDTH 0.02

#define DEFAULT_WHEEL_X (DEFAULT_BODY_X + 0.03)
#define DEFAULT_WHEEL_Y (DEFAULT_BODY_Y + 0.045)
#define DEFAULT_WHEEL_Z (DEFAULT_BODY_Z - 0.025)

#define DEFAULT_WHEEL_MASS 0.01

#define DEFAULT_BALL_RADIUS 0.01

#define DEFAULT_BALL_X (DEFAULT_BODY_X - 0.05)
#define DEFAULT_BALL_Y DEFAULT_BODY_Y
#define DEFAULT_BALL_Z (DEFAULT_BODY_Z - 0.035)

#define DEFAULT_BALL_MASS 0.01

#define DEFAULT_TOUCHSENSOR_LENGTH 0.035
#define DEFAULT_TOUCHSENSOR_WIDTH 0.02
#define DEFAULT_TOUCHSENSOR_HEIGHT 0.02

#define DEFAULT_TOUCHSENSOR_X (DEFAULT_BODY_X + 0.15)
#define DEFAULT_TOUCHSENSOR_Y (DEFAULT_BODY_Y + DEFAULT_BODY_WIDTH/2.0 + DEFAULT_TOUCHSENSOR_WIDTH/2.0 + DEFAULT_STICK_WIDTH*2)
#define DEFAULT_TOUCHSENSOR_Z (DEFAULT_BODY_Z)

#define DEFAULT_TOUCHSENSOR_MASS 0.01

#define DEFAULT_TOUCHSENSOR2_MOVE 0.004
#define DEFAULT_TOUCHSENSOR2_LENGTH (DEFAULT_TOUCHSENSOR2_MOVE*2)
#define DEFAULT_TOUCHSENSOR2_WIDTH (DEFAULT_TOUCHSENSOR_WIDTH*0.8)
#define DEFAULT_TOUCHSENSOR2_HEIGHT (DEFAULT_TOUCHSENSOR_HEIGHT*0.8)
#define DEFAULT_TOUCHSENSOR2_X (DEFAULT_TOUCHSENSOR_X + DEFAULT_TOUCHSENSOR_LENGTH/2.0)
#define DEFAULT_TOUCHSENSOR2_Y DEFAULT_TOUCHSENSOR_Y
#define DEFAULT_TOUCHSENSOR2_Z DEFAULT_TOUCHSENSOR_Z
#define DEFAULT_TOUCHSENSOR2_MASS 0.01

#define DEFAULT_SPRING_VALUE 10
#define DEFAULT_DUMPER_VALUE 1

#define DEFAULT_GYROSENSOR_LENGTH 0.02
#define DEFAULT_GYROSENSOR_WIDTH 0.04
#define DEFAULT_GYROSENSOR_HEIGHT 0.02

#define DEFAULT_GYROSENSOR_X (DEFAULT_BODY_X - 0.06)
#define DEFAULT_GYROSENSOR_Y DEFAULT_BODY_Y
#define DEFAULT_GYROSENSOR_Z (DEFAULT_BODY_Z + 0.06)

#define DEFAULT_GYROSENSOR_MASS 0.01


#define DEFAULT_COLOURSENSOR_LENGTH 0.01
#define DEFAULT_COLOURSENSOR_WIDTH 0.01
#define DEFAULT_COLOURSENSOR_HEIGHT 0.03


#define DEFAULT_COLOURSENSOR_X (DEFAULT_BODY_X + 0.07)
#define DEFAULT_COLOURSENSOR_Y (DEFAULT_BODY_Y + 0.02)
#define DEFAULT_COLOURSENSOR_Z (DEFAULT_BODY_Z - 0.01)

#define DEFAULT_COLOURSENSOR_MASS 0.01


#define DEFAULT_MMOTOR_LENGTH 0.01
#define DEFAULT_MMOTOR_WIDTH 0.03
#define DEFAULT_MMOTOR_HEIGHT 0.01

#define DEFAULT_MMOTOR_X (DEFAULT_BODY_X + 0.06)
#define DEFAULT_MMOTOR_Y DEFAULT_BODY_Y
#define DEFAULT_MMOTOR_Z DEFAULT_BODY_Z

#define DEFAULT_MMOTOR_MASS 0.01


#define DEFAULT_ULTRASONICSENSOR_LENGTH 0.01
#define DEFAULT_ULTRASONICSENSOR_WIDTH 0.03
#define DEFAULT_ULTRASONICSENSOR_HEIGHT 0.01


#define DEFAULT_ULTRASONICSENSOR_X (DEFAULT_MMOTOR_X + 0.07)
#define DEFAULT_ULTRASONICSENSOR_Y DEFAULT_MMOTOR_Y
#define DEFAULT_ULTRASONICSENSOR_Z (DEFAULT_MMOTOR_Z + 0.06)

#define DEFAULT_ULTRASONICSENSOR_MASS 0.01

#define DEFAULT_MAX_ULTRASONICSENSOR_DISTANCE 100.0


#define DEFAULT_BLOCK_MASS 100


#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif





/**
* @struct MyLink
*@brief �{�f�B�I�u�W�F�N�g
*�ڑ�����W���C���g���܂�
*/
typedef struct {
  dBodyID  body;
  dGeomID  geom;
  dJointID joint;
  int  dir;
  float  red, green, blue;
  dReal    m,r,x,y,z,lx,ly,lz, the, dthe, ddthe, axisx, axisy, axisz, Tthe, Tdthe, Tddthe, tau, jx,jy,jz;
} MyLink;

/**
* @class EV3_Obj
*@brief EV3�̊e��f�[�^�i�[�N���X
*/
class EV3Obj
{
public:
	EV3Obj();
	void setTargetVelocity(double vx, double va);
	void setCurrentPosition(double px, double py, double pa);
	void setUltrasonicSensorPos(double x, double y, double z);
	void setColourSensorPos(double x, double y, double z);
	double calcUltrasonicSensorDistance(double x, double y, double z);
	double calcColourSensorDistance(double x, double y, double z);
	void resetUltrasonicSensorDistance();
	void resetColourSensorDistance();
	bool getRightTouch(double limit);
	bool getLeftTouch(double limit);
	double target_vx, target_va;
	double current_px, current_py, current_pa;
	double gyro_data;
	double target_mangle, velocity_mmotor;
	double current_ultrasonicSensorData, current_colourSensorData;
	double right_touch_value, left_touch_value;
	bool right_touch, left_touch;
	double current_ultrasonicSensor_x, current_ultrasonicSensor_y, current_ultrasonicSensor_z;
	double current_colourSensor_x, current_colourSensor_y, current_colourSensor_z;
};
/**
* @class EV3SimulatorObj
*@brief �V�~�����[�V�����̑�������邽�߂̃N���X
*/
class EV3SimulatorObj
{
public:
	/**
	*@brief �R���X�g���N�^
	*/
	EV3SimulatorObj();
	/**
	*@brief �f�X�g���N�^
	*/
	~EV3SimulatorObj();
	coil::Mutex mu;
	MyLink EV3Block, wheelLeft, wheelRight, wheelBall, touchSensorLeft[4], touchSensorRight[4], gyroSensor[3], colourSensor, mmotor, ultrasonicSensor[3];
	dGeomID ultrasonicSensor_ray, colourSensor_ray;
	std::vector<MyLink> blocks;
	bool plane_exist;
	MyLink plane;
	double st;
	double gravity;
	bool pause;
	dWorldID      world;       
	dSpaceID      space;       
	dGeomID       ground;       
	dJointGroupID contactgroup;
	EV3Obj ev3;

	
	/**
	*@brief �����g�����Z���T��ݒ�
	*/
	void setUltrasonicSensorRay();
	/**
	*@brief �J���[�Z���T��ݒ�
	*/
	void setColourSensorRay();
	/**
	*@brief �e�p�����[�^�̏��������s��
	*@param offset_z �����𒲐�
	*/
	void makeParam(double offset_z=0);
	/**
	*@brief �����̍쐬
	* @param body �{�f�B�I�u�W�F�N�g
	*/
	void setBox(MyLink *body);
	/**
	*@brief �~���쐬
	* @param body �{�f�B�I�u�W�F�N�g
	*/
	void setCylinder(MyLink *body);
	/**
	*@brief ���쐬
	* @param body �{�f�B�I�u�W�F�N�g
	*/
	void setSphere(MyLink *body);
	/**
	*@brief �q���W�W���C���g�쐬
	* @param body1 �{�f�B1
	* @param body2 �{�f�B2
	*/
	void setHinge(MyLink *body1, MyLink *body2);
	/**
	*@brief �X���C�_�[�W���C���g�쐬
	* @param body1 �{�f�B1
	* @param body2 �{�f�B2
	*/
	void setSlider(MyLink *body1, MyLink *body2);
	/**
	*@brief �Œ�W���C���g�쐬
	* @param body1 �{�f�B1
	* @param body2 �{�f�B2
	*/
	void setFixed(MyLink *body1, MyLink *body2);
	/**
	*@brief �{�[���W���C���g�쐬
	* @param body1 �{�f�B1
	* @param body2 �{�f�B2
	*/
	void setBall(MyLink *body1, MyLink *body2);
	/**
	*@brief �S�{�f�B�A�ڑ�����S�W���C���g����
	*/
	void makeRobot();
	/**
	*@brief �q���W�W���C���g����
	* @param body �{�f�B�I�u�W�F�N�g
	* @param theta �q���W�W���C���g�̈ʒu
	*/
	void controlHinge(MyLink *body, dReal theta);
	/**
	*@brief �q���W�W���C���g����
	* @param body �{�f�B�I�u�W�F�N�g
	* @param theta �q���W�W���C���g�̈ʒu
	* @param vel �q���W�W���C���g�̑��x
	*/
	void controlHinge(MyLink *body, dReal theta, dReal vel);
	/**
	*@brief �X���C�_�[�W���C���g����
	* @param body �{�f�B�I�u�W�F�N�g
	* @param length �X���C�_�[�W���C���g�̈ʒu
	*/
	void controlSlider(MyLink *body, dReal length);
	/**
	*@brief �S�W���C���g����
	*/
	void control();
	/**
	*@brief �X�V
	*/
	void update();
	/**
	*@brief �S�{�f�B�A�ڑ�����S�W���C���g����
	*/
	void destroyRobot();

	/**
	*@brief �ڐG�R�[���o�b�N
	* @param o1 �W�I���g��1
	* @param o2 �W�I���g��2
	*/
	void m_nearCallback(dGeomID o1, dGeomID o2);
	/**
	*@brief ���ݕ��ݒ�
	* @param s �T���v�����O����
	*/
	void setSamplingTime(double s);


	/**
	*@brief �n�ʐ���
	*/
	void makePlane(double x, double y, double lx, double ly, double lz);

	/**
	*@brief ��Q������
	*/
	void makeBlock(double x, double y, double z, double lx, double ly, double lz, double r);

	/**
	*@brief �t�@�C�������Q���͈ʒu�ǂݍ���
	*@return �ǂݍ��ݐ���(true)�A���s(false)
	*/
	bool loadBlocksData(std::string fname);


};



#endif