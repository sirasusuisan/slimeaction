#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�M�~�b�N-��
//-------------------------------------------------------------------
#include "BChara.h"

namespace  GDoor
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("��");	//�O���[�v��
	const  string  defName("NoName");		//�^�X�N��
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
		string Doorimg;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_, ML::Vec2& position);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize(ML::Vec2& position);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
	//�ύX������������������������������������������������������
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����
		//void Object::Received(BChara* from_);

	};
}