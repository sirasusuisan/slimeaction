//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->stopImage = "stopImage";
		this->moveImage = "moveImage";
		this->jumpImage = "jumpImage";
		this->verticalStopImage = "vStopImg";
		this->verticalMoveImage = "vMoveImg";
		this->verticalJumpImage = "vJumpImg";
		this->sideStopImage = "sStopImg";
		this->sideMoveImage = "sMoveImg";
		this->sideJumpImage = "sJumpImg";
		DG::Image_Create(this->stopImage, "./data/image/静止スライム.png");
		DG::Image_Create(this->moveImage, "./data/image/移動スライム.png");
		DG::Image_Create(this->jumpImage, "./data/image/ジャンプスライム.png");
		DG::Image_Create(this->verticalStopImage, "./data/image/縦変形静止スライム.png");
		DG::Image_Create(this->verticalMoveImage, "./data/image/縦変形移動スライム.png");
		DG::Image_Create(this->verticalJumpImage, "./data/image/縦変形ジャンプスライム.png");
		DG::Image_Create(this->sideStopImage, "./data/image/横変形静止スライム.png");
		DG::Image_Create(this->sideMoveImage, "./data/image/横変形移動スライム.png");
		DG::Image_Create(this->sideJumpImage, "./data/image/横変形ジャンプスライム.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->stopImage);
		DG::Image_Erase(this->moveImage);
		DG::Image_Erase(this->jumpImage);
		DG::Image_Erase(this->verticalStopImage);
		DG::Image_Erase(this->verticalMoveImage);
		DG::Image_Erase(this->verticalJumpImage);
		DG::Image_Erase(this->sideStopImage);
		DG::Image_Erase(this->sideMoveImage);
		DG::Image_Erase(this->sideJumpImage);
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
		this->render2D_Priority[1] = 0.5f; //手前表示

		this->hitBase = ML::Box2D(-32, 0, 64, 64);
		this->angle_LR = Right; //デフォルトは右向き
		this->controllerName = "P1";
		this->motion = Stand;	//デフォルトは立ち状態
		this->form_VS = Side;	//デフォルトは横
		this->jumPow = -6.0f;	//ジャンプ力
		this->fallSpeed = 0.0f;
		this->maxFallSpeed = 10.0f;	//落下最大速度
		this->gravity = ML::Gravity(16) * 4; //重力
		this->dSpeed = 5.0f;	//ダッシュ時速度
		this->nSpeed = 2.0f;	//通常時速度
		this->moveVec.x = 0;    //移動量
		this->moveVec.y = 0;
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
		this->moveCnt++;
		this->animCnt++;
		this->Think();	//思考・状況判断
		this->Move();	//モーションに対応した処理
						//めり込まない移動処理
		ML::Vec2 est = this->moveVec;
		this->CheckMove(est);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		DG::Image_Draw(di.ret, di.draw, di.src);
	}
	//-------------------------------------------------------------------
	//思考・状況判断 モーション決定
	void Object::Think()
	{
		//モーションの変更以外の処理はしない
		auto in = DI::GPad_GetState(this->controllerName);
		//現在の状態を指定
		BChara::Motion nm = this->motion;
		switch (nm)
		{
			//立ち
		case Stand:
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B1.down) { nm = Jump; }
			if (in.B2.on) { nm = Transform_Stand; }
			break;
			// 歩き
		case Walk:
			if (in.LStick.L.off && in.LStick.R.off) { nm = Stand; }
			if (in.B1.down) { nm = Jump; }
			if (in.B2.on) { nm = Transform_Stand; }
			if (this->CheckFoot() == false) { nm = Fall; }
			break;
			//上昇中
		case Jump:
			if (this->moveVec.y >= 0) { nm = Fall; }
			break;
			//落下中
		case Fall:
			if (this->CheckFoot() == true) { nm = Landing; }
			break;
		case Dash://ダッシュ中 
			break;
		case TakeOff://飛び立つ瞬間(ジャンプ予備動作)
			break;
			//着地(ジャンプ後動作)
		case Landing:
			if (this->moveCnt >= 3) { nm = Stand; }
			//if (this->CheckFoot() == false) { nm = Fall; }
			break;
			//変形(静止)
		case Transform_Stand:
			if (in.LStick.L.on) { nm = Transform_Move; }
			if (in.LStick.R.on) { nm = Transform_Move; }
			if (in.B1.down) { nm = Transform_Jump; }
			if (in.B2.off) { nm = Stand; }
			break;
			//変形（移動）
		case Transform_Move:
			if (in.LStick.L.off && in.LStick.R.off) { nm = Transform_Stand; }
			if (in.B1.down) { nm = Transform_Jump; }
			if (in.B2.off) { nm = Stand; }
			break;
			//変形（ジャンプ）
		case Transform_Jump:
			if (this->moveVec.y >= 0) { nm = Transform_Fall; }
			break;
			//変形（落下）
		case Transform_Fall:
			if (this->CheckFoot() == true) { nm = Landing; }
			break;
		}
		this->UpdateMotion(nm);
	}
	//-----------------------------------------------------------------------------
	void Object::Move()//モーションに対応した処理
	{
		//(モーションは変更しない）
		auto in = DI::GPad_GetState(this->controllerName);
		//重力の処理
		switch (this->motion)
		{
		default:
			if (this->moveVec.y < 0 || this->CheckFoot() == false) //上昇中or足元に地面がない場合
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->maxFallSpeed);
			else //足元に地面がある場合
				this->moveVec.y = 0;
			//this->fallSpeed = 0;
			break;
			//重力加速を無効化するモーションは下に書く
		case Unnon: break;
		}
		//移動速度減衰
		switch (this->motion)
		{
		default:
			if (this->moveVec.x < 0)
				this->moveVec.x = min(this->moveVec.x + this->dSpeed, 0);
			else
				this->moveVec.x = max(this->moveVec.x - this->dSpeed, 0);
			break;
			//移動速度減衰を無効化するモーションは下に書く
		case Unnon: break;
		}
		//あたり判定
		switch (this->motion)
		{
			//基本時
		default:
			this->hitBase = ML::Box2D(-32, 0, 64, 64);
			break;
			//変形時(静止)
		case Transform_Stand:
		case Transform_Move:
		case Transform_Jump:
		case Transform_Fall:
			//縦
			if (form_VS == Vertical)
			{
				this->hitBase = ML::Box2D(-16, 0, 32, 64);
			}
			//横
			if (form_VS == Side)
			{
				this->hitBase = ML::Box2D(-32, 32, 64, 32);
			}
			break;
		}
		//モーションごとの固有の処理
		switch (this->motion)
		{
		case Stand: //立ち
			break;
			// 歩き
		case Walk:
		case Transform_Move:
			if (in.LStick.L.on)
			{
				this->angle_LR = Left;
				this->moveVec.x = -this->nSpeed;
				//this->CheckMove(moveVec);				
			}
			if (in.LStick.R.on)
			{
				this->angle_LR = Right;
				this->moveVec.x = this->nSpeed;
				//this->CheckMove(moveVec);		
			}
			break;
			//上昇中
		case Jump:
		case Transform_Jump:
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->jumPow;//初速設定	
			}
			if (this->CheckHead() == true)
			{
				this->moveVec.y = 0;
			}
			if (in.LStick.L.on)
			{
				this->angle_LR = Left;
				this->moveVec.x = -this->nSpeed;
				//this->CheckMove(moveVec);
			}
			if (in.LStick.R.on)
			{
				this->angle_LR = Right;
				this->moveVec.x = this->nSpeed;
				//this->CheckMove(moveVec);
			}
			break;
			//落下中
		case Fall:
		case Transform_Fall:
			/*	this->fallSpeed = this->gravity;
			this->moveVec.y += this->fallSpeed;	*/ //なくても機能する
			if (in.LStick.L.on)
			{
				this->angle_LR = Left;
				this->moveVec.x = -this->nSpeed;
				//this->CheckMove(moveVec);
			}
			if (in.LStick.R.on)
			{
				this->angle_LR = Right;
				this->moveVec.x = this->nSpeed;
				//this->CheckMove(moveVec);
			}
			break;
		case Dash: //ダッシュ中
			break;
		case TakeOff:	//飛び立つ瞬間(ジャンプ予備動作)
			break;
		case Landing:	//着地(ジャンプ後動作)
			break;
			//変形(静止)
		case Transform_Stand:
			if (in.LStick.U.on)
			{
				this->form_VS = Vertical;
			}
			if (in.LStick.D.on)
			{
				this->form_VS = Side;
			}
			break;
		}
	}
	//-----------------------------------------------------------------------------
	BChara::DrawInfo Object::Anim() //アニメーション制御
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//string ret;
		//描画設定（サイズは当たり判定と連動）
		BChara::DrawInfo imageTable[] =
		{
			//	draw		src						color
			/*0*/{ this->hitBase, ML::Box2D(0,0,64,64), ML::Color(1,1,1,1), this->res->stopImage },	//停止1
			{ this->hitBase, ML::Box2D(64,0,64,64), ML::Color(1,1,1,1), this->res->stopImage }, //停止2
			{ this->hitBase, ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //移動1
			{ this->hitBase, ML::Box2D(64, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //移動2
			{ this->hitBase, ML::Box2D(128, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //移動3
																									//{ this->hitBase, ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ前1
																									//{ this->hitBase, ML::Box2D(64 , 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ前2
																									/*5*/{ this->hitBase, ML::Box2D(64 * 2, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ前
																									{ this->hitBase, ML::Box2D(64 * 3, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//上昇
																									{ this->hitBase, ML::Box2D(64 * 4, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//下降
																									{ this->hitBase, ML::Box2D(64 * 5, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ後1
																									{ this->hitBase, ML::Box2D(64 * 6, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ後2
																									/*10*/{ this->hitBase, ML::Box2D(0, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalStopImage },//縦変形静止1
																									{ this->hitBase, ML::Box2D(32, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalStopImage },//縦変形静止2
																									{ this->hitBase, ML::Box2D(0, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//縦変形移動1
																									{ this->hitBase, ML::Box2D(32, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//縦変形移動2
																									{ this->hitBase, ML::Box2D(64, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//縦変形移動3
																									/*15*/{ this->hitBase, ML::Box2D(32 * 2, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形ジャンプ前
																									{ this->hitBase, ML::Box2D(32 * 3, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形上昇
																									{ this->hitBase, ML::Box2D(32 * 4, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形下降
																									{ this->hitBase, ML::Box2D(32 * 5, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形ジャンプ後1
																									{ this->hitBase, ML::Box2D(32 * 6, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形ジャンプ後2
																									/*20*/{ this->hitBase, ML::Box2D(0, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideStopImage },//横変形静止1
																									{ this->hitBase, ML::Box2D(64, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideStopImage },//横変形静止2
																									{ this->hitBase, ML::Box2D(0, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//横変形移動1
																									{ this->hitBase, ML::Box2D(64, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//横変形移動2
																									{ this->hitBase, ML::Box2D(64 * 2, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//横変形移動3
																									/*25*/{ this->hitBase, ML::Box2D(64 * 2 , 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形ジャンプ前
																									{ this->hitBase, ML::Box2D(64 * 3, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形上昇
																									{ this->hitBase, ML::Box2D(64 * 4, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形下降
																									{ this->hitBase, ML::Box2D(64 * 5, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形ジャンプ後																							{ this->hitBase, ML::Box2D(64 * 6, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形ジャンプ後2
		};
		BChara::DrawInfo rtv;
		int	work;
		//アニメーションを各モーションに対応
		switch (this->motion)
		{
		default: break;
			//立ち
		case Stand:
			work = this->animCnt / 40;
			rtv = imageTable[work % 2];
			break;
			// 歩き
		case Walk:
			work = this->animCnt / 20;
			rtv = imageTable[(work % 3) + 2];
			break;
			//上昇中
		case Jump:
			work = this->animCnt / 30;
			rtv = imageTable[(work % 2) + 5];
			break;
			//落下中			
		case Fall:
			work = this->animCnt / 30;
			rtv = imageTable[(work % 3) + 7];
			break;
			//ダッシュ中
		case Dash:
			break;
			//飛び立つ瞬間(ジャンプ予備動作)
		case TakeOff:
			break;
			//着地(ジャンプ後動作)		
		case Landing:
			rtv = imageTable[0];
			break;
			//変形(静止)
		case Transform_Stand:
			//縦
			if (this->form_VS == Vertical)
			{
				work = this->animCnt / 40;
				rtv = imageTable[(work % 2) + 10];
			}
			//横
			if (this->form_VS == Side)
			{
				work = this->animCnt / 40;
				rtv = imageTable[(work % 2) + 20];
			}
			break;
			//変形(移動)
		case Transform_Move:
			//縦
			if (this->form_VS == Vertical)
			{
				work = this->animCnt / 20;
				rtv = imageTable[(work % 3) + 12];
			}
			//横
			if (this->form_VS == Side)
			{
				work = this->animCnt / 20;
				rtv = imageTable[(work % 3) + 22];
			}
			break;
			//変形(ジャンプ)
		case Transform_Jump:
			//縦
			if (this->form_VS == Vertical)
			{
				work = this->animCnt / 30;
				rtv = imageTable[(work % 2) + 15];
			}
			//横
			else if (this->form_VS == Side)
			{
				work = this->animCnt / 30;
				rtv = imageTable[(work % 2) + 25];
			}
			else
			{
				throw std::runtime_error("Error:Player::form_VS");
			}
			break;
			//変形（落下）
		case Transform_Fall:
			//縦
			if (this->form_VS == Vertical)
			{
				work = this->animCnt / 20;
				rtv = imageTable[(work % 3) + 17];
			}
			//横
			if (this->form_VS == Side)
			{
				work = this->animCnt / 20;
				rtv = imageTable[(work % 3) + 27];
			}
			break;
		}
		//画像の左右反転処理
		if (false == this->angle_LR)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	void Object::Received(BChara* from_) //アイテムや敵との接触時の処理(受け身)
	{
		if (this->playerhp <= 0) { this->Kill(); }
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