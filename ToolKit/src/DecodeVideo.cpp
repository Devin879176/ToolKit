#include "DecodeVideo.h"


DecodeVideo::DecodeVideo(vector<string> videoNameVec){
	if (videoNameVec.size() <= 0)
	{
		cout << "no video path!" << endl;
		getchar();
		exit(0);
	}
	mVideoNames = videoNameVec;
	mVideoSize = mVideoNames.size();
}

int DecodeVideo::PopFrame(Mat* Frame){

	curCapture >> *Frame;
	if (Frame->empty() || Frame->cols <= 0 || Frame->rows <= 0)
	{
		if (mVideoIndex < mVideoSize)
		{
			curCapture.release();
			curCapture.open(mVideoNames[mVideoIndex]);

			if (!curCapture.isOpened()){
				cout << "����Ƶ�ļ�:" << mVideoNames[mVideoIndex] << " ʧ�ܣ�" << endl;
			}

			printf("��ǰ������Ƶ�ļ�=%s ��%d��\n", mVideoNames[mVideoIndex].c_str(), mVideoIndex);
			curCapture >> *Frame;
			mVideoIndex++;
		}
		else{
			return 0;
		}
	}
	return 1;
}