#pragma once
typedef struct req {
    char *method;           //请求方式
    char *file;             //请求文件
    char *host;             //主机名称
    char *accept;           //请求文件类型
    char *accept_Language;  //请求语言
    char *accept_Encoding;  //客户端支持编码
    char *connection;       //链接状态
    char *user_Agent;       //用户环境
    //form表单 暂不处理
}Request;
Request parseRequest(char *buffer);     //根据请求头构造请求信息
