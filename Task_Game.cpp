//-------------------------------------------------------------------
//ゲーム本編
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//DG::Font_Create("FontImage", "游ゴシック" , 16, 16);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		//DG::Font_Erase("FontImage");	
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(int sn)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		ge->camera2D = ML::Box2D(-200, -100, 640, 480);//取りあえず初期値設定
		//this->render2D_Priority[1] = 0.1f;
		//★タスクの生成
		//背景
		Back::Object::Create(true);
		
		//マップ
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
		//スタート
		auto s = Start::Object::Create(true);
		//プレイヤー
		/*auto pl = Player::Object::Create(true);
		//pl->pos = m->arr[pl->pos.y][pl->pos.x];
		pl->pos.x = 64;//480 / 2;
		pl->pos.y = 430;//270 / 2;*/
		//えねみー
		/*auto e = Enemy00::Object::Create(true);
		e->pos.x = 300;
		e->pos.y = 500;*/
		//カメラ
		auto camera = Camera::Object::Create(true);
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤー");
		camera->pos.x = pl->pos.x + 10;
		camera->pos.y = pl->pos.y + 10;
		camera->target = pl;
		//UI
		HP::Object::Create(true);
		KEY::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("フィールド");
		ge->KillAll_G("背景");
		ge->KillAll_G("プレイヤー");
		ge->KillAll_G("UI");
		ge->KillAll_G("ゴール");
		ge->KillAll_G("敵");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			if (flag)
			{
				auto  nextTask = Result::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		if (ge->GetTask_One_G<Goal::Object>("ゴール")->GetClearFlag())
		{
			flag = true;
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		/*auto pl = Player::Object::Create(true);
		ML::Box2D textBox(0, 0, 100, 48);
		ML::Color color(1, 1, 1, 1);
		string text = to_string(pl->pos.y);
		DG::Font_Draw("FontImage",textBox, text, color);*/
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
	Object::SP  Object::Create(bool  flagGameEnginePushBack_,int stagenumber)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->Initialize(stagenumber)) {
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