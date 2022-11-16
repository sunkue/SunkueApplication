#include "Shader.h"

__interface IUBO {
	void Set(const void* data);
	void Bind(const Shader& shader, const std::string& name);
};

static GLuint getNextBindingPoint() {
	static GLuint bindpoint = 1;
	return bindpoint++;
}

template<class T>
class UBO : public IUBO
{
public:
	UBO();
	void Set(const void* data) final;
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
	glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, sizeof(T));
}

template<class T>
inline void UBO<T>::Set(const void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<class T>
inline void UBO<T>::Bind(const Shader& shader, const std::string& name)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	GLuint index = glGetUniformBlockIndex(shader.GetShaderId(), name.data());
	glUniformBlockBinding(shader.GetShaderId(), index, binding);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
