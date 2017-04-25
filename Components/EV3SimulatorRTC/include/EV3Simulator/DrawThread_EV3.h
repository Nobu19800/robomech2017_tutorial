/*!
* @file  DrawThread.h
* @brief �V�~�����[�V�����`��֘A�̃N���X
*
*/

#ifndef DRAWTHREAD_H
#define DRAWTHREAD_H


#include <coil/Task.h>

#include <stdio.h>
#include <stdlib.h>
#include <drawstuff/drawstuff.h>
#include "EV3SimulatorObj.h"



/**
* @class DrawThread
*@brief �V�~�����[�V�����̕`�������X���b�h
*/
class DrawThread_EV3 : public virtual coil::Task
{
public:
		/**
		*@brief �R���X�g���N�^
		*/
		DrawThread_EV3(EV3SimulatorObj *so, double dt);
		
		/**
		*@brief �X���b�h���s�֐�
		* @return 
		*/
		virtual int svc();
		/**
		*@brief DrawStuff������
		*/
		void setDrawStuff();
		/**
		*@brief �����̕`��
		* @param body �{�f�B�I�u�W�F�N�g
		*/
		void drawBox(MyLink *body);
		/**
		*@brief �~���`��
		* @param body �{�f�B�I�u�W�F�N�g
		*/
		void drawCylinder(MyLink *body);
		/**
		*@brief ���`��
		* @param body �{�f�B�I�u�W�F�N�g
		*/
		void drawSphere(MyLink *body);
		/**
		*@brief �S�{�f�B�`��
		*/
		void drawRobot();
		/**
		*@brief �J�����ʒu�Đݒ�
		*/
		void resetCameraPosition();
		/**
		*@brief �J�����ʒu�Đݒ�t���O�𗧂Ă�
		*/
		void setRCPFlag();
		/**
		*@brief �`����~����
		*/
		void stop();

		double fps;
		bool RCP_flag;
		bool stop_flag;

private:
	EV3SimulatorObj *m_so;
	dsFunctions   fn;
	

};

#endif