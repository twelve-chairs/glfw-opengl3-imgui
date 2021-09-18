//
// Created by VokamisAir on 3/18/21.
//

#include "common.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#endif


//The easy and flexible way is to render to texture (which is something you can figure out on OpenGL side, it's
//not really an ImGui thing). Once you have a texture you can just call ImGui::Image() with your texture identifier.