// amplifies the left and right channels separately, and writes them
// to sound files.

#include "MarSystemManager.h"
using namespace std;
using namespace Marsyas;

void sfplay(string sfName, float gain1, float gain2) {
	MarSystemManager mng;

// start at 44100
	MarSystem* playbacknet = mng.create("Series", "playbacknet");
	playbacknet->addMarSystem(mng.create("SoundFileSource", "src"));
	playbacknet->updctrl("SoundFileSource/src/mrs_string/filename", sfName);

	playbacknet->addMarSystem(mng.create("SoundFileSink", "snk"));
// srate 44100
	playbacknet->updctrl("SoundFileSink/snk/mrs_string/filename", "both.wav");

	MarSystem* left = mng.create("Series", "left");
	left->addMarSystem(mng.create("Gain", "gain1"));
	left->updctrl("Gain/gain1/mrs_real/gain", gain1);
	left->addMarSystem(mng.create("SoundFileSink", "destL"));
// srate 22050
	left->updctrl("SoundFileSink/destL/mrs_string/filename", "left.wav");

	MarSystem* right = mng.create("Series", "right");
	right->addMarSystem(mng.create("Gain", "gain2"));
	right->updctrl("Gain/gain2/mrs_real/gain", gain2);
	right->addMarSystem(mng.create("SoundFileSink", "destR"));
// srate 22050
	right->updctrl("SoundFileSink/destR/mrs_string/filename", "right.wav");

	MarSystem* para = mng.create("Parallel", "para");
	para->addMarSystem(left);
	para->addMarSystem(right);
	playbacknet->addMarSystem(para);

	while ( playbacknet->getctrl("SoundFileSource/src/mrs_bool/notEmpty")->toBool() ) {
		playbacknet->tick();
	}
	delete playbacknet;
}

int main(int argc, const char **argv) {
	string fileName;
	float gain1, gain2;
	if (argc<2) { cout<<"Please enter filename."<<endl; exit(1); } else 
		{ fileName = argv[1]; }
	if (argc<3) { gain1 = 1; } else
		{ gain1 = atof(argv[2]); }
	if (argc<4) { gain2 = 1; } else
		{ gain2 = atof(argv[3]); }
	cout << "Splitting stereo file " << fileName <<
		" into separate files at volumes " <<gain1 << " and "<<gain2<<endl;

	sfplay(fileName,gain1,gain2);
	exit(0);
}

