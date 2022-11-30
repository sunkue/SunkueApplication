
#include "Application/Application"
#include "Application/Object.h"

// xyz or pwn
// ���ÿ����� ���� ������ ���� ����Ʈ �Լ�����. �� �̰� �� �ʿ䰡 �����
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
	/*
	// 58 ��° �� ���� ���� �˴ϴ�.
	Helper::Directory::SetMainDir("D:\\");
	auto analyzer = anal3(Helper::Directory::AutoDir("_����PCD\\Raw221103_ec20_ov1_vc1.75_in.pwn"));
	In3D::Model::Kernel::In3DModelKernelAccessor::Up2dateData(analyzer.getModel(),
		In3D::Model::Kernel::eBaseSystem::OP3D, In3D::Model::Kernel::eDataSetType::PCD);
	auto& op3dPcdData = analyzer.getModel().getOp3DPcdData();

	auto mainProcessedModel =
		Process::In3DMainProcessPipeline{ analyzer.getModel() }
		.cgalPoisson(16, 0.15, 0, 500).elapsed("poisson::")
		.getModel();

	In3D::Model::Kernel::In3DModelKernelAccessor::Up2dateData(mainProcessedModel,
		In3D::Model::Kernel::eBaseSystem::OP3D, In3D::Model::Kernel::eDataSetType::MESH);
	auto& op3dMeshData = mainProcessedModel.getOp3DMeshData();

	*/

	auto data = ReadPcdFile("D:\\_����PCD\\Raw221103_ec20_ov1_vc1.75_in.pwn");

	// ������ â�� �����Ѵ�. 
// ���⼭ opgl �ʱ�ȭ�� �����ϱ� ������, Object �������� ����� �־�� �Ѵ�.
	SunkueWindow win(1240, 720);

	// Object �����ϱ�����, ���̴� ������ ��ġ�� �˷��ش�.
	Shader::ShaderDir() = "Application/src/shader";

	// Object �� �����Ѵ�.
	auto pcd = In3D::App::CreatePcdObject("pcd", data.first, data.second);

	
//	auto pcd = In3D::App::CreatePcdObject("pcd", op3dPcdData->points_, op3dPcdData->normals_);
//	auto mesh = In3D::App::CreateMeshObject("mesh", op3dMeshData->vertices_, op3dMeshData->vertex_normals_, op3dMeshData->triangles_);
	

	// std::string �� vector<double> �� Feature �� �����.
	// ���� �̸��� �����̴�.
//	for (const auto& f : analyzer.GetFeatureMap()) {
//		pcd.AddFeature(f.first, f.second);
//	}

	// ���� ���÷��� �� ���ĸ� �̸��� ���� �����Ѵ�.
	pcd.SetActivefeature("Distance");

	// ȭ�鿡 �׸� ģ������ ���� ����Ѵ�.  // Draw �Լ��� �� ������ �Ҹ���.
//	win.renderer()->Regist(mesh);
	win.renderer()->Regist(pcd);

	// GUI �� ����� ģ������ ���� ����Ѵ�. // DrawGui �Լ��� �� ������ �Ҹ���.
//	win.gui()->Regist(mesh);
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
//mesh.transform().translate({ mesh.bound().size().x() * 1.5f, 0, 0 });

	// �������� ��ϵǾ��ִ� ������Ʈ���� ��� �� ȭ�鿡 �׷��� �� �ֵ��� ���ش�. R Ű�� ������ �� �Լ��� �Ҹ���.
	win.renderer()->ResetCamera();

	// ������Ʈ ������ �����Ѵ�.
	win.run();
}


