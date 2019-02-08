//-------------------------------------------------------------------
//�G���f�B���O���
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_End.h"
#include  "Task_Title.h"

namespace  End
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->EndBG = "EndBG";
		DG::Image_Create(this->EndBG, "./data/image/EndBG.png");
		this->FOBG = "FOBG";
		DG::Image_Create(this->FOBG, "./data/image/�t�F�[�h�A�E�g�p.jpg");
		this->stBG = "stBG";
		DG::Image_Create(this->stBG, "./data/image/st.jpg");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->EndBG);
		DG::Image_Erase(this->FOBG);
		DG::Image_Erase(this->stBG);
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
		this->TimeCount = 0;
		this->stCount = 0;
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
			auto NextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (this->TimeCount < 2000) {
			this->TimeCount += 2;
		}
		else if (this->stCount < 199){ this->stCount++; }

		auto in = DI::GPad_GetState("P1");
		if (in.ST.down) {
			//���g�ɏ��ŗv��
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, ge->screen2DWidth, ge->screen2DHeight);
		ML::Box2D src(0, this->TimeCount, 644, 480);

		ML::Box2D draw1(0, 0, 640, 480);
		ML::Box2D src1(0, 0, 640, 480);
		DG::Image_Draw(this->res->EndBG,draw,src);//�X�N���[��������
		if (this->stCount > 100)
		{
			DG::Image_Draw(this->res->stBG, draw1, src1);
		}
		if (this->stCount < 100) {
			DG::Image_Draw(this->res->FOBG, draw, src, ML::Color(this->stCount % 100 * 0.01, 1, 1, 1));
		}
		else { DG::Image_Draw(this->res->FOBG, draw, src, ML::Color(1.0f-float(this->stCount % 100 * 0.01), 1, 1, 1)); }
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