#include "testApp.h"

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
	
	baseRadius = 100.0f;
	maxRadius = 300.0f;
	
	for (int i = 0; i < 6; i++)
	{
		dists.push_back(ofRandom(4000));
	}
	
	serial.setup("/dev/cu.usbmodem1411", 9600);
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
	float deg = 270.0f;
	
	ofNoFill();
	
	// ベースライン
	ofSetColor(0, 0, 255);
	ofCircle(circleCenterX, circleCenterY, baseRadius);
	// マックスライン
	ofSetColor(255, 0, 0);
	ofCircle(circleCenterX, circleCenterY, maxRadius);
	
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

ofPoint testApp::pointInScreen(int dist, int sensorID)
{
	const float deg = 270.0f + (15.0f * sensorID);
	const float rad = ofDegToRad(deg);
	
	dist = dist / 20;
	
	ofPoint pos;
	pos.x = cos(rad) * (baseRadius + dist) + circleCenterX;
	pos.y = sin(rad) * (baseRadius + dist) + circleCenterY;
	
	return pos;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	for (int i = 0; i < 6; i++)
	{
		dists[i] = ofRandom(4000);
	}
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