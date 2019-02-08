//-------------------------------------------------------------------
//�}�b�v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Goal.h"
#include  "Task_Toge.h"
#include  "Task_Block.h"
#include  "Task_Player.h"
#include  "Task_Enemy00.h"
#include  "Task_GDoor.h"
#include  "Task_GKey.h"
#include  "Task_Yazirusi.h"
namespace  Map2D
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
		this->render2D_Priority[1] = 0.9f;
		this->imageName = "MapChipImg";
		//�}�b�v�̃[���N���A
		/*for (int y = 0; y < 100; ++y) {
			for (int x = 0; x < 100; ++x) {
				this->arr[y][x] = 0;
			}
		}*/
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);
		//�}�b�v�`�b�v���̏�����
		for (int c = 0; c < 8 * 5; ++c) {
			int x = (c % 8);
			int y = (c / 8);
			this->chip[c] = ML::Box2D(x * 32, y * 32, 32, 32);
		}
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�J���������S�Ƀ}�b�v�O���w���Ă��邩���ׂ�
		if (false == this->hitBase.Hit(ge->camera2D)) {
			return; //���S�ɊO�ɏo�Ă����炻�̎��_�ŕ`�揈��������߂�
		}
		////�J�����ƃ}�b�v���d�Ȃ��Ă���͈͂����̋�`�����
		//RECT  c = {
		//	ge->camera2D.x,
		//	ge->camera2D.y,
		//	ge->camera2D.x + ge->camera2D.w,
		//	ge->camera2D.y + ge->camera2D.h };
		//RECT  m = {
		//	this->hitBase.x,
		//	this->hitBase.y,
		//	this->hitBase.x + this->hitBase.w,
		//	this->hitBase.y + this->hitBase.h };
		////�Q�̋�`�̏d�Ȃ��Ă���͈͂����̋�`�����߂�
		//RECT  isr;
		//isr.left = max(c.left, m.left);
		//isr.top = max(c.top, m.top);
		//isr.right = min(c.right, m.right);
		//isr.bottom = min(c.bottom, m.bottom);

		////���[�v�͈͂�����
		//int sx, sy, ex, ey;
		//sx = isr.left / 32;
		//sy = isr.top / 32;
		//ex = (isr.right - 1) / 32;
		//ey = (isr.bottom - 1) / 32;
		for (int y = 0; y < this->sizeY; ++y) {
			for (int x = 0; x < this->sizeX; ++x) {
				if (this->arr[y][x] > -1) {
					ML::Box2D  draw(x *32, y *32, 32, 32);

					draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
					DG::Image_Draw(this->imageName, draw, this->chip[this->arr[y][x]]);
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�}�b�v�t�@�C���ǂݍ���(���͉������j
	bool Object::Load(const  string&  fpath_)
	{
		//�t�@�C���p�X�����
		string  filePath = fpath_;
		//�t�@�C�����J���i�ǂݍ��݁j
		ifstream   fin(filePath);
		if (!fin) { return  false; }//�ǂݍ��ݎ��s  
        //�`�b�v�t�@�C�����̓ǂݍ��݂ƁA�摜�̃��[�h 
		DG::Image_Create(this->imageName, "./data/mapChip/TestMapChip01.png");
	
		//�X�e�[�W�͈͂𒲂ׂ�------------------------------------------
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		string setX, setY;//sX,sY�ɑ��������
		stringstream sX, sY;//sizeX,sizeY�ɑ��������

		getline(fin, setX);
		istringstream  ss_sX(setX);
		getline(fin, setY);
		istringstream  ss_sY(setY);	
		sX << setX;
		sX >> this->sizeX;
		sY << setY;
		sY >> this->sizeY;

		this->arr.resize(sizeY);
		for (int i = 0; i < sizeY; ++i)
		{
			this->arr[i].resize(sizeX);
		}
		///->hitBase = ML::Box2D(0, 0, this->sizeX * 32, this->sizeY * 32);
		///�}�b�v�z��f�[�^�̓ǂݍ���
		///for (int y = 0; y < this->sizeY; ++y) {
		///	for (int x = 0; x < this->sizeX; ++x) {
		///		fin >> this->arr[y][x];

		//�z��Ƀf�[�^����荞��  //CSV�Ή�
		for (int y = 0; y < this->sizeY; ++y)
		{	
			string lineText;
			getline(fin, lineText);
			istringstream  ss_lt(lineText);
			for (int x = 0; x < this->sizeX; ++x)
			{
				string  tc;
				getline(ss_lt, tc, ',');
				stringstream ss;
				ss << tc;
				ss >> this->arr[y][x];
				if (this->arr[y][x] != -1)
				{
					switch (this->arr[y][x])
					{
					case -2://�S�[��
					{
						 //����M�~�b�N�̐���
						//Gimmic g = Gimmic::Create(x * 32,y * 32);
						auto gl = Goal::Object::Create(true, ML::Vec2(x * 32, y * 32));
						break;
					}
					case -3://�g�Q
					{
						auto tg = Toge::Object::Create(true, ML::Vec2(x * 32, y * 32));
						break;
					}
					case -4://�u���b�N��
					{
						auto bl = Block::Object::Create(true, ML::Vec2(x * 32, y * 32),Block::Type::left);
						break;
					}
					case -5://�u���b�N�E
					{
						auto bl = Block::Object::Create(true, ML::Vec2(x * 32, y * 32), Block::Type::right);
						break;
					}
					case -10://�v���C���[
					{
						auto pl = Player::Object::Create(true,ML::Vec2(x * 32, y * 32));
						break;
					}
					case -11://���˂݁[
					{
						auto e = Enemy00::Object::Create(true,ML::Vec2(x*32,y*32));
						break;
					}
					case -12://��
					{
						auto K = Gkey::Object::Create(true, ML::Vec2(x * 32, y * 32));
						break;
					}
					case -13://����
					{
						auto KD = GDoor::Object::Create(true, ML::Vec2(x * 32, y * 32));
						break;
					}
					case -14://���Ŕ�
					{
						auto yk = Yazirusi::Object::Create(true, ML::Vec2(x * 32, y * 32),Yazirusi::Type::left);
						break;
					}
					case -15://���Ŕ���
					{
						auto yk = Yazirusi::Object::Create(true, ML::Vec2(x * 32, y * 32), Yazirusi::Type::upleft);
						break;
					}
					case -16://���ŉE
					{
						auto yk = Yazirusi::Object::Create(true, ML::Vec2(x * 32, y * 32), Yazirusi::Type::right);
						break;
					}
					case -17://���ŔE��
					{
						auto yk = Yazirusi::Object::Create(true, ML::Vec2(x*32 , y *32), Yazirusi::Type::upright);
						break;
					}

					default:
						//�ʏ�}�b�v�̐���
						this->collision.push_back(ML::Box2D(x * 32, y * 32, 32, 32));
						break;
					}
				}
				/*else
					this->collision.push_back(ML::Box2D(0, 0, 0, 0));*/
			}
		}
		//�}�b�v�S�̂̋�`
		this->hitBase = ML::Box2D(0, 0, this->sizeX * 32, this->sizeY * 32);
		//this->arr[0][0] = -1;
		//if (this->arr[y][x] == -1)
		//			this->hitBase = ML::Box2D(0, 0, 0, 0);	
		
		//�t�@�C�������
		fin.close();
		return  true;
	}
	//�����蔻��
	bool Object::CheckHit(const ML::Box2D& h_)
	{
		//�����ɂ����蔻��ihitBase�j���Ăяo��
		for (size_t i = 0; i < this->collision.size(); ++i)
		{
			if (this->collision[i].Hit(h_))
			{
				return true;
			}
		}
		return false;

		//RECT r = { h_.x, h_.y, h_.x + h_.w, h_.y + h_.h }; //�n���ꂽ��̋�`����
		//RECT m = { this->hitBase.x, this->hitBase.y, this->hitBase.x + this->hitBase.w, this->hitBase.y + this->hitBase.h }; //�}�b�v�S�̋�`
		////�ۂߍ���
		//if (r.left < m.left) { r.left = m.left; }
		//if (r.top < m.top) { r.top = m.top; }
		//if (r.right > m.right) { r.right = m.right; }
		//if (r.bottom > m.bottom) { r.bottom = m.bottom; }
		
	/*	int sx, sy, ex, ey;
		sx = r.left / 32;
		sy = r.top / 32;
		ex = (r.right - 1) / 32;
		ey = (r.bottom - 1) / 32;*/
		////
		//for (int y = sy; y <= ey; ++y)
		//{
		//	for (int x = sx; x <= ex; ++x)
		//	{
		//		if (-1 != this->arr[y][x])
		//		{
		//			return true;
		//		}
		//	}
		//}
		//return false;
	}
	
	//-------------------------------------------------------------------
	//�}�b�v�O�������Ȃ��悤�ɃJ�������ʒu��������
	void  Object::AjastCameraPos()
	{
		//�J�����ƃ}�b�v�͈̔͂�p��
		RECT  c = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h };
		RECT  m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h };

		//�J�����̈ʒu�𒲐�
		if (c.right  > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top  < m.top) { ge->camera2D.y = m.top; }
		//�}�b�v���J������菬�����ꍇ
		if (this->hitBase.w < ge->camera2D.w) { ge->camera2D.x = m.left; }
		if (this->hitBase.h < ge->camera2D.h) { ge->camera2D.y = m.top; }
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