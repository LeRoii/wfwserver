/*
 * @Description: 
 * @Version: 1.0
 * @Autor: LiLeTian
 * @Date: 2023-06-15 15:07:21
 * @LastEditors: LiLeTian
 * @LastEditTime: 2023-06-29 15:32:33 
 */
#include "remote.h"
#include <string>
#include <iostream>
#include "utils.h"
#include "spdlog/spdlog.h"

using namespace std;
using namespace XT_RPC;
Remote *remote = new Remote();
std::string echo(std::string JsonReq)
{
    printf("function test: echo start \n");
    std::string respons = "ServerA:" + JsonReq;
    return respons;
}
std::string echo1(std::string JsonReq)
{
    printf("function test: echo1 start \n");
    std::string server = "ServerB";
    std::string func = "echo1";
    std::string data = "ServerA:" + JsonReq;
    std::string respons;
    remote->Call(server, func, data, respons);
    return respons;
}

std::string echo2(std::string JsonReq)
{
    printf("function test: echo2 start \n");
    std::string server = "ServerB";
    std::string func = "echo2";
    std::string data = "ServerA:" + JsonReq;
    std::string respons;
    remote->Call(server, func, data, respons);
    printf("function test: echo2 start \n");
    server = "ServerC";
    func = "echo";
    data = respons;
    remote->Call(server, func, data, respons);
    return respons;
}

std::string TsFusionSrv(std::string JsonReq)
{
    printf("TsFusionSrv start \n");
    std::string ret;
    StTsFusInput tsfusinput;
    StTsFusResultOutput tsfusoutput;
    Json2TsFusInput(JsonReq.c_str(), tsfusinput);

    TsFusResult2Js(tsfusoutput, ret);


    std::string respons = "ServerA:" + JsonReq;
    return ret;
}



int main(int argc, char *argv[])
{   
    spdlog::set_level(spdlog::level::debug);
    // int ret = remote->init("ServerA", "localhost:1234", "172.16.2.2:2379");
    int ret = remote->init("ServerA", "localhost:1234", "localhost:2379");
    printf("init ServerA:%d\n", ret);
    remote->register_handler("echo", echo);
    remote->register_handler("tsfus", TsFusionSrv);
    remote->register_handler("echo2", echo2);
    getchar();
    delete remote;
    return 0;
}