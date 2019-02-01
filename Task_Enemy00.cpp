//-------------------------------------------------------------------
//敵
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy00.h"

namespace  Enemy00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->EnemyImg = "EnemyImg";
		DG::Image_Create(this->EnemyImg, "./data/image/Enemy00.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->EnemyImg);
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

		this->hitBase = ML::Box2D(-32, -32, 64, 64);
		this->angle_LR = Right; //デフォルトは右向き
		this->motion = Stand;	//デフォルトは立ち状態
		this->jumPow = -6.0f;	//ジャンプ力
		this->fallSpeed = 0.0f; 
		this->maxFallSpeed = 10.0f;	//落下最大速度
		this->gravity = ML::Gravity(16) * 4; //重力
		this->dSpeed = 5.0f;	//ダッシュ時速度
		this->nSpeed = 2.0f;	//通常時速度
		this->moveVec.x = 10;    //移動量
		this->moveVec.y = 10;
		this->pos.x = position.x;
		this->pos.y = position.y;
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

		//プレイヤー当たり判定
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("プレイヤー");
			for (auto it = targets->begin(); it != targets->end(); ++it)//Enemy数分繰り返し
			{
				//相手に接触の有無を確認させる
				if ((*it)->CheckHit(me))
				{
					//相手にダメージの処理を行わせる
					(*it)->Received(this);
					break;
				}
			}
		}

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(di.ret, di.draw, di.src);
	}


	//-------------------------------------------------------------------
	//思考・状況判断 モーション決定
	void Object::Think()
	{
		//モーションの変更以外の処理はしない
		BChara::Motion nm = this->motion;	//現在の状態を指定
		switch (nm)
		{
			//立ち
		case Stand: 
			if (this->CheckFoot() == false) { nm = Fall; }
			nm = Walk;
			break;
			// 歩き
		case Walk: 
			if (this->CheckLeft() == true|| this->CheckRight() == true)
			{
				nm = Turn;
			}
			if (this->CheckFoot() == false) { nm = Fall; }
			break;
			//上昇中
		case Turn:
			if (this->moveCnt >= 5) { nm = Walk; }
			//this->Kill(); //最終手段
			break;
		case Jump:
			if (this->moveVec.y >= 0)      { nm = Fall; }
			break; 
			//落下中
		case Fall:
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
			//ダッシュ中
		case Dash: 
			break;
			//飛び立つ瞬間(ジャンプ予備動作)
		case TakeOff:	
			break;
			//着地(ジャンプ後動作)
		case Landing:	
			if (this->moveCnt >= 3) { 
				nm = Stand; }
			//if (this->CheckFoot() == false) { nm = Fall; }
			break;
		}
		this->UpdateMotion(nm);
	}
	//-----------------------------------------------------------------------------
	void Object::Move()//モーションに対応した処理
	{
		//(モーションは変更しない）
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
		switch (this->motion)//モーションごとの固有の処理
		{
		case Stand: //立ち
			break;
		case Walk: // 歩き
			if (this->angle_LR == Left)
			{
				this->moveVec.x = 3;
			}
			if (this->angle_LR == Right)
			{
				this->moveVec.x = -3;
			}
			break;
		case Jump: //上昇中
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->jumPow;//初速設定	
			}
			if (this->CheckHead() == true)
			{
				this->moveVec.y = 0;
			}
			break;
		case Fall: //落下中
		/*	this->fallSpeed = this->gravity;
			this->moveVec.y += this->fallSpeed;	*/ //なくても機能する
			break;
		case Dash: //ダッシュ中
			break;
		case TakeOff:	//飛び立つ瞬間(ジャンプ予備動作)
			break;
		case Landing:	//着地(ジャンプ後動作)
			break;
		case Turn:
			if (this->moveCnt == 3)
			{
					if (this->angle_LR == Left) { 
						this->angle_LR = Right; 
					}
					else this->angle_LR = Left;
			}
			break;
		}
	}
	//-----------------------------------------------------------------------------
	BChara::DrawInfo Object::Anim() //アニメーション制御
	{
		string ret;
		BChara::DrawInfo imageTable[] =
		{
			{ ML::Box2D(-32,-32,64,64),ML::Box2D(0,0,64,64),ML::Color(1,1,1,1),this->res->EnemyImg},
			{ ML::Box2D(-32,-32,64,64), ML::Box2D(64*1,0,64,64), ML::Color(1,1,1,1),this->res->EnemyImg},
			{ ML::Box2D(-32,-32,64,64), ML::Box2D(64*2,0,64,64), ML::Color(1,1,1,1),this->res->EnemyImg },
			{ ML::Box2D(-32,-32,64,64), ML::Box2D(64*3,0,64,64), ML::Color(1,1,1,1),this->res->EnemyImg },
		};
		BChara::DrawInfo rtv;
		int	work;
		switch (this->motion)
		{
		default: break;
		//立ち
		case Stand: 
			work = this->animCnt / 60;
			rtv = imageTable[0]; 
			break;
		// 歩き
		case Walk: 
			work = this->animCnt / 20;
			rtv = imageTable[1+work%3];
			break;
		//上昇中
		case Jump: 
			rtv = imageTable[0];
			break;	
		//落下中			
		case Fall: 
			rtv = imageTable[0];
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
		case Turn:
			rtv = imageTable[0];
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