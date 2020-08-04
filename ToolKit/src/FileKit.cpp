#include "FileKit.h"


int FileKit::getFiles(std::string path, std::vector<std::string>& files, int category)
{
	//文件句柄
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;

	std::string p;

	if (category == 0){

		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1){

			do {
				//如果是目录,迭代
				//如果不是,加入列表
				if ((fileinfo.attrib  &  _A_SUBDIR)) {
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFiles(p.assign(path).append("\\").append(fileinfo.name), files, category);
				}
				else {
					if (strstr(fileinfo.name, "png") != NULL || strstr(fileinfo.name, "bmp") != NULL || \
						strstr(fileinfo.name, "jpg") != NULL || strstr(fileinfo.name, "tif") != NULL  \
						)
						files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}

		return 0;
	}
	else if (category == 1){

		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  {

			do {
				//如果是目录,迭代之
				//如果不是,加入列表
				if ((fileinfo.attrib  &  _A_SUBDIR)) {
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFiles(p.assign(path).append("\\").append(fileinfo.name), files, category);
				}
				else {
					if (
						strstr(fileinfo.name, "avi") != NULL || strstr(fileinfo.name, "mp4") != NULL || \
						strstr(fileinfo.name, "dav") != NULL)
						files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
		return 0;
	}
	else{
		std::cout << "input category param failed!" << std::endl;
		return -1;
	}
}

std::string FileKit::splitStr(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));

	return *v.rbegin();
}