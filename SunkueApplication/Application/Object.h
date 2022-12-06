#pragma once

#include "Application";

namespace In3D {
	namespace App {
		class Values : public std::vector<double> {
		public:
			Values() = default;
			Values(const std::vector<double>& src) :std::vector<double>{ src } { init(); }
			Values(std::vector<double>&& src) :std::vector<double>{ std::move(src) } { init(); }
		public:
			mutable double min{ -1 };
			mutable double max{ -1 };
			mutable double mean{ -1 };
			mutable double stdDev{ -1 };
			void init()const { setMinMax(); setStdDev(); }
		private:
			void setMinMax() const { min = *std::min_element(begin(), end()); max = *std::max_element(begin(), end()); }
			void setStdDev() const { auto res = StandardDeviation(*this); mean = res.first; stdDev = res.second; }
			// mean , stdDev
			static std::pair<double, double> StandardDeviation(const std::vector<double>& src) {
				double mean = std::accumulate(src.begin(), src.end(), 0.) / src.size();
				double devSum = std::inner_product(src.begin(), src.end(), src.begin(), 0.,
					[](double sum, double dev) {return sum + dev; },
					[mean](double a, double b) {return (a - mean) * (a - mean); }
				);
				double stdDev = std::sqrt(devSum / (src.size() - 1));
				return std::make_pair(mean, stdDev);
			}
		};

		namespace detail {
			using Features = std::map<std::string, Values>;
			struct PseudoColor {
				ImVec4 color;
				float value;
			};
			using vec3 = Eigen::Vector3d; // 어떤 형식이든지 시퀀셜하게 이어지는 double 3개 짜리 메모리.
			using index3 = Eigen::Vector3i; // 어떤 형식이든지 시퀀셜하게 이어지는 int 3개 짜리 메모리.
			class ObjectMaker;
		}

		using Feature = detail::Features::iterator;

		class Object : public DrawAble, public GuiObject
		{
			friend class detail::ObjectMaker;
		private:
			detail::Features features;
			std::array<detail::PseudoColor, 8> pseudoColor{}; // 최대 8개까지.
		private:
			SunkueMakeGetSet(std::string, name);
			struct Detail {
				Feature currentFeature{};
				GLfloat pointSize = 0.25;
				bool showNormalColor = false;
				bool pseudoColorLerp = false;
				int pseudoColorNum = 4; // [2, pseudoColor.size()]
			}detail;
		private:
			void init(const std::string& name) {
				pseudoColor[0].color = { 1,0,0,1 };
				pseudoColor[1].color = { 0,1,0,1 };
				pseudoColor[2].color = { 0,0,1,1 };
				pseudoColor[3].color = { 0.5,0.5,0.5,1 };
				pseudoColor[4].color = { 1,1,0,1 };
				pseudoColor[5].color = { 0,1,1,1 };
				pseudoColor[6].color = { 1,0,1,1 };
				pseudoColor[7].color = { 0,0,0,1 };
				pseudoColor[1].value = 0;
				pseudoColor[2].value = 1;
				pseudoColor[3].value = 2;
				_name = name;
			};
		protected:
			// pcd
			explicit Object(const std::string& name, const std::vector<detail::vec3>& points, const std::vector<detail::vec3>& normals) : DrawAble{ Shader::PhongScalarPcd() } {
				Init<double>(points, normals, std::vector<double>(points.size(), 1));
				init(name);
			}

			// triangleMesh
			explicit Object(const std::string& name, const std::vector<detail::vec3>& points, const std::vector<detail::vec3>& normals, const std::vector<detail::index3>& indices)
				: DrawAble{ Shader::PhongScalarMesh() } {
				std::vector<unsigned int> opglIndices; opglIndices.reserve(3 * indices.size());
				for (const auto& t : indices) {
					opglIndices.push_back(static_cast<unsigned int>(t.x()));
					opglIndices.push_back(static_cast<unsigned int>(t.y()));
					opglIndices.push_back(static_cast<unsigned int>(t.z()));
				}
				Init<double>(points, normals, std::vector<double>(points.size(), 1), opglIndices);
				init(name);
			}

		public:
			void AddFeature(const std::string& name, const Values& values) {
				AddFeature(name, Values(values));
			}

			void AddFeature(const std::string& name, const std::vector<double>& values) {
				AddFeature(name, Values(values));
			}

			void AddFeature(const std::string& name, Values&& values) {
				if (values.stdDev == -1) values.init();
				features[name] = std::move(values);
			}

			void AddFeature(const std::string& name, std::vector<double>&& values) {
				AddFeature(name, Values(std::move(values)));
			}

