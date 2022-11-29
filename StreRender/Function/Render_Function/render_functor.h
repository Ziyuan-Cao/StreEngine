#pragma once
#include <queue>
#include <functional>
#include "stre_render_api.h"

typedef std::function<bool(s_directx_render* in_render)>
dx_function;


//functor
template<class t_function>
class function_command
{
public:

	std::queue<t_function> command_queue;
};


