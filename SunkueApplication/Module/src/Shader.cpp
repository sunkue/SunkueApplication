#include "Shader.h"
#include "UniformBlockObject.hpp"

Shader::UniformBlockObjectMap Shader::UboMap;

Shader& Shader::Basic()
{
	bool inited = false; 
	std::vector<std::string> VS;
	std::vector<std::string> FS;
	std::vector<std::string> GS;
	if(!inited){
		VS.clear(); VS.emplace_back(ShaderDir() + "/default.vert");
		FS.clear(); FS.emplace_back(ShaderDir() + "/default.frag");
		GS.clear();
	}
	static Shader Basic(VS, FS, GS);
	if (!inited) {
		Basic.BindUbo<Eigen::Matrix4d>("VP_MAT");
		inited = true;
	}
	return Basic;
}

template<class T>
void Shader::BindUbo(const std::string& name){
	if (0 == UboMap.count(name)) {
		UboMap.emplace(name, *(new UBO<T>()));
	}
	UboMap.at(name).get().Bind(*this, name);
}

void Shader::SetUbo(const std::string& name, void* data){
	UboMap.at(name).get().Set(data);
}

Shader::Shader(const std::vector<std::string>& filenameVS, const std::vector<std::string>& filenameFS, const std::vector<std::string>& filenameGS)
	: shaderId{ compileShader(filenameVS, filenameFS, filenameGS) } {}

/// //////////////////////////////////////////////

bool readFile(const std::string& filename, std::string& target) {
	std::ifstream file(filename.data());
	if (file.fail()) {
		std::cerr << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (std::getline(file, line)) {
		target.append(line.c_str());
		target.append("\n");
	}
	return true;
}

/// //////////////////////////////////////////////

void Shader::addShader(GLuint shader_program, const char* raw_shader, GLenum shader_type) {
	GLuint shaderObj = glCreateShader(shader_type);
	if (shaderObj == 0) {
		std::cerr << "Error creating shader type " << shader_type << "\n";
	}
	const GLchar* p[1] = { raw_shader };
	GLint lengths[1] = {(GLint)strlen(raw_shader)};
	glShaderSource(shaderObj, 1, p, lengths);
	glCompileShader(shaderObj);
	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		std::cerr << "Error compiling shader type " << shader_type << "\n";
		std::cerr << raw_shader << "\n";
	}
	glAttachShader(shader_program, shaderObj);
	glDeleteShader(shaderObj);
}

GLuint Shader::compileShader(const std::vector<std::string>& filenameVS, const std::vector<std::string>& filenameFS, const std::vector<std::string>& filenameGS)
{
	GLuint shaderProgram = glCreateProgram();
	if (shaderProgram == 0) {
		std::cerr << "Error creating shader program" << "\n";
	}
	std::string vs, fs, gs;
	for (auto& vertex : filenameVS) {
		std::string temp;
		if (!readFile(vertex, temp)) {
			std::cerr << "Error compiling vertex shader" << "\n";
			return -1;
		};
		vs += temp;
	}
	for (auto& fragment : filenameFS)
	{
		std::string temp;
		if (!readFile(fragment, temp)) {
			std::cerr << "Error compiling fragment shader" << "\n";
			return -1;
		};
		fs += temp;
	}
	for (auto& geometry : filenameGS)
	{
		std::string temp;
		if (!readFile(geometry, temp)) {
			std::cerr << "Error compiling geometry shader" << "\n";
			return -1;
		};
		gs += temp;
	}
	addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
	if (filenameGS.empty()) const_cast<std::vector<std::string>&>(filenameGS).emplace_back("none_geometry");
	else addShader(shaderProgram, gs.c_str(), GL_GEOMETRY_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = {};
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << filenameVS.back() << ", " << filenameFS.back() << ", " << filenameGS.back() << " Error linking shader program\n" << ErrorLog;
		return -1;
	}
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << filenameVS.back() << ", " << filenameFS.back() << ", " << filenameGS.back() << " Error validating shader program\n" << ErrorLog;
		return -1;
	}
	glUseProgram(0);
	std::cerr << filenameVS.back() << ", " << filenameFS.back() << ", " << filenameGS.back() << " Shader compiling is done.\n";
	return shaderProgram;
}

/// //////////////////////////////////////////////

void Shader::Use()const {
	glUseProgram(shaderId);
}

