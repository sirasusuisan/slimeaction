//
//�L�����N�^�ėp�X�[�p�[�N���X
//
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map2D.h" 
#include  "Task_Player.h"
#include  "Task_Block.h"
//���[�V�����X�V �ύX���Ȃ��ꍇfalse
bool BChara::UpdateMotion(Motion nm_)
{
	if (nm_ == this->motion)//�O��̃��[�V�����������Ȃ�
	{
		return false;
	}
	else//�O��̃��[�V�������Ⴄ�Ȃ�
	{
		this->motion = nm_;	//���[�V�����ύX
		this->moveCnt = 0;	//�s���J�E���^���Z�b�g
		this->animCnt = 0;	//�A�j���[�V�����J�E���^���Z�b�g
		return true;
	}
}
//����ڐG����
bool BChara::CheckHead() 
{
	ML::Box2D head(this->hitBase.x, this->hitBase.y - 1, this->hitBase.w, 1); //�����`����
	head.Offset((int)this->pos.x, (int)this->pos.y);	//���ݒn�I�t�Z�b�g

	auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	auto block = ge->GetTask_Group_G<Block::Object>("�u���b�N");
	if (nullptr == map) { return false; } //�}�b�v���Ȃ���Δ��肵�Ȃ�
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
//�߂荞�܂Ȃ��ړ�����
void BChara::CheckMove(ML::Vec2& e_) 
{
	auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v"); //�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	if (nullptr == map) { return; } //�}�b�v���Ȃ���Δ��肵�Ȃ�

    //�}�b�v�����݂��A�L�����N�^���Î~��Ԃ���Ȃ����[���ړ�]
	while (e_.x != 0) 
	{
		float preX = this->pos.x;	//�ړ��O�̃L�����N�^�ʒu
		if (e_.x >= 1) { this->pos.x += 1; e_.x -= 1; } //�E
		else if (e_.x <= -1) { this->pos.x -= 1; e_.x += 1; }//��
		else { this->pos.x += e_.x; e_.x = 0; }
		//�����ʒu�I�t�Z�b�g
		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit))
		{
			this->pos.x = preX;	//�ړ��L�����Z��
			break;
		}
	}
	//�c���ɑ΂���ړ�
	while (e_.y != 0) {
		float  preY = this->pos.y;
		if (e_.y >= 1) { this->pos.y += 1;		e_.y -= 1; }
		else if (e_.y <= -1) { this->pos.y -= 1;		e_.y += 1; }
		else { this->pos.y += e_.y;		e_.y = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.y = preY;		//�ړ����L�����Z��
			break;
		}
	}
}
//�����ڐG����
bool BChara::CheckFoot()
{
	ML::Box2D foot(this->hitBase.x, this->hitBase.y + this->hitBase.h, this->hitBase.w, 1); //������`����
	foot.Offset(this->pos);	//���ݒn�I�t�Z�b�g

	auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	auto block = ge->GetTask_Group_G<Block::Object>("�u���b�N");
	//if (nullptr == map) { 
		//return false; } //�}�b�v���Ȃ���Δ��肵�Ȃ�
	for (auto id = block->begin(); id != block->end(); ++id)
	{
		if ((*id)->CheckHit(foot))
		{
			return true;
		}
	}
	return map->CheckHit(foot);
}
//���ڐG����
bool BChara::CheckLeft() 
{
	ML::Box2D left(this->hitBase.x - 1, this->hitBase.y, 1, this->hitBase.h); //�����`����
	left.Offset(this->pos);	//���ݒn�I�t�Z�b�g

	auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	if (nullptr == map) { return false; } //�}�b�v���Ȃ���Δ��肵�Ȃ�
	return map->CheckHit(left);
	
}
//�E�ڐG����
bool BChara::CheckRight() 
{
	ML::Box2D right(this->hitBase.x + this->hitBase.w, this->hitBase.y, 1, this->hitBase.h); //�E���`����
	right.Offset(this->pos);	//���ݒn�I�t�Z�b�g

	auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	if (nullptr == map) { return false; } //�}�b�v���Ȃ���Δ��肵�Ȃ�
	return map->CheckHit(right);
}
//�ڐG����
bool BChara::CheckHit(const ML::Box2D& hit_)
{
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	return me.Hit(hit_);
}
void BChara::Damage()
{
	auto pl = ge->GetTask_One_G<Player::Object>("�v���C���[");
	pl->playerhp--;
}
//
void BChara::Received(BChara* from_)
{
	ML::MsgBox("��������Ă��܂���");
}