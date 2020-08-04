//#include <io.h>
//#include <memory>
//#include <iostream>
//#include <fstream> 
//#include <math.h>
//#include <windows.h>
//#include <direct.h>
//#include <time.h>
//#include <tchar.h>
//#include "cv.hpp"
//
//
///**
//* ��ȡָ��·���µ�ͼƬ������Ƶ�ļ�
//* @param[path][�����·��]
//* @param[files][�洢ָ��Ŀ¼��item]
//* @param[category][Ҫ��ȡ�ĵ�item���ͣ�0��ʾͼƬ��1��ʾ��Ƶ]
//* @return[�ɹ�����0��ʧ�ܷ���-1]
//*/
//int GetItems(std::string path, std::vector<std::string>& files, int category)
//{
//	//�ļ����
//	intptr_t   hFile = 0;
//	//�ļ���Ϣ
//	struct _finddata_t fileinfo;
//
//	std::string p;
//
//	if (category == 0){
//
//		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  {
//
//			do {
//				//�����Ŀ¼,����֮
//				//�������,�����б�
//				if ((fileinfo.attrib  &  _A_SUBDIR)) {
//					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
//						GetItems(p.assign(path).append("\\").append(fileinfo.name), files, category);
//				}
//				else {
//					if (strstr(fileinfo.name, "png") != NULL || strstr(fileinfo.name, "bmp") != NULL || \
//						strstr(fileinfo.name, "jpg") != NULL || strstr(fileinfo.name, "tif") != NULL  \
//						)
//						files.push_back(p.assign(path).append("\\").append(fileinfo.name));
//				}
//			} while (_findnext(hFile, &fileinfo) == 0);
//			_findclose(hFile);
//		}
//
//		return 0;
//	}
//	else if (category == 1){
//
//		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  {
//
//			do {
//				//�����Ŀ¼,����֮
//				//�������,�����б�
//				if ((fileinfo.attrib  &  _A_SUBDIR)) {
//					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
//						GetItems(p.assign(path).append("\\").append(fileinfo.name), files, category);
//				}
//				else {
//					if (
//						strstr(fileinfo.name, "avi") != NULL || strstr(fileinfo.name, "mp4") != NULL || \
//						strstr(fileinfo.name, "dav") != NULL)
//						files.push_back(p.assign(path).append("\\").append(fileinfo.name));
//				}
//			} while (_findnext(hFile, &fileinfo) == 0);
//			_findclose(hFile);
//		}
//
//		return 0;
//	}
//	else{
//		std::cout << "input category param failed!" << std::endl;
//
//		return -1;
//	}
//}
//
//
//
//std::string SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
//{
//	std::string::size_type pos1, pos2;
//	pos2 = s.find(c);
//	pos1 = 0;
//	while (std::string::npos != pos2)
//	{
//		v.push_back(s.substr(pos1, pos2 - pos1));
//
//		pos1 = pos2 + c.size();
//		pos2 = s.find(c, pos1);
//	}
//	if (pos1 != s.length())
//		v.push_back(s.substr(pos1));
//
//	return *v.rbegin();
//}
//
//
////angle�����������أ���������֮���ҵ��Ƕȵ�����ֵ��
//double angle(CvPoint* pt1, CvPoint* pt2, CvPoint* pt0)
//{
//	double dx1 = pt1->x - pt0->x;
//	double dy1 = pt1->y - pt0->y;
//	double dx2 = pt2->x - pt0->x;
//	double dy2 = pt2->y - pt0->y;
//	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
//}
//
//// ����ͼ�����ҵ��������������У��������д洢���ڴ�洢����
//
//CvSeq* findSquares4(IplImage* img, CvMemStorage* storage, int thresh)
//{
//	CvSeq* contours;
//	int i, c, l, N = 11;
//	CvSize sz = cvSize(img->width & -2, img->height & -2);
//
//	IplImage* timg = cvCloneImage(img);
//	IplImage* gray = cvCreateImage(sz, 8, 1);
//	IplImage* pyr = cvCreateImage(cvSize(sz.width / 2, sz.height / 2), 8, 3);
//	IplImage* tgray;
//	CvSeq* result;
//	double s, t;
//	// ����һ�����������ڴ洢�����ǵ�
//	CvSeq* squares = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
//
//	cvSetImageROI(timg, cvRect(0, 0, sz.width, sz.height));
//	// ��������
//	cvPyrDown(timg, pyr, 7);
//	cvPyrUp(pyr, timg, 7);
//	tgray = cvCreateImage(sz, 8, 1);
//
//	// ������3ɫ�ֱ�����ȡ
//	for (c = 0; c < 3; c++)
//	{
//		// ��ȡ the c-th color plane
//		cvSetImageCOI(timg, c + 1);
//		cvCopy(timg, tgray, 0);
//
//		// ���Ը�����ֵ��ȡ�õ��ģ�N=11��
//		for (l = 0; l < N; l++)
//		{
//			// apply Canny. Take the upper threshold from slider
//			// Canny helps to catch squares with gradient shading
//			if (l == 0)
//			{
//				cvCanny(tgray, gray, 0, thresh, 5);
//				//ʹ������ṹԪ������ͼ��
//				cvDilate(gray, gray, 0, 1);
//			}
//			else
//			{
//				// apply threshold if l!=0:
//				cvThreshold(tgray, gray, (l + 1) * 255 / N, 255, CV_THRESH_BINARY);
//			}
//
//			// �ҵ������������Ҵ洢��������
//			cvFindContours(gray, storage, &contours, sizeof(CvContour),
//				CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
//
//			// �����ҵ���ÿ������contours
//			while (contours)
//			{
//				//��ָ�����ȱƽ����������
//				result = cvApproxPoly(contours, sizeof(CvContour), storage,
//					CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
//
//
//				if (result->total == 4 &&
//					fabs(cvContourArea(result, CV_WHOLE_SEQ)) > 500 &&
//					fabs(cvContourArea(result, CV_WHOLE_SEQ)) < 100000 &&
//					cvCheckContourConvexity(result))
//				{
//					s = 0;
//
//					for (i = 0; i < 5; i++)
//					{
//						// find minimum angle between joint edges (maximum of cosine)
//						if (i >= 2)
//						{
//							t = fabs(angle(
//								(CvPoint*)cvGetSeqElem(result, i),
//								(CvPoint*)cvGetSeqElem(result, i - 2),
//								(CvPoint*)cvGetSeqElem(result, i - 1)));
//							s = s > t ? s : t;
//						}
//					}
//
//					// if ����ֵ �㹻С�������϶��Ƕ�Ϊ90��ֱ��
//					//cos0.1=83�ȣ��ܽϺõ�����ֱ��
//					if (s < 0.1)
//						for (i = 0; i < 4; i++)
//							cvSeqPush(squares,
//							(CvPoint*)cvGetSeqElem(result, i));
//				}
//
//				// ����������һ������
//				contours = contours->h_next;
//			}
//		}
//	}
//	cvReleaseImage(&gray);
//	cvReleaseImage(&pyr);
//	cvReleaseImage(&tgray);
//	cvReleaseImage(&timg);
//
//	return squares;
//}
//
//
////drawSquares��������������ͼ�����ҵ�����������������
//void drawSquares(IplImage* img, CvSeq* squares, const char * wndname)
//{
//	CvSeqReader reader;
//	IplImage* cpy = cvCloneImage(img);
//	int i;
//	cvStartReadSeq(squares, &reader, 0);
//
//	// read 4 sequence elements at a time (all vertices of a square)
//	for (i = 0; i < squares->total; i += 4)
//	{
//		CvPoint pt[4], *rect = pt;
//		int count = 4;
//
//		// read 4 vertices
//		CV_READ_SEQ_ELEM(pt[0], reader);
//		CV_READ_SEQ_ELEM(pt[1], reader);
//		CV_READ_SEQ_ELEM(pt[2], reader);
//		CV_READ_SEQ_ELEM(pt[3], reader);
//
//		// draw the square as a closed polyline
//		cvPolyLine(cpy, &rect, &count, 1, 1, CV_RGB(0, 255, 0), 2, CV_AA, 0);
//	}
//
//	cvShowImage(wndname, cpy);
//	cvReleaseImage(&cpy);
//}
//
//int main___()
//{
//	int thresh = 50;
//	IplImage* img = NULL;
//	IplImage* img0 = NULL;
//	CvMemStorage* storage = NULL;
//	const char * wndname = "�����μ�� demo";
//
//	std::string imageDir = "F:\\Project\\TrafficData\\pic\\shelter\\result";
//	std::vector<std::string> names;
//	int type = 0;
//	int res = GetItems(imageDir, names, type);
//
//
//	int i, c;
//	storage = cvCreateMemStorage(0);
//	for (i = 0; i < names.size(); i++)
//	{
//		img0 = cvLoadImage(names[i].c_str(), 1);
//		if (!img0)
//		{
//			std::cout << "��������" << names[i] << "������һ��ͼƬ" << std::endl;
//			continue;
//		}
//		img = cvCloneImage(img0);
//		cvNamedWindow(wndname, 1);
//
//		// find and draw the squares
//		drawSquares(img, findSquares4(img, storage, thresh), wndname);
//
//		c = cvWaitKey(0);
//
//		cvReleaseImage(&img);
//		cvReleaseImage(&img0);
//
//		cvClearMemStorage(storage);
//
//		if ((char)c == 27)
//			break;
//	}
//
//	cvDestroyWindow(wndname);
//	return 0;
//
//
//}
