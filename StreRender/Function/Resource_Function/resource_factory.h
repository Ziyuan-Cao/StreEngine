#pragma once
#include "Core/Memory/s_memory.h"
#include "stre_render.h"
#include "gpu_resource.h"
#include "Function/Render_Function/render_functor.h"
#include "Core/Math/stre_math.h"

extern function_command<dx_function> dx_pass_command;

extern function_command<dx_function> dx_shader_resource_command;



template<typename t_cpu_res_type>
struct custom_manager :public s_custom_manager<t_cpu_res_type>
{
	virtual t_cpu_res_type* create_resource(bool in_can_update = false) override
	{
		auto instance = new t_cpu_res_type(in_can_update);
		generate_unique_identifier<t_cpu_res_type>(instance->uid);
		return instance;
	}

	//初始内存将处于全0状态
	virtual t_cpu_res_type* create_resource(size_t in_element_number, bool in_can_update = false) override
	{
		t_cpu_res_type* cpu_ptr = create_resource(in_can_update);
		cpu_ptr->data = custom_allocate(in_element_number * cpu_ptr->get_element_size());
		cpu_ptr->count = in_element_number;

		return cpu_ptr;
	}
	//??? 缺读取资源
	virtual t_cpu_res_type* load_resource(wchar_t* in_path) override
	{
		return nullptr;
	}
	virtual void update_gpu(t_cpu_res_type* in_cpu_data) override
	{

		dx_function sr_functor = [in_cpu_data](s_directx_render* in_render)->bool
		{
			std::shared_ptr<gpu_shader_resource> sr_ptr = in_cpu_data->gpu_sr_ptr;

			return in_render->update_gpu_resource(sr_ptr, in_cpu_data->get_data(), 0, sr_ptr->element_count);
		};
		dx_shader_resource_command.command_queue.push(sr_functor);

	}

	virtual void dx_allocate_gpu_resource(t_cpu_res_type* in_cpu_data,
		gpu_shader_resource::SHADER_RESOURCE_TYPE in_sr_type) override
	{
		dx_function sr_functor = [in_cpu_data, in_sr_type](s_directx_render* in_render)->bool
		{
			bool execute_successed = true;
			//共享指针自动释放 所以没有delete
			
			//构建描述符
			in_cpu_data->gpu_sr_ptr = in_render->allocate_shader_resource(in_sr_type);
			
			switch (in_sr_type)
			{
				//自定义内存手动申请空间
			case gpu_shader_resource::SHADER_RESOURCE_TYPE_CUSTOM_BUFFER:
			case gpu_shader_resource::SHADER_RESOURCE_TYPE_CUSTOM_BUFFER_FOLLOW_MESH:
			case gpu_shader_resource::SHADER_RESOURCE_TYPE_CUSTOM_BUFFER_GROUP:
			{
				//构建内存
				if (in_cpu_data->can_update)
				{
					execute_successed &= in_render->allocate_upload_resource(
						in_cpu_data->gpu_sr_ptr,
						in_cpu_data->get_element_size(),
						in_cpu_data->get_element_count());
				}
				else
				{
					execute_successed &= in_render->allocate_default_resource(
						in_cpu_data->gpu_sr_ptr,
						in_cpu_data->get_element_size(),
						in_cpu_data->get_element_count(),
						in_cpu_data->data);
				}
			}
			default:
				break;
			}
			return execute_successed;
		};

		dx_shader_resource_command.command_queue.push(sr_functor);
	}
};

struct material_manager : public s_material_manager
{
public:
	virtual cpu_material* create_resource(bool in_can_update = false) override
	{
		return custom_manager<cpu_material>().create_resource();
	}

	virtual cpu_material* create_resource(size_t in_element_number, bool in_can_update = false) override
	{
		return custom_manager<cpu_material>().create_resource(in_element_number);
	}

	//??? 缺读取资源
	virtual cpu_material* load_resource(wchar_t* in_path) override
	{
		return custom_manager<cpu_material>().load_resource(in_path);
	}

	virtual void update_gpu(cpu_material* in_cpu_data) override
	{
		custom_manager<cpu_material>().update_gpu(in_cpu_data);
	}

	virtual void dx_allocate_gpu_resource(cpu_material* in_cpu_data,
		gpu_shader_resource::SHADER_RESOURCE_TYPE in_sr_type) override
	{
		//不允许材质独立申请GPU内存
		//custom_manager<cpu_material>().dx_allocate_gpu_resource(in_cpu_data, in_sr_type);
	}

	//特殊的处理函数
	//回调绑定关系
	virtual void change_texture() override;

	virtual void add_texture() override;

	virtual void release_texture() override;
};

struct texture_manager : public s_texture_manager
{
public:
	virtual cpu_texture* create_resource(bool in_can_update = false) override
	{
		return custom_manager<cpu_texture>().create_resource(in_can_update);
	}

	virtual cpu_texture* create_resource(size_t in_element_number, bool in_can_update = false) override
	{
		return custom_manager<cpu_texture>().create_resource(in_element_number, in_can_update);
	}

	//??? 缺读取资源
	virtual cpu_texture* load_resource(wchar_t* in_path) override
	{
		return custom_manager<cpu_texture>().load_resource(in_path);
	}

	virtual void update_gpu(cpu_texture* in_cpu_data) override
	{
		custom_manager<cpu_texture>().update_gpu(in_cpu_data);
	}

	//??? 贴图的导入还没写
	virtual void dx_allocate_gpu_resource(cpu_texture* in_cpu_data,
		gpu_shader_resource::SHADER_RESOURCE_TYPE in_sr_type) override
	{
		custom_manager<cpu_texture>().dx_allocate_gpu_resource(in_cpu_data, in_sr_type);
	}

	virtual void package_textures(
		std::vector<cpu_texture*> in_texture_group,
		cpu_texture* in_out_table) override
	{
		dx_function sr_functor = [in_texture_group, in_out_table](s_directx_render* in_render)->bool
		{
			std::vector<std::shared_ptr<gpu_shader_resource>> package_group;

			for (auto it : in_texture_group)
			{
				package_group.push_back(it->gpu_sr_ptr);
			}

			return in_render->package_textures(package_group, in_out_table->gpu_sr_ptr);
		};
		dx_shader_resource_command.command_queue.push(sr_functor);
	}

};

struct mesh_manager : public s_mesh_manager
{
public:
	virtual cpu_mesh* create_resource() override;

	//??? 缺读取资源
	virtual cpu_mesh* load_resource(const wchar_t* in_path) override;


	virtual void update_gpu(cpu_mesh* in_cpu_data) override;


	virtual cpu_mesh* load_fbx(const wchar_t* in_path) override;

	virtual void dx_allocate_gpu_resource(cpu_mesh* in_cpu_data) override;
};



struct sence_manager : public s_sence_manager
{
public:
	virtual cpu_sence* create_resource() override;

	//??? 缺读取资源
	virtual cpu_sence* load_resource(wchar_t* in_path) override;

	virtual void update_gpu(cpu_sence* in_cpu_data) override;

	virtual void dx_allocate_gpu_resource(cpu_sence* in_cpu_data) override;
};

