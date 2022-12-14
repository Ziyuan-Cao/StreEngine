#pragma once
#include <Windows.h>
#include <list>
#include "stre_configuration.h"
#include "cpu_resource.h"
#include "gpu_resource.h"
#include "stre_render_api.h"
#ifdef  DLL_GRAPHICS_API
#else
#define DLL_GRAPHICS_API _declspec(dllimport)
#endif


///rendering type
//渲染器

/// <summary>
/// 渲染系统，外部控制GPU的基本类型，控制总绘制流程。
/// </summary>
class s_render_system
{
public:

	virtual void draw_pass(s_pass* in_pass) = 0;

	/// <summary>
	/// //遍历所有刷新函数
	/// </summary>
	/// <returns>false代表有错误需要去获取exception</returns>
	virtual bool update_gpu_memory() = 0;

	virtual void execute_command() = 0;

	virtual void reset_command() = 0;

	virtual void init_in_HWND(HWND in_HWND, UINT in_width, UINT in_height) = 0;

	virtual void init_new_window(HINSTANCE in_instance, UINT in_width, UINT in_height) = 0;

	virtual void over() = 0;
};


class DLL_GRAPHICS_API render_factory
{
public:
	template<class t_render>
	s_render_system* create_render_system();

	template<>
	s_render_system* create_render_system<s_directx_render>();
};

/***
************************************************************
*
* Resource Manager
*
************************************************************
*/

/// <summary>
/// 自定义资源控制器
/// </summary>
/// <typeparam name="t_cpu_res_type">自定义资源</typeparam>
template<typename t_cpu_res_type>
struct s_custom_manager
{
	virtual t_cpu_res_type* create_resource(bool can_update = false) = 0;

	virtual t_cpu_res_type* create_resource(size_t in_element_number, bool can_update = false) = 0;

	//??? 缺读取资源
	virtual t_cpu_res_type* load_resource(wchar_t* in_path) = 0;

	virtual void update_gpu(t_cpu_res_type* in_cpu_data) = 0;

	virtual void dx_allocate_gpu_resource(t_cpu_res_type* in_cpu_data,
		gpu_shader_resource::SHADER_RESOURCE_TYPE in_sr_type) = 0;
};

/// <summary>
/// 默认材质资源控制器
/// </summary>
struct s_material_manager : public s_custom_manager<cpu_material>
{
public:
	//特殊的处理函数
	//回调绑定关系
	virtual void change_texture() = 0;

	virtual void add_texture() = 0;

	virtual void release_texture() = 0;
};



/// <summary>
/// 默认贴图资源控制器
/// </summary>
struct s_texture_manager : public s_custom_manager<cpu_texture>
{
public:
	virtual void package_textures(
		std::vector<cpu_texture*> in_texture_group,
		cpu_texture* in_out_table) = 0;
};


/// <summary>
/// 默认模型控制器
/// </summary>
struct s_mesh_manager
{
public:
	virtual cpu_mesh* create_resource() = 0;

	virtual cpu_mesh* load_resource(const wchar_t* in_path) = 0;

	virtual void update_gpu(cpu_mesh* in_cpu_data) = 0;
	//特殊的处理函数
	virtual cpu_mesh* load_fbx(const wchar_t* in_path) = 0;

	virtual void dx_allocate_gpu_resource(cpu_mesh* in_cpu_data) = 0;
};

/// <summary>
/// 默认场景控制器  可以拆散，作为自定义资源输入pass
/// </summary>
/// <returns></returns>
struct s_sence_manager
{
public:
	virtual cpu_sence* create_resource() = 0;

	virtual cpu_sence* load_resource(wchar_t* in_path) = 0;

	virtual void update_gpu(cpu_sence* in_cpu_data) = 0;

	virtual void dx_allocate_gpu_resource(cpu_sence* in_cpu_data) = 0;
};

/// <summary>
/// 资源控制器工厂
/// </summary>
class DLL_GRAPHICS_API resource_manager_factory
{
public:
	template<typename t_cpu_res_type>
	s_custom_manager<t_cpu_res_type>* create_manager();
	
	//需要显示定义模板，否则不会编译到库中
	template<>
	s_custom_manager<cpu_vertex>* create_manager();

	template<>
	s_custom_manager<cpu_index>* create_manager();

	template<>
	s_custom_manager<cpu_camera>* create_manager();

	template<>
	s_custom_manager<cpu_light>* create_manager();

	//有特殊函数的就要独立编写
	s_material_manager* create_material_manager();

	s_texture_manager* create_texture_manager();

	s_mesh_manager* create_mesh_manager();

	s_sence_manager* create_sence_manager();
};



/***
************************************************************
*
* Pass Manager
*
************************************************************
*/

/// <summary>
/// 绘制过程工厂
/// </summary>
class DLL_GRAPHICS_API pass_factory
{
public:
	//创建pass
	s_pass* create_pass();
	void dx_allocate_gpu_pass(s_pass* in_out_pass);

	bool release_gpu_pass(s_pass* in_out_pass);
	//??? 差刷新pass函数

	bool check_pass(s_pass* in_out_pass);

	bool add_mesh(s_pass* in_out_pass, const cpu_mesh* in_mesh);

	//cpu_resource
	template<class t_cpu_resource>
	bool add_shader_resource(s_pass* in_out_pass, const t_cpu_resource* in_sr);

	template<>
	bool add_shader_resource<cpu_texture>(s_pass* in_out_pass, const cpu_texture* in_sr);

	template<>
	bool add_shader_resource<cpu_camera>(s_pass* in_out_pass, const cpu_camera* in_sr);

	template<>
	bool add_shader_resource<cpu_light>(s_pass* in_out_pass, const cpu_light* in_sr);

	bool add_render_target(s_pass* in_out_pass, const cpu_texture* in_gpu_rt);

	bool remove_mesh(s_pass* in_out_pass, const cpu_mesh* in_mesh);

	template<class t_cpu_resource>
	bool remove_shader_resource(s_pass* in_out_pass, const t_cpu_resource* in_gpu_sr);

	template<>
	bool remove_shader_resource<cpu_texture>(s_pass* in_out_pass, const cpu_texture* in_sr);

	template<>
	bool remove_shader_resource<cpu_camera>(s_pass* in_out_pass, const cpu_camera* in_sr);

	template<>
	bool remove_shader_resource<cpu_light>(s_pass* in_out_pass, const cpu_light* in_sr);


	bool remove_render_target(s_pass* in_out_pass, const cpu_texture* in_gpu_rt);

	bool set_shader_layout(s_pass* in_out_pass, const shader_layout in_shade_layout);

	bool remove_shader_layout(s_pass* in_out_pass);
};


/***
************************************************************
*
* Exception
*
************************************************************
*/

namespace stre_exception
{
	DLL_GRAPHICS_API extern std::list<std::string> exception_output_str_group;
	DLL_GRAPHICS_API extern int fence; //字符串数组上一次获取的位置
};


