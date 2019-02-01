//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
//#include  "Task_StageNumber.h"
#include  "Task_Stageserect.h"
#include  "Task_Game.h"
#include  "easing.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "TitleLogoImg";
		this->imageName2 = "PressKeyImg";
		this->imageName3 = "HiluImg";
		this->cloudImage1 = "Cloud1";		//20190118����
		this->cloudImage2 = "Cloud2";		//20190118����

		DG::Image_Create(this->imageName2, "./data/image/PressKey.png");
		DG::Image_Create(this->imageName3, "./data/image/hiru.png");
		DG::Image_Create(this->imageName, "./data/image/Title.png");
		DG::Image_Create(this->cloudImage1, "./data/image/cloud1.png");
		DG::Image_Create(this->cloudImage2, "./data/image/cloud2.png");

		//BGM--------------------------------------------------------
		//DM::Sound_CreateStream("BGM", "./data/BGM/Title.wav");
		//DM::Sound_Play("BGM", false);//�Đ��J�n
		//DM::Sound_Volume("BGM", 1000);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->imageName2);
		DG::Image_Erase(this->imageName3);
		DG::Image_Erase(this->cloudImage1);
		DG::Image_Erase(this->cloudImage2);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->logoPosY = 0;
		this->count = 0;
		this->cloudposX = 40;
		this->cloudposX1 = 360;
		this->cloudposY1 = 40;
		easing::Init();
		//����
		easing::Create("TitleLogoImg", easing::CIRCINOUT, -480, 100, 80);//easing�J�n�n�_�A�I���n�_�A�����鎞��

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
			auto nextTask = Stageserect::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in1 = DI::GPad_GetState("P1");
		easing::UpDate();
		this->count++;
		this->cloudposX += 2;
		if (cloudposX >= 600)
		{
			cloudposX = -480;
		}
		this->cloudposX1++;
		if (cloudposX1 >= 480)
		{
			cloudposX1 = -440;
		}
		/*this->logoPosY += 48;
		if (this->logoPosY >= 0) {
			this->logoPosY = 0;
		}
		*/
		if (this->logoPosY == 0) {
			//this->logoPosY++;
			easing::Start("TitleLogoImg");
		}
		/*if (easing::GetState("TitleLogoImg") == easing::EQ_STATE::EQ_END)
			{
				
			}*/

		if (in1.ST.down) {
			//���g�ɏ��ŗv��
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//Title
		ML::Box2D  draw(0, 0, 640, 100);
		ML::Box2D  src(0, 0, 640, 100);

		//Presskey
		ML::Box2D  draw2(125, 300, 400, 80);
		ML::Box2D  src2(0, 0, 550, 80);

		//���̔w�i
		ML::Box2D  draw3(0, 0, 640, 480);
		ML::Box2D  src3(0, 0, 1280, 720);

		//�_�P
		ML::Box2D draw4(40, 10, 300, 200);
		ML::Box2D src4(0, 0, 200, 180);

		draw4.Offset(cloudposX , 10);
		//�_2
		ML::Box2D draw5(360, 200, 120, 100);
		ML::Box2D src5(0, 0, 120, 100);

		draw5.Offset(cloudposX1, cloudposY1);
		//draw.Offset(0, logoPosY);
		draw.Offset(-10,(int)easing::GetPos("TitleLogoImg"));
		DG::Image_Draw(this->res->imageName3, draw3, src3);
		DG::Image_Draw(this->res->cloudImage1, draw4, src4);
		DG::Image_Draw(this->res->cloudImage2, draw5, src5);
		DG::Image_Draw(this->res->imageName, draw, src);
		if (this->count > 0)
		{
			if ((this->count / 32) %2 == 0)
			{
				return;
			}
		}
		DG::Image_Draw(this->res->imageName2, draw2, src2);
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
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