#include "Shader.h"

__interface IUBO {
	void Set(void* data);
	void Bind(const Shader& shader, const std::string& name);
};

template<class T>
class UBO : public IUBO
{
	static GLuint getNextBindingPoint() {
		static GLuint bindpoint = 0;
		return bindpoint++;
	}

public:
	UBO();
	void Set(void* data) final;
	void Bind(const Shader& shader, const std::string& name) final;
private:
	GLuint ubo;
	GLuint binding;
};

template<class T>
inline UBO<T>::UBO()
{
	binding = getNextBindingPoint();
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<class T>
inline void UBO<T>::Set(void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<class T>
inline void UBO<T>::Bind(const Shader& shader, const std::string& name)
{
	GLuint index = glGetUniformBlockIndex(shader.GetShaderId(), name.data());
	glUniformBlockBinding(shader.GetShaderId(), index, binding);
}