		protected:
			virtual void Draw()override {
				shader.Use();
				shader.Set("u_model_mat", transform().matrix());
				shader.Set("u_radius", detail.pointSize);
				shader.Set("u_shininess", shininess);
				for (int i = 0; i < detail.pseudoColorNum; i++) {
					shader.Set("u_pseudo_color[" + std::to_string(i) + "].color", pseudoColor[i].color);
					shader.Set("u_pseudo_color[" + std::to_string(i) + "].value", pseudoColor[i].value);
				}
				shader.Set("u_show_normal_color", detail.showNormalColor);
				shader.Set("u_pseudo_color_count", detail.pseudoColorNum);
				shader.Set("u_pseudo_color_lerp", detail.pseudoColorLerp);
				glBindVertexArray(vao);
				0 < indexNum
					? glDrawElements(GL_TRIANGLES, indexNum, GL_UNSIGNED_INT, nullptr)
					: glDrawArrays(GL_POINTS, 0, vertexNum);
				glBindVertexArray(0);
				glUseProgram(0);
			}
			virtual void DrawGui()override {
				if (ImGui::Begin(_name.c_str())) {
					DrawFeatureSelector();
					ImGui::Checkbox("ShowNormalAsColor", &detail.showNormalColor);
					TreeNode("Histogram", [&]() {
						DrawHistogram();
						});

					TabBar("ModelActor TabBar", [&]() {
						TabItem("scalar", [&]() {
							TreeNode("pseudo color", [&]() { DrawPseudoColor(); });
							});
						TabItem("etc.", [&]() {
							TreeNode("points", [&]() {
								ImGui::DragFloat("pointSize", &detail.pointSize, 0.01, 0, 2, "%.3f", 1);
								});
							TreeNode("light", [&]() {
								ImGui::DragFloat("shininess", &shininess, 0.1, 0, 200, "%.1f", 1);
								});
							});
						});

					ImGui::End();
				}
			}
		private:
			void DrawPseudoColor() {
				ImGui::SetNextItemWidth(100);
				ImGui::Checkbox("Lerp", &detail.pseudoColorLerp);
				ImGui::SetNextItemWidth(100);
				for (int i = 0; i < detail.pseudoColorNum; i++) {
					ImGui::SetNextItemWidth(100);
					ImGui::DragFloat(("V" + std::to_string(i)).c_str(), &pseudoColor[i].value, 0.001, -50000, 50000, "%.3f");
					if(detail.pseudoColorLerp)ImGui::SameLine();
					ImGui::ColorEdit4(("PC" + std::to_string(i)).c_str(), &pseudoColor[i].color.x);
				}
				ImGui::SetNextItemWidth(100);
				if (ImGui::Button("-")) {
					detail.pseudoColorNum = std::max(2, detail.pseudoColorNum - 1);
					SetPseudoColorThresholdByDefault();
				}
				ImGui::SameLine();
				if (ImGui::Button("+")) {
					detail.pseudoColorNum = std::min(8, detail.pseudoColorNum + 1);
					SetPseudoColorThresholdByDefault();
				}
				ImGui::SameLine();
				if (ImGui::Button("Reset")) { SetPseudoColorThresholdByDefault(); }
			}
			// pseudoColorLerp
			void DrawFeatureSelector() {
				if (!detail.currentFeature._Ptr) return;
				ImGui::SetNextItemWidth(200);
				if (ImGui::BeginCombo("Feature##combo", detail.currentFeature->first.c_str())) {
					for (const auto& f : features) {
						bool isSelected = (detail.currentFeature->first == f.first);
						if (ImGui::Selectable(f.first.c_str(), isSelected))
							SetActivefeature(f.first);
						if (isSelected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}
			void DrawHistogram() {
				if (!detail.currentFeature._Ptr) return;
				const auto& f = detail.currentFeature;
				static ImPlotHistogramFlags histFlags = ImPlotHistogramFlags_Density;
				static ImPlotRect rect(0, 1, 0, 1);
				static ImPlotBin_ bin = ImPlotBin_Sqrt;
				static bool magnifying = false;
				static bool pcTag = false;
				static bool meanTag = false;
				static bool sigmaTag = false;
				ImGui::SetNextItemWidth(100);
				ImGui::CheckboxFlags("Density", (unsigned int*)&histFlags, ImPlotHistogramFlags_Density);
				ImGui::SameLine();
				ImGui::CheckboxFlags("Cumulative", (unsigned int*)&histFlags, ImPlotHistogramFlags_Cumulative);
				ImGui::SetNextItemWidth(100);
				ImGui::Checkbox("pcTag", &pcTag);
				if (pcTag) {
					ImGui::SameLine();
					ImGui::Checkbox("mean", &meanTag);
					ImGui::SameLine();
					ImGui::Checkbox("sigma", &sigmaTag);
				}
				auto drawPcTagOnHistogram = [&]() {
					if (pcTag) {
						for (int i = 0; i < detail.pseudoColorNum; i++) {
							double value = static_cast<double>(pseudoColor[i].value);
							ImPlot::DragLineX(0, &value, pseudoColor[i].color, 0.1f, ImPlotDragToolFlags_NoFit | ImPlotDragToolFlags_NoCursors | ImPlotDragToolFlags_NoInputs);
							ImPlot::TagX(value, pseudoColor[i].color, "%.3f", sigmaTag ? (value - f->second.mean) / f->second.stdDev : value);
							pseudoColor[i].value = static_cast<float>(value);
						}
						if (meanTag) {
							ImPlot::DragLineX(0, &f->second.mean, ImVec4(1, 1, 1, 0.5), 2.f, ImPlotDragToolFlags_NoFit | ImPlotDragToolFlags_NoCursors | ImPlotDragToolFlags_NoInputs);
							ImPlot::TagX(f->second.mean, ImVec4(1, 1, 1, 0.5), "%.3f", sigmaTag ? 0 : f->second.mean);
						}
					}
				};
				ImGui::SetNextItemWidth(100);
				ImGui::Checkbox("magnifying", &magnifying);
				if (magnifying) {
					ImGui::SetNextItemWidth(200);
					static float speed = 0.01;
					static float rectSize[4];
					rectSize[0] = rect.X.Min; rectSize[1] = rect.X.Max;
					rectSize[2] = rect.Y.Min; rectSize[3] = rect.Y.Max;
					if (ImGui::DragFloat4("size", rectSize, speed)) {
						rect.X.Min = rectSize[0]; rect.X.Max = rectSize[1];
						rect.Y.Min = rectSize[2]; rect.Y.Max = rectSize[3];
					}
					ImGui::SameLine();
					ImGui::SetNextItemWidth(200);
					ImGui::DragFloat("speed", &speed, 0.01, 0, 1000);
				}
				if (ImPlot::BeginPlot((f->first + "##Histograms").c_str())) {
					ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside | ImPlotLegendFlags_Sort);
					ImPlot::SetupAxes("value", NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
					ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
					ImPlot::PlotHistogram(f->first.c_str(), f->second.data(), f->second.size(),
						bin, 1.0, ImPlotRange(f->second.min, f->second.max), histFlags);
					if (magnifying) ImPlot::DragRect(0, &rect.X.Min, &rect.Y.Min, &rect.X.Max, &rect.Y.Max, ImVec4(1, 0, 1, 1));
					drawPcTagOnHistogram();
					ImPlot::EndPlot();
				}
				if (magnifying) {
					if (ImPlot::BeginPlot("##rect", ImVec2(-1, 150), ImPlotFlags_CanvasOnly)) {
						ImPlot::SetupAxes("value", NULL, ImPlotAxisFlags_None, ImPlotAxisFlags_None);
						ImPlot::SetupAxesLimits(rect.X.Min, rect.X.Max, rect.Y.Min, rect.Y.Max, ImGuiCond_Always);
						ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
						ImPlot::PlotHistogram(f->first.c_str(), f->second.data(), f->second.size(),
							bin, 1.0, ImPlotRange(f->second.min, f->second.max), histFlags);
						drawPcTagOnHistogram();
						ImPlot::EndPlot();
					}
				}
			}
		private:
			void SetPseudoColorThresholdByDefault() {
				if (!detail.currentFeature._Ptr) return;
				const auto& scalar = detail.currentFeature->second;
				float minScalar = scalar.min;
				float maxScalar = scalar.max;
				float meanScalar = scalar.mean;
				float stdDev = scalar.stdDev;
				int half = detail.pseudoColorNum / 2;
				bool even = 1 - detail.pseudoColorNum % 2;
				for (int i = 0; i < detail.pseudoColorNum; i++) {
					bool under = i < half;
					float step = (under ? minScalar - meanScalar : maxScalar - meanScalar) / half;
					pseudoColor[i].value = meanScalar + (under ? (half - i) : (i - half + even)) * step;
				}
			}
		public:
			void SetActivefeature(const std::string& featureName) {
				detail.currentFeature = features.find(featureName);
				SetPseudoColorThresholdByDefault();
				const auto& scalar = detail.currentFeature->second;
				const auto sizeofVec3 = sizeof(detail::vec3);
				const auto sizeofdouble = sizeof(double);
				const auto attributeSize = scalar.size();
				const auto bufferSize3 = attributeSize * sizeofVec3;
				const auto bufferSize1 = attributeSize * sizeofdouble;
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferSubData(GL_ARRAY_BUFFER, bufferSize3 * 2, bufferSize1, scalar.data());
				glBindVertexArray(0);
			}
		};

		namespace detail {
			class ObjectMaker {
			public:
				static Object CreatePcdObject(const std::string& name, const std::vector<vec3>& points, const std::vector<vec3>& normals) {
					return Object::Object(name, points, normals);
				}
				static Object CreateMeshObject(const std::string& name, const std::vector<vec3>& points, const std::vector<vec3>& normals, const std::vector<index3>& indices) {
					return Object::Object(name, points, normals, indices);
				}
			};
		}

		inline Object CreatePcdObject(const std::string& name, const std::vector<detail::vec3>& points, const std::vector<detail::vec3>& normals) {
			return detail::ObjectMaker::CreatePcdObject(name, points, normals);
		}
		inline Object CreateMeshObject(const std::string& name, const std::vector<detail::vec3>& points, const std::vector<detail::vec3>& normals, const std::vector<detail::index3>& indices) {
			return detail::ObjectMaker::CreateMeshObject(name, points, normals, indices);
		}

	}
}