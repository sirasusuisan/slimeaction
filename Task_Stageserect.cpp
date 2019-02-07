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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->backimg = "BackImg";
		this->tyutorialse = "TSimg";
		DG::Image_Create(this->backimg, "data/image/black.png");
		DG::Image_Create(this->tyutorialse, "data/image/Tyutorialse.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->backimg);
		DG::Image_Erase(this->tyutorialse);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->stageNum = -1;
		this->count = 0;
		this->tyutorial = false;
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		
			if (!ge->QuitFlag() && this->nextTaskCreate) {
				if (this->tyutorial == true)
				{
					//★引き継ぎタスクの生成
					auto nextTask = StageNum::Object::Create(true,stageNum);
				}
			}
		

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		this->count++;

		if (in.L1.down)//Q
		{
			this->stageNum = 0;
			this->tyutorial = true;
			this->Kill();
		}
		if (in.R1.down)//w
		{
			this->stageNum = 1;
			this->tyutorial = true;
			this->Kill();
		}
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//haikei
		ML::Box2D draw(0, 0, 640, 480);
		ML::Box2D src(0, 0, 640, 480);

		//tyutorialserect
		ML::Box2D draw1(0, 60, 640, 80);
		ML::Box2D src1(0, 0, 892, 80);

		DG::Image_Draw(this->res->backimg, draw, src);
		if (this->count > 0)
		{
			if ((this->count / 32) % 2 == 0 )
			{
				return;
			}
		}
		DG::Image_Draw(this->res->tyutorialse, draw1, src1);
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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