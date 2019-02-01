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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "StageNumImg";
		this->haikeiImg = "HaikeiImg";
		DG::Image_Create(this->imageName, "./data/image/Tyutorial.png");
		DG::Image_Create(this->haikeiImg, "./data/image/black.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->haikeiImg);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(int stageNumber)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();
		number = stageNumber;
		//★データ初期化
		easing::Init();

		this->timecount = 0;

		this->logoPosY = -480;
		//生成
		easing::Create("StageNumImg", easing::CIRCINOUT, -480, 0, 80);//easingの開始地点、終了地点、かかる時間
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto nextTask = Game::Object::Create(true , number);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
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
	//	//自身に消滅要請
	//	this->Kill();
	//}
	//if (easing::GetState("StageNumImg") == easing::EQ_STATE::EQ_END)
	//{
	//
	//}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw(0, 100, 570, 80);
		ML::Box2D  src(0, 0, 570, 80);

		//背景
		ML::Box2D	draw2(0, 0, 640, 480);
		ML::Box2D	src2(0, 0, 640, 480);
		draw.Offset(0, logoPosY);
		draw.Offset(0, (int)easing::GetPos("StageNumImg"));
		DG::Image_Draw(this->res->haikeiImg, draw2, src2);
		DG::Image_Draw(this->res->imageName, draw, src);
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_,int stageNumber)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->Initialize(stageNumber)) {
				ob->Kill();//イニシャライズに失敗したらKill
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