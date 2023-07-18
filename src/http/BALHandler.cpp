/*******************
 * AUTHOR：FEX
 * DATE：2023-05-16
 * HTTP服务：BALHandle.cpp
 * 业务层代码
 * 实现每一个http请求的实际处理对象
 * 
 * 
 * 
 * 
 * 
*********************/
#include "spdlog/spdlog.h"

#include "BALHandler.h"
#include "common.h"
#include "utils.h"

#include <dirent.h>
#include <fstream>

map<string, string> TargetTypeDic{{"person", "人员"}, {"soldier","士兵"}, {"vehicle", "车辆"}, {"millitary vehicle", "装甲车"}};
map<string, string> TargetTypeMap{{ "人", "person"}, { "人员", "person"}, {"士兵", "soldier"}, {"车", "vehicle"}, {"车辆", "vehicle"}, {"装甲车", "millitary vehicle"}};


vector<string> files;
void SendQBFusResult(std::string& ret);

void getFiles(const string path, vector<string>& files )  
{
    files.clear();
    DIR* d = opendir(path.c_str());
    if(d == NULL)
    {
        std::cout<<"save file path is NULL"<<std::endl;
        return;
    }

    std::cout<<std::endl<<"path:"<<path<<std::endl;
    struct dirent* entry;
    while((entry=readdir(d)) != NULL)
    {
        std::string dPath(path);
        dPath.append(entry->d_name);

        std::cout<<"entry->d_name:"<<entry->d_name<<std::endl;

        //文件名称长度不小于4字节
        if(strlen(entry->d_name) >= 4)
        {
            //处理图片类型文件
            if((strncmp(entry->d_name+strlen(entry->d_name)-4, ".jpg", 4) == 0) 
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".JPG", 4) == 0) 
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".png", 4) == 0)
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".PNG", 4) == 0)
                )
            {
                std::cout<<"files vector add picture file:"<<dPath<<endl;
                //添加图片文件到vector
                files.push_back(dPath);
            }
            //处理视频类型文件
            else if((strncmp(entry->d_name+strlen(entry->d_name)-4, ".mp4", 4) == 0)
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".MP4", 4) == 0)
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".Mp4", 4) == 0)
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".mP4", 4) == 0)
                )
            {
                std::cout<<"files vector add video file:"<<dPath<<endl;
                //添加图片文件到vector
                files.push_back(dPath);

            }
            //音频文件
            else if((strncmp(entry->d_name+strlen(entry->d_name)-4, ".wav", 4) == 0)
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".WAV", 4) == 0)
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".mp3", 4) == 0)
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".MP3", 4) == 0)
                )
            {
                std::cout<<"files vector add audio file:"<<dPath<<endl;
                //添加图片文件到vector
                files.push_back(dPath);

            }
            //文本文件
            else if((strncmp(entry->d_name+strlen(entry->d_name)-4, ".txt", 4) == 0)
                || (strncmp(entry->d_name+strlen(entry->d_name)-4, ".TXT", 4) == 0)
                )
            {
                std::cout<<"files vector add text file:"<<dPath<<endl;
                //添加图片文件到vector
                files.push_back(dPath);
            }
            else
            {
                //删除非jpg、png文件
                if(remove(dPath.c_str()) == 0)
                {
                    std::cout<<"Remove file:"<<dPath<<"succeed!"<<std::endl;
                }
                else
                {
                    std::cout<<"Remove file:"<<dPath<<"fail!"<<std::endl;
                }
            }
        }
        else
        {
            //.和..文件跳过
            if((strncmp(entry->d_name+strlen(entry->d_name)-1, ".", 1) == 0) 
                || (strncmp(entry->d_name+strlen(entry->d_name)-2, "..", 2) == 0))
            {
                continue;
            }

            //删除非视频、图片、音频、文本文件
            if(remove(dPath.c_str()) == 0)
            {
                std::cout<<"Remove file:"<<dPath<<"succeed!"<<std::endl;
            }
            else
            {
                std::cout<<"Remove file:"<<dPath<<"fail!"<<std::endl;
            }
        }
    }

    closedir(d);
    
}


