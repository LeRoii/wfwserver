/*
 * @Description: 
 * @Version: 1.0
 * @Autor: LiLeTian
 * @Date: 2023-06-13 09:14:46
 * @LastEditors: LiLeTian
 * @LastEditTime: 2023-06-29 13:53:04
 */
#ifndef XT_RPC_H
#define XT_RPC_H
#include <string>
#include <map>
#include <iostream>
#include <functional>

typedef void (*error_handler)(std::string errorstr);
typedef std::string (*handler)(std::string JsonReq);

namespace XT_RPC
{
  class Remote
  {
    public:
        Remote();
        ~Remote();
        // 服务注册， 生成3个文件
        bool init(const std::string &server_name, const std::string &agent_address, const std::string &register_address);
        // 远程调度
        bool Call(const std::string &server_name, std::string const& function_name, std::string JsonReq, std::string &rep);
        // 方法注册
        bool register_handler(const std::string &function_name, handler function);
    private:
        std::string register_address_;
        std::string register_ip_;

  };
}
#endif
