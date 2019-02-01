//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_StageNumber.h"
#include  "Task_Game.h"
#include  "easing.h"

namespace  StageNum
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "StageNumImg";
		this->haikeiImg = "HaikeiImg";
		DG::Image_Create(this->imageName, "./data/image/Tyutorial.png");
		DG::Image_Create(this->haikeiImg, "./data/image/black.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->haikeiImg);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(int stageNumber)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();
		number = stageNumber;
		//���f�[�^������
		easing::Init();

		this->timecount = 0;

		this->logoPosY = -480;
		//����
		easing::Create("StageNumImg", easing::CIRCINOUT, -480, 0, 80);//easing�̊J�n�n�_�A�I���n�_�A�����鎞��
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto nextTask = Game::Object::Create(true , number);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in1 = DI::GPad_GetState("P1");
		easing::UpDate();

		this->logoPosY += 48;
		if (this->logoPosY >= 0) {
			this->logoPosY = 0;
		}

		if (this->logoPosY == 0) {
			this->logoPosY++;
			easing::Start("StageNumImg");
		}
		if (logoPosY >= 0)
		{
			if (this->timecount <= 300)
			{
				this->timecount++;
			}
			if (timecount == 300)
			{
				this->Kill();
			}
		}
		
		
	}
	//if (in1.ST.down) {
	//	//���g�ɏ��ŗv��
	//	this->Kill();
	//}
	//if (easing::GetState("StageNumImg") == easing::EQ_STATE::EQ_END)
	//{
	//
	//}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw(0, 100, 570, 80);
		ML::Box2D  src(0, 0, 570, 80);

		//�w�i
		ML::Box2D	draw2(0, 0, 640, 480);
		ML::Box2D	src2(0, 0, 640, 480);
		draw.Offset(0, logoPosY);
		draw.Offset(0, (int)easing::GetPos("StageNumImg"));
		DG::Image_Draw(this->res->haikeiImg, draw2, src2);
		DG::Image_Draw(this->res->imageName, draw, src);
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_,int stageNumber)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->Initialize(stageNumber)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}