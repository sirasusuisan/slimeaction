//-------------------------------------------------------------------
//エンディング画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_End.h"
#include  "Task_Title.h"

namespace  End
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->EndBG = "EndBG";
		DG::Image_Create(this->EndBG, "./data/image/EndBG.png");
		this->FOBG = "FOBG";
		DG::Image_Create(this->FOBG, "./data/image/フェードアウト用.jpg");
		this->stBG = "stBG";
		DG::Image_Create(this->stBG, "./data/image/st.jpg");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->EndBG);
		DG::Image_Erase(this->FOBG);
		DG::Image_Erase(this->stBG);
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
		this->TimeCount = 0;
		this->stCount = 0;
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
			auto NextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (this->TimeCount < 2000) {
			this->TimeCount += 2;
		}
		else if (this->stCount < 199){ this->stCount++; }

		auto in = DI::GPad_GetState("P1");
		if (in.ST.down) {
			//自身に消滅要請
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, ge->screen2DWidth, ge->screen2DHeight);
		ML::Box2D src(0, this->TimeCount, 644, 480);

		ML::Box2D draw1(0, 0, 640, 480);
		ML::Box2D src1(0, 0, 640, 480);
		DG::Image_Draw(this->res->EndBG,draw,src);//スクロールするやつ
		if (this->stCount > 100)
		{
			DG::Image_Draw(this->res->stBG, draw1, src1);
		}
		if (this->stCount < 100) {
			DG::Image_Draw(this->res->FOBG, draw, src, ML::Color(this->stCount % 100 * 0.01, 1, 1, 1));
		}
		else { DG::Image_Draw(this->res->FOBG, draw, src, ML::Color(1.0f-float(this->stCount % 100 * 0.01), 1, 1, 1)); }
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