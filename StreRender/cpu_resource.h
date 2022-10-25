#pragma once
#include "base_type.h"
#include "gpu_reource.h"
/***
************************************************************
*
* Resource
*
************************************************************
*/

//���߳̿�����С��λ

struct s_light
{
	//ֻ�з���⣿
	s_float3 strength = { 10.0f, 10.0f, 10.0f };
	s_float fall_off_start = 1.0f;
	s_float3 Direction = { 0.0f, -1.0f, 0.0f };// directional/spot light only
	s_float fall_off_end = 10.0f;                           // point/spot light only
	s_float3 position = { 0.0f, 0.0f, 0.0f };  // point/spot light only
	s_float spot_power = 64.0f;
	//...
};

struct s_camera
{
	s_float4x4 View = s_float4x4::identity4x4();
	s_float4x4 InvView = s_float4x4::identity4x4();
	s_float4x4 Proj = s_float4x4::identity4x4();
	s_float4x4 InvProj = s_float4x4::identity4x4();
	s_float4x4 ViewProj = s_float4x4::identity4x4();
	s_float4x4 InvViewProj = s_float4x4::identity4x4();
	s_float4x4 ShadowTransform = s_float4x4::identity4x4();
	s_float3 EyePosW = { 0.0f, 0.0f, 0.0f };
	s_float2 RenderTargetSize = { 0.0f, 0.0f };
	s_float2 InvRenderTargetSize = { 0.0f, 0.0f };
	s_float NearZ = 0.0f;
	s_float FarZ = 0.0f;

	s_float4 AmbientLight = { 0.0f, 0.0f, 0.0f, 1.0f };
	//...
};

struct s_material
{
	//...
	//��ͼ���� ˢ��ʱҪ�ĳɶ�Ӧpass����ͼ���
	unsigned int texture_index1;
	unsigned int texture_index2;
	unsigned int texture_index3;
	unsigned int texture_index4;
};

struct s_object_constant
{
	s_float4x4 world_position = s_float4x4::identity4x4();
	s_float4x4 tex_transform = s_float4x4::identity4x4();
	struct 
	{
		s_float3 min_position; // bound_box
		s_float3 max_position; // bound_box
	}object_bound_box;
	s_float material_index = 0;
};
struct s_sence_constant
{
	s_float TotalTime = 0.0f;
	s_float DeltaTime = 0.0f;
};

typedef UINT s_index;

typedef void s_texture;

template<typename t_element>
struct cpu_resource
{
	//??? �Զ�����
	s_uid uid;

	t_element* data = nullptr;

	size_t count = 0;

	t_element* get_data() { return data; };

	size_t get_element_size() { return sizeof(t_element); };

	size_t get_element_count() { return count; };

	gpu_shader_resource* gpu_sr_ptr;
	//��������
};


typedef cpu_resource<s_vertex> cpu_vertex;						//һ�鶥��
typedef cpu_resource<s_index> cpu_index;						//һ������
typedef cpu_resource<s_light> cpu_light;						//һ��ƹ�
typedef cpu_resource<s_camera> cpu_camera;						//һ�����
typedef cpu_resource<s_material> cpu_material;					//һ�����
typedef cpu_resource<s_texture> cpu_texture;					//һ����ͼ ��ͼֻ��һ��
typedef cpu_resource<s_object_constant> cpu_object_constant;	//һ�����峣��
typedef cpu_resource<s_sence_constant> cpu_sence_constant;		//һ��ֻ��һ����������

//���ʵ��ʱ��������ֻ�ǹ���ĳЩ��Դ
template<
	class t_vertex = cpu_vertex,
	class t_index = cpu_index,
	class t_material = cpu_material,
	class t_texture = cpu_texture,
	class t_object_constant = cpu_object_constant>
	struct s_mesh
{
	s_uid uid;

	t_vertex* vertex_ptr;
	t_index* index_ptr;
	std::vector <size_t> index_offset;

	t_object_constant* object_constant_ptr;
	t_material* material_ptr;
	std::vector< t_texture*> texture_ptr;
};


template<
	class t_light = cpu_light,
	class t_camera = cpu_camera,
	class t_sence_constant = cpu_sence_constant>
	struct s_sence
{
	s_uid uid;

	std::vector < t_light*> light_ptr;
	std::vector < t_camera*> camera_ptr;
	t_sence_constant* sence_constant;
};


typedef s_mesh<> cpu_mesh;
typedef s_sence<> cpu_sence;



