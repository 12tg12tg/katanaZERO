#include "framework.h"
#include "boss.h"
#include "BossFSM.h"
#include "effect.h"
boss::boss()
{
}

boss::~boss()
{
}

HRESULT boss::init(float x, float y)
{
    imageInit();

    _z = ZUNIT;
    _x = x;
    _y = y;
    _img = IMAGE->findImage("headhunter_all");
    _bwimg = nullptr;
    _col = COLLISION->addCollider(Vec2(_x + _img->getFrameWidth() / 2, _y + _img->getFrameHeight() / 2),
        Vec2(_img->getFrameWidth() / 5, _img->getFrameHeight() * 3 / 5), COLLIDER_TYPE::ENEMY_UNIT, _z);
    _ani = ANIMATION->addNoneKeyAnimation("headhunter_all", 431, 420, 15, false, true);
    _foward = FOWARD::LEFT;

    _headImg = IMAGE->findImage("headhunter_head");
    _headAni = ANIMATION->addNoneKeyAnimation("headhunter_head", 0, 7, 15, false, true);
    _headjumpPower = 10.0f;
    _headgravity = 0.4f;
    _headmaxGravity = 10.0f;
    _headX = _headY = 0.f;
    _headLand = false;
    _headbreak = 0.2f;
    _headSpeed = 5.f;

    _hp = 9;

    stateInit();
    soundInit();

    reInit();

    _isSideCol = false;
    _isLand = false;
    _hasAi = true;
    
    _bm = new bossBulletManater;
    _bm->init();
    return S_OK;
}

void boss::release()
{
    COLLISION->erase(_col);
    _bm->release();
    _FSM->release();
    SAFE_DELETE(_FSM);
}

void boss::update()
{
    cheetMode();
    _bm->update();
    setpos();
    _FSM->update();
    setCollider();
    checkHit();
    if (_isCut) {
        headFly();
    }
}

void boss::render()
{
    _bm->render();
    _bottom = _col->getRect().bottom;
    ZORDER->ZorderAniRender(_img, _z, _bottom, _x, _y, _ani);
    ZORDER->SaveAniRender(_img, _bwimg, _z, _bottom, _x, _y, _ani);

    //사망시
    if (_isCut) {
        ZORDER->ZorderAniRender(_headImg, _z, _bottom + 1, _headX, _headY, _headAni);
    }
    //=============================================for DEGUG =============================================================
    //ZORDER->UITextOut("보스위치 : " + to_string(_x) + ", " + to_string(_y), ZUIFIRST, 600, 700, RGB(255, 255, 255));
    //ZORDER->UITextOut("헤드위치 : " + to_string(_headX) + ", " + to_string(_headY), ZUIFIRST, 600, 720, RGB(255, 255, 255));
}

void boss::reInit()
{
}

void boss::stateInit()
{
    _FSM = new BossFSM;
    _FSM->AddState(new Boss_Idle, this);
    _FSM->AddState(new Boss_Shoot, this);
    _FSM->AddState(new Boss_Rifle, this);
    _FSM->AddState(new Boss_RollJumpShot, this);
    _FSM->AddState(new Boss_Dash, this);
    _FSM->AddState(new Boss_Sweep, this);
    _FSM->AddState(new Boss_verLaser, this);
    _FSM->AddState(new Boss_Hurt, this);
    _FSM->AddState(new Boss_Dead, this);

    _FSM->SetState(BOSSSTATE::IDLE);
}

void boss::soundInit()
{
    SOUND->addSound("aimground", "sound/boss/aimground.wav", false, false);
    SOUND->addSound("aimtop", "sound/boss/aimtop.wav", false, false);
    SOUND->addSound("boom", "sound/boss/boom.wav", false, false);
    SOUND->addSound("dash", "sound/boss/dash.wav", false, false);
    SOUND->addSound("lazer", "sound/boss/lazer.wav", false, false);
    SOUND->addSound("predash", "sound/boss/predash.wav", false, false);
    SOUND->addSound("shoot", "sound/boss/shoot.wav", false, false);
    SOUND->addSound("sticky", "sound/boss/sticky.wav", false, false);
    SOUND->addSound("teleport", "sound/boss/teleport.wav", false, false);

    SOUND->addSound("sound_boss_huntresslaser_shot_1", "sound/boss/sound_boss_huntresslaser_shot_1.wav", false, false);
    SOUND->addSound("sound_boss_huntresslaser_shot_2", "sound/boss/sound_boss_huntresslaser_shot_2.wav", false, false);
    SOUND->addSound("sound_boss_huntresslaser_shot_3", "sound/boss/sound_boss_huntresslaser_shot_3.wav", false, false);
}

void boss::imageInit()
{
    IMAGE->addFrameImage("headhunter_all", "images/boss/headhunter_all.bmp", 1980*2, 2162*2, 18, 46, true);
    IMAGE->addFrameImage("headhunter_head", "images/boss/boss_headfly.bmp", 320, 168, 8, 4, true);
}

void boss::setCollider()
{
    _col->setPos(Vec2(_x + _img->getFrameWidth() / 2, _y + _img->getFrameHeight() / 2));
}

void boss::setpos()
{
    _isSideCol = false;
    _isLand = false;
    if (_x > 1066) {
        _x = 1066;
        _isSideCol = true;
    }
    if (_x < 73) {
        _x = 73;
        _isSideCol = true;
    }
    if (_y > 523) {
        _y = 523;
        _isLand = true;
    }

    if (_isCut) {
        if (_headX > 1149) {
            _headX = 1149;
        }
        if (_headX < 180) {
            _headX = 180;
        }
        if (_headY > 567) {
            _headY = 567;
            _headLand = true;
        }
    }
}

