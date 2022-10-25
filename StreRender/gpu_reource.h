#pragma once
#include "base_type.h"
#include <vector>
#include <string>

//���߳̿�����С��λ

//����ˢ�£����߳��������ݿ�
//custom buffer
//index
//vertex



//Ӧ����ģ�壿ֻΪ��Ӧ��api�ṩ��Ӧ����Ϣ��
struct gpu_shader_resource
{
	enum SHADER_RESOURCE_TYPE
	{
		SHADER_RESOURCE_TYPE_CUSTOM_BUFFER, // CSV
		SHADER_RESOURCE_TYPE_CUSTOM_BUFFER_GROUP, //SRV
		SHADER_RESOURCE_TYPE_TEXTURE, //SRV
		SHADER_RESOURCE_TYPE_TEXTURE_GROUP, //TABLE
		SHADER_RESOURCE_TYPE_RENDER_TARGET, //SRV
		SHADER_RESOURCE_TYPE_RENDER_TARGET_GROUP, // TABLE
		SHADER_RESOURCE_TYPE_RENDER_DEPTH_STENCIL // DSV
	}shader_resource_type;

	size_t register_index = 0; //ʹ�õļĴ������

	std::string name;
	//ֻ���ϴ�����Ч
	BYTE* mapped_data = nullptr;

	size_t element_size = 0;
	size_t element_count = 0;

	

	//�����ٸ�Ԫ��Ϊһ��
	std::vector<UINT> element_group_number; //���ڴ洢index�����ݵ��ڲ�ƫ��( [0] ��0���ж��ٸ�Ԫ��)

	//...
};

struct gpu_rander_target
{
	s_uid uid;

	gpu_shader_resource* gpu_sr_ptr;
};