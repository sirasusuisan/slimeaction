//
//キャラクタ汎用スーパークラス
//
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map2D.h" 
#include  "Task_Player.h"
#include  "Task_Block.h"
//モーション更新 変更しない場合false
bool BChara::UpdateMotion(Motion nm_)
{
	if (nm_ == this->motion)//前後のモーションが同じなら
	{
		return false;
	}
	else//前後のモーションが違うなら
	{
		this->motion = nm_;	//モーション変更
		this->moveCnt = 0;	//行動カウンタリセット
		this->animCnt = 0;	//アニメーションカウンタリセット
		return true;
	}
}
//頭上接触判定
bool BChara::CheckHead() 
{
	ML::Box2D head(this->hitBase.x, this->hitBase.y - 1, this->hitBase.w, 1); //頭上矩形生成
	head.Offset((int)this->pos.x, (int)this->pos.y);	//現在地オフセット

	auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	auto block = ge->GetTask_Group_G<Block::Object>("ブロック");
	if (nullptr == map) { return false; } //マップがなければ判定しない
	if (map->CheckHit(head))
	{
		return true;
	}
	for (int i = 0; i < block->size(); ++i)
	{
		if (true == (*block)[i]->CheckHit(head))
		{
			return true;
		}
	}
	return false;
}
//めり込まない移動処理
void BChara::CheckMove(ML::Vec2& e_) 
{
	auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ"); //マップが存在するか調べてからアクセス
	if (nullptr == map) { return; } //マップがなければ判定しない

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
	}
	//縦軸に対する移動
	while (e_.y != 0) {
		float  preY = this->pos.y;
		if (e_.y >= 1) { this->pos.y += 1;		e_.y -= 1; }
		else if (e_.y <= -1) { this->pos.y -= 1;		e_.y += 1; }
		else { this->pos.y += e_.y;		e_.y = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.y = preY;		//移動をキャンセル
			break;
		}
	}
}
//足元接触判定
bool BChara::CheckFoot()
{
	ML::Box2D foot(this->hitBase.x, this->hitBase.y + this->hitBase.h, this->hitBase.w, 1); //足元矩形生成
	foot.Offset(this->pos);	//現在地オフセット

	auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	auto block = ge->GetTask_Group_G<Block::Object>("ブロック");
	//if (nullptr == map) { 
		//return false; } //マップがなければ判定しない
	for (auto id = block->begin(); id != block->end(); ++id)
	{
		if ((*id)->CheckHit(foot))
		{
			return true;
		}
	}
	return map->CheckHit(foot);
}
//左接触判定
bool BChara::CheckLeft() 
{
	ML::Box2D left(this->hitBase.x - 1, this->hitBase.y, 1, this->hitBase.h); //左上矩形生成
	left.Offset(this->pos);	//現在地オフセット

	auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	if (nullptr == map) { return false; } //マップがなければ判定しない
	return map->CheckHit(left);
	
}
//右接触判定
bool BChara::CheckRight() 
{
	ML::Box2D right(this->hitBase.x + this->hitBase.w, this->hitBase.y, 1, this->hitBase.h); //右上矩形生成
	right.Offset(this->pos);	//現在地オフセット

	auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	if (nullptr == map) { return false; } //マップがなければ判定しない
	return map->CheckHit(right);
}
//接触判定
bool BChara::CheckHit(const ML::Box2D& hit_)
{
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	return me.Hit(hit_);
}
void BChara::Damage()
{
	auto pl = ge->GetTask_One_G<Player::Object>("プレイヤー");
	pl->playerhp--;
}
//
void BChara::Received(BChara* from_)
{
	ML::MsgBox("実装されていません");
}