#pragma warning(disable:4996)
#pragma once
//
//�L�����N�^�ėp�X�[�p�[�N���X
//
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������
	//�L�����N�^���ʃ����o�ϐ�
	ML::Vec2	pos;//�L�����N�^�ʒu
	ML::Box2D	hitBase;//�����蔻��͈�
	ML::Box2D   lvHitBase;//�ǒ���t���p
	ML::Box2D   rvHitBase;
	//���S�_
	ML::Box2D   footCenter;
	ML::Box2D   leftCenter;
	ML::Box2D   rightCenter;
	ML::Box2D   headCenter;
	ML::Vec2	moveVec;//�ړ��x�N�g��
	int			moveCnt;//�s���J�E���^
    //���E�̌���
	enum Angle_LR { Left, Right };
	Angle_LR   angle_LR;
	WP target;
	//�ό`�̕���
	enum Form_VS { Vertical, Side };
	Form_VS form_VS;
	Form_VS preForm_VS;
	//�i��ł������
	enum Direction_Move { floor, lWall, rWall, ceiling };
	Direction_Move dMove;
	//Form_VS preForm_VS;
	//�L�����N�^�[�̏��
	enum Motion
	{
		Unnon = -1,	//����
		Stand,		//��~
		Walk,		//���s
		Jump,		//�W�����v
		Fall,		//����
		Dash,		//�_�b�V��
		//�ό`
		Transform_Stand, 
		Transform_Move,
		Transform_Jump,
		Transform_Fall,
		//����t��
		Stick_Stand,
		Stick_Move,
		TakeOff,	//��ї��u��(�W�����v�\������)
		Landing,	//���n(�W�����v�㓮��)
		Bound,      //���ꎞ
		TransBound,
		Death,      //���S��	

		//�G
		Turn,
	};
	Motion		motion;
	Motion		preMotion;
	int			animCnt = 0;//�A�j���[�V�����J�E���^
	float		jumPow;	//�W�����v����
	float       fallSpeed;//�������x
	float		maxFallSpeed;//�����ő呬�x
	float		gravity;//�d�� �t���[���P�ʂ̉��Z��
	float		dSpeed;//�_�b�V�������x
	float		nSpeed;//�ʏ펞���x
	int			playerhp;//�v���C���[HP
	int			enemyhp;//�GHP
	//int		shothp;//�U���I�u�W�F�N�gHP (�e�Ƃ��̂��)
	int			unHitTime;//�v���C���[���G����[

	//�����o�ϐ��̏�����
	//�ϐ��ǉ������珉�������Ă�
	BChara()
		: pos(0, 0)//�L�����N�^�ʒu
		, hitBase(0, 0, 0, 0)//�����蔻��͈�
		, moveVec(0, 0)//�ړ��x�N�g��
		, moveCnt(0)//�s���J�E���^
		, angle_LR(Right)//���E�̌��� �����l�͉E
		, motion(Stand) //�L�����N�^�[�̏�� �����l�͒ʏ�
		, jumPow(0)//�W�����v����
		, fallSpeed(0)//�������x
		, maxFallSpeed(0)//�����ő呬�x
		, gravity(0)//�d��
		, dSpeed(0)//�_�b�V�������x
		, nSpeed(0)//�ʏ펞���x
		, playerhp(3)//�v���C���[HP
		, enemyhp(1)//�G�l�~�[HP
		, unHitTime(0)//���G����
	{
	}
	virtual ~BChara() {}

	//�L�����N�^���ʃ��\�b�h
	virtual void CheckMove(ML::Vec2& est_);//�߂荞�܂Ȃ��ړ�����
	virtual bool CheckHead();//����ڐG����
	virtual bool CheckFoot();//�����ڐG����
	virtual bool CheckLeft();//���ڐG����
	virtual bool CheckRight();//�E�ڐG����
	bool UpdateMotion(Motion nm_);//���[�V�����ύX �ύX���Ȃ��ꍇfalse
	struct DrawInfo
	{
		ML::Box2D draw, src;
		ML::Color color;
		string		ret;
	};
	virtual void Received(BChara* from_);//�ڐG���̉�������
	virtual bool CheckHit(const ML::Box2D& hit_);//�ڐG����
	virtual void Damage();//�v���C���[�̃_���[�W�֐�
	bool HaveKey;//�J�M�̗L�薳������
	bool Playerliveflag;//�v���C���[�̐����t���O
};