FilePartHandle::FilePartHandle()
{
    memset(m_SaveDir, 0 , MAX_DIRPATH);
    strcpy(m_SaveDir, "/app/recvdata/");
    // m_SaveDir = "/app/recvdata";

    // MQTTCLIENT::Common::GetValueByKeyFromConfig("HTTP_FILE_SAVE_DIR", m_SaveDir, MQTTCLIENT::MAX_DIRPATH);
}

void FilePartHandle::handlePart(const Poco::Net::MessageHeader& header, std::istream& stream)
{
    auto type = header.get("Content-Type", "(unspecified)");
    std::string name="(unnamed)";
    std::string file_name = "unnamed";

    if(header.has("Content-Disposition"))
    {
        std::string disp;
        Poco::Net::NameValueCollection params;
        Poco::Net::MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
        name = params.get("name", "(unnamed)");
        file_name = params.get("filename", "(unnamed)");
    }

    std::shared_ptr< std::vector<std::uint8_t> > content(new std::vector<std::uint8_t>());

    while(stream)
    {
        int ch = stream.get();
        content->emplace_back(ch);
    }

    file_parts_.push_back({name, file_name, content});
}


void FilePartHandle::Print()
{
    std::cout<<"lsit size："<<file_parts_.size()<<std::endl;

    for_each(file_parts_.begin(), file_parts_.end(), [&](const FilePartHandle::FilePart &file_b){
        std::cout<<"name："<<file_b.name<<std::endl;
        std::cout<<"file_name："<<file_b.file_name<<std::endl;
        std::cout<<"content："<<std::string(file_b.content->begin(), file_b.content->end())<<std::endl;
    }

    );
}


void FilePartHandle::SaveFile()
{
    try
    {

        for_each(file_parts_.begin(), file_parts_.end(), [&](const FilePartHandle::FilePart &file_b){
            std::string filePath(m_SaveDir);
            //保存文件
            Poco::FileOutputStream fos(filePath+file_b.file_name, std::ios::binary);
            //不输出end位置的字符
            std::string s_Out(file_b.content->begin(), file_b.content->end()-1);
            fos << s_Out;
            fos.close();

            std::cout << "file<<" << filePath << file_b.file_name << 
                        ">>" << "save succeed!" << std::endl;


            char script[MSG_LEN] = {0};
            //执行.zip文件解压
            if(strncmp(file_b.file_name.substr(file_b.file_name.length()-4).c_str(), ".zip", 4) == 0)
            {
                sprintf(script, "unzip -q   %s%s   -d %s ", filePath.c_str(), (file_b.file_name).c_str(), filePath.c_str());

                std::cout<<"zip decompression command:\n"<<script<<std::endl;
                system(script);
                

                
                //获取该路径下的所有png、jpg文件
                getFiles(filePath, files);

                std::cout<<"recv QB data files.size:"<<files.size()<<std::endl;

                for(auto& recvfile:files)
                {
                    spdlog::debug("{}", recvfile);
                }

                //情报融合处理
                this->QBFusion();

                
            }
            // //执行.rar文件解压
            // else if(strncmp(file_b.file_name.substr(file_b.file_name.length()-4).c_str(), ".rar", 4) == 0)
            // {
            //     // sprintf(script, "unzip  -d %s   %s%s ", filePath, filePath, file_b.file_name);

            //     // system(script);
            //     std::cout<<"rar decompression"<<std::endl;
            // }
            //执行.tar解压
            // else if(strncmp(file_b.file_name.substr(file_b.file_name.length()-4).c_str(), ".tar", 4) == 0)
            // {
            //     sprintf(script, "tar  -xvf  %s%s   -C  %s ", filePath.c_str(), (file_b.file_name).c_str(), filePath.c_str());


            //     std::cout<<"tar decompression command:"<<script<<std::endl;

            //     system(script);


            // }
            // //执行tar.gz解压
            // else if(strncmp(file_b.file_name.substr(file_b.file_name.length()-7).c_str(), ".tar.gz", 7) == 0)
            // {
            //     sprintf(script, "tar  -zxvf  %s%s   -C  %s ", filePath.c_str(), (file_b.file_name).c_str(), filePath.c_str());


            //     std::cout<<"gz decompression command:"<<script<<std::endl;


            //     system(script);

            // }
            // //执行.tar.bz2解压
            // else if(strncmp(file_b.file_name.substr(file_b.file_name.length()-8).c_str(), ".tar.bz2", 8) == 0)
            // {

            //     std::cout<<"bz2 decompression"<<std::endl;
            // }
            else
            {
                sprintf(script, "%s%s", filePath.c_str(), (file_b.file_name).c_str());

                //删除非zip文件
                if(remove(script) == 0)
                {
                    std::cout<<"Remove file:"<<script<<"succeed!"<<std::endl;
                }
                else
                {
                    std::cout<<"Remove file:"<<script<<"fail!"<<std::endl;
                }
            }

        }

        );
    }
    catch(Poco::Exception& e)
    {
        std::cerr << "保存文件出错：" << e.what() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "保存文件出错：" << e.what() << std::endl;
    }   
    
}

