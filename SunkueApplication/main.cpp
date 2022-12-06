
#include "Application/Application"
#include "Application/Object.h"

// xyz or pwn
// ���ÿ����� ���� ������ PCD ���� ����Ʈ �Լ�����. �� �̰� �� �ʿ�� �����.
std::pair<std::vector<Eigen::Vector3d>, std::vector<Eigen::Vector3d>> ReadPcdFile(const std::string& fileName) {
	auto dotPos = fileName.find_last_of(".");
	auto extStr = fileName.substr(dotPos + 1);
	std::ifstream file{ fileName };
	if (!file) std::cout << "[ERR] :: invalid file name\n";
	std::vector<Eigen::Vector3d> points;
	std::vector<Eigen::Vector3d> normals;
	while (!file.eof()) {
		if (extStr == "xyz") {
			Eigen::Vector3d p;
			file >> p.x() >> p.y() >> p.z();
			points.push_back(p);
		}
		if (extStr == "pwn") {
			Eigen::Vector3d p;
			file >> p.x() >> p.y() >> p.z();
			points.push_back(p);
			Eigen::Vector3d n;
			file >> n.x() >> n.y() >> n.z();
			normals.push_back(n);
		}
		file.peek();
	}
	return make_pair(points, normals);
}

int main()
{
	auto data = ReadPcdFile("D:\\_����PCD\\Raw221103_ec20_ov1_vc1.75_wlop_in.pwn");

	// ������ â�� �����Ѵ�. 
	// ���⼭ opgl �ʱ�ȭ�� �����ϱ� ������, Object �������� ����� �־�� �Ѵ�.
	SunkueWindow win(1240, 720);

	// Object �����ϱ�����, ���̴� ������ ��ġ�� �˷��ش�.
	Shader::ShaderDir() = "Application/src/shader";

	// Object �� �����Ѵ�.
	auto pcd = In3D::App::CreatePcdObject("pcd", data.first, data.second);
	auto pcd2 = In3D::App::CreatePcdObject("pcd2", data.first, data.second);

	// std::string �� vector<double> �� Feature �� �����.
	// ���� �̸��� �����̴�.
	std::vector<double> s1; s1.reserve(data.first.size()); for (int i = 0; i < data.first.size(); i++)s1.push_back(i);
	In3D::App::Values   s2; s2.reserve(data.first.size()); for (int i = 0; i < data.first.size(); i++)s2.push_back(0.01 * double(i % 1000));
	pcd.AddFeature("sample1", s1);
	pcd.AddFeature("sample2", s2);

	// ���� ���÷��� �� ���ĸ� �̸��� ���� �����Ѵ�.
	pcd.SetActivefeature("sample1");

	// ȭ�鿡 �׸� ģ������ ���� ����Ѵ�.  // Draw �Լ��� �� ������ �Ҹ���.
	win.renderer()->Regist(pcd.name(), pcd);
	win.renderer()->Regist(pcd2.name(), pcd2);
	pcd2.transform().translate({ 100,0,0 });

	// GUI �� ����� ģ������ ���� ����Ѵ�. // DrawGui �Լ��� �� ������ �Ҹ���.
	win.gui()->Regist(pcd);
	win.gui()->Regist(pcd2);
	win.gui()->Regist(win);
	win.gui()->Regist(win.renderer()->mainCamera());
	win.gui()->Regist(win.renderer()->sun());

	// �������� ��ϵǾ��ִ� ������Ʈ���� ��� �� ȭ�鿡 �׷��� �� �ֵ��� ���ش�. R Ű�� ������ �� �Լ��� �Ҹ���.
	win.renderer()->ResetCamera();

	// ������Ʈ ������ �����Ѵ�.
	win.run();
}


// �Ʒ��� �� �ٸ� ����. (������ ��� �� �޽� ����)
int main2()
{
	/*
	SunkueWindow win(1240, 720);
	Shader::ShaderDir() = "Application/src/shader";

	auto pcd = In3D::App::CreatePcdObject("pcd", op3dPcdData->points_, op3dPcdData->normals_);
	auto mesh = In3D::App::CreateMeshObject("mesh", op3dMeshData->vertices_, op3dMeshData->vertex_normals_, op3dMeshData->triangles_);

	for (const auto& f : analyzer.GetFeatureMap()) {
		pcd.AddFeature(f.first, f.second);
	}
	pcd.SetActivefeature("Distance");

	win.renderer()->Regist(mesh);
	win.renderer()->Regist(pcd);

	win.gui()->Regist(mesh);
	win.gui()->Regist(pcd);
	win.gui()->Regist(win.renderer()->mainCamera());
	win.gui()->Regist(win.renderer()->sun());

	// transform �� �����Ͽ� �̵�, �����ϸ�, ȸ�� ���� �� �� �ִ�. 
	// aabb�ٿ�尡 CreateObject �� �Ѱ��� �Ķ���͸� ���� �ڵ����� ���ȴ�. 
	// (�ٿ��� �̿ϼ��̴�. transform �� ������ ���� �ʴ´�.)
	// auto m = item.transform().modelMatrix();
	// auto& min = m * item..bound().minimum();
	// auto& max = m * item..bound().maximum();
	// transform �� ����� �ٿ��� ���� ���� ���ؼ� ����ϸ� �ȴ�.
	mesh.transform().translate({ mesh.bound().size().x() * 1.5f, 0, 0 });

	win.renderer()->ResetCamera();

	win.run();
	*/
}


