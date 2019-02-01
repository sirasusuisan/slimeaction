//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include   "Task_Map2D.h"
#include  "Task_Block.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->stopImage = "stopImage";//静止
		this->moveImage = "moveImage";//移動
		this->jumpImage = "jumpImage";//ジャンプ
		this->verticalStopImage = "vStopImg";//縦変形静止
		this->verticalMoveImage = "vMoveImg";//縦変形移動
		this->verticalJumpImage = "vJumpImg";//縦変形ジャンプ
		this->sideStopImage = "sStopImg";//横変形静止
		this->sideMoveImage = "sMoveImg";//横変形移動
		this->sideJumpImage = "sJumpImg";//横変形ジャンプ
		this->DamageImage = "damageImage";//ダメージ
		this->DeathImage = "deathImage";//死亡
		DG::Image_Create(this->stopImage, "./data/image/静止スライム.png");
		DG::Image_Create(this->moveImage, "./data/image/移動スライム.png");
		DG::Image_Create(this->jumpImage, "./data/image/ジャンプスライム.png");
		DG::Image_Create(this->verticalStopImage, "./data/image/縦変形静止スライム.png");
		DG::Image_Create(this->verticalMoveImage, "./data/image/縦変形移動スライム.png");
		DG::Image_Create(this->verticalJumpImage, "./data/image/縦変形ジャンプスライム.png");
		DG::Image_Create(this->sideStopImage, "./data/image/横変形静止スライム.png");
		DG::Image_Create(this->sideMoveImage, "./data/image/横変形移動スライム.png");
		DG::Image_Create(this->sideJumpImage, "./data/image/横変形ジャンプスライム.png");
		DG::Image_Create(this->DamageImage, "./data/image/やられスライム.png");
		DG::Image_Create(this->DeathImage, "./data/image/死亡スライム.png");
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
		DG::Image_Erase(this->DamageImage);
		DG::Image_Erase(this->DeathImage);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(ML::Vec2& position)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->render2D_Priority[1] = 0.5f; //手前表示
		this->angle_LR = Right; //デフォルトは右向き
		this->form_VS = Side;//デフォルトは横
		this->lvHitBase = ML::Box2D(0, 0, 0, 0);//左
		this->rvHitBase = ML::Box2D(0, 0, 0, 0);//右
		this->footCenter = ML::Box2D(0, 0, 0, 0);//足元真ん中
		this->leftCenter = ML::Box2D(0, 0, 0, 0);//左の真ん中
		this->rightCenter = ML::Box2D(0, 0, 0, 0);//右の真ん中
		this->headCenter = ML::Box2D(0, 0, 0, 0);//頭の真ん中
		this->dMove = floor;//進んでる方向
		this->controllerName = "P1";
		this->motion = Stand;	//デフォルトは立ち状態
		this->jumPow = -6.0f;	//ジャンプ力
		this->fallSpeed = 0.0f;//落下速度
		this->maxFallSpeed = 10.0f;	//落下最大速度
		this->gravity = ML::Gravity(16) * 4; //重力
		this->dSpeed = 5.0f;	//ダッシュ時速度
		this->nSpeed = 2.0f;	//通常時速度
		this->moveVec.x = 0;    //移動量
		this->moveVec.y = 0;
		this->pos.x = position.x;//座標
		this->pos.y = position.y;
		this->playerhp = 3;//hp
		this->unHitTime = 0;//無敵時間用カウンタ
		this->moveCnt = 0;//行動カウンタ
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
		this->unHitTime--;
		this->moveCnt++;
		this->animCnt++;
		this->Think();	//思考・状況判断
		this->Move();	//モーションに対応した処理
		//めり込まない移動処理
		ML::Vec2 est = this->moveVec;
		this->CheckMove_TEST(est);//複数のブロックのあたり判定などに対応する関数
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//点滅
		if (this->playerhp != 0)
		{
			if (unHitTime > 0)
			{
				if ((this->unHitTime / 4) % 2 == 0)
				{
					return;
				}
			}
		}
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		Rotation();
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(di.ret, di.draw, di.src);
	}
	void Object::CheckMove_TEST(ML::Vec2& e_)//複数のブロックのあたり判定などに対応する関数
	{
		auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ"); //マップが存在するか調べてからアクセス
		auto block = ge->GetTask_Group_G<Block::Object>("ブロック");
		//if (nullptr == map) { return; } //マップがなければ判定しない

		//マップが存在し、キャラクタが静止状態じゃなければ[横移動]
		while (e_.x != 0)
		{
			float preX = this->pos.x;	//移動前のキャラクタ位置
			if (e_.x >= 1) { this->pos.x += 1; e_.x -= 1; } //右
			else if (e_.x <= -1) { this->pos.x -= 1; e_.x += 1; }//左
			else { this->pos.x += e_.x; e_.x = 0; }
			//引数位置オフセット
			ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
			if (true == map->CheckHit(hit))
			{
				this->pos.x = preX;	//移動キャンセル
				break;
			}
			for (int i = 0; i < block->size(); ++i)
			{
				if (true == (*block)[i]->CheckHit(hit))
				{
					this->pos.x = preX;
					break;
				}
			}
		}
		//縦軸に対する移動
		while (e_.y != 0) 
		{
			float  preY = this->pos.y;//移動前のキャラクタ位置
			if (e_.y >= 1) { this->pos.y += 1; e_.y -= 1; }
			else if (e_.y <= -1) { this->pos.y -= 1; e_.y += 1; }
			else { this->pos.y += e_.y;		e_.y = 0; }
			ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
			if (true == map->CheckHit(hit))
			{
				this->pos.y = preY;		//移動をキャンセル
				break;
			}
			for (int i = 0; i < block->size(); ++i)
			{
				if (true == (*block)[i]->CheckHit(hit))
				{
					this->pos.y = preY;
					break;
				}
			}
		}
	}
	//----------------------------------------------------------------------------
	//回転量
	void Object::Rotation()
	{
		BChara::DrawInfo di = this->Anim();
		switch (this->dMove)
		{
		default:
			DG::Image_Rotation(di.ret, 0.0f, ML::Vec2(di.draw.w, di.draw.h));
			break;
		case lWall:
			DG::Image_Rotation(di.ret, -300.0f/*90度？*/, ML::Vec2(di.draw.w / 2, di.draw.h / 2));
			break;
		case rWall:
			DG::Image_Rotation(di.ret, 300.0f/*90度？*/, ML::Vec2(di.draw.w / 2, di.draw.h / 2));
			break;
		case ceiling:
			DG::Image_Rotation(di.ret, 600.0f/*90度？*/, ML::Vec2(di.draw.w / 2, di.draw.h / 2));
			break;
		}
		if (!(this->motion == Stick_Stand || this->motion == Stick_Move))
		{
			DG::Image_Rotation(di.ret, 0.0f, ML::Vec2(di.draw.w, di.draw.h));
		}
	}
	//-------------------------------------------------------------------
	//思考・状況判断 モーション決定
	void Object::Think()
	{
		//モーションの変更以外の処理はしない
		auto in = DI::GPad_GetState(this->controllerName);
		//現在の状態を指定
		BChara::Motion nm = this->motion;	
		this->preMotion = nm;
		this->preForm_VS = this->form_VS;
		switch (nm)
		{
		//立ち
		case Stand: 
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B1.down)     { nm = Jump; }
			if (in.B2.down)       { nm = Transform_Stand; }
			if (this->CheckFoot() == false)         { nm = Fall; }
			if (in.B3.on) { nm = Stick_Stand; }
			break;
		// 歩き
		case Walk: 
			if (in.LStick.L.off && in.LStick.R.off) { nm = Stand; }
			if (in.B1.down)                         { nm = Jump; }
			if (in.B2.down)                           { nm = Transform_Stand; }
			if (this->CheckFoot() == false)         { nm = Fall; }
			if (in.B3.on) { nm = Stick_Stand; }
			break;
		//上昇中
		case Jump:
			if (this->moveVec.y >= 0)      { nm = Fall; }
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
		//壁張り付き--------------------------------------------------------
		case Stick_Stand:
			if (in.B3.off)
			{
				this->dMove = floor;
				nm = Stand;
				this->gravity = ML::Gravity(16) * 4;
			}
			if (in.B3.off && CheckFoot() == false)
			{
				nm = Fall;
				this->gravity = ML::Gravity(16) * 4;
			}
			if (in.LStick.L.on) { nm = Stick_Move; }
			if (in.LStick.R.on) { nm = Stick_Move; }
			break;
		//移動
		case Stick_Move:
			if (in.LStick.L.off && in.LStick.R.off) { nm = Stick_Stand; }
			if (in.B3.off)
			{
				this->dMove = floor;
				nm = Stand;
				this->gravity = ML::Gravity(16) * 4;
			}
			break;
        //変形------------------------------------------------------------
		//（静止）
		case Transform_Stand:
			if (in.LStick.L.on) { nm = Transform_Move; }
			if (in.LStick.R.on) { nm = Transform_Move; }
			if (in.B1.down)     { nm = Transform_Jump; }
			if (in.B2.down)      { nm = Stand; }
			if (this->CheckFoot() == false) { nm = Transform_Fall; }
			break;
		//（移動）
		case Transform_Move:
			if (in.LStick.L.off && in.LStick.R.off) { nm = Transform_Stand; }
			if (in.B1.down) { nm = Transform_Jump; }
			if (in.B2.down) { nm = Stand; }
			if (this->CheckFoot() == false) { nm = Transform_Fall; }
			break;
		//（ジャンプ）
		case Transform_Jump:
			if (this->moveVec.y >= 0) { nm = Transform_Fall; }
			break;
		//（落下）
		case Transform_Fall:
			if (this->CheckFoot() == true) { nm = Transform_Stand; }
			break;
			//やられ時
		case Bound:
			this->moveCnt++;
			if (this->moveCnt >= 60 && this->CheckFoot()==true)
			{			 		
				nm = Stand;
				this->moveCnt = 0;
			}
			break;
			//死亡時
		case Death:break;
		}
		this->UpdateMotion(nm);
	}
	//-----------------------------------------------------------------------------
	//モーションに対応した処理
	void Object::Move()
	{
		//(モーションは変更しない）
		auto in = DI::GPad_GetState(this->controllerName);
		//マップタスク呼び出し
		auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
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
		case Stick_Stand:
		case Stick_Move:
			this->moveVec.y = 0;
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
		case Bound:
		case Unnon: break;
		}
		//あたり判定
		if (this->preMotion != this->motion)
		{	
			switch (this->motion)
			{
				//基本時
			default:
				//マップと変更後のあたり判定＋現在位置のBox2Dの判定を行う
				if (!map->CheckHit(ML::Box2D(-32, 0, 64, 64).OffsetCopy(this->pos)))
				{
					this->hitBase = ML::Box2D(-32, 0, 64, 64);
				}
				//当たってるなら変形しないからmotionを戻す
				else { this->motion = this->preMotion; }
				break;
				//変形時(静止)
			case Transform_Stand:
			case Transform_Move:
			case Transform_Jump:
			case Transform_Fall:
				//縦
				if (form_VS == Vertical)
				{
					//マップと変更後のあたり判定＋現在位置のBox2Dの判定を行う
					if (!map->CheckHit(ML::Box2D(-16, 0, 32, 64).OffsetCopy(this->pos)))
					{
						this->hitBase = ML::Box2D(-16, 0, 32, 64);
					}
					//当たってるなら変形しないからmotionを戻す
					else { this->motion = this->preMotion; }
				}
				//横
				if (form_VS == Side)
				{
					//マップと変更後のあたり判定＋現在位置のBox2Dの判定を行う
					if (!map->CheckHit(ML::Box2D(-32, 32, 64, 32).OffsetCopy(this->pos)))
					{
						this->hitBase = ML::Box2D(-32, 32, 64, 32);
					}//当たってるなら変形しないからmotionを戻す
					else { this->motion = this->preMotion; }
				}
				break;
			}
			this->lvHitBase   = ML::Box2D(this->hitBase.x * 2,                     this->hitBase.y ,                   this->hitBase.w, this->hitBase.h);
			this->rvHitBase   = ML::Box2D(this->hitBase.w ,                        this->hitBase.y ,                   this->hitBase.w, this->hitBase.h);
			this->footCenter  = ML::Box2D((this->hitBase.x + this->hitBase.w / 2), this->hitBase.y + this->hitBase.h,      1, 1);
			this->leftCenter  = ML::Box2D(this->hitBase.x - 1,                    (this->hitBase.y + this->hitBase.h / 2), 1, 1);
			this->rightCenter = ML::Box2D(this->hitBase.x + this->hitBase.w,      (this->hitBase.y + this->hitBase.h / 2), 1, 1);
			this->headCenter  = ML::Box2D((this->hitBase.x + this->hitBase.w / 2), this->hitBase.y - 1,                    1, 1);
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
		//張り付き
		case Stick_Stand:
			break;
		case Stick_Move:
			//床接触時-----------------------------------------------------------------
			if (this->CheckFoot() == true)
			{
				this->dMove = floor;
				if (in.LStick.L.on)
				{
					this->angle_LR = Left;
					this->moveVec.x = -this->nSpeed;
				}
				if (in.LStick.R.on)
				{
					this->angle_LR = Right;
					this->moveVec.x = this->nSpeed;
				}
				//壁移動へ切り替え
				if (!map->CheckHit(footCenter.OffsetCopy(this->pos)))
				{
					this->moveVec.y = this->nSpeed;
					//this->moveVec.x = 0;
				}
			}

			//プレイヤの左側が触れたら---------------------------------------------------
			if (this->CheckLeft() == true)
			{
				this->dMove = lWall;
				if (in.LStick.L.on)
				{
					this->angle_LR = Left;
					this->moveVec.y = -this->nSpeed;
				}
				if (in.LStick.R.on)
				{
					this->angle_LR = Right;
					this->moveVec.y = this->nSpeed;
				}
				//床移動へ切り替え		
				if (!map->CheckHit(leftCenter.OffsetCopy(this->pos)) && 
					!map->CheckHit(lvHitBase.OffsetCopy(this->pos)))
				{
					//this->dMove = floor;				
					this->moveVec.x = -this->nSpeed;
					//this->moveVec.y = 0;			
				}
			}

			//プレイヤの右側が触れたら---------------------------------------------------
			if (this->CheckRight() == true)
			{
				this->dMove = rWall;
				if (in.LStick.L.on)
				{
					this->angle_LR = Left;
					this->moveVec.y = this->nSpeed;
				}
				if (in.LStick.R.on)
				{
					this->angle_LR = Right;
					this->moveVec.y = -this->nSpeed;
				}
				//床移動へ切り替え	
				if (!map->CheckHit(rightCenter.OffsetCopy(this->pos))&&
					!map->CheckHit(rvHitBase.OffsetCopy(this->pos)))
				{
					//this->dMove = floor;		
					this->moveVec.x = this->nSpeed;
					//this->moveVec.y = 0;
				}
			}

			//プレイヤの頭上が触れたら---------------------------------------------------
			//if (this->CheckHead() == true)
			//{
			//	this->dMove = ceiling;
			//	if (in.LStick.L.on)
			//	{
			//		this->angle_LR = Left;
			//		this->moveVec.x = -this->nSpeed;
			//	}
			//	if (in.LStick.R.on)
			//	{
			//		this->angle_LR = Right;
			//		this->moveVec.x = this->nSpeed;
			//	}	
			//	//壁移動へ切り替え
			//	if (!map->CheckHit(footCenter.OffsetCopy(this->pos)))		
			//	{				
			//		this->moveVec.y = this->nSpeed;
			//	}	
			//}
			break;
		//変形(静止)
		case Transform_Stand:
			if (in.LStick.U.on)
			{	
				//マップと変更後のあたり判定＋現在位置のBox2Dの判定を行う
				if (!map->CheckHit(ML::Box2D(-16, 0, 32, 64).OffsetCopy(this->pos)))
				{
					this->hitBase = ML::Box2D(-16, 0, 32, 64);
					this->form_VS = Vertical;
				}
				else
				{
					//当たってるなら変形しないからmotionを戻す
					//this->form_VS = this->preForm_VS;
				}	
			}
			if (in.LStick.D.on)
			{
				//マップと変更後のあたり判定＋現在位置のBox2Dの判定を行う
				if (!map->CheckHit(ML::Box2D(-32, 32, 64, 32).OffsetCopy(this->pos)))
				{
					this->hitBase = ML::Box2D(-32, 32, 64, 32);
					this->form_VS = Side;
				}
				else
				{
					//当たってるなら変形しないからmotionを戻す
					//this->form_VS = this->preForm_VS;
				}
			}
			break;	
		//やられ時
		case Bound:
			break;
		//死亡時
		case Death:
			if(this->animCnt>=120)
			this->Kill();
			break;
		}
	}
	//-----------------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo Object::Anim() 
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//string ret;
		//描画設定（サイズは当たり判定と連動）
		BChara::DrawInfo imageTable[] =
		{
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(0,0,64,64), ML::Color(1,1,1,1), this->res->stopImage },	//停止1
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(64,0,64,64), ML::Color(1,1,1,1), this->res->stopImage }, //停止2
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //移動1
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(64, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //移動2
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(128, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //移動3
																												//      { this->drawBase, ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ前1
																												//		{ this->drawBase, ML::Box2D(64 , 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ前2

		/*5*/{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(64 * 2, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ前
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(64 * 3, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//上昇
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(64 * 4, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//下降
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(64 * 5, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ後1
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(64 * 6, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ後2

			/*10*/{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(0, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalStopImage },//縦変形静止1
			{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(32, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalStopImage },//縦変形静止2

			{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(0, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//縦変形移動1
			{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(32, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//縦変形移動2
			{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(64, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//縦変形移動3

		/*15*/{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(32 * 2, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形ジャンプ前
		{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(32 * 3, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形上昇
			{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(32 * 4, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形下降
			{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(32 * 5, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形ジャンプ後1
			{ ML::Box2D(-16, 0, 32, 64), ML::Box2D(32 * 6, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形ジャンプ後2
		/*20*/{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(0, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideStopImage },//横変形静止1
			{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(64, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideStopImage },//横変形静止2

			{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(0, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//横変形移動1
			{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(64, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//横変形移動2
			{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(64 * 2, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//横変形移動3

		/*25*/{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(64 * 2 , 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形ジャンプ前
			{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(64 * 3, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形上昇
			{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(64 * 4, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形下降
			{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(64 * 5, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形ジャンプ後1
			{ ML::Box2D(-32, 32, 64, 32), ML::Box2D(64 * 6, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形ジャンプ後2

		/*30*/{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(0 , 0, 64, 64), ML::Color(1,1,1,1), this->res->DamageImage },//ダメージ受け時
				{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->DeathImage },//死亡時
			{ ML::Box2D(-32, 0, 64, 64), ML::Box2D(64, 0, 64, 64), ML::Color(1,1,1,1), this->res->DeathImage },//死亡時2
	//		//	draw		src						color
	///*0*/   { this->hitBase, ML::Box2D(0,0,64,64), ML::Color(1,1,1,1), this->res->stopImage },	//停止1
	//		{ this->hitBase, ML::Box2D(64,0,64,64), ML::Color(1,1,1,1), this->res->stopImage }, //停止2

	//		{ this->hitBase, ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //移動1
	//		{ this->hitBase, ML::Box2D(64, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //移動2
	//		{ this->hitBase, ML::Box2D(128, 0, 64, 64), ML::Color(1,1,1,1), this->res->moveImage }, //移動3

	////      { this->hitBase, ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ前1
	////		{ this->hitBase, ML::Box2D(64 , 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ前2
 //   /*5*/   { this->hitBase, ML::Box2D(64 * 2, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ前
	//		{ this->hitBase, ML::Box2D(64 * 3, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//上昇
	//		{ this->hitBase, ML::Box2D(64 * 4, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//下降
	//		{ this->hitBase, ML::Box2D(64 * 5, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ後1
 //           { this->hitBase, ML::Box2D(64 * 6, 0, 64, 64), ML::Color(1,1,1,1), this->res->jumpImage },//ジャンプ後2

 //   /*10*/  { this->hitBase, ML::Box2D(0, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalStopImage },//縦変形静止1
	//	    { this->hitBase, ML::Box2D(32, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalStopImage },//縦変形静止2

	//	    { this->hitBase, ML::Box2D(0, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//縦変形移動1
	//    	{ this->hitBase, ML::Box2D(32, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//縦変形移動2
	//    	{ this->hitBase, ML::Box2D(64, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalMoveImage },//縦変形移動3

 //   /*15*/ 	{ this->hitBase, ML::Box2D(32 * 2, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形ジャンプ前
	//    	{ this->hitBase, ML::Box2D(32 * 3, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形上昇
	//    	{ this->hitBase, ML::Box2D(32 * 4, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形下降
	//    	{ this->hitBase, ML::Box2D(32 * 5, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形ジャンプ後1
	//    	{ this->hitBase, ML::Box2D(32 * 6, 0, 32, 64), ML::Color(1,1,1,1), this->res->verticalJumpImage },//縦変形ジャンプ後2

	///*20*/  { this->hitBase, ML::Box2D(0, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideStopImage },//横変形静止1
	//	    { this->hitBase, ML::Box2D(64, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideStopImage },//横変形静止2

	//	    { this->hitBase, ML::Box2D(0, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//横変形移動1
	//	    { this->hitBase, ML::Box2D(64, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//横変形移動2
	//    	{ this->hitBase, ML::Box2D(64 * 2, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideMoveImage },//横変形移動3

	///*25*/	{ this->hitBase, ML::Box2D(64 * 2 , 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形ジャンプ前
	//	    { this->hitBase, ML::Box2D(64 * 3, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形上昇
	//	    { this->hitBase, ML::Box2D(64 * 4, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形下降
	//	    { this->hitBase, ML::Box2D(64 * 5, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形ジャンプ後1
	//	    { this->hitBase, ML::Box2D(64 * 6, 0, 64, 32), ML::Color(1,1,1,1), this->res->sideJumpImage },//横変形ジャンプ後2
	///*30*/	{ this->hitBase, ML::Box2D(0 , 0, 64, 64), ML::Color(1,1,1,1), this->res->DamageImage },//ダメージ受け時
	//		{ this->hitBase, ML::Box2D(0, 0, 64, 64), ML::Color(1,1,1,1), this->res->DeathImage },//死亡時
	//	    { this->hitBase, ML::Box2D(64, 0, 64, 64), ML::Color(1,1,1,1), this->res->DeathImage },//死亡時2
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
			rtv = imageTable[work%2]; 
			break;
		// 歩き
		case Walk: 
			work = this->animCnt / 20;
			rtv = imageTable[(work % 3)+2];
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
		//張り付き
		case Stick_Stand:
			work = this->animCnt / 40;
			rtv = imageTable[work % 2];
			break;
		case Stick_Move:
			work = this->animCnt / 40;
			rtv = imageTable[(work % 3) + 2];
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
			if (this->form_VS == Side)
			{
				work = this->animCnt / 30;
				rtv = imageTable[(work % 2) + 25];
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
		//やられ
		case Bound:			
			rtv = imageTable[30];
			break;
		//死亡
		case Death:
			work = this->animCnt / 60;
			rtv = imageTable[(work % 2) + 31];
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
	//アイテムや敵との接触時の処理(受け身)
	void Object::Received(BChara* from_) 
	{		
		/*if (this->motion == Transform_Stand ||
			this->motion == Transform_Move ||
			this->motion == Transform_Jump ||
			this->motion == Transform_Fall)
		{
			this->
		}*/
		//無敵時間中はダメージ受けない
		if (this->unHitTime > 0) {
			return;
		}
		this->unHitTime = 120;
		Damage();

		//吹き飛ばし
		if (this->pos.x < from_->pos.x) { this->moveVec = ML::Vec2(-4, -5); }
		else this->moveVec = ML::Vec2(+4, -5); 
		this->UpdateMotion(Bound);
		//(*ge->GetTask_Group_G<BChara>("UI")->begin())->Received(this);
		if (this->playerhp <= 0) 
		{
			//string e = "death";
			//auto E = Effects::Object::Create(true);
			//E->ReceiveCode = e;
			this->motion = Death;
		}	
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, ML::Vec2& position)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->Initialize(position)) {
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