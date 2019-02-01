//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Block.h"
#include  "Task_Player.h"

namespace  Block
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->blockImg="Block";
		DG::Image_Create(this->blockImg, "./data/image/Block.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->blockImg);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(ML::Vec2& position,Type type)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		switch (type)
		{
		case Block::Type::up:
		{

		}
			break;
		case Block::Type::down:
		{

		}
			break;
		case Block::Type::left:
		{
			this->hitBase = ML::Box2D(position.x, position.y, 16, 32);
			this->srcBase = ML::Box2D(0, 0, 16, 32);
		}
			break;
		case Block::Type::right:
		{
			this->hitBase = ML::Box2D(position.x + 16, position.y, 16, 32);
			this->srcBase = ML::Box2D(0, 0, 16, 32);
		}
			break;
		default:
			break;
		}
		
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
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		/*auto pl = ge->GetTask_One_G<Player::Object>("�v���C���[");
		if (pl != nullptr)
		{
			//CheckMove(pl->moveVec);//�߂荞�܂Ȃ�����
			//ML::Vec2 playerPos = pl->pos;
			//if (pl->CheckHit(this->hitBase))
			//{
				
			//}
		}*/
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
			ML::Box2D draw(hitBase.x, hitBase.y, 16, 32);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			DG::Image_Draw(this->res->blockImg, draw, srcBase);
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, ML::Vec2& position,Type type )
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->Initialize(position,type)) {
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