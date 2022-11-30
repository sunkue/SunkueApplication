
#include "Application/Application"
#include "Application/Object.h"

// xyz or pwn
// 샘플용으로 만든 간단한 파일 임포트 함수에요. 꼭 이걸 쓸 필요가 없어요
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
	// 58 번째 줄 부터 보면 됩니다.
	Helper::Directory::SetMainDir("D:\\");
	auto analyzer = anal3(Helper::Directory::AutoDir("_샘플PCD\\Raw221103_ec20_ov1_vc1.75_in.pwn"));
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

	auto data = ReadPcdFile("D:\\_샘플PCD\\Raw221103_ec20_ov1_vc1.75_in.pwn");

	// 윈도우 창을 생성한다. 
// 여기서 opgl 초기화를 진행하기 때문에, Object 생성전에 만들어 주어야 한다.
	SunkueWindow win(1240, 720);

	// Object 생성하기전에, 셰이더 폴더의 위치를 알려준다.
	Shader::ShaderDir() = "Application/src/shader";

	// Object 를 생성한다.
	auto pcd = In3D::App::CreatePcdObject("pcd", data.first, data.second);

	
//	auto pcd = In3D::App::CreatePcdObject("pcd", op3dPcdData->points_, op3dPcdData->normals_);
//	auto mesh = In3D::App::CreateMeshObject("mesh", op3dMeshData->vertices_, op3dMeshData->vertex_normals_, op3dMeshData->triangles_);
	

	// std::string 과 vector<double> 로 Feature 를 만든다.
	// 각각 이름과 정보이다.
//	for (const auto& f : analyzer.GetFeatureMap()) {
//		pcd.AddFeature(f.first, f.second);
//	}

	// 지금 디스플레이 할 피쳐를 이름을 통해 선택한다.
	pcd.SetActivefeature("Distance");

	// 화면에 그릴 친구들은 여기 등록한다.  // Draw 함수가 매 프레임 불린다.
//	win.renderer()->Regist(mesh);
	win.renderer()->Regist(pcd);

	// GUI 를 사용할 친구들은 여기 등록한다. // DrawGui 함수가 매 프레임 불린다.
//	win.gui()->Regist(mesh);
	win.gui()->Regist(pcd);
	win.gui()->Regist(win.renderer()->mainCamera());
	win.gui()->Regist(win.renderer()->sun());

	// transform 에 접근하여 이동, 스케일링, 회전 등을 할 수 있다. 
	// aabb바운드가 CreateObject 에 넘겨준 파라미터를 통해 자동으로 계산된다. 
	// (바운드는 미완성이다. transform 의 영향을 받지 않는다.)
	// auto m = item.transform().modelMatrix();
	// auto& min = m * item..bound().minimum();
	// auto& max = m * item..bound().maximum();
	// transform 이 적용된 바운드는 위와 같이 구해서 사용하면 된다.
//mesh.transform().translate({ mesh.bound().size().x() * 1.5f, 0, 0 });

	// 렌더러에 등록되어있는 오브젝트들을 모두 한 화면에 그려질 수 있도록 해준다. R 키를 누르면 이 함수가 불린다.
	win.renderer()->ResetCamera();

	// 업데이트 루프를 실행한다.
	win.run();
}


