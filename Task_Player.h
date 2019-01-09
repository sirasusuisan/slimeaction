#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Player
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName(		"�v���C���[");	//�O���[�v��
	const  string  defName(				"��");		//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
		string	stopImage;
		string	moveImage;
		string  jumpImage;
		string  verticalStopImage;
		string  verticalMoveImage;
		string  verticalJumpImage;
		string  sideStopImage;
		string  sideMoveImage;
		string  sideJumpImage;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
	//�ύX������������������������������������������������������
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����
		//�u�ϐ��錾�������v
		string	controllerName;
		//�u�ǉ����\�b�h�������v
		void Think();	//�v�l&�󋵔��f(��Ԍ���)
		void Move();	//Think()�Ŋm�肵����ԂɑΉ����鏈��
		void Rotation(); //��]
		BChara::DrawInfo Anim(); //�A�j���[�V��������
		void	Received(BChara* from_);//�ڐG���̉�������
		
	};
}