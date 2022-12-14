#pragma once
#ifdef  DLL_GRAPHICS_API
#else
#define DLL_GRAPHICS_API _declspec(dllexport)
#endif
#include "Core/Render_Core/directx_render.h"
#include "gpu_resource.h"

//策略模式 选择渲染策略
template<typename t_render>
class render_system : public s_render_system
{
public:
	//render_system(HINSTANCE in_instance) { init(in_instance); };
	render_system() {};
protected:
	
	//不许复制
	render_system(const render_system&) {};
	render_system& operator=(const render_system&) {};
	~render_system()
	{
		delete(render_window);
		delete(renderer);
		over();
	};
protected:
	s_window* render_window = nullptr;
	t_render* renderer = nullptr;

public:

	virtual void draw_pass(s_pass* in_pass) override;

	//遍历所有刷新数
	virtual bool update_gpu_memory() override;

	virtual void execute_command() override;

	virtual void reset_command() override;
private:
	virtual void init_in_HWND(HWND in_HWND, UINT in_width, UINT in_height) override;

	virtual void init_new_window(HINSTANCE in_instance, UINT in_width, UINT in_height) override;

	virtual void over() override  {};
};



