#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace Map2D
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("フィールド");	//グループ名
	const  string  defName("マップ");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		//変数宣言
		string          imageName;  //マップはリソース共有しない
		//int             arr[100][100];
		std::vector<std::vector<int>> arr;
		std::vector<ML::Box2D> collision;
		int             sizeY, sizeX;
		ML::Box2D		hitBase;   //ピクセル単位のマップサイズを持つ
		ML::Box2D		chip[19*6];
		//追加メソッド
		//マップファイル読み込み(今は仮処理）
		bool Load(const  string&  fpath_);
		//あたり判定
		bool CheckHit(const  ML::Box2D&  h_);
		//マップ外を見せないようにカメラを位置調整する
		void  AjastCameraPos();
	};
}