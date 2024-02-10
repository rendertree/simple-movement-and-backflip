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

#include "RLCamera.h"

RLCamera::RLCamera()
{
    position    = Vector3{ 10.0f, 15.0f, 10.0f };  // Camera position
    target      = Vector3{ 0.0f, 2.0f, 2.0f };     // Camera looking at point
    up          = Vector3{ 0.0f, 1.0f, 0.0f };     // Camera up vector (rotation towards target)
    fovy        = 45.0f;                           // Camera field-of-view Y
    projection  = CAMERA_PERSPECTIVE;              // Camera projection type
}

void RLCamera::Update(const Vector3& targetPos)
{
    _offset = Vector3{ 
        _offset.x + (GetMouseWheelMove() * -1.0f), 
        _offset.y + (GetMouseWheelMove() * -1.0f), 
        _offset.z + (GetMouseWheelMove() * -1.0f) 
    };

    position = targetPos + _offset;
    target = targetPos;
}