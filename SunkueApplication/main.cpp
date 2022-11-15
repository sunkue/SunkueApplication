#include "Module/Application";

int main()
{
	Shader::ShaderDir() = "Module/src/shader";
	SunkueWindow win;
	TestActor a; a.Init();
	a.EnableGui();
	win.gui()->Regist(a);
	win.renderer()->Regist(a);
	win.run();
	std::cout << "end";
}