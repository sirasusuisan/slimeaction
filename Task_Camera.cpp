//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Camera.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"

namespace Camera
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		/*this->imageName = "SpriteImg";
		DG::Image_Create(this->imageName, "./data/image/妖精.png");*/
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		/*DG::Image_Erase(this->imageName);*/
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

	/*	this->render2D_Priority[1] = 0.5f;*/
		//★データ初期化
		
		this->pos.x = 0;
		this->pos.y = 0;

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
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		CameraPosition();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		/*ML::Box2D draw(-16, -16, 32, 32);
		draw.Offset(this->pos);
		ML::Box2D src(0, 0, 32, 32);

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 1, 1, 1));*/
	}
	//-------------------------------------------------------------------
	void Object::CameraPosition()
	{
		if (auto tg = this->target.lock()) {
			if (tg->angle_LR == Left) {
				//ターゲットへの相対座標を求める
				ML::Vec2 toVec = tg->pos - this->pos;
				toVec.x -= 100;
				//ターゲットに５％近づく
				this->pos += toVec * 0.05f;
			}
		}

		if (auto tg = this->target.lock()) {
			if (tg->angle_LR == Right) {
				//ターゲットへの相対座標を求める
				ML::Vec2 toVec = tg->pos - this->pos;
				toVec.x += 100;
				//ターゲットに５％近づく
				this->pos += toVec * 0.05f;
			}
		}

			//カメラ位置の調整
		{
			//自分の位置
			int px = ge->camera2D.w / 2;
			int py = ge->camera2D.h / 2;
			//カメラの左上座標
			int cx = int(this->pos.x) - px;
			int cy = int(this->pos.y) - py;
			//カメラ更新
			ge->camera2D.x = cx;
			ge->camera2D.y = cy;

			////カメラ更新
			//ge->camera2D.x = this->pos.x;
			//ge->camera2D.y = this->pos.y;
		}

		//マップ外を写さない
		auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
		if (nullptr != map)
		{
			map->AjastCameraPos();
		}
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