#pragma once

#include "../Base"
class Shader;
__interface IUBO;

class Shader
{
public:
	enum class ShaderType {
		normal,
		phong,
		unlit
	};
	using ShaderMap_t = std::unordered_map<ShaderType, Shader>;
	using UniformBlockObjectMap = std::unordered_map<std::string, std::reference_wrapper<IUBO>>;
public:
	static ShaderMap_t ShaderMap;
	static std::string& ShaderDir() {
		static std::string ShaderDir;
		return ShaderDir;
	}
	static Shader& Basic();

public:
	void Use()const;
	template<class T> void Set(const std::string& uniform_var_name, const T& value)const;
public:
	GLuint GetShaderId()const { return shaderId; }
public:
	static UniformBlockObjectMap UboMap;
	static void SetUbo(const std::string& name, void* data);
	template<class T> void BindUbo(const std::string& name);
private:
	Shader(const std::vector<std::string>& filenameVS, const std::vector<std::string>& filenameFS, const std::vector<std::string>& filenameGS);

private:
	void addShader(GLuint ShaderProgram, const char* pShaderText, GLenum shader_type);
	GLuint compileShader(const std::vector<std::string>& filenameVS, const std::vector<std::string>& filenameFS, const std::vector<std::string>& filenameGS);
	
private:
	GLuint shaderId;
};

#include "Shader.hpp"


