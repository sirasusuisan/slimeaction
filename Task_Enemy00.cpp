//-------------------------------------------------------------------
//�G
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy00.h"

namespace  Enemy00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->EnemyImg = "EnemyImg";
		DG::Image_Create(this->EnemyImg, "./data/image/Enemy00.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->EnemyImg);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(ML::Vec2& position)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.5f; //��O�\��

		this->hitBase = ML::Box2D(-32, -32, 64, 64);
		this->angle_LR = Right; //�f�t�H���g�͉E����
		this->motion = Stand;	//�f�t�H���g�͗������
		this->jumPow = -6.0f;	//�W�����v��
		this->fallSpeed = 0.0f; 
		this->maxFallSpeed = 10.0f;	//�����ő呬�x
		this->gravity = ML::Gravity(16) * 4; //�d��
		this->dSpeed = 5.0f;	//�_�b�V�������x
		this->nSpeed = 2.0f;	//�ʏ펞���x
		this->moveVec.x = 10;    //�ړ���
		this->moveVec.y = 10;
		this->pos.x = position.x;
		this->pos.y = position.y;
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
		this->moveCnt++;
		this->animCnt++;
		this->Think();	//�v�l�E�󋵔��f
		this->Move();	//���[�V�����ɑΉ���������
		//�߂荞�܂Ȃ��ړ�����
		ML::Vec2 est = this->moveVec;
		this->CheckMove(est);

		//�v���C���[�����蔻��
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("�v���C���[");
			for (auto it = targets->begin(); it != targets->end(); ++it)//Enemy�����J��Ԃ�
			{
				//����ɐڐG�̗L�����m�F������
				if ((*it)->CheckHit(me))
				{
					//����Ƀ_���[�W�̏������s�킹��
					(*it)->Received(this);
					break;
				}
			}
		}

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(di.ret, di.draw, di.src);
	}


	//-------------------------------------------------------------------
	//�v�l�E�󋵔��f ���[�V��������
	void Object::Think()
	{
		//���[�V�����̕ύX�ȊO�̏����͂��Ȃ�
		BChara::Motion nm = this->motion;	//���݂̏�Ԃ��w��
		switch (nm)
		{
			//����
		case Stand: 
			if (this->CheckFoot() == false) { nm = Fall; }
			nm = Walk;
			break;
			// ����
		case Walk: 
			if (this->CheckLeft() == true|| this->CheckRight() == true)
			{
				nm = Turn;
			}
			if (this->CheckFoot() == false) { nm = Fall; }
			break;
			//�㏸��
		case Turn:
			if (this->moveCnt >= 5) { nm = Walk; }
			//this->Kill(); //�ŏI��i
			break;
		case Jump:
			if (this->moveVec.y >= 0)      { nm = Fall; }
			break; 
			//������
		case Fall:
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
			//�_�b�V����
		case Dash: 
			break;
			//��ї��u��(�W�����v�\������)
		case TakeOff:	
			break;
			//���n(�W�����v�㓮��)
		case Landing:	
			if (this->moveCnt >= 3) { 
				nm = Stand; }
			//if (this->CheckFoot() == false) { nm = Fall; }
			break;
		}
		this->UpdateMotion(nm);
	}
	//-----------------------------------------------------------------------------
	void Object::Move()//���[�V�����ɑΉ���������
	{
		//(���[�V�����͕ύX���Ȃ��j
		//�d�͂̏���
		switch (this->motion) 
		{
		default:
			if (this->moveVec.y < 0 || this->CheckFoot() == false) //�㏸��or�����ɒn�ʂ��Ȃ��ꍇ
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->maxFallSpeed);
			else //�����ɒn�ʂ�����ꍇ
				this->moveVec.y = 0; 
		    	//this->fallSpeed = 0;
			break;
			//�d�͉����𖳌������郂�[�V�����͉��ɏ���
		case Unnon: break;
		}
		//�ړ����x����
		switch (this->motion)
		{
		default:
			if (this->moveVec.x < 0)
				this->moveVec.x = min(this->moveVec.x + this->dSpeed, 0);
			else
				this->moveVec.x = max(this->moveVec.x - this->dSpeed, 0);
			break;
			//�ړ����x�����𖳌������郂�[�V�����͉��ɏ���
		case Unnon: break;
		}
		switch (this->motion)//���[�V�������Ƃ̌ŗL�̏���
		{
		case Stand: //����
			break;
		case Walk: // ����
			if (this->angle_LR == Left)
			{
				this->moveVec.x = 3;
			}
			if (this->angle_LR == Right)
			{
				this->moveVec.x = -3;
			}
			break;
		case Jump: //�㏸��
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->jumPow;//�����ݒ�	
			}
			if (this->CheckHead() == true)
			{
				this->moveVec.y = 0;
			}
			break;
		case Fall: //������
		/*	this->fallSpeed = this->gravity;
			this->moveVec.y += this->fallSpeed;	*/ //�Ȃ��Ă��@�\����
			break;
		case Dash: //�_�b�V����
			break;
		case TakeOff:	//��ї��u��(�W�����v�\������)
			break;
		case Landing:	//���n(�W�����v�㓮��)
			break;
		case Turn:
			if (this->moveCnt == 3)
			{
					if (this->angle_LR == Left) { 
						this->angle_LR = Right; 
					}
					else this->angle_LR = Left;
			}
			break;
		}
	}
	//-----------------------------------------------------------------------------
	BChara::DrawInfo Object::Anim() //�A�j���[�V��������
	{
		string ret;
		BChara::DrawInfo imageTable[] =
		{
			{ ML::Box2D(-32,-32,64,64),ML::Box2D(0,0,64,64),ML::Color(1,1,1,1),this->res->EnemyImg},
			{ ML::Box2D(-32,-32,64,64), ML::Box2D(64*1,0,64,64), ML::Color(1,1,1,1),this->res->EnemyImg},
			{ ML::Box2D(-32,-32,64,64), ML::Box2D(64*2,0,64,64), ML::Color(1,1,1,1),this->res->EnemyImg },
			{ ML::Box2D(-32,-32,64,64), ML::Box2D(64*3,0,64,64), ML::Color(1,1,1,1),this->res->EnemyImg },
		};
		BChara::DrawInfo rtv;
		int	work;
		switch (this->motion)
		{
		default: break;
		//����
		case Stand: 
			work = this->animCnt / 60;
			rtv = imageTable[0]; 
			break;
		// ����
		case Walk: 
			work = this->animCnt / 20;
			rtv = imageTable[1+work%3];
			break;
		//�㏸��
		case Jump: 
			rtv = imageTable[0];
			break;	
		//������			
		case Fall: 
			rtv = imageTable[0];
			break;		
		//�_�b�V����
		case Dash: 
			break;		
		//��ї��u��(�W�����v�\������)
		case TakeOff:
			break;	
			//���n(�W�����v�㓮��)		
		case Landing: 
			rtv = imageTable[0];
			break;		
		case Turn:
			rtv = imageTable[0];
		} 
		//�摜�̍��E���]����
		if (false == this->angle_LR)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	void Object::Received(BChara* from_) //�A�C�e����G�Ƃ̐ڐG���̏���(�󂯐g)
	{
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, ML::Vec2& position)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->Initialize(position)) {
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