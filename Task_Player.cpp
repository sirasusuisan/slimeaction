//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include   "Task_Map2D.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->stopImage = "stopImage";
		this->moveImage = "moveImage";
		this->jumpImage = "jumpImage";
		this->verticalStopImage = "vStopImg";
		this->verticalMoveImage = "vMoveImg";
		this->verticalJumpImage = "vJumpImg";
		this->sideStopImage = "sStopImg";
		this->sideMoveImage = "sMoveImg";
		this->sideJumpImage = "sJumpImg";
		DG::Image_Create(this->stopImage, "./data/image/�Î~�X���C��.png");
		DG::Image_Create(this->moveImage, "./data/image/�ړ��X���C��.png");
		DG::Image_Create(this->jumpImage, "./data/image/�W�����v�X���C��.png");
		DG::Image_Create(this->verticalStopImage, "./data/image/�c�ό`�Î~�X���C��.png");
		DG::Image_Create(this->verticalMoveImage, "./data/image/�c�ό`�ړ��X���C��.png");
		DG::Image_Create(this->verticalJumpImage, "./data/image/�c�ό`�W�����v�X���C��.png");
        DG::Image_Create(this->sideStopImage, "./data/image/���ό`�Î~�X���C��.png");
		DG::Image_Create(this->sideMoveImage, "./data/image/���ό`�ړ��X���C��.png");
		DG::Image_Create(this->sideJumpImage, "./data/image/���ό`�W�����v�X���C��.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->stopImage);
		DG::Image_Erase(this->moveImage);
		DG::Image_Erase(this->jumpImage);
		DG::Image_Erase(this->verticalStopImage);
		DG::Image_Erase(this->verticalMoveImage);
		DG::Image_Erase(this->verticalJumpImage);
        DG::Image_Erase(this->sideStopImage);
		DG::Image_Erase(this->sideMoveImage);
		DG::Image_Erase(this->sideJumpImage);
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
		this->render2D_Priority[1] = 0.5f; //��O�\��
		this->angle_LR = Right; //�f�t�H���g�͉E����
		this->form_VS = Side;//�f�t�H���g�͉�
		this->hitBase = ML::Box2D(0, 0, 0, 0);
		this->controllerName = "P1";
		this->motion = Stand;	//�f�t�H���g�͗������
		this->jumPow = -5.0f;	//�W�����v��
		this->fallSpeed = 0.0f; 
		this->maxFallSpeed = 10.0f;	//�����ő呬�x
		this->gravity = ML::Gravity(16) * 4; //�d��
		this->dSpeed = 5.0f;	//�_�b�V�������x
		this->nSpeed = 2.0f;	//�ʏ펞���x
		this->moveVec.x = 0;    //�ړ���
		this->moveVec.y = 0;
		this->pos.x = 300;
		this->pos.y = 50;
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		Rotation();
		DG::Image_Draw(di.ret, di.draw, di.src);
	}
	//----------------------------------------------------------------------------
	//��]��
	void Object::Rotation()
	{
		BChara::DrawInfo di = this->Anim();
		switch (this->motion)
		{
		default:
			DG::Image_Rotation(di.ret, 0.0f, ML::Vec2(di.draw.w, di.draw.h));
			break;
		case Stick_Stand:
		case Stick_Move:
			if (this->CheckRight() == true)
			{
				DG::Image_Rotation(di.ret, 300.0f/*90�x�H*/, ML::Vec2(di.draw.w / 2, di.draw.h / 2));
			}
			break;
		}

	}
	//-------------------------------------------------------------------
	//�v�l�E�󋵔��f ���[�V��������
	void Object::Think()
	{
		//���[�V�����̕ύX�ȊO�̏����͂��Ȃ�
		auto in = DI::GPad_GetState(this->controllerName);
		//���݂̏�Ԃ��w��
		BChara::Motion nm = this->motion;	
		this->preMotion = nm;
		this->preForm_VS = this->form_VS;
		switch (nm)
		{
		//����
		case Stand: 
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B1.down)     { nm = Jump; }
			if (in.B2.down)       { nm = Transform_Stand; }
			if (this->CheckFoot() == false)         { nm = Fall; }
			if (in.B3.on) { nm = Stick_Stand; }
			break;
		// ����
		case Walk: 
			if (in.LStick.L.off && in.LStick.R.off) { nm = Stand; }
			if (in.B1.down)                         { nm = Jump; }
			if (in.B2.down)                           { nm = Transform_Stand; }
			if (this->CheckFoot() == false)         { nm = Fall; }
			if (in.B3.on) { nm = Stick_Stand; }
			break;
		//�㏸��
		case Jump:
			if (this->moveVec.y >= 0)      { nm = Fall; }
			break; 
		//������
		case Fall:
			if (this->CheckFoot() == true) { nm = Landing; }
			break;	
		case Dash://�_�b�V���� 
			break;
		case TakeOff://��ї��u��(�W�����v�\������)
			break;
		//���n(�W�����v�㓮��)
		case Landing:	
			if (this->moveCnt >= 3) { nm = Stand; }
			//if (this->CheckFoot() == false) { nm = Fall; }
			break;
		//�ǒ���t��--------------------------------------------------------
		case Stick_Stand:
			if (in.B3.off)
			{
				nm = Stand;
				this->gravity = ML::Gravity(16) * 4;
			}
			if (in.B3.off && CheckFoot() == false)
			{
				nm = Fall;
				this->gravity = ML::Gravity(16) * 4;
			}
			if (in.LStick.L.on) { nm = Stick_Move; }
			if (in.LStick.R.on) { nm = Stick_Move; }
			break;
		//�ړ�
		case Stick_Move:
			if (in.LStick.L.off && in.LStick.R.off) { nm = Stick_Stand; }
			break;
        //�ό`------------------------------------------------------------
		//�i�Î~�j
		case Transform_Stand:
			if (in.LStick.L.on) { nm = Transform_Move; }
			if (in.LStick.R.on) { nm = Transform_Move; }
			if (in.B1.down)     { nm = Transform_Jump; }
			if (in.B2.down)      { nm = Stand; }
			if (this->CheckFoot() == false) { nm = Transform_Fall; }
			break;
		//�i�ړ��j
		case Transform_Move:
			if (in.LStick.L.off && in.LStick.R.off) { nm = Transform_Stand; }
			if (in.B1.down) { nm = Transform_Jump; }
			if (in.B2.down) { nm = Stand; }
			if (this->CheckFoot() == false) { nm = Transform_Fall; }
			break;
		//�i�W�����v�j
		case Transform_Jump:
			if (this->moveVec.y >= 0) { nm = Transform_Fall; }
			break;
		//�i�����j
		case Transform_Fall:
			if (this->CheckFoot() == true) { nm = Transform_Stand; }
			break;
		}
		this->UpdateMotion(nm);
	}
	//-----------------------------------------------------------------------------
	//���[�V�����ɑΉ���������
	void Object::Move()
	{
		//(���[�V�����͕ύX���Ȃ��j
		auto in = DI::GPad_GetState(this->controllerName);
		//�}�b�v�^�X�N�Ăяo��
		auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
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
		case Stick_Stand:
		case Stick_Move:
			this->moveVec.y = 0;
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
		//�����蔻��
		if (this->preMotion != this->motion)
		{	
			switch (this->motion)
			{
				//��{��
			default:
				//�}�b�v�ƕύX��̂����蔻��{���݈ʒu��Box2D�̔�����s��
				if (!map->CheckHit(ML::Box2D(-32, 0, 64, 64).OffsetCopy(this->pos)))
				{
					this->hitBase = ML::Box2D(-32, 0, 64, 64);
				}
				else
				{
					//�������Ă�Ȃ�ό`���Ȃ�����motion��߂�
					this->motion = this->preMotion;
				}
				break;
				//�ό`��(�Î~)
			case Transform_Stand:
			case Transform_Move:
			case Transform_Jump:
			case Transform_Fall:
				//�c
				if (form_VS == Vertical)
				{
					//�}�b�v�ƕύX��̂����蔻��{���݈ʒu��Box2D�̔�����s��
					if (!map->CheckHit(ML::Box2D(-16, 0, 32, 64).OffsetCopy(this->pos)))
					{
						this->hitBase = ML::Box2D(-16, 0, 32, 64);
					}
					else
					{
						//�������Ă�Ȃ�ό`���Ȃ�����motion��߂�
						this->motion = this->preMotion;
					}
				}
				//��
				if (form_VS == Side)
				{
					//�}�b�v�ƕύX��̂����蔻��{���݈ʒu��Box2D�̔�����s��
					if (!map->CheckHit(ML::Box2D(-32, 32, 64, 32).OffsetCopy(this->pos)))
					{
						this->hitBase = ML::Box2D(-32, 32, 64, 32);
					}
					else
					{
						//�������Ă�Ȃ�ό`���Ȃ�����motion��߂�
						this->motion = this->preMotion;
					}
				}
				break;
			}
		}
		//���[�V�������Ƃ̌ŗL�̏���
		switch (this->motion)
		{
		case Stand: //����
			break; 
		// ����
		case Walk:
		case Transform_Move:
			if (in.LStick.L.on)
			{
				this->angle_LR = Left;			
				this->moveVec.x = -this->nSpeed;
				//this->CheckMove(moveVec);				
			}
			if (in.LStick.R.on)
			{
				this->angle_LR = Right;				
  				this->moveVec.x = this->nSpeed;
			    //this->CheckMove(moveVec);		
			}
			break;
		//�㏸��
		case Jump: 
		case Transform_Jump:
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->jumPow;//�����ݒ�	
			}
			if (this->CheckHead() == true)
			{
				this->moveVec.y = 0;
			}
			if (in.LStick.L.on)
			{
				this->angle_LR = Left;
				this->moveVec.x = -this->nSpeed;
				//this->CheckMove(moveVec);
			}
			if (in.LStick.R.on)
			{
				this->angle_LR = Right;
				this->moveVec.x = this->nSpeed;
				//this->CheckMove(moveVec);
			}
			break;
		//������
		case Fall: 
		case Transform_Fall:
		/*	this->fallSpeed = this->gravity;
			this->moveVec.y += this->fallSpeed;	*/ //�Ȃ��Ă��@�\����
			if (in.LStick.L.on)
			{
				this->angle_LR = Left;
				this->moveVec.x = -this->nSpeed;
				//this->CheckMove(moveVec);
			}
			if (in.LStick.R.on)
			{
				this->angle_LR = Right;
				this->moveVec.x = this->nSpeed;
				//this->CheckMove(moveVec);
			}
			break;
		case Dash: //�_�b�V����
			break;
		case TakeOff:	//��ї��u��(�W�����v�\������)
			break;
		case Landing:	//���n(�W�����v�㓮��)
			break;
		//����t��
		case Stick_Stand:
			break;
		case Stick_Move:
			if (this->CheckRight() == true)
			{
				if (in.LStick.L.on)
				{
					this->angle_LR = Left;
					this->moveVec.y = this->nSpeed;
				}
				if (in.LStick.R.on)
				{
					this->angle_LR = Right;
					this->moveVec.y = -this->nSpeed;
				}
			}
			break;
		//�ό`(�Î~)
		case Transform_Stand:
			if (in.LStick.U.on)
			{	
				//�}�b�v�ƕύX��̂����蔻��{���݈ʒu��Box2D�̔�����s��
				if (!map->CheckHit(ML::Box2D(-16, 0, 32, 64).OffsetCopy(this->pos)))
				{
					this->hitBase = ML::Box2D(-16, 0, 32, 64);
					this->form_VS = Vertical;
				}
				else
				{
					//�������Ă�Ȃ�ό`���Ȃ�����motion��߂�
					//this->form_VS = this->preForm_VS;
				}	
			}
			if (in.LStick.D.on)
			{
				//�}�b�v�ƕύX��̂����蔻��{���݈ʒu��Box2D�̔�����s��
				if (!map->CheckHit(ML::Box2D(-32, 32, 64, 32).OffsetCopy(this->pos)))
				{
					this->hitBase = ML::Box2D(-32, 32, 64, 32);
					this->form_VS = Side;
				}
				else
				{
					//�������Ă�Ȃ�ό`���Ȃ�����motion��߂�
					//this->form_VS = this->preForm_VS;
				}
			}
			break;
		}
	}
	//-----------------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo Object::Anim() 
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//string ret;
		//�`��ݒ�i�T�C�Y�͓����蔻��ƘA���j
		BChara::DrawInfo imageTable[] =
		{
			//	draw		src						color
	/*0*/   { this->hitBase, ML::Box2D(0,0,64,64), ML::Color(1,1,1,1), this->res->stopImage },	//��~1
			{ this->hitBase, ML::Box2D(64,0,64,64), ML::Color(1,1,1,1), this->res->stopImage }, //��~2

			{ this->hitBase, ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //�ړ�1
			{ this->hitBase, ML::Box2D(64, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //�ړ�2
			{ this->hitBase, ML::Box2D(128, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //�ړ�3

	//      { this->hitBase, ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//�W�����v�O1
	//		{ this->hitBase, ML::Box2D(64 , 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//�W�����v�O2
    /*5*/   { this->hitBase, ML::Box2D(64 * 2, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//�W�����v�O
			{ this->hitBase, ML::Box2D(64 * 3, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//�㏸
			{ this->hitBase, ML::Box2D(64 * 4, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//���~
			{ this->hitBase, ML::Box2D(64 * 5, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//�W�����v��1
            { this->hitBase, ML::Box2D(64 * 6, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//�W�����v��2

    /*10*/  { this->hitBase, ML::Box2D(0, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalStopImage },//�c�ό`�Î~1
		    { this->hitBase, ML::Box2D(32, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalStopImage },//�c�ό`�Î~2

		    { this->hitBase, ML::Box2D(0, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//�c�ό`�ړ�1
	    	{ this->hitBase, ML::Box2D(32, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//�c�ό`�ړ�2
	    	{ this->hitBase, ML::Box2D(64, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//�c�ό`�ړ�3

    /*15*/ 	{ this->hitBase, ML::Box2D(32 * 2, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//�c�ό`�W�����v�O
	    	{ this->hitBase, ML::Box2D(32 * 3, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//�c�ό`�㏸
	    	{ this->hitBase, ML::Box2D(32 * 4, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//�c�ό`���~
	    	{ this->hitBase, ML::Box2D(32 * 5, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//�c�ό`�W�����v��1
	    	{ this->hitBase, ML::Box2D(32 * 6, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//�c�ό`�W�����v��2

	/*20*/  { this->hitBase, ML::Box2D(0, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideStopImage },//���ό`�Î~1
		    { this->hitBase, ML::Box2D(64, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideStopImage },//���ό`�Î~2

		    { this->hitBase, ML::Box2D(0, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//���ό`�ړ�1
		    { this->hitBase, ML::Box2D(64, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//���ό`�ړ�2
	    	{ this->hitBase, ML::Box2D(64 * 2, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//���ό`�ړ�3

	/*25*/	{ this->hitBase, ML::Box2D(64 * 2 , 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//���ό`�W�����v�O
		    { this->hitBase, ML::Box2D(64 * 3, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//���ό`�㏸
		    { this->hitBase, ML::Box2D(64 * 4, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//���ό`���~
		    { this->hitBase, ML::Box2D(64 * 5, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//���ό`�W�����v��1
		    { this->hitBase, ML::Box2D(64 * 6, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//���ό`�W�����v��2
		};
		BChara::DrawInfo rtv;
		int	work;
		//�A�j���[�V�������e���[�V�����ɑΉ�
		switch (this->motion)
		{
		default: break;
		//����
		case Stand: 
			work = this->animCnt / 40;
			rtv = imageTable[work%2]; 
			break;
		// ����
		case Walk: 
			work = this->animCnt / 20;
			rtv = imageTable[(work % 3)+2];
			break;
		//�㏸��
		case Jump: 
			work = this->animCnt / 30;
			rtv = imageTable[(work % 2) + 5];
			break;	
		//������			
		case Fall: 
			work = this->animCnt / 30;
			rtv = imageTable[(work % 3) + 7];
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
		//����t��
		case Stick_Stand:
			work = this->animCnt / 40;
			rtv = imageTable[work % 2];
			break;
		case Stick_Move:
			work = this->animCnt / 40;
			rtv = imageTable[(work % 3) + 2];
			break;
		//�ό`(�Î~)
		case Transform_Stand:
			//�c
			if (this->form_VS == Vertical)
			{
				work = this->animCnt / 40;
				rtv = imageTable[(work % 2) + 10];
			}
			//��
			if (this->form_VS == Side)
			{
				work = this->animCnt / 40;
				rtv = imageTable[(work % 2) + 20];
			}
			break;
		//�ό`(�ړ�)
		case Transform_Move:
			//�c
			if (this->form_VS == Vertical)
			{
				work = this->animCnt / 20;
				rtv = imageTable[(work % 3) + 12];
			}
			//��
			if (this->form_VS == Side)
			{
				work = this->animCnt / 20;
			    rtv = imageTable[(work % 3) + 22];
			}
			break;
			//�ό`(�W�����v)
		case Transform_Jump:
			//�c
			if (this->form_VS == Vertical)
			{
				work = this->animCnt / 30;
				rtv = imageTable[(work % 2) + 15];
			}
			//��
			if (this->form_VS == Side)
			{
				work = this->animCnt / 30;
				rtv = imageTable[(work % 2) + 25];
			}
			break;
		//�ό`�i�����j
		case Transform_Fall:
			//�c
			if (this->form_VS == Vertical)
			{
				work = this->animCnt / 20;
				rtv = imageTable[(work % 3) + 17];
			}
			//��
			if (this->form_VS == Side)
			{
				work = this->animCnt / 20;
				rtv = imageTable[(work % 3) + 27];
			}
			break;
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
	//�A�C�e����G�Ƃ̐ڐG���̏���(�󂯐g)
	void Object::Received(BChara* from_) 
	{
		if (this->playerhp <= 0) { this->Kill(); }
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