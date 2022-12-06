
#include "Application/Application"
#include "Application/Object.h"

// xyz or pwn
// 샘플용으로 만든 간단한 PCD 파일 임포트 함수에요. 꼭 이걸 쓸 필요는 없어요.
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
	auto data = ReadPcdFile("D:\\_샘플PCD\\Raw221103_ec20_ov1_vc1.75_wlop_in.pwn");

	// 윈도우 창을 생성한다. 
	// 여기서 opgl 초기화를 진행하기 때문에, Object 생성전에 만들어 주어야 한다.
	SunkueWindow win(1240, 720);

	// Object 생성하기전에, 셰이더 폴더의 위치를 알려준다.
	Shader::ShaderDir() = "Application/src/shader";

	// Object 를 생성한다.
	auto pcd = In3D::App::CreatePcdObject("pcd", data.first, data.second);
	auto pcd2 = In3D::App::CreatePcdObject("pcd2", data.first, data.second);

	// std::string 과 vector<double> 로 Feature 를 만든다.
	// 각각 이름과 정보이다.
	std::vector<double> s1; s1.reserve(data.first.size()); for (int i = 0; i < data.first.size(); i++)s1.push_back(i);
	In3D::App::Values   s2; s2.reserve(data.first.size()); for (int i = 0; i < data.first.size(); i++)s2.push_back(0.01 * double(i % 1000));
	pcd.AddFeature("sample1", s1);
	pcd.AddFeature("sample2", s2);

	// 지금 디스플레이 할 피쳐를 이름을 통해 선택한다.
	pcd.SetActivefeature("sample1");

	// 화면에 그릴 친구들은 여기 등록한다.  // Draw 함수가 매 프레임 불린다.
	win.renderer()->Regist(pcd.name(), pcd);
	win.renderer()->Regist(pcd2.name(), pcd2);
	pcd2.transform().translate({ 100,0,0 });

	// GUI 를 사용할 친구들은 여기 등록한다. // DrawGui 함수가 매 프레임 불린다.
	win.gui()->Regist(pcd);
	win.gui()->Regist(pcd2);
	win.gui()->Regist(win);
	win.gui()->Regist(win.renderer()->mainCamera());
	win.gui()->Regist(win.renderer()->sun());

	// 렌더러에 등록되어있는 오브젝트들을 모두 한 화면에 그려질 수 있도록 해준다. R 키를 누르면 이 함수가 불린다.
	win.renderer()->ResetCamera();

	// 업데이트 루프를 실행한다.
	win.run();
}


// 아래는 또 다른 예시. (여러개 등록 및 메쉬 띄우기)
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

	// transform 에 접근하여 이동, 스케일링, 회전 등을 할 수 있다. 
	// aabb바운드가 CreateObject 에 넘겨준 파라미터를 통해 자동으로 계산된다. 
	// (바운드는 미완성이다. transform 의 영향을 받지 않는다.)
	// auto m = item.transform().modelMatrix();
	// auto& min = m * item..bound().minimum();
	// auto& max = m * item..bound().maximum();
	// transform 이 적용된 바운드는 위와 같이 구해서 사용하면 된다.
	mesh.transform().translate({ mesh.bound().size().x() * 1.5f, 0, 0 });

	win.renderer()->ResetCamera();

	win.run();
	*/
}


