//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_End.h"
#include  "Task_Back.h"
#include  "Task_Map2D.h"
#include  "Task_Start.h"
#include  "Task_Player.h"
#include  "Task_HP.h"
#include  "Task_Key.h"
#include  "Task_Camera.h"
#include "Task_Goal.h"
#include  "Task_Result.h"
#include  "Task_Enemy00.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//DG::Font_Create("FontImage", "���S�V�b�N" , 16, 16);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		//DG::Font_Erase("FontImage");	
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(int sn)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		ge->camera2D = ML::Box2D(-200, -100, 640, 480);//��肠���������l�ݒ�
		//this->render2D_Priority[1] = 0.1f;
		//���^�X�N�̐���
		//�w�i
		Back::Object::Create(true);
		
		//�}�b�v
		auto m = Map2D::Object::Create(true);
		switch (sn)
		{
		case 0:
			break;
		case 1:
			break;
		default:
			m->Load("./data/Map/tutorial01.csv");
			break;
		}
		//�X�^�[�g
		auto s = Start::Object::Create(true);
		//�v���C���[
		/*auto pl = Player::Object::Create(true);
		//pl->pos = m->arr[pl->pos.y][pl->pos.x];
		pl->pos.x = 64;//480 / 2;
		pl->pos.y = 430;//270 / 2;*/
		//���˂݁[
		/*auto e = Enemy00::Object::Create(true);
		e->pos.x = 300;
		e->pos.y = 500;*/
		//�J����
		auto camera = Camera::Object::Create(true);
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C���[");
		camera->pos.x = pl->pos.x + 10;
		camera->pos.y = pl->pos.y + 10;
		camera->target = pl;
		//UI
		HP::Object::Create(true);
		KEY::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�t�B�[���h");
		ge->KillAll_G("�w�i");
		ge->KillAll_G("�v���C���[");
		ge->KillAll_G("UI");
		ge->KillAll_G("�S�[��");
		ge->KillAll_G("�G");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			if (flag)
			{
				auto  nextTask = Result::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		if (ge->GetTask_One_G<Goal::Object>("�S�[��")->GetClearFlag())
		{
			flag = true;
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		/*auto pl = Player::Object::Create(true);
		ML::Box2D textBox(0, 0, 100, 48);
		ML::Color color(1, 1, 1, 1);
		string text = to_string(pl->pos.y);
		DG::Font_Draw("FontImage",textBox, text, color);*/
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
	Object::SP  Object::Create(bool  flagGameEnginePushBack_,int stagenumber)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->Initialize(stagenumber)) {
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