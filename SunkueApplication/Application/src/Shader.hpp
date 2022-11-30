#pragma once

#include "Shader.h"
#include <array>

/////////////////////////////////////////

#define TEMPLATE_SHADER_SET(type) template<> inline void Shader::Set<type>(const std::string& uniform_var_name, const type& value)const
#define UNIFORM_LOCATION glGetUniformLocation(shaderId, uniform_var_name.data())
#define UNIFORM_LOCATION_STRUCT(mem) glGetUniformLocation(shaderId, uniform_var_name.data() + #mem)

/////////////////////////////////////////

template<class T>
inline void Shader::Set(const std::string& uniform_var, const T& value)const
{
	static_assert(std::_Always_false<T>, "Shader::set T&, come to Shader.hpp and specialize template for type.");
}

/////////////////////////////////////////

TEMPLATE_SHADER_SET(bool)
{
	glUniform1i(UNIFORM_LOCATION, value);
}

TEMPLATE_SHADER_SET(int)
{
	glUniform1i(UNIFORM_LOCATION, value);
}

TEMPLATE_SHADER_SET(float)
{
	glUniform1f(UNIFORM_LOCATION, value);
}

TEMPLATE_SHADER_SET(Eigen::Matrix4f)
{
	glUniformMatrix4fv(UNIFORM_LOCATION, 1, GL_FALSE, value.data());
}

TEMPLATE_SHADER_SET(Eigen::Affine3f)
{
	glUniformMatrix4fv(UNIFORM_LOCATION, 1, GL_FALSE, value.matrix().data());
}

TEMPLATE_SHADER_SET(ImVec4)
{
	glUniform4fv(UNIFORM_LOCATION, 1, &value.x);
}




#define SET_LIGHT_POWER()								\
set(uniform_var_name + ".power.ambient", value->ambient);		\
set(uniform_var_name + ".power.diffuse", value->diffuse);		\
set(uniform_var_name + ".power.specular", value->specular);

/////////////////////////////////////////


/////////////////////////////////////////

#undef TEMPLATE_SHADER_SET
#undef UNIFORM_LOCATION
#undef UNIFORM_LOCATION_STRUCT
#undef SET_LIGHT_POWER