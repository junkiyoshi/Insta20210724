#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->font_size = 80;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);

	this->noise_param_list.push_back(ofRandom(1000));
}


//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 60 < 45) {

		auto noise_param = this->noise_param_list.back() + ofMap(ofGetFrameNum() % 60, 0, 45, 0.02, 0.001);
		this->noise_param_list.push_back(noise_param);
	}
	else {

		auto noise_param = this->noise_param_list.back();
		this->noise_param_list.push_back(noise_param);
	}

	while (this->noise_param_list.size() > 10) {

		this->noise_param_list.erase(this->noise_param_list.begin());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	string word = "A";
	int sample_count = 100;
	int span = 200;
	vector<glm::vec3> base_location = {

		glm::vec3(-100, -100, 0), glm::vec3(0, -100, 0), glm::vec3(100, -100, 0),
		glm::vec3(-100, 0, 0), glm::vec3(0, 0, 0), glm::vec3(100, 0, 0),
		glm::vec3(-100, 100, 0), glm::vec3(0, 100, 0), glm::vec3(100, 100, 0),
	};

	for (int k = 0; k < base_location.size(); k++) {

		for (int radius = 230; radius <= 230; radius += 100) {

			auto noise_param = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
			for (int i = 0; i < word.size(); i++) {

				ofPath chara_path = this->font.getCharacterAsPoints(word[i], true, false);
				vector<ofPolyline> outline = chara_path.getOutline();

				ofFill();
				ofSetColor(239);
				ofBeginShape();
				for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

					if (outline_index != 0) { ofNextContour(true); }

					auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
					for (auto& vertex : vertices) {

						auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 0);

						auto angle_x = ofMap(ofNoise(noise_param.x, location.x * 0.001 + this->noise_param_list.back()), 0, 1, -PI * 2, PI * 2);
						auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));

						location = glm::vec4(location, 0) * rotation_x;
						location += base_location[k];

						ofVertex(location);
					}
				}
				ofEndShape();

				for (int m = 0; m < this->noise_param_list.size(); m++) {

					ofNoFill();
					ofSetColor(39);
					ofBeginShape();
					for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

						if (outline_index != 0) { ofNextContour(true); }

						auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
						for (auto& vertex : vertices) {

							auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 0);

							auto angle_x = ofMap(ofNoise(noise_param.x, location.x * 0.001 + this->noise_param_list[m]), 0, 1, -PI * 2, PI * 2);
							auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));

							location = glm::vec4(location, 0) * rotation_x;
							location += base_location[k];

							ofVertex(location);
						}
					}
					ofEndShape(true);
				}
			}
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}