void boss::headFly()
{
    if (!_headLand) {
        if (_foward == FOWARD::RIGHT) _headX -= _headSpeed;
        else _headX += _headSpeed;
    }
    else {
        _headSpeed -= _headbreak;
        if (_headSpeed < 0) _headSpeed = 0;

        if (_foward == FOWARD::RIGHT) _headX -= _headSpeed;
        else _headX += _headSpeed;
    }

    if (_headjumpPower > 0) {
        _headY -= _headjumpPower;
        _headjumpPower -= _headgravity;
        if (_headjumpPower < 0) {
            _headjumpPower = 0;
            _headgravity = 0;
        }
    }
    else {
        _headY += _headgravity;
        _headgravity += 0.4f;
        if (_headgravity > _headmaxGravity)_headgravity = _headmaxGravity;
    }
}

void boss::cheetMode()
{
    if (INPUT->isOnceKeyDown(VK_F8)) _hasAi = !_hasAi;          /*치트모드진입*/
    if (_hasAi) return;
    //==========================================
    if (INPUT->isStayKeyDown(VK_RIGHT)) _x += 3;
    if (INPUT->isStayKeyDown(VK_LEFT)) _x -= 3;
    if (INPUT->isStayKeyDown(VK_DOWN)) _y += 3;
    if (INPUT->isStayKeyDown(VK_UP)) _y -= 3;
    //if (INPUT->isStayKeyDown(VK_RIGHT)) _headX += 3;
    //if (INPUT->isStayKeyDown(VK_LEFT)) _headX -= 3;
    //if (INPUT->isStayKeyDown(VK_DOWN)) _headY += 3;
    //if (INPUT->isStayKeyDown(VK_UP)) _headY -= 3;
    if (INPUT->isOnceKeyDown('1')) _FSM->ChangeState(BOSSSTATE::SHOOT);
    if (INPUT->isOnceKeyDown('2')) _FSM->ChangeState(BOSSSTATE::RIFLE);
    if (INPUT->isOnceKeyDown('3')) _FSM->ChangeState(BOSSSTATE::ROLL_JUMP_SHOTGUN);
    if (INPUT->isOnceKeyDown('4')) _FSM->ChangeState(BOSSSTATE::DASH);
    if (INPUT->isOnceKeyDown('5')) _FSM->ChangeState(BOSSSTATE::SWEEP);
    if (INPUT->isOnceKeyDown('6')) _FSM->ChangeState(BOSSSTATE::VERTICALLASER);
    if (INPUT->isOnceKeyDown('7')) _FSM->ChangeState(BOSSSTATE::HURT);
    if (INPUT->isOnceKeyDown('8')) _FSM->ChangeState(BOSSSTATE::DEAD);
    if (INPUT->isOnceKeyDown('R')) this->init(880, 523);
    //if (INPUT->isOnceKeyDown(VK_RBUTTON))
    //    _bm->getNormalBullet()->fire(WINSIZEX / 2, WINSIZEY / 2, 
    //        UTIL::getAngle(WINSIZEX / 2, WINSIZEY / 2, CAMERA->getRelativeMouse().x, CAMERA->getRelativeMouse().y));
    //if (INPUT->isOnceKeyDown(VK_RBUTTON))
    //	_bm->getVerLaser()->fire(CAMERA->getRelativeMouse().x, CAMERA->getRelativeMouse().y);
    //if (INPUT->isOnceKeyDown(VK_RBUTTON))
    //    _bm->getSweepLaser()->fire(CAMERA->getRelativeMouse().x, CAMERA->getRelativeMouse().y, FOWARD::RIGHT);
    //if (INPUT->isOnceKeyDown(VK_RBUTTON))
    //    _bm->getholLaser()->fire(CAMERA->getRelativeMouse().x, CAMERA->getRelativeMouse().y, FOWARD::LEFT);
    //if (INPUT->isOnceKeyDown(VK_RBUTTON))
    //    _bm->getCirBomb()->fire(CAMERA->getRelativeMouse().x, CAMERA->getRelativeMouse().y, FOWARD::LEFT);
    //==========================================
}

void boss::checkHit()
{
    if (!_isGracePeriod && _col->isEnterThere(COLLIDER_TYPE::BULLET_PLAYER)) {
        //이펙트
        _effectAngle = PLAYER->getCollider()->getPos().AngleTo(_col->getPos());
        float startX = _col->getPos().x + 1566 * cos((float)PI + _effectAngle);
        float startY = _col->getPos().y - 1566 * sinf((float)PI + _effectAngle);
        _hitEffect = EFFECT->play("hitEffect", ZEFFECT2, startX, startY, _effectAngle, 150);
        //쉐이크
        CAMERA->setShake(20, 10, 1);

        //사망인지 HURT인지 확인
        if (_state != BOSSSTATE::DEAD) {
            --_hp;
            if (_hp <= 0) _FSM->ChangeState(BOSSSTATE::DEAD);
            else _FSM->ChangeState(BOSSSTATE::HURT);
        }
        else {
            _isCut = true;
        }
    }

    //이펙트이동
    if (_hitEffect != nullptr) {
        _hitEffect->offsetEffect(cosf(_effectAngle) * 150, -sinf(_effectAngle) * 150);
        if (!_hitEffect->getIsRunning()) _hitEffect = nullptr;		//자기이펙트끝나면 칼같이 손절
    }
}
