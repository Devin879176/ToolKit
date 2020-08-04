#ifndef FileKit_H 
#define FileKit_H

#include <io.h>
#include <vector>
#include <iostream>


class FileKit{

public:
	FileKit(){}
	~FileKit(){}
	//* function:��ȡָ��·���µ�ͼƬ������Ƶ�ļ�
	//* @param[path][����ȡͼƬ����Ƶ·��]
	//* @param[files][�洢��ȡ������Ƶ·��]
	//* @param[category][�ָ���]
	int getFiles(std::string path, std::vector<std::string>& files, int category=0);
	
	//* function:�ָ������ַ���
	//* @param[orgStr][���ָ��ַ���]
	//* @param[resList][�洢�ָ���]
	//* @param[Label][�ָ���]
	std::string splitStr(const std::string& orgStr, std::vector<std::string>& resList, const std::string& Label = "/");
};

#endif