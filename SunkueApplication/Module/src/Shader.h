#pragma once

#include "../Base"


class Shader
{
public:
	enum class ShaderType {
		normal,
		phong,
		unlit
	};
	using ShaderMap_t = std::map<ShaderType, Shader>;
	static ShaderMap_t ShaderMap;

	void Use()const;

	template<class T> void Set(const std::string& uniform_var_name, const T& value)const;

	static Shader& Basic()
	{
		std::vector<std::string> VS;
		std::vector<std::string> FS;
		std::vector<std::string> GS;
		VS.clear(); VS.emplace_back("./Shader/test_vertex.glsl");
		FS.clear(); FS.emplace_back("./Shader/default_fragment.glsl");
		GS.clear();
		static Shader Basic(VS, FS, GS);
		return Basic;
	}

private:
	Shader(const std::vector<std::string>& filenameVS, const std::vector<std::string>& filenameFS, const std::vector<std::string>& filenameGS);

private:
	void addShader(GLuint ShaderProgram, const char* pShaderText, GLenum shader_type);
	GLuint compileShader(const std::vector<std::string>& filenameVS, const std::vector<std::string>& filenameFS, const std::vector<std::string>& filenameGS);

private:
	GLuint shaderId;
};

#include "Shader.hpp"