std::list<FilePartHandle::FilePart>& FilePartHandle::file_parts()
{
    return file_parts_;
}


void FilePartHandle::QBFusion()
{
    // if(MQTTCLIENT::Common::mosq != NULL && MQTTCLIENT::Common::mosq != nullptr)
    // {

        std::cout<<"QBFusion start"<<std::endl;
        char data[MAX_MSG_LEN]={0};
        
        
        // MQTTCLIENT::ST_ANALYSISFUSION_RECV obj;

        // memset(&obj, 0, sizeof(obj));
       

        // MQTTCLIENT::ST_ANALYSISFUSION_RET objRet;
        // memset(&objRet, 0, sizeof(objRet));

        StIntelFusResultData IntelFusResultData;

        std::string s_text;
        s_text.clear();


        // obj.st_type = 2;
        // // memset(obj.st_api, 0, STR_LEN);
        // memcpy(obj.st_api, "mapPlugin/ai", strlen("mapPlugin/ai"));

        // std::cout<<"recv QB data files.size:"<<files.size()<<std::endl;
        spdlog::debug("recv QB data files.size:{}", files.size());
        //执行底层算法
        for(int i=0; i< files.size();i++)
        {
            // std::cout<<std::endl<<"DetectorRun:"<<files[i]<<std::endl<<std::endl;
            spdlog::debug("process QB data files:{}", files[i]);
            //处理图片类型文件
            if(
                (strncmp(files[i].substr(files[i].length()-4).c_str(), ".jpg", 4) == 0)
                || (strncmp(files[i].substr(files[i].length()-4).c_str(), ".JPG", 4) == 0)
                || (strncmp(files[i].substr(files[i].length()-4).c_str(), ".png", 4) == 0)
                || (strncmp(files[i].substr(files[i].length()-4).c_str(), ".PNG", 4) == 0)
                )
            {
                std::cout<<"\n\nDeal Fusion picture begin!!!!"<<std::endl;
                cv::Mat img = cv::imread(files[i]);

                printf("img is empty:%d. %s\n", img.empty(), files[i].c_str());
                std::vector<std::string> detret;
                DetectorRun(img, detret);

                // for(int k=0;k<detret.size();k++)
                // {
                //     std::cout<<"\n\nPicture detret:"<<detret[k]<<std::endl<<std::endl;
                // }

                int targetNum = detret.size()/2;
                for(int j=0;j<targetNum;j++)
                {
                    StIntelFusResultObj obj;
                    obj.id = j;
                    obj.lon_lat = "";
                    // ST_ANALYSISFUSION_OBJ objitem;
                    // memset(&objitem, 0, sizeof(objitem));
                    // strcpy(objitem.lon_lat, "");
                    // objitem.id = j;

                    auto now = std::chrono::system_clock::now();
                    //通过不同精度获取相差的毫秒数
                    uint64_t dis_millseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
                    - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;
                    time_t tt = std::chrono::system_clock::to_time_t(now);
                    auto time_tm = localtime(&tt);
                    char strTime[25] = { 0 };
                    sprintf(strTime, "%d-%02d-%02d %02d:%02d:%02d", time_tm->tm_year + 1900,
                    time_tm->tm_mon + 1, time_tm->tm_mday, time_tm->tm_hour,
                    time_tm->tm_min, time_tm->tm_sec);
                    // std::cout << strTime << std::endl;

                    obj.sTime = strTime;
                    obj.mapType = detret[j*2];
                    obj.obj_bbox = detret[j*2+1];
                    obj.camp = "";
                    obj.trend = "";
                    obj.isWeapon = false;

                    IntelFusResultData.objs.push_back(obj);

                    // strcpy(objitem.mapType, detret[j*2].c_str());
                    // strcpy(objitem.obj_bbox, detret[j*2+1].c_str());

                    // strcpy(objitem.trend, "");
                    // strcpy(objitem.camp, "");

                    // objRet.vect.push_back(objitem);


                    s_text.append("目标类型:");

                    std::map<string, string>::iterator itr;
                    itr = TargetTypeDic.find(detret[j*2]);

                    if(itr != TargetTypeDic.end())
                    {
                        s_text.append(itr->second);
                        
                    }
                    else
                    {
                        s_text.append(detret[j*2]);
                        
                    }

                    s_text.append(";时间:");
                    s_text.append(obj.sTime);
                    s_text.append(";威胁度:三级。");

                }
                std::cout<<"Deal Fusion picture end!!!!\n"<<std::endl;
            }
            // 处理视频类型文件
            else if(
                (strncmp(files[i].substr(files[i].length()-4).c_str(), ".mp4", 4) == 0)
                || (strncmp(files[i].substr(files[i].length()-4).c_str(), ".MP4", 4) == 0)
                || (strncmp(files[i].substr(files[i].length()-4).c_str(), ".Mp4", 4) == 0)
                || (strncmp(files[i].substr(files[i].length()-4).c_str(), ".mP4", 4) == 0)
                )
            {
                std::cout<<"\n\nDeal Fusion video begin!!!!"<<std::endl;
                //处理视频
                cv::VideoCapture cap;
                cap.open(files[i].c_str());
                if(!cap.isOpened())
                {
                    printf("\n\nvideo open failed:%s\n\n", files[i].c_str());

                    //删除本地文件
                    if(remove(files[i].c_str()) == 0)
                    {
                        std::cout<<"Remove file:"<<files[i]<<"succeed!"<<std::endl;
                    }
                    else
                    {
                        std::cout<<"Remove file:"<<files[i]<<"fail!"<<std::endl;
                    }
                    continue;
                }
                int frameNum = cap.get(cv::CAP_PROP_FRAME_COUNT); 
                cv::Mat frame;
                std::vector<std::string> detret, finalret;
                for(int i=0;i<frameNum;i++)
                {
                    cap >> frame;
                    DetectorRun(frame, detret);
                    if(detret.size() > finalret.size())
                        finalret = detret;
                }
                int targetNum = finalret.size()/2;
                for(int j=0;j<targetNum;j++)
                {
                    StIntelFusResultObj obj;
                    obj.id = j;
                    obj.lon_lat = "";
                    // ST_ANALYSISFUSION_OBJ objitem;
                    // memset(&objitem, 0, sizeof(objitem));
                    // strcpy(objitem.lon_lat, "");
                    // objitem.id = j;

                    auto now = std::chrono::system_clock::now();
                    //过通不同精度获取相差的毫秒数
                    uint64_t dis_millseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
                    - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;
                    time_t tt = std::chrono::system_clock::to_time_t(now);
                    auto time_tm = localtime(&tt);
                    char strTime[25] = { 0 };
                    sprintf(strTime, "%d-%02d-%02d %02d:%02d:%02d", time_tm->tm_year + 1900,
                    time_tm->tm_mon + 1, time_tm->tm_mday, time_tm->tm_hour,
                    time_tm->tm_min, time_tm->tm_sec);
                    // std::cout << strTime << std::endl;

                    obj.sTime = strTime;
                    obj.mapType = detret[j*2];
                    obj.obj_bbox = detret[j*2+1];
                    obj.camp = "";
                    obj.trend = "";

                    IntelFusResultData.objs.push_back(obj);

                    // strcpy(objitem.mapType, detret[j*2].c_str());
                    // strcpy(objitem.obj_bbox, detret[j*2+1].c_str());

                    // strcpy(objitem.trend, "");
                    // strcpy(objitem.camp, "");


                    // objRet.vect.push_back(objitem);


                    s_text.append("目标类型:");
                    std::map<string, string>::iterator itr;
                    itr=TargetTypeDic.find(detret[j*2]);

                    if(itr!=TargetTypeDic.end())
                    {
                        s_text.append(itr->second);
                    }
                    else
                    {
                        s_text.append(detret[j*2]);
                    }
                    s_text.append(";时间:");
                    s_text.append(obj.sTime);
                    s_text.append(";威胁度:三级。");

                }
                printf("video ret num:%d\n", finalret.size());

                std::cout<<"Deal Fusion video end!!!!\n"<<std::endl;

            }
            //处理音频类型文件
            else if((strncmp(files[i].substr(files[i].length()-4).c_str(), ".wav", 4) == 0)
                || (strncmp(files[i].substr(files[i].length()-4).c_str(), ".WAV", 4) == 0)
                )
            {
                
                std::string s_str;
                s_str.append("/app/main -l zh -m /app/vbase.bin -f  ");
                s_str.append(files[i]);

                std::cout<<"\n\naudio file execute command:\n"<<s_str<<std::endl<<std::endl<<std::endl;
                system(s_str.c_str());

                std::string path;
                path.append(files[i]+".txt");

                ifstream ifs;
                ifs.open(path, ios::in);

                if(!ifs.is_open())
                {
                    std::cout<<"open file fail:"<<path<<std::endl;
                }

                std::stringstream buffer;
                buffer << ifs.rdbuf();  // 将文件内容读取到字符串流中
                ifs.close();  // 关闭文件
                std::string content = buffer.str();

                spdlog::debug("voice rec result:{}", content);

                s_text.append(string(content));

                
                //删除语音转换生成的临时txt文件
                if(remove(path.c_str()) == 0)
                {
                    std::cout<<"Remove audio txt file:"<<path<<"succeed!"<<std::endl;
                }
                else
                {
                    std::cout<<"Remove audio txt file:"<<path<<"fail!"<<std::endl;
                }
            }
            //处理文本类型文件
            else if(
                (strncmp(files[i].substr(files[i].length()-4).c_str(), ".txt", 4) == 0)
                || (strncmp(files[i].substr(files[i].length()-4).c_str(), ".TXT", 4) == 0)
                )
            {
                
                std::vector<std::string> strs;
                string input_file=files[i];
                string query;
                string output;
                std::ifstream in(input_file);
                while(getline(in,query))
                {
                    // cout<<query<<endl;
                    strs.push_back(query);
                }
                // for(auto &s:strs)
                for(int j=0;j<strs.size();j++)
                {
                    string str = strs[j];
                    cout<<str<<endl;	
                    output += "情报" + std::to_string(j+1) + ":\n";
                    int n = str.size();
                    for (int k = 0; k < n; ++k){
                        if (str[k] == ','){
                            str[k] = ' ';
                        }
                    }
                    istringstream out(str);
                    string ss;
                    while (out >> ss){
                        output += ss + "\n";
                        // cout << str <<endl;
                        // std::size_t pos = str.find("发现");
                        // std::string str3 = str.substr (pos); 
                        // cout << str <<endl;
                    }
                    output += "\n";
                    cout << output << endl;



                    // std::string str = "前方发现敌人目标大约距离500米,向西北边行驶,速度大约是100米每分钟经度 维度 高度 分别是116.00005309025627 40.14032998316538 747.553";
                    std::string numStr = "";
                    double num = 0.0;
                    bool hasDecimal = false;
                    std::vector<std::string> nums;
                    std::string threat[7] = {"一级", "二级", "三级", "二级", "三级", "二级", "三级"};
                    std::string dir[8] = {"东北", "东南", "西北", "西南", "东", "西", "南", "北"};
                    std::string objtypes[2] = {"人员", "车辆"};
                    std::string personDict[2] = {"人员", "人"};
                    std::string vehicleDict[3] = {"车", "车辆", "驶"};
                    std::string camp;
                    std::string MoveDir = "";
                    std::size_t pos = str.find("敌");
                    std::string objtype = objtypes[0];

                    if (pos!=std::string::npos)
                        camp = "敌方";
                    std::cout << "camp pos:" << pos << std::endl;

                    for(int ii=0;ii<3;ii++)
                    {
                        pos = str.find(vehicleDict[ii]);
                        if (pos!=std::string::npos)
                        {
                            objtype = objtypes[1];
                            break;
                        }
                    }

                    
                    for(int l=0; l<8;l++)
                    {
                        pos = str.find(dir[l]);
                        if (pos!=std::string::npos)
                        {
                            MoveDir = dir[l];
                            break;
                        }
                    }

                    for (char c : str) {
                        if (isdigit(c)) {
                            numStr += c;
                        }
                        else if (c == '.' && !hasDecimal) {
                            numStr += c;
                            hasDecimal = true;
                        }
                        else if (!numStr.empty()) {
                            // num = stod(numStr);
                            
                            nums.push_back(numStr);
                            std::cout << "numStr:" << numStr << std::endl;
                            std::cout << "nums back:" << nums.back() << std::endl;
                            numStr = "";
                            num = 0.0;
                            hasDecimal = false;
                        }
                    }

                    if (!numStr.empty()) {
                        // num = stod(numStr);
                        std::cout << numStr << std::endl;
                        nums.push_back(numStr);
                    }

                    printf("nums:%d\n", nums.size());

                    std::string loglat = "";
                    std::string threatlvl = "";
                    int lonlatcnt = 0;
                    for(auto &s:nums)
                    {
                        printf("len:%d\n", s.size());
                        if(lonlatcnt > 1)
                            break;
                        if(s.size() > 8)
                        {
                            loglat += s;
                            if(lonlatcnt == 1)
                                break;
                            loglat += ",";
                            lonlatcnt++;
                        }
                    }

                    std::srand(std::time(nullptr));
                    int threatlvlidx = std::rand() % 7 + 1;
                    threatlvl = threat[threatlvlidx-1];

                    std::cout << "loglat:" << loglat << std::endl;
                    std::cout << camp << std::endl;
                    std::cout << threatlvl << std::endl;
                    std::cout << MoveDir << std::endl;

                    
                    // ST_ANALYSISFUSION_OBJ objitem;
                    // memset(&objitem, 0, sizeof(objitem));
                    // objitem.id = j;

                    StIntelFusResultObj obj;
                    obj.id = j;
                    
                    std::map<string, string>::iterator itr;
                    itr=TargetTypeMap.find(objtype);

                    // memset(objitem.mapType, 0, sizeof(objitem.mapType));
                    if(itr!=TargetTypeMap.end())
                    {
                        obj.mapType = itr->second;
                        // strncpy(objitem.mapType, itr->second.c_str(), itr->second.length());
                    }
                    else
                    {
                        obj.mapType = objtype;
                        // strncpy(objitem.mapType, objtype.c_str(), objtype.length());
                    }
                    //strncpy(objitem.mapType, objtype.c_str(), objtype.length());
                    // strncpy(objitem.camp, camp.c_str(), camp.length());
                    // strncpy(objitem.lon_lat, loglat.c_str(), loglat.length());
                    // strncpy(objitem.threat_degree, threatlvl.c_str(), threatlvl.length());
                    // strncpy(objitem.trend, MoveDir.c_str(), MoveDir.length());
                    // objRet.vect.push_back(objitem);

                    obj.camp = camp;
                    obj.lon_lat = loglat;
                    obj.sThreatDegree = threatlvl;
                    obj.trend = MoveDir;

                    IntelFusResultData.objs.push_back(obj);

                    
                }

                //output 赋值给text
                s_text.append(output);
            }
            //非视频、音频、语音、文本类型文件
            else
            {

            }

            
            //删除文件
            if(remove(files[i].c_str()) == 0)
            {
                std::cout<<"Remove file:"<<files[i]<<"succeed!"<<std::endl;
            }
            else
            {
                std::cout<<"Remove file:"<<files[i]<<"fail!"<<std::endl;
            }

        }

        IntelFusResultData.text = s_text;
        // printf("s_text:%s\nIntelFusResultData size:%d", s_text.c_str(), IntelFusResultData.objs.size());

        spdlog::debug("qb fusion s_text:{}, IntelFusResultData size:{}\n", s_text.c_str(), IntelFusResultData.objs.size());

        std::string retjs;
        IntelResult2Js(IntelFusResultData, retjs);
        spdlog::debug("qb fusion complete!\n {}", retjs);
        SendQBFusResult(retjs);
        
        

        // strncpy(objRet.text, s_text.c_str(), s_text.length()>MSG_LEN-1?MSG_LEN-1:s_text.length());

        // obj.st_obj = objRet;

        // //struct转JSON
        // MQTTCLIENT::Common::FusionST_ConvertTo_JSON(obj, data, MAX_MSG_LEN);
        
        // //发布回应消息
        // int res = mosquitto_publish(MQTTCLIENT::Common::mosq , NULL, MQTTCLIENT::Common::top_Rcv, strlen(data), data, 2, true);
        // if(res == MOSQ_ERR_SUCCESS)
        // {
        //     //std::cout<<"QB fusion send msg succeed:\n"<<data<<std::endl;
        //     std::cout<<"QB fusion send msg succeed:\n"<<std::endl;
        // }

        // s_text.clear();

        // //memset(&objRet.text, 0, sizeof(objRet.text));

        // memset(&objRet, 0, sizeof(objRet)); 
        // memset(&obj, 0, sizeof(obj));
        // //清空files
        // MQTTCLIENT::Common::files.clear();
    // }
    // else
    // {
    //     std::cout<<std::endl<<"QB fusion Analysis fail where mosquitto object is NULL!\n"<<std::endl;
    // }

    std::cout<<"QBFusion end"<<std::endl;    
}




