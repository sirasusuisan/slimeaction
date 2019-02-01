//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
//#include  "Task_StageNumber.h"
#include  "Task_Stageserect.h"
#include  "Task_Game.h"
#include  "easing.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "TitleLogoImg";
		this->imageName2 = "PressKeyImg";
		this->imageName3 = "HiluImg";
		this->cloudImage1 = "Cloud1";		//20190118実装
		this->cloudImage2 = "Cloud2";		//20190118実装

		DG::Image_Create(this->imageName2, "./data/image/PressKey.png");
		DG::Image_Create(this->imageName3, "./data/image/hiru.png");
		DG::Image_Create(this->imageName, "./data/image/Title.png");
		DG::Image_Create(this->cloudImage1, "./data/image/cloud1.png");
		DG::Image_Create(this->cloudImage2, "./data/image/cloud2.png");

		//BGM--------------------------------------------------------
		//DM::Sound_CreateStream("BGM", "./data/BGM/Title.wav");
		//DM::Sound_Play("BGM", false);//再生開始
		//DM::Sound_Volume("BGM", 1000);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->imageName2);
		DG::Image_Erase(this->imageName3);
		DG::Image_Erase(this->cloudImage1);
		DG::Image_Erase(this->cloudImage2);
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
		this->logoPosY = 0;
		this->count = 0;
		this->cloudposX = 40;
		this->cloudposX1 = 360;
		this->cloudposY1 = 40;
		easing::Init();
		//生成
		easing::Create("TitleLogoImg", easing::CIRCINOUT, -480, 100, 80);//easing開始地点、終了地点、かかる時間

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
			auto nextTask = Stageserect::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in1 = DI::GPad_GetState("P1");
		easing::UpDate();
		this->count++;
		this->cloudposX += 2;
		if (cloudposX >= 600)
		{
			cloudposX = -480;
		}
		this->cloudposX1++;
		if (cloudposX1 >= 480)
		{
			cloudposX1 = -440;
		}
		/*this->logoPosY += 48;
		if (this->logoPosY >= 0) {
			this->logoPosY = 0;
		}
		*/
		if (this->logoPosY == 0) {
			//this->logoPosY++;
			easing::Start("TitleLogoImg");
		}
		/*if (easing::GetState("TitleLogoImg") == easing::EQ_STATE::EQ_END)
			{
				
			}*/

		if (in1.ST.down) {
			//自身に消滅要請
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//Title
		ML::Box2D  draw(0, 0, 640, 100);
		ML::Box2D  src(0, 0, 640, 100);

		//Presskey
		ML::Box2D  draw2(125, 300, 400, 80);
		ML::Box2D  src2(0, 0, 550, 80);

		//昼の背景
		ML::Box2D  draw3(0, 0, 640, 480);
		ML::Box2D  src3(0, 0, 1280, 720);

		//雲１
		ML::Box2D draw4(40, 10, 300, 200);
		ML::Box2D src4(0, 0, 200, 180);

		draw4.Offset(cloudposX , 10);
		//雲2
		ML::Box2D draw5(360, 200, 120, 100);
		ML::Box2D src5(0, 0, 120, 100);

		draw5.Offset(cloudposX1, cloudposY1);
		//draw.Offset(0, logoPosY);
		draw.Offset(-10,(int)easing::GetPos("TitleLogoImg"));
		DG::Image_Draw(this->res->imageName3, draw3, src3);
		DG::Image_Draw(this->res->cloudImage1, draw4, src4);
		DG::Image_Draw(this->res->cloudImage2, draw5, src5);
		DG::Image_Draw(this->res->imageName, draw, src);
		if (this->count > 0)
		{
			if ((this->count / 32) %2 == 0)
			{
				return;
			}
		}
		DG::Image_Draw(this->res->imageName2, draw2, src2);
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