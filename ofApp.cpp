#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);

	auto span = 20;
	for (int x = -300; x <= 300; x += span) {

		for (int y = -300; y <= 300; y += span) {

			for (int z = -300; z <= 300; z += span) {

				this->location_list.push_back(glm::vec3(x, y, z));
			}
		}
	}

	for (auto& location : this->location_list) {

		vector<int> next_index = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				next_index.push_back(index);
			}
		}

		this->next_index_list.push_back(next_index);
	}

	for (int i = 0; i < 2; i++) {

		this->noise_seed_list.push_back(glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000)));
		this->noise_location_list.push_back(glm::vec3());
	}
}


//--------------------------------------------------------------
void ofApp::update() {

	int frame_span = 15;
	int prev_index_size = 0;

	for (int i = 0; i < this->noise_seed_list.size(); i++) {

		this->noise_location_list[i] = glm::vec3(
			ofMap(ofNoise(this->noise_seed_list[i].x, ofGetFrameNum() * 0.008), 0, 1, -350, 350),
			ofMap(ofNoise(this->noise_seed_list[i].y, ofGetFrameNum() * 0.008), 0, 1, -350, 350),
			ofMap(ofNoise(this->noise_seed_list[i].z, ofGetFrameNum() * 0.008), 0, 1, -350, 350));

		for (int k = 0; k < this->location_list.size(); k++) {

			auto distance = glm::distance(this->noise_location_list[i], this->location_list[k]);
			if (distance < 10) {

				this->actor_list.push_back(make_unique<Actor>(k));
			}
		}
	}

	for (auto& actor : this->actor_list) {

		actor->update(frame_span, this->location_list, this->next_index_list);
	}

	for (int i = this->actor_list.size() - 1; i >= 0; i--) {

		if (this->actor_list[i]->isDead()) {

			this->actor_list.erase(this->actor_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotate(90);
	ofRotateY(180);
	ofRotateX(ofGetFrameNum() * 0.5);

	for (auto& actor : this->actor_list) {

		actor->draw();
	}

	for (auto& noise_location : this->noise_location_list) {

		ofDrawSphere(noise_location, 10);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}