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

#include "spdlog/spdlog.h"

#include "HttpServer.h"
#include "algo.h"
#include "utils.h"



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

std::string TsService(std::string JsonReq)
{
    printf("TsService start \n");

    EnTaskCls enTaskCls = EN_TS_NONE;
    ParseJsonInput(JsonReq.c_str(), enTaskCls);

    std::string ret;
    if(EN_TS_FUS == enTaskCls)
    {
        StTsFusInput tsfusinput;
        StTsFusResultOutput tsfusoutput;
        Json2TsFusInput(JsonReq.c_str(), tsfusinput);
        Ts_Fus(tsfusinput, tsfusoutput);
        TsFusResult2Js(tsfusoutput, ret);
    }
    else if(EN_TS_ANA == enTaskCls)
    {
        StTsAnaInput tsanainput;
        StTsAnaResultOutput tsanaoutput;
        Json2TsAnaInput(JsonReq.c_str(), tsanainput);
        Ts_Anals(tsanainput, tsanaoutput);
        TsAnaResult2Js(tsanaoutput, ret);
    }

    return ret;
}

int main(int argc, char *argv[])
{   
    spdlog::set_level(spdlog::level::debug);
    // int ret = remote->init("ServerA", "localhost:1234", "172.16.2.2:2379");
    int ret = remote->init("ServerA", "localhost:1234", "localhost:2379");
    printf("init ServerA:%d\n", ret);
    remote->register_handler("tsfus", TsService);

/*################################## HTTP SERVER Start #######################################################*/

	HttpApplication app;
	//拥塞
	app.run(argc, argv);
/*################################## HTTP SERVER End #######################################################*/


    getchar();
    delete remote;
    return 0;
}