//上传文件
void UploadRequestHandler::sendResponseResult(HTTPServerRequest & request, 
    HTTPServerResponse & response, std::string result_info)
{
    bool _bEnc = request.getChunkedTransferEncoding();
    response.setChunkedTransferEncoding(_bEnc);
    //设置内容格式为json
    response.setContentType("application/json");
    //允许跨域访问
    response.add("Access-Control-Allow-Origin", "*");
    response.add("Access-Control-Allow-Methods", "POST,GET,OPTIONS,DELETE");
    response.add("Access-Control-Max-Age", "3600");
    response.add("Access-Control-Allow-Headers", "x-requested-with,content-type");
    response.add("Access-Control-Allow-Credentials", "true");
    //响应反馈数据
    std::ostream& response_stream = response.send();
    response_stream << result_info;
    response_stream.flush();
    
    //std::cout<<"request.stream："<<osstr<<std::endl;
    std::cout<<"respond.stream："<<result_info<<std::endl;
}

void UploadRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	//读取数据时request.stream()只能使用一次，
    try
    {
        std::istream& istr = request.stream();

        // //直接读取content内容，succeed
        // std::string data(std::istreambuf_iterator<char>(istr), {});
        // std::cout<<std::endl<<"***************************** Content start *****************************"
        //          <<std::endl<<data
        //          <<"***************************** Content start *****************************"
        //          <<std::endl<<std::endl;

        // //处理数据
        // //判断当前数据类型是否是
        // if(strncmp(contentType, "multipart/form-data", strlen("multipart/form-data")) == 0)
        // {

        // }



        FilePartHandle fpHandle;

        //表单读取content内容，succeed
        Poco::Net::HTMLForm form(request, istr, fpHandle);
        
        //打印表单信息
        //std::cout<<"boundary："<<form.boundary()<<std::endl;
        //std::cout<<"encoding："<<form.getEncoding()<<std::endl;

        //打印文件信息
        //fpHandle.Print();
        //保存文件信息
        fpHandle.SaveFile();

		
		//respond回应
		sendResponseResult(request,response,"{\"result\":\"OK\"}");

    }
    catch (Poco::Exception& e)
    {
    //handle exception 
        //std::cout<<"handleRequest exception："<<e.displayText()<<std::endl;
        
        std::cerr << "UploadRequestHandler exception：" << e.what() << std::endl;
    }




}
