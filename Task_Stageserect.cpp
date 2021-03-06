//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Stageserect.h"
#include  "Task_StageNumber.h"
#include "Task_Game.h"


namespace  Stageserect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->backimg = "BackImg";
		this->tyutorialse = "TSimg";
		this->firstse = "firstse";
		DG::Image_Create(this->backimg, "data/image/black.png");
		DG::Image_Create(this->tyutorialse, "data/image/Tyutorialse.png");
		DG::Image_Create(this->firstse, "data/image/firstse.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->backimg);
		DG::Image_Erase(this->tyutorialse);
		DG::Image_Erase(this->firstse);
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		this->stageNum = -1;
		this->count = 0;
	/*	this->tyutorial = false;
		this->first = false;*/
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		
			if (!ge->QuitFlag() && this->nextTaskCreate) {
				//ψ«p¬^XNΜΆ¬
				auto nextTask = StageNum::Object::Create(true, stageNum);
			/*	if (this->tyutorial == true || this->first == true )
				{
					
				}*/
			}
		

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		this->count++;

		if (in.L1.down)
		{
			this->stageNum = 0;
			/*this->tyutorial = true;*/
			this->Kill();
		}
		if (in.R1.down)
		{
			this->stageNum = 1;
		/*	this->first = true;*/
			this->Kill();
		}
		
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		//haikei
		ML::Box2D draw(0, 0, 640, 480);
		ML::Box2D src(0, 0, 640, 480);

		//tyutorialserect
		ML::Box2D draw1(0, 60, 640, 80);
		ML::Box2D src1(0, 0, 892, 80);

		//firststageserect
		ML::Box2D draw2(0, 160, 640, 80);
		ML::Box2D src2(0, 0, 796, 86);
		DG::Image_Draw(this->res->backimg, draw, src);
		if (this->count > 0)
		{
			if ((this->count / 64) % 2 == 0 )
			{
				return;
			}
		}
		DG::Image_Draw(this->res->tyutorialse, draw1, src1);
		DG::Image_Draw(this->res->firstse, draw2, src2);
	}

	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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