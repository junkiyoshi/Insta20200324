#include "Actor.h"

//--------------------------------------------------------------
Actor::Actor(vector<glm::vec3>& location_list) {

	this->select_index = ofRandom(location_list.size());
	this->next_index = this->select_index;
	this->life = 300;
}

//--------------------------------------------------------------
Actor::Actor(int select_index) {

	this->select_index = select_index;
	this->next_index = this->select_index;
	this->life = 300;
}

//--------------------------------------------------------------
void Actor::update(int& frame_span, vector<glm::vec3>& location_list, vector<vector<int>>& next_index_list) {

	if (ofGetFrameNum() % frame_span == 0) {

		this->select_index = this->next_index;
		this->next_index = next_index_list[this->select_index][(int)ofRandom(next_index_list[this->select_index].size())];
	}

	auto param = ofGetFrameNum() % frame_span;
	auto distance = location_list[this->next_index] - location_list[this->select_index];
	this->location = location_list[this->select_index] + distance / frame_span * param;

	this->log.push_front(this->location);
	while (this->log.size() > 100) { this->log.pop_back(); }

	this->life--;
}

//--------------------------------------------------------------
void Actor::draw() {

	this->life < 100 ? ofSetColor(ofMap(this->life, 0, 100, 239, 39)) : ofSetColor(39);

	ofFill();
	ofDrawSphere(this->log.front(), 3);

	ofNoFill();
	ofBeginShape();
	for (auto& l : this->log) {

		ofVertex(l);
	}
	ofEndShape();
}

//--------------------------------------------------------------
bool Actor::isDead() {

	return this->life < 0;
}