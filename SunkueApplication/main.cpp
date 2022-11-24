#include "Module/Application";

#include "Example.h"

TestActor MakeTestActor();

int main()
{
	Shader::ShaderDir() = "Module/src/shader";

	SunkueWindow win(1240, 720);

	TestActor a = MakeTestActor();
	a.EnableGui();
	a.AddUpdateFunc([&](float t) { a.rotate(Eigen::Quaternionf(Eigen::AngleAxisf((a.movingSpeed() * t), Eigen::Vector3f(0, 1, 0).normalized()))); });

	win.renderer()->mainCamera().EnableGui();
	win.renderer()->sun().EnableGui();
	win.renderer()->Regist(a);
	win.renderer()->ResetCamera();
	win.system()->Regist(a);
	
	win.gui()->Regist(a);
	win.gui()->Regist(win.renderer()->mainCamera());
	win.gui()->Regist(win.renderer()->sun());
	
	win.run();
	
	std::cout << "end";
}

TestActor MakeTestActor() {
	std::vector<Eigen::Vector3f> points
	{
		{
			Eigen::Vector3f{ 1, -1, -1},
			Eigen::Vector3f{ 1, -1, 1 },
			Eigen::Vector3f{ -1, -1, 1 },
			Eigen::Vector3f{ -1, -1, -1 },
			Eigen::Vector3f{ 1, 1, -1 },
			Eigen::Vector3f{ 1, 1, 1 },
			Eigen::Vector3f{ -1, 1, 1 },
			Eigen::Vector3f{ -1, 1, -1 }
		}
	};
	std::vector<Eigen::Vector3f> normals;
	std::transform(points.begin(), points.end(), std::back_inserter(normals),
		[](Eigen::Vector3f& p) { return p.normalized(); }
	);
	std::vector<Eigen::Vector3f> colors
	{
		{
			Eigen::Vector3f{ 1, 0, 0},
			Eigen::Vector3f{ 1, 0, 1 },
			Eigen::Vector3f{ 0, 0, 1 },
			Eigen::Vector3f{ 0, 0, 0 },
			Eigen::Vector3f{ 1, 1, 0 },
			Eigen::Vector3f{ 1, 1, 1 },
			Eigen::Vector3f{ 0, 1, 1 },
			Eigen::Vector3f{ 0, 1, 0 }
		}
	};
	TestActor a(Shader::PhongPcd());
	a.Init(points, normals, colors);
	return a;
}