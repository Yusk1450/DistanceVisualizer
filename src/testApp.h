#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp
{
private:
	int circleCenterX;
	int circleCenterY;
	float baseRadius;
	float maxRadius;

	float topSensorDeg;
	float sensorIntervalDeg;
	
	ofSerial serial;
	string bufSerialStr;
	vector<int> dists;
	float pitch;
	float roll;
	float yaw;
	
private:
	ofVec3f transformAffine3DX(ofVec3f v, float deg);
	ofPoint pointInScreen(int dist, int sensorID);
	
public:
	vector<double> readSerialCSV();
	
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
};
