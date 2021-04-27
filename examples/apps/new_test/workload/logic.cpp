/* Copyright 2020 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <string>
#include "logic.h"
#include "io_helper.h"

#define USER_FILES_PATH "/tmp/tutorial"
// Add actual workload implementation here ..
std::string ProcessUseData(std::string in_str) {
	std::string func;
	std::string data;
	std::string out_str;
	int switch_num;
	std::size_t pos = in_str.find(":");
	if(pos == std::string::npos) {
		out_str = "your in_str is: " + in_str + ", it has no dataplace";
		return out_str;}
	else {
		func = in_str.substr(0,pos);
		data = in_str.substr(pos+1,in_str.length()-1-pos);}
	switch_num = std::stoul(func);
	switch(switch_num)
	{
		case 0:
			out_str = plus(data);
			break;
		case 1:
			out_str = write_file(data);
			break;
		case 2:
			out_str = build_file(data);
			break;
	}
	return out_str;
}

std::string plus(std::string in_str){
	std::string out_str;
	std::string file_path;
        std::string file_name;
        std::string key;
	std::string filedata;
        std::size_t pos = in_str.find("/");
        if(pos == std::string::npos) {
                out_str = "cant find filename or data when you try to plus";
                return out_str;}
        else {
                file_name = in_str.substr(0,pos);
		key = in_str.substr(pos+1,in_str.length()-1-pos);}
        file_path = USER_FILES_PATH + file_name;
        IoHelper io_helper(file_path);
	io_helper.SetKey(key);
	if(io_helper.ReadFile(filedata)==0){
		size_t data = std::stoul(filedata);
		data += 2;
		filedata = std::to_string(data);
		io_helper.WriteFile(filedata);
		out_str = "Write SUCCESS, the data in " + file_name +" now is " + filedata; 
	}
	return out_str;
}
std::string write_file(std::string in_str){
	std::string out_str;
        std::string file_path;
	std::string file_name;
	std::string data;
	std::size_t pos = in_str.find("/");
        if(pos == std::string::npos) {
                out_str = "cant find filename or data when you want to write file";
                return out_str;}
        else {
                file_name = in_str.substr(0,pos);
                data = in_str.substr(pos+1,in_str.length()-1-pos);}
	file_path = USER_FILES_PATH + file_name;
	IoHelper io_helper(file_path);
	// 未完成，因需要修改传入数据的方法
	out_str = "the write file functon is not done!";
	return out_str;
}
std::string build_file(std::string filename){
	std::string out_str;
	std::string key;
	std::string file_path = USER_FILES_PATH + filename;
	IoHelper io_helper(file_path);
	key = io_helper.GenerateKey();
	io_helper.SetKey(key);
	io_helper.WriteFile("100");
	out_str = "Gnerate new file success!The key is: " + key;
	return out_str;
}
