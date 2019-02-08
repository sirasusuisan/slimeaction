//-------------------------------------------------------------------
//マップ
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		this->render2D_Priority[1] = 0.9f;
		this->imageName = "MapChipImg";
		//マップのゼロクリア
		/*for (int y = 0; y < 100; ++y) {
			for (int x = 0; x < 100; ++x) {
				this->arr[y][x] = 0;
			}
		}*/
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);
		//マップチップ情報の初期化
		for (int c = 0; c < 8 * 5; ++c) {
			int x = (c % 8);
			int y = (c / 8);
			this->chip[c] = ML::Box2D(x * 32, y * 32, 32, 32);
		}
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//カメラが完全にマップ外を指しているか調べる
		if (false == this->hitBase.Hit(ge->camera2D)) {
			return; //完全に外に出ていたらその時点で描画処理を取りやめる
		}
		////カメラとマップが重なっている範囲だけの矩形を作る
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
		////２つの矩形の重なっている範囲だけの矩形を求める
		//RECT  isr;
		//isr.left = max(c.left, m.left);
		//isr.top = max(c.top, m.top);
		//isr.right = min(c.right, m.right);
		//isr.bottom = min(c.bottom, m.bottom);

		////ループ範囲を決定
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
	//マップファイル読み込み(今は仮処理）
	bool Object::Load(const  string&  fpath_)
	{
		//ファイルパスを作る
		string  filePath = fpath_;
		//ファイルを開く（読み込み）
		ifstream   fin(filePath);
		if (!fin) { return  false; }//読み込み失敗  
        //チップファイル名の読み込みと、画像のロード 
		DG::Image_Create(this->imageName, "./data/mapChip/TestMapChip01.png");
	
		//ステージ範囲を調べる------------------------------------------
		//マップ配列サイズの読み込み
		string setX, setY;//sX,sYに代入するやつ
		stringstream sX, sY;//sizeX,sizeYに代入するやつ

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
		///マップ配列データの読み込み
		///for (int y = 0; y < this->sizeY; ++y) {
		///	for (int x = 0; x < this->sizeX; ++x) {
		///		fin >> this->arr[y][x];

		//配列にデータを取り込む  //CSV対応
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
					case -2://ゴール
					{
						 //あるギミックの生成
						//Gimmic g = Gimmic::Create(x * 32,y * 32);
						auto gl = Goal::Object::Create(true, ML::Vec2(x * 32, y * 32));
						break;
					}
					case -3://トゲ
					{
						auto tg = Toge::Object::Create(true, ML::Vec2(x * 32, y * 32));
						break;
					}
					case -4://ブロック左
					{
						auto bl = Block::Object::Create(true, ML::Vec2(x * 32, y * 32),Block::Type::left);
						break;
					}
					case -5://ブロック右
					{
						auto bl = Block::Object::Create(true, ML::Vec2(x * 32, y * 32), Block::Type::right);
						break;
					}
					case -10://プレイヤー
					{
						auto pl = Player::Object::Create(true,ML::Vec2(x * 32, y * 32));
						break;
					}
					case -11://えねみー
					{
						auto e = Enemy00::Object::Create(true,ML::Vec2(x*32,y*32));
						break;
					}
					case -12://鍵
					{
						auto K = Gkey::Object::Create(true, ML::Vec2(x * 32, y * 32));
						break;
					}
					case -13://鍵扉
					{
						auto KD = GDoor::Object::Create(true, ML::Vec2(x * 32, y * 32));
						break;
					}
					case -14://矢印看板左
					{
						auto yk = Yazirusi::Object::Create(true, ML::Vec2(x * 32, y * 32),Yazirusi::Type::left);
						break;
					}
					case -15://矢印看板左上
					{
						auto yk = Yazirusi::Object::Create(true, ML::Vec2(x * 32, y * 32), Yazirusi::Type::upleft);
						break;
					}
					case -16://矢印看右
					{
						auto yk = Yazirusi::Object::Create(true, ML::Vec2(x * 32, y * 32), Yazirusi::Type::right);
						break;
					}
					case -17://矢印看板右上
					{
						auto yk = Yazirusi::Object::Create(true, ML::Vec2(x*32 , y *32), Yazirusi::Type::upright);
						break;
					}

					default:
						//通常マップの生成
						this->collision.push_back(ML::Box2D(x * 32, y * 32, 32, 32));
						break;
					}
				}
				/*else
					this->collision.push_back(ML::Box2D(0, 0, 0, 0));*/
			}
		}
		//マップ全体の矩形
		this->hitBase = ML::Box2D(0, 0, this->sizeX * 32, this->sizeY * 32);
		//this->arr[0][0] = -1;
		//if (this->arr[y][x] == -1)
		//			this->hitBase = ML::Box2D(0, 0, 0, 0);	
		
		//ファイルを閉じる
		fin.close();
		return  true;
	}
	//当たり判定
	bool Object::CheckHit(const ML::Box2D& h_)
	{
		//ここにあたり判定（hitBase）を呼び出す
		for (size_t i = 0; i < this->collision.size(); ++i)
		{
			if (this->collision[i].Hit(h_))
			{
				return true;
			}
		}
		return false;

		//RECT r = { h_.x, h_.y, h_.x + h_.w, h_.y + h_.h }; //渡されたやつの矩形生成
		//RECT m = { this->hitBase.x, this->hitBase.y, this->hitBase.x + this->hitBase.w, this->hitBase.y + this->hitBase.h }; //マップ全体矩形
		////丸め込み
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
	//マップ外を見せないようにカメラを位置調整する
	void  Object::AjastCameraPos()
	{
		//カメラとマップの範囲を用意
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

		//カメラの位置を調整
		if (c.right  > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top  < m.top) { ge->camera2D.y = m.top; }
		//マップがカメラより小さい場合
		if (this->hitBase.w < ge->camera2D.w) { ge->camera2D.x = m.left; }
		if (this->hitBase.h < ge->camera2D.h) { ge->camera2D.y = m.top; }
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