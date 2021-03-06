#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Player
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName(		"プレイヤー");	//グループ名
	const  string  defName(				"仮");		//タスク名
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
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		string	stopImage;
		string	moveImage;
		string  jumpImage;
		string  verticalStopImage;
		string  verticalMoveImage;
		string  verticalJumpImage;
		string  sideStopImage;
		string  sideMoveImage;
		string  sideJumpImage;
		string  DamageImage;
		string  DeathImage;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_, ML::Vec2& position);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize(ML::Vec2& position);	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//追加したい変数・メソッドはここに追加する
		//BCharaに含まれないモノのみここに追加する
		//「変数宣言を書く」
		string	controllerName;
		//「追加メソッドを書く」
		void Think();	//思考&状況判断(状態決定)
		void Move();	//Think()で確定した状態に対応する処理
		void Rotation(); //回転
		BChara::DrawInfo Anim(); //アニメーション制御
		void Received(BChara* from_) override;//接触時の応答処理
		void CheckMove_TEST(ML::Vec2& e_);
		
	};
}