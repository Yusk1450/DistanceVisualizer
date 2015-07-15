
#include "testApp.h"
#include <math.h>
#include "ofxVectorMath.h"

vector<string> split(const string &str, const string &delim)
{
	vector<string> res;
	
	size_t current = 0;
	size_t found;
	size_t delimlen = delim.size();
	
	while ((found = str.find(delim, current)) != string::npos)
	{
		res.push_back(string(str, current, found - current));
		current = found + delimlen;
	}
	res.push_back(string(str, current, str.size() - current));
	return res;
}

int countStr(const string &str, const string &searchStr)
{
	size_t current = 0;
	size_t found = 0;
	size_t searchStrLen = searchStr.size();
	
	int count = 0;
	while ((found = str.find(searchStr, current)) != string::npos)
	{
		count++;
		current = found + searchStrLen;
	}
	
	return count;
}

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetWindowShape(840, 740);
	ofBackground(255, 255, 255);
	ofSetCircleResolution(1000);
	
	baseRadius = 100.0f;			// ベースラインの半径
	maxRadius = 300.0f;				// 最大距離時の半径
	topSensorDeg = 90.0f;			// センサ0番目の角度
	sensorIntervalDeg = 15.0f;		// センサ角度
	
	for (int i = 0; i < 6; i++)
	{
		dists.push_back(0.0f);
//		dists.push_back(ofRandom(4000.0f));
	}
	
	string port = ofBufferFromFile("port.txt");
	serial.setup(port, 9600);
}

//--------------------------------------------------------------
void testApp::update()
{
	circleCenterX = ofGetWindowWidth() / 2;
	circleCenterY = 400;
	
	vector<double> csv = readSerialCSV();
	for (int i = 0; i < 6; i++)
	{
		if (csv[i] != 0)
		{
			dists[i] = csv[i];
		}
	}
	pitch = csv[6] * 180.0f;
	roll = csv[7];
	yaw = csv[8];
}

vector<double> testApp::readSerialCSV()
{
	vector<double> re;
	for (int i = 0; i < 9; i++)
	{
		re.push_back(0.0);
	}
	
	unsigned char bytesReturned[64];
	memset(bytesReturned, 0, 64);
	
	int read = 0;
	while (true)
	{
		int r = serial.readBytes(bytesReturned, 64);
		if (r < 0)
		{
			break;
		}
		read = r;
	}
	
	if (read > 0)
	{
		string str = ofToString(bytesReturned);
		
		if (str.find("\n", 0) != string::npos)
		{
			vector<string> strs = split(str, "\n");
			string result = bufSerialStr + strs[0];
			bufSerialStr = "";
			if (strs.size() > 1)
			{
				bufSerialStr = strs[1];
			}
			
			if (countStr(result, ",") == 8)
			{
				vector<string> valStrs = split(result, ",");
				for (int i = 0; i < valStrs.size(); i++)
				{
					stringstream ss(valStrs[i]);
					ss >> re[i];
				}
			}
		}
		else
		{
			bufSerialStr += str;
		}
	}
	
	return re;
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofNoFill();
	
	// ベースライン
	ofSetColor(0, 0, 255);
	ofCircle(circleCenterX, circleCenterY, baseRadius);
	// マックスライン
	ofSetColor(255, 0, 0);
	ofCircle(circleCenterX, circleCenterY, maxRadius);
	
	// 座標変換
//	for (int i = 0; i < 6; i++)
//	{
//		if (dists[i] != 0)
//		{
//			const float deg = topSensorDeg + (sensorIntervalDeg * i);
//			const float rad = ofDegToRad(deg);
//
//			float srcX = cos(rad) * dists[i];
//			float srcY = sin(rad) * dists[i];
//			float srcZ = 0.0f;
//			
//			// ピッチの値180度？
//			ofxVec3f vec = transformAffine3DX(ofVec3f(srcX, srcY, srcZ), pitch);
//			dists[i] = vec.y;
//			
//		}
//	}
//	cout << "---------------------------" << endl;
	
	for (int i = 0; i < 6; i++)
	{
		ofPoint basePos = pointInScreen(0, i);
		ofSetColor(0, 0, 255);
		ofCircle(basePos.x, basePos.y, 4);
		
		if (dists[i] != 0)
		{
			ofPoint pos = pointInScreen(dists[i], i);
			ofSetColor(255, 0, 0);
			ofCircle(pos.x, pos.y, 4);

			ofSetColor(0, 255, 0);
			ofLine(basePos.x, basePos.y, pos.x, pos.y);
			
			ofSetColor(0, 0, 0);
			ofDrawBitmapString(ofToString(dists[i]), pos.x - 10, pos.y - 10);
		}
	}

	ofDrawBitmapString("DistVisualizer Beta", 0, 10);
}

ofxVec3f testApp::transformAffine3DX(ofVec3f v, float deg)
{
	ofxVec3f res;
	const float rad = ofDegToRad(deg);
	
	res.x = v.x;
	res.y = (v.y * cos(rad)) + (v.y * -sin(rad));
	res.z = (v.z * sin(rad)) + (v.z * cos(rad));
	
//	cout << v.y << "|" << cos(rad) << "|" << -sin(rad) << "| res=" << res.y << endl;
	
	return res;
}

ofPoint testApp::pointInScreen(int dist, int sensorID)
{
	const float deg = topSensorDeg + (sensorIntervalDeg * sensorID);
	const float rad = ofDegToRad(deg);
	
	dist = dist / 20;
	
	ofPoint pos;
	pos.x = cos(rad) * -1 * (baseRadius + dist) + circleCenterX;
	pos.y = sin(rad) * -1 * (baseRadius + dist) + circleCenterY;
	
	return pos;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}