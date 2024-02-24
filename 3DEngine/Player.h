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

#pragma once

#include "raylib.h"
#include "rlgl.h"
#include "MathLib.h"

#include <cassert>
#include <string>
#include <unordered_map>
#include <functional>

class Player;

typedef std::function<void(Player&)> AnimStateFunction;

class Player
{
public:
	Player();
	~Player();
	
	inline Vector3 GetPosition() const 
	{ 
		return _position; 
	}

	inline Vector3 GetDestination() const
	{
		return _destination;
	}

	void Update(const Camera& camera);
	void Draw() const;

private:
	void Idle(Player&);
	void Walk(Player&);
	void Run(Player&);
	void Backflip(Player&);
	void SetAnimState(const std::string& newState);

private:
	float _speed = 0.0f;
	float _scale = 1.0f;
	int _animsCount = 0;
	unsigned int _animIndex = 0;
	unsigned int _animCurrentFrame = 0;
	Vector3 _destination = {};
	Matrix _transformMatrix = {};
	std::unordered_map<std::string, 
	AnimStateFunction> _animStateActions = {};
	ModelAnimation* _modelAnimations = nullptr;
	float _backflipDuration = 0.0f;
	std::string _animState = {};
	Transform _transform = {};
	Quaternion _rotation = {};
	Vector3 _position = {};
	Model _model = {};
};