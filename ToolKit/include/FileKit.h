#ifndef FileKit_H 
#define FileKit_H

#include <io.h>
#include <vector>
#include <iostream>


class FileKit{

public:
	FileKit(){}
	~FileKit(){}
	//* function:获取指定路径下的图片或者视频文件
	//* @param[path][待获取图片或视频路径]
	//* @param[files][存储获取到的视频路径]
	//* @param[category][分割标记]
	int getFiles(std::string path, std::vector<std::string>& files, int category=0);
	
	//* function:分割输入字符串
	//* @param[orgStr][待分割字符串]
	//* @param[resList][存储分割结果]
	//* @param[Label][分割标记]
	std::string splitStr(const std::string& orgStr, std::vector<std::string>& resList, const std::string& Label = "/");
};

#endif