// Copyright (c) 2024 Wildan R Wijanarko (@wildan9)

// This software is provided "as-is", without any express or implied warranty. In no event 
// will the authors be held liable for any damages arising from the use of this software.

// Permission is granted to anyone to use this software for any purpose, including commercial 
// applications, and to alter it and redistribute it freely, subject to the following restrictions:

//   1. The origin of this software must not be misrepresented; you must not claim that you 
//   wrote the original software. If you use this software in a product, an acknowledgment 
//   in the product documentation would be appreciated but is not required.

//   2. Altered source versions must be plainly marked as such, and must not be misrepresented
//   as being the original software.

//   3. This notice may not be removed or altered from any source distribution.

#include "Player.h"

Player::Player()
{
    _model = LoadModel("resources/models/mixamo/model1.glb");
    _modelAnimations = LoadModelAnimations("resources/models/mixamo/model1.glb", &_animsCount);
    
    if (_modelAnimations == nullptr)
    {
        TraceLog(LOG_ERROR, "Failed to load model animations");
    }

    _position    = Vector3Zero();
    _destination = Vector3Zero();
    _rotation    = QuaternionIdentity();
    _scale       = Vector3{ 1.0f, 1.0f, 1.0f };

    _model.transform = MatrixTranslateV(_position) * QuaternionToMatrix(QuaternionFromEuler(90.0f, 0.0f, 0.0f)) * MatrixScaleV(_scale);

    _animState = "Idle";

    _animStateActions.insert({ "Idle",      [this](Player& anim)   { Idle(anim); } });
    _animStateActions.insert({ "Walk",      [this](Player& anim)   { Walk(anim); } });
    _animStateActions.insert({ "Run",       [this](Player& anim)   { Run(anim); } });
    _animStateActions.insert({ "Backflip",  [this](Player& anim)   { Backflip(anim); } });
}

Player::~Player()
{
    UnloadModel(_model);

    if (_modelAnimations != nullptr)
    {
        UnloadModelAnimation(*_modelAnimations);
    }
}

void Player::Idle(Player&)
{
    _animIndex = 1;
}

void Player::Walk(Player&)
{
    _animIndex = 3;
    _movementSpeed = 2.5f;
}

void Player::Run(Player&)
{
    _animIndex = 2;
    _movementSpeed = 7.0f;
}

void Player::Backflip(Player&)
{
    _animIndex = 0;

    if (_backflipDuration > 0)
    {
        _backflipDuration -= GetFrameTime();
    }
}

void Player::SetAnimState(const std::string& newState)
{
    auto it = _animStateActions.find(newState);
    if (it != _animStateActions.end())
    {
        if (_animState != newState)
        {
            _animCurrentFrame = 0;
        }
        _animState = newState;
    }
    else
    {
        TRACELOG(LOG_ERROR, "Invalid state");
    }
}

void Player::Update(const Camera& camera)
{
    Vector3 dir{};
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        Ray ray = GetMouseRay(GetMousePosition(), camera);
        Vector3 groundNormal{ 0.0f, 1.0f, 0.0f };

        float hitDist = -((Vector3DotProduct(ray.position, groundNormal) + 0) / Vector3DotProduct(ray.direction, groundNormal));

        _destination = ray.position + Vector3Scale(ray.direction, hitDist);
    }

    if (Vector3Distance(_destination, _position) > 0.1f && _backflipDuration < 0.1f) dir = Vector3Normalize(_destination - _position);
    else dir = Vector3Zero();

    _position = _position + Vector3Scale(dir, _movementSpeed * GetFrameTime());

    const bool onMove = (Vector3Length(dir) != 0 && _backflipDuration < 0.1f) ? true : false;

    if (onMove && IsKeyDown(KEY_LEFT_SHIFT))
    {
        SetAnimState("Run");
    }
    else if (onMove)
    {
        SetAnimState("Walk");
    }
    else if (_backflipDuration > 0)
    {
        SetAnimState("Backflip");
    }
    else
    {
        SetAnimState("Idle");
    }

    if (onMove)
    {
        const Vector3 vecUp{ 0.0f, 0.0f, 1.0f };
        float cos2Theta = vecUp.x * dir.x + vecUp.y * dir.y + vecUp.z * dir.z;
        Vector3 cross{ vecUp.y * dir.z - vecUp.z * dir.y, vecUp.z * dir.x - vecUp.x * dir.z, vecUp.x * dir.y - vecUp.y * dir.x };

        _rotation.x = cross.x;
        _rotation.y = cross.y;
        _rotation.z = cross.z;
        _rotation.w = 1.0f + cos2Theta;

        _rotation = QuaternionNormalize(_rotation);
    }

    _transform.position = Vector3{ _position.x, _position.y + 1.0f, _position.z };
    _transform.rotation = _rotation;
    _transform.scale    = _scale;

    auto action = _animStateActions.find(_animState);
    if (action != _animStateActions.end())
    {
        action->second(*this);
    }

    if (IsKeyPressed(KEY_SPACE) && !onMove)
    {
        _backflipDuration = 1.7f;
    }

#ifdef _DEBUG
    assert(_modelAnimations != nullptr);
#else
    if (_modelAnimations == nullptr) return;
#endif
    ModelAnimation& anim = _modelAnimations[_animIndex];
    _animCurrentFrame = (_animCurrentFrame + 1) % anim.frameCount;
    UpdateModelAnimation(_model, anim, _animCurrentFrame);
}

void Player::Draw() const
{
    rlPushMatrix();
    rlMultMatrixf(MatrixToFloat(_transform.ToMatrix()));
    DrawModel(_model, Vector3Zero(), 0.02f, WHITE);
    rlPopMatrix();
}