#pragma warning(disable:4996)
#pragma once
//
//キャラクタ汎用スーパークラス
//
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ変数
	ML::Vec2	pos;//キャラクタ位置
	ML::Box2D	hitBase;//当たり判定範囲
	ML::Box2D   lvHitBase;//壁張り付き用
	ML::Box2D   rvHitBase;
	//中心点
	ML::Box2D   footCenter;
	ML::Box2D   leftCenter;
	ML::Box2D   rightCenter;
	ML::Box2D   headCenter;
	ML::Vec2	moveVec;//移動ベクトル
	int			moveCnt;//行動カウンタ
    //左右の向き
	enum Angle_LR { Left, Right };
	Angle_LR   angle_LR;
	WP target;
	//変形の方向
	enum Form_VS { Vertical, Side };
	Form_VS form_VS;
	Form_VS preForm_VS;
	//進んでいる方向
	enum Direction_Move { floor, lWall, rWall, ceiling };
	Direction_Move dMove;
	//Form_VS preForm_VS;
	//キャラクターの状態
	enum Motion
	{
		Unnon = -1,	//無効
		Stand,		//停止
		Walk,		//歩行
		Jump,		//ジャンプ
		Fall,		//落下
		Dash,		//ダッシュ
		//変形
		Transform_Stand, 
		Transform_Move,
		Transform_Jump,
		Transform_Fall,
		//張り付き
		Stick_Stand,
		Stick_Move,
		TakeOff,	//飛び立つ瞬間(ジャンプ予備動作)
		Landing,	//着地(ジャンプ後動作)
		Bound,      //やられ時
		TransBound,
		Death,      //死亡時	

		//敵
		Turn,
	};
	Motion		motion;
	Motion		preMotion;
	int			animCnt = 0;//アニメーションカウンタ
	float		jumPow;	//ジャンプ初速
	float       fallSpeed;//落下速度
	float		maxFallSpeed;//落下最大速度
	float		gravity;//重力 フレーム単位の加算量
	float		dSpeed;//ダッシュ時速度
	float		nSpeed;//通常時速度
	int			playerhp;//プレイヤーHP
	int			enemyhp;//敵HP
	//int		shothp;//攻撃オブジェクトHP (弾とかのやつ)
	int			unHitTime;//プレイヤー無敵時間[

	//メンバ変数の初期化
	//変数追加したら初期化してね
	BChara()
		: pos(0, 0)//キャラクタ位置
		, hitBase(0, 0, 0, 0)//当たり判定範囲
		, moveVec(0, 0)//移動ベクトル
		, moveCnt(0)//行動カウンタ
		, angle_LR(Right)//左右の向き 初期値は右
		, motion(Stand) //キャラクターの状態 初期値は通常
		, jumPow(0)//ジャンプ初速
		, fallSpeed(0)//落下速度
		, maxFallSpeed(0)//落下最大速度
		, gravity(0)//重力
		, dSpeed(0)//ダッシュ時速度
		, nSpeed(0)//通常時速度
		, playerhp(3)//プレイヤーHP
		, enemyhp(1)//エネミーHP
		, unHitTime(0)//無敵時間
	{
	}
	virtual ~BChara() {}

	//キャラクタ共通メソッド
	virtual void CheckMove(ML::Vec2& est_);//めり込まない移動処理
	virtual bool CheckHead();//頭上接触判定
	virtual bool CheckFoot();//足元接触判定
	virtual bool CheckLeft();//左接触判定
	virtual bool CheckRight();//右接触判定
	bool UpdateMotion(Motion nm_);//モーション変更 変更しない場合false
	struct DrawInfo
	{
		ML::Box2D draw, src;
		ML::Color color;
		string		ret;
	};
	virtual void Received(BChara* from_);//接触所の応答処理
	virtual bool CheckHit(const ML::Box2D& hit_);//接触判定
	virtual void Damage();//プレイヤーのダメージ関数
	bool HaveKey;//カギの有り無し判定
	bool Playerliveflag;//プレイヤーの生存フラグ
};
