
#include "spdlog/spdlog.h"
#include"common.h"
#include "utils.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"


//detailInfo is json object
// void Json2TsFusInput(const char* str, const uint32_t len, StTsFusInput& TsFusInput)
// {
//     rapidjson::Document dom;
//     if(dom.Parse(str).HasParseError())
//     {
//         printf("json parse failed\n");
//         return;
//     }

//     TsFusInput.sType = dom["type"].GetString();
//     TsFusInput.sUserid = dom["userid"].GetString();
//     TsFusInput.data.clear();

//     for(auto &fuInputData:dom["data"])
//     {
//         StTsFusInputData dataElement;
//         dataElement.source = fuInputData["source"].GetString();
//         dataElement.i32Type = fuInputData["type"].GetInt();
        
//         rapidjson::Value& jsMarkerInfo = fuInputData["markerInfo"];
        
//         dataElement.MapMarker.timestampAndUserId = jsMarkerInfo["timestampAndUserId"].GetString();
//         dataElement.MapMarker.latitude = jsMarkerInfo["latitude"].GetDouble();
//         dataElement.MapMarker.longitude = jsMarkerInfo["longitude"].GetDouble();
//         dataElement.MapMarker.markerUrl = jsMarkerInfo["markerUrl"].GetString();
//         dataElement.MapMarker.jbMarkerCode = jsMarkerInfo["jbMarkerCode"].GetUint();
//         dataElement.MapMarker.jbColor = jsMarkerInfo["jbColor"].GetString();
//         dataElement.MapMarker.setOption = jsMarkerInfo["setOption"].GetString();
//         dataElement.MapMarker.addMarkerTime = jsMarkerInfo["addMarkerTime"].GetUint();
//         dataElement.MapMarker.delMarkerTime = jsMarkerInfo["delMarkerTime"].GetUint();
//         dataElement.MapMarker.updateMarkerTime = jsMarkerInfo["updateMarkerTime"].GetUint();
//         dataElement.MapMarker.publisherUserId = jsMarkerInfo["publisherUserId"].GetString();

//         rapidjson::Value& jsMarkerDetailInfo = jsMarkerInfo["detailInfo"];

//         dataElement.MapMarker.detailInfo.camp = jsMarkerDetailInfo["camp"].GetString();
//         dataElement.MapMarker.detailInfo.targetName = jsMarkerDetailInfo["targetName"].GetString();
//         dataElement.MapMarker.detailInfo.latitude = jsMarkerDetailInfo["latitude"].GetDouble();
//         dataElement.MapMarker.detailInfo.longitude = jsMarkerDetailInfo["longitude"].GetDouble();
//         dataElement.MapMarker.detailInfo.targetDirection = jsMarkerDetailInfo["targetDirection"].GetString();
//         dataElement.MapMarker.detailInfo.targetSpeed = jsMarkerDetailInfo["targetSpeed"].GetDouble();
//         dataElement.MapMarker.detailInfo.targetState = jsMarkerDetailInfo["targetState"].GetUint();
//         dataElement.MapMarker.detailInfo.isWeapon = jsMarkerDetailInfo["isWeapon"].GetUint();
//         dataElement.MapMarker.detailInfo.mHitRadius = jsMarkerDetailInfo["hitRadius"].GetUint();
//         dataElement.MapMarker.detailInfo.targetType = jsMarkerDetailInfo["targetType"].GetString();

//         TsFusInput.data.push_back(dataElement);
//     }
// }

// void TsFusResult2Js(StTsFusResultOutput& result, char* str)
// // void TsFusResult2Js(StTsFusResultData& result, char* str)
// {
//     rapidjson::Document dom;
//     dom.SetObject();
//     rapidjson::Document::AllocatorType& allocator = dom.GetAllocator();
//     rapidjson::Value v;
//     v.SetInt(0);
//     dom.AddMember("type", v, allocator);

//     v.SetString("mapPlugin/ai");
//     dom.AddMember("api", v, allocator);

//     rapidjson::Value dataValue(rapidjson::Type::kArrayType);

//     for(auto &dataElement:result.data)
//     {
//         rapidjson::Value dataElementValue(rapidjson::Type::kObjectType);
//         dataElement.AddMember("timestampAndUserId", dataElement.timestampAndUserId, allocator);
//         dataElement.AddMember("type", dataElement.i32Type, allocator);
//         dataElement.AddMember("res", dataElement.i32Res, allocator);
//         dataElement.AddMember("aiSource", "zx", allocator);

//         rapidjson::Value markerInfoValue(rapidjson::Type::kObjectType);
//         markerInfoValue.AddMember("timestampAndUserId", dataElement.markerInfo.timestampAndUserId , allocator);
//         markerInfoValue.AddMember("latitude", dataElement.markerInfo.latitude , allocator);
//         markerInfoValue.AddMember("longitude", dataElement.markerInfo.longitude , allocator);
//         markerInfoValue.AddMember("markerUrl", dataElement.markerInfo.markerUrl , allocator);
//         markerInfoValue.AddMember("jbMarkerCode", dataElement.markerInfo.jbMarkerCode , allocator);
//         markerInfoValue.AddMember("jbColor", dataElement.markerInfo.jbColor , allocator);
//         markerInfoValue.AddMember("setOption", dataElement.markerInfo.setOption , allocator);
//         markerInfoValue.AddMember("addMarkerTime", dataElement.markerInfo.addMarkerTime , allocator);
//         markerInfoValue.AddMember("delMarkerTime", dataElement.markerInfo.delMarkerTime , allocator);
//         markerInfoValue.AddMember("updateMarkerTime", dataElement.markerInfo.updateMarkerTime , allocator);
//         markerInfoValue.AddMember("publisherUserId", dataElement.markerInfo.publisherUserId , allocator);

//         rapidjson::Value markerDetailInfoValue(rapidjson::Type::kObjectType);
//         markerDetailInfoValue.AddMember("camp", dataElement.markerInfo.detailInfo.camp , allocator);
//         markerDetailInfoValue.AddMember("targetName", dataElement.markerInfo.detailInfo.targetName , allocator);
//         markerDetailInfoValue.AddMember("latitude", dataElement.markerInfo.detailInfo.latitude , allocator);
//         markerDetailInfoValue.AddMember("longitude", dataElement.markerInfo.detailInfo.longitude , allocator);
//         markerDetailInfoValue.AddMember("targetDirection", dataElement.markerInfo.detailInfo.targetDirection , allocator);
//         markerDetailInfoValue.AddMember("targetSpeed", dataElement.markerInfo.detailInfo.targetSpeed , allocator);
//         markerDetailInfoValue.AddMember("targetState", dataElement.markerInfo.detailInfo.targetState , allocator);
//         markerDetailInfoValue.AddMember("isWeapon", dataElement.markerInfo.detailInfo.isWeapon , allocator);
//         markerDetailInfoValue.AddMember("mHitRadius", dataElement.markerInfo.detailInfo.hitRadius , allocator);
//         markerDetailInfoValue.AddMember("targetType", dataElement.markerInfo.detailInfo.targetType , allocator);

//         markerInfoValue.AddMember("detailInfo", markerDetailInfoValue, allocator);

//         dataElement.AddMember("markerInfo", markerInfoValue., allocator);

//         dataValue.PushBack(dataElement, allocator);
//     }

//     dom.AddMember("data", dataValue, allocator);

//     printf("\nTsFusResult2Js result:\n");
//     rapidjson::StringBuffer sb;
//     rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
//     dom.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
//     puts(sb.GetString());
//     str = sb.GetString();
// }


void ParseJsonInput(const char* str, EnTaskCls& enTaskCls)
{
    spdlog::debug("ParseJsonInput start!");
    rapidjson::Document dom;
    printf("input data:%s\n", str);
    if(dom.Parse(str).HasParseError())
    {
        printf("json parse failed\n");
        return;
    }

    if (dom.HasMember("userid") && dom["userid"].IsString())
    {
        std::string userid = dom["userid"].GetString();
        if(userid.compare("/AIService/TS/Send") == 0)
        {
            enTaskCls = EN_TS_FUS;
        }
        else if(userid.compare("/AIService/analysis/Send") == 0)
        {
            enTaskCls = EN_TS_ANA;
        }
    }
    spdlog::debug("ParseJsonInput end!");
    return;
}



//detailInfo is string
void Json2TsFusInput(const char* str, StTsFusInput& TsFusInput)
{
    spdlog::debug("Json2TsFusInput start!");
    rapidjson::Document dom;
    // printf("input data:%s\n", str);
    if(dom.Parse(str).HasParseError())
    {
        printf("json parse failed\n");
        return;
    }

    TsFusInput.sType = dom["type"].GetString();
    TsFusInput.sUserid = dom["userid"].GetString();
    TsFusInput.data.clear();

    for(auto &fuInputData:dom["data"].GetArray())
    {
        StTsFusInputData dataElement;
        dataElement.source = fuInputData["source"].GetString();
        dataElement.i32Type = fuInputData["type"].GetInt();
        
        rapidjson::Value& jsMarkerInfo = fuInputData["markerInfo"];
        
        dataElement.MapMarker.timestampAndUserId = jsMarkerInfo["timestampAndUserId"].GetString();
        dataElement.MapMarker.latitude = jsMarkerInfo["latitude"].GetDouble();
        dataElement.MapMarker.longitude = jsMarkerInfo["longitude"].GetDouble();
        dataElement.MapMarker.markerUrl = jsMarkerInfo["markerUrl"].GetString();
        dataElement.MapMarker.jbMarkerCode = jsMarkerInfo["jbMarkerCode"].GetUint();
        dataElement.MapMarker.jbColor = jsMarkerInfo["jbColor"].GetString();
        dataElement.MapMarker.setOption = jsMarkerInfo["setOption"].GetString();
        dataElement.MapMarker.addMarkerTime = jsMarkerInfo["addMarkerTime"].GetUint64();
        dataElement.MapMarker.delMarkerTime = jsMarkerInfo["delMarkerTime"].GetUint64();
        dataElement.MapMarker.updateMarkerTime = jsMarkerInfo["updateMarkerTime"].GetUint64();
        dataElement.MapMarker.publisherUserId = jsMarkerInfo["publisherUserId"].GetString();
        dataElement.MapMarker.elevation = jsMarkerInfo["elevation"].GetString();
        dataElement.MapMarker.intent = jsMarkerInfo["intent"].GetString();

        std::string detailInfo = jsMarkerInfo["markerDetailInfo"].GetString();

        rapidjson::Document detailInfoDom;
        if(detailInfoDom.Parse(detailInfo.c_str()).HasParseError())
        {
            printf("json parse detailInfo failed\n");
            return;
        }

        printf("json parse detailInfo \n");

        // rapidjson::Value& jsMarkerDetailInfo = jsMarkerInfo["detailInfo"];

        dataElement.MapMarker.detailInfo.camp = detailInfoDom["camp"].GetString();
        dataElement.MapMarker.detailInfo.targetName = detailInfoDom["targetName"].GetString();
        dataElement.MapMarker.detailInfo.latitude = detailInfoDom["latitude"].GetDouble();
        dataElement.MapMarker.detailInfo.longitude = detailInfoDom["longitude"].GetDouble();
        dataElement.MapMarker.detailInfo.targetCount = detailInfoDom["targetCount"].GetInt();
        dataElement.MapMarker.detailInfo.targetDirection = detailInfoDom["targetDirection"].GetString();
        dataElement.MapMarker.detailInfo.targetSpeed = detailInfoDom["targetSpeed"].GetDouble();
        dataElement.MapMarker.detailInfo.targetState = detailInfoDom["targetState"].GetInt();
        dataElement.MapMarker.detailInfo.isWeapon = detailInfoDom["isWeapon"].GetUint();
        dataElement.MapMarker.detailInfo.hitRadius = detailInfoDom["mHitRadius"].GetUint();
        dataElement.MapMarker.detailInfo.targetType = detailInfoDom["targetType"].GetString();

        TsFusInput.data.push_back(dataElement);
    }

    spdlog::debug("Json2TsFusInput complete!");
}

void TsFusResult2Js(StTsFusResultOutput& result, std::string& str)
// void TsFusResult2Js(StTsFusResultData& result, char* str)
{
    spdlog::debug("TsFusResult2Js start!");
    rapidjson::Document dom;
    dom.SetObject();
    rapidjson::Document::AllocatorType& allocator = dom.GetAllocator();
    rapidjson::Value v;
    v.SetInt(0);
    dom.AddMember("type", v, allocator);

    v.SetString("mapPlugin/ai");
    dom.AddMember("api", v, allocator);

    rapidjson::Value dataValue(rapidjson::Type::kArrayType);

    for(auto &dataElement:result.data)
    {
        rapidjson::Value dataElementValue(rapidjson::Type::kObjectType);
        dataElementValue.AddMember("timestampAndUserId", rapidjson::StringRef(dataElement.timestampAndUserId.c_str()), allocator);
        dataElementValue.AddMember("type", dataElement.i32Type, allocator);
        dataElementValue.AddMember("res", dataElement.i32Res, allocator);
        dataElementValue.AddMember("aiSource", "zx", allocator);

        rapidjson::Value markerInfoValue(rapidjson::Type::kObjectType);
        markerInfoValue.AddMember("timestampAndUserId", rapidjson::StringRef(dataElement.markerInfo.timestampAndUserId.c_str()), allocator);
        markerInfoValue.AddMember("latitude", dataElement.markerInfo.latitude , allocator);
        markerInfoValue.AddMember("longitude", dataElement.markerInfo.longitude , allocator);
        markerInfoValue.AddMember("markerUrl", rapidjson::StringRef(dataElement.markerInfo.markerUrl.c_str()) , allocator);

        rapidjson::Value markerDetailInfoValue(rapidjson::Type::kObjectType);
        markerDetailInfoValue.AddMember("camp", rapidjson::StringRef(dataElement.markerInfo.detailInfo.camp.c_str()) , allocator);
        markerDetailInfoValue.AddMember("targetName", rapidjson::StringRef(dataElement.markerInfo.detailInfo.targetName.c_str()) , allocator);
        markerDetailInfoValue.AddMember("latitude", dataElement.markerInfo.detailInfo.latitude , allocator);
        markerDetailInfoValue.AddMember("longitude", dataElement.markerInfo.detailInfo.longitude , allocator);
        markerDetailInfoValue.AddMember("targetCount", dataElement.markerInfo.detailInfo.targetCount , allocator);
        markerDetailInfoValue.AddMember("targetDirection", rapidjson::StringRef(dataElement.markerInfo.detailInfo.targetDirection.c_str()) , allocator);
        markerDetailInfoValue.AddMember("targetSpeed", dataElement.markerInfo.detailInfo.targetSpeed , allocator);
        markerDetailInfoValue.AddMember("targetState", dataElement.markerInfo.detailInfo.targetState , allocator);
        markerDetailInfoValue.AddMember("isWeapon", dataElement.markerInfo.detailInfo.isWeapon , allocator);
        markerDetailInfoValue.AddMember("mHitRadius", dataElement.markerInfo.detailInfo.hitRadius , allocator);
        markerDetailInfoValue.AddMember("targetType", rapidjson::StringRef(dataElement.markerInfo.detailInfo.targetType.c_str()) , allocator);

        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
        markerDetailInfoValue.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
        // puts(sb.GetString());
        // str = sb.GetString();
        // markerInfoValue.AddMember("markerDetailInfo", rapidjson::StringRef(sb.GetString()), allocator);
        markerInfoValue.AddMember("markerDetailInfo", rapidjson::Value(sb.GetString(), allocator), allocator);

        markerInfoValue.AddMember("jbMarkerCode", dataElement.markerInfo.jbMarkerCode , allocator);
        markerInfoValue.AddMember("jbColor", rapidjson::StringRef(dataElement.markerInfo.jbColor.c_str()) , allocator);
        markerInfoValue.AddMember("setOption", rapidjson::StringRef(dataElement.markerInfo.setOption.c_str()) , allocator);
        markerInfoValue.AddMember("addMarkerTime", dataElement.markerInfo.addMarkerTime , allocator);
        markerInfoValue.AddMember("delMarkerTime", dataElement.markerInfo.delMarkerTime , allocator);
        markerInfoValue.AddMember("updateMarkerTime", dataElement.markerInfo.updateMarkerTime , allocator);
        markerInfoValue.AddMember("publisherUserId", rapidjson::StringRef(dataElement.markerInfo.publisherUserId.c_str()) , allocator);
        markerInfoValue.AddMember("elevation", rapidjson::StringRef(dataElement.markerInfo.elevation.c_str()) , allocator);
        markerInfoValue.AddMember("intent", rapidjson::StringRef(dataElement.markerInfo.intent.c_str()) , allocator);

        dataElementValue.AddMember("markerInfo", markerInfoValue, allocator);

        dataValue.PushBack(dataElementValue, allocator);
    }

    dom.AddMember("data", dataValue, allocator);

    printf("\nTsFusResult2Js result:\n");
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    dom.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
    // puts(sb.GetString());
    // str = sb.GetString();
    str = std::string(sb.GetString());

    spdlog::debug("TsFusResult2Js end!");
}


void Json2TsAnaInput(const char* str, StTsAnaInput& TsAnaInput)
{
    spdlog::debug("Json2TsAnaInput start!");
    rapidjson::Document dom;
    if(dom.Parse(str).HasParseError())
    {
        printf("Json2TsAnaInput json parse failed\n");
        return;
    }

    TsAnaInput.sType = dom["type"].GetString();
    TsAnaInput.sUserid = dom["userid"].GetString();

    rapidjson::Value& jsTsAnaData = dom["data"];

    TsAnaInput.data.source = jsTsAnaData["source"].GetString();
    TsAnaInput.data.i32Type = jsTsAnaData["type"].GetInt();
    TsAnaInput.data.i32Time = jsTsAnaData["time"].GetInt();

    rapidjson::Value& jsMarkerInfo = jsTsAnaData["MarkerInfo"];

    TsAnaInput.data.MapMarker.timestampAndUserId = jsMarkerInfo["timestampAndUserId"].GetString();
    TsAnaInput.data.MapMarker.latitude = jsMarkerInfo["latitude"].GetDouble();
    TsAnaInput.data.MapMarker.longitude = jsMarkerInfo["longitude"].GetDouble();
    TsAnaInput.data.MapMarker.markerUrl = jsMarkerInfo["markerUrl"].GetString();
    TsAnaInput.data.MapMarker.jbMarkerCode = jsMarkerInfo["jbMarkerCode"].GetUint();
    TsAnaInput.data.MapMarker.jbColor = jsMarkerInfo["jbColor"].GetString();
    TsAnaInput.data.MapMarker.setOption = jsMarkerInfo["setOption"].GetString();
    TsAnaInput.data.MapMarker.addMarkerTime = jsMarkerInfo["addMarkerTime"].GetUint64();
    TsAnaInput.data.MapMarker.delMarkerTime = jsMarkerInfo["delMarkerTime"].GetUint64();
    TsAnaInput.data.MapMarker.updateMarkerTime = jsMarkerInfo["updateMarkerTime"].GetUint64();
    TsAnaInput.data.MapMarker.publisherUserId = jsMarkerInfo["publisherUserId"].GetString();
    TsAnaInput.data.MapMarker.publisherUserId = jsMarkerInfo["elevation"].GetString();
    TsAnaInput.data.MapMarker.publisherUserId = jsMarkerInfo["intent"].GetString();

    // rapidjson::Value& jsMarkerDetailInfo = jsMarkerInfo["markerDetailInfo"];

    std::string detailInfo = jsMarkerInfo["markerDetailInfo"].GetString();
    rapidjson::Document detailInfoDom;
    if(detailInfoDom.Parse(detailInfo.c_str()).HasParseError())
    {
        printf("json parse detailInfo failed\n");
        return;
    }

    TsAnaInput.data.MapMarker.detailInfo.camp = detailInfoDom["camp"].GetString();
    TsAnaInput.data.MapMarker.detailInfo.targetName = detailInfoDom["targetName"].GetString();
    TsAnaInput.data.MapMarker.detailInfo.latitude = detailInfoDom["latitude"].GetDouble();
    TsAnaInput.data.MapMarker.detailInfo.longitude = detailInfoDom["longitude"].GetDouble();
    TsAnaInput.data.MapMarker.detailInfo.longitude = detailInfoDom["targetCount"].GetInt();
    TsAnaInput.data.MapMarker.detailInfo.targetDirection = detailInfoDom["targetDirection"].GetString();
    TsAnaInput.data.MapMarker.detailInfo.targetSpeed = detailInfoDom["targetSpeed"].GetDouble();
    TsAnaInput.data.MapMarker.detailInfo.targetState = detailInfoDom["targetState"].GetUint();
    TsAnaInput.data.MapMarker.detailInfo.isWeapon = detailInfoDom["isWeapon"].GetUint();
    TsAnaInput.data.MapMarker.detailInfo.hitRadius = detailInfoDom["mHitRadius"].GetUint();
    TsAnaInput.data.MapMarker.detailInfo.targetType = detailInfoDom["targetType"].GetString();

    spdlog::debug("Json2TsAnaInput end!");
}


static void MarkerInfoSt2Js(StMapMarkerInfo& markerinfo, rapidjson::Value& val, rapidjson::Document::AllocatorType& allocator)
{
    val.AddMember("timestampAndUserId", rapidjson::StringRef(markerinfo.timestampAndUserId.c_str()) , allocator);
    val.AddMember("latitude", markerinfo.latitude , allocator);
    val.AddMember("longitude", markerinfo.longitude , allocator);
    val.AddMember("markerUrl", rapidjson::StringRef(markerinfo.markerUrl.c_str()) , allocator);
    
    rapidjson::Value markerDetailInfoValue(rapidjson::Type::kObjectType);
    markerDetailInfoValue.AddMember("camp", rapidjson::StringRef(markerinfo.detailInfo.camp.c_str()) , allocator);
    markerDetailInfoValue.AddMember("targetName", rapidjson::StringRef(markerinfo.detailInfo.targetName.c_str()) , allocator);
    markerDetailInfoValue.AddMember("latitude", markerinfo.detailInfo.latitude , allocator);
    markerDetailInfoValue.AddMember("longitude", markerinfo.detailInfo.longitude , allocator);
    markerDetailInfoValue.AddMember("targetCount", markerinfo.detailInfo.targetCount , allocator);
    markerDetailInfoValue.AddMember("targetDirection", rapidjson::StringRef(markerinfo.detailInfo.targetDirection.c_str()) , allocator);
    markerDetailInfoValue.AddMember("targetSpeed", markerinfo.detailInfo.targetSpeed , allocator);
    markerDetailInfoValue.AddMember("targetState", markerinfo.detailInfo.targetState , allocator);
    markerDetailInfoValue.AddMember("isWeapon", markerinfo.detailInfo.isWeapon , allocator);
    markerDetailInfoValue.AddMember("mHitRadius", markerinfo.detailInfo.hitRadius , allocator);
    markerDetailInfoValue.AddMember("targetType", rapidjson::StringRef(markerinfo.detailInfo.targetType.c_str()) , allocator);

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    markerDetailInfoValue.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
    // puts(sb.GetString());
    // str = sb.GetString();
    // val.AddMember("markerDetailInfo", rapidjson::StringRef(sb.GetString()), allocator);
    val.AddMember("markerDetailInfo", rapidjson::Value(sb.GetString(), allocator), allocator);

    val.AddMember("jbMarkerCode", markerinfo.jbMarkerCode , allocator);
    val.AddMember("jbColor", rapidjson::StringRef(markerinfo.jbColor.c_str()) , allocator);
    val.AddMember("setOption", rapidjson::StringRef(markerinfo.setOption.c_str()) , allocator);
    val.AddMember("addMarkerTime", markerinfo.addMarkerTime , allocator);
    val.AddMember("delMarkerTime", markerinfo.delMarkerTime , allocator);
    val.AddMember("updateMarkerTime", markerinfo.updateMarkerTime , allocator);
    val.AddMember("publisherUserId", rapidjson::StringRef(markerinfo.publisherUserId.c_str()) , allocator);
    val.AddMember("elevation", rapidjson::StringRef(markerinfo.elevation.c_str()) , allocator);
    val.AddMember("intent", rapidjson::StringRef(markerinfo.intent.c_str()) , allocator);

}

void TsAnaResult2Js(StTsAnaResultOutput& result, std::string& str)
{
    spdlog::debug("TsAnaResult2Js start!");
    rapidjson::Document dom;
    dom.SetObject();
    rapidjson::Document::AllocatorType& allocator = dom.GetAllocator();
    rapidjson::Value v;
    v.SetInt(1);
    dom.AddMember("type", v, allocator);

    v.SetString("mapPlugin/ai");
    dom.AddMember("api", v, allocator);

    rapidjson::Value dataValue(rapidjson::Type::kArrayType);

    for(auto &dataElement:result.data)
    {
        rapidjson::Value dataElementValue(rapidjson::Type::kObjectType);
        dataElementValue.AddMember("type", dataElement.i32Type, allocator);
        dataElementValue.AddMember("longitude", dataElement.longitude, allocator);
        dataElementValue.AddMember("latitude", dataElement.latitude, allocator);
        dataElementValue.AddMember("angle", dataElement.angle, allocator);

        rapidjson::Value lineValue(rapidjson::Type::kArrayType);
        rapidjson::Value intentValue(rapidjson::Type::kArrayType);
        rapidjson::Value shapeValue(rapidjson::Type::kArrayType);
        rapidjson::Value forceValue(rapidjson::Type::kArrayType);

        rapidjson::Value intentElementValue(rapidjson::Type::kObjectType);
        MarkerInfoSt2Js(dataElement.intent[0], intentElementValue, allocator);
        intentValue.PushBack(intentElementValue, allocator);

        for(auto &pt:dataElement.line)
        {
            rapidjson::Value MarkerInfoValue(rapidjson::Type::kObjectType);
            MarkerInfoSt2Js(pt, MarkerInfoValue, allocator);
            lineValue.PushBack(MarkerInfoValue, allocator);
        }

        dataElementValue.AddMember("line", lineValue, allocator);
        dataElementValue.AddMember("intent", intentValue, allocator);

        for(auto &pt:dataElement.shape)
        {
            rapidjson::Value MarkerInfoValue(rapidjson::Type::kObjectType);
            MarkerInfoSt2Js(pt, MarkerInfoValue, allocator);
            shapeValue.PushBack(MarkerInfoValue, allocator);
        }

        dataElementValue.AddMember("shape", shapeValue, allocator);

        for(auto &pt:dataElement.force)
        {
            rapidjson::Value MarkerInfoValue(rapidjson::Type::kObjectType);
            MarkerInfoSt2Js(pt, MarkerInfoValue, allocator);
            forceValue.PushBack(MarkerInfoValue, allocator);
        }

        dataElementValue.AddMember("force", forceValue, allocator);

        dataValue.PushBack(dataElementValue, allocator);
    }

    dom.AddMember("data", dataValue, allocator);

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    dom.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
    // puts(sb.GetString());
    // str = sb.GetString();
    str = std::string(sb.GetString());
    spdlog::debug("TsAnaResult2Js end!");
}

void IntelResult2Js(StIntelFusResultData& result, std::string& str)
{
    spdlog::debug("IntelResult2Js start!");
    rapidjson::Document dom;
    dom.SetObject();
    rapidjson::Document::AllocatorType& allocator = dom.GetAllocator();
    rapidjson::Value v;
    v.SetInt(2);
    dom.AddMember("type", v, allocator);

    rapidjson::Value dataValue(rapidjson::Type::kObjectType);
    rapidjson::Value ObjsValue(rapidjson::Type::kArrayType);
    dataValue.AddMember("text", rapidjson::StringRef(result.text.c_str()), allocator);

    for(auto &obj:result.objs)
    {
        rapidjson::Value ObjValue(rapidjson::Type::kObjectType);
        ObjValue.AddMember("id", obj.id, allocator);
        ObjValue.AddMember("mapType", rapidjson::StringRef(obj.mapType.c_str()), allocator);
        ObjValue.AddMember("camp", rapidjson::StringRef(obj.camp.c_str()), allocator);
        ObjValue.AddMember("isWeapon", obj.isWeapon, allocator);
        ObjValue.AddMember("lon_lat", rapidjson::StringRef(obj.lon_lat.c_str()), allocator);
        ObjValue.AddMember("obj_bbox", rapidjson::StringRef(obj.obj_bbox.c_str()), allocator);
        ObjValue.AddMember("sTime", rapidjson::StringRef(obj.sTime.c_str()), allocator);
        ObjValue.AddMember("sThreatDegree", rapidjson::StringRef(obj.sThreatDegree.c_str()), allocator);
        ObjValue.AddMember("trend", rapidjson::StringRef(obj.trend.c_str()), allocator);

        ObjsValue.PushBack(ObjValue, allocator);
    }

    dataValue.AddMember("map", ObjsValue, allocator);

    dom.AddMember("data", dataValue, allocator);

    v.SetString("mapPlugin/ai");
    dom.AddMember("api", v, allocator);

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    dom.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
    // puts(sb.GetString());
    // str = sb.GetString();
    str = std::string(sb.GetString());

    spdlog::debug("IntelResult2Js end!");
}

void ThreatResult2Js(StThreatResultOutput& result, std::string& str)
{
    spdlog::debug("ThreatResult2Js start!");
    rapidjson::Document dom;
    dom.SetObject();
    rapidjson::Document::AllocatorType& allocator = dom.GetAllocator();
    rapidjson::Value v;
    v.SetInt(3);
    dom.AddMember("type", v, allocator);

    rapidjson::Value dataValue(rapidjson::Type::kArrayType);

    for(auto& dataElement:result.data)
    {
        rapidjson::Value dataElementValue(rapidjson::Type::kObjectType);
        dataElementValue.AddMember("id", dataElement.id, allocator);
        dataElementValue.AddMember("targetName", rapidjson::StringRef(dataElement.targetName.c_str()), allocator);
        dataElementValue.AddMember("grade", rapidjson::StringRef(dataElement.grade.c_str()), allocator);

        dataValue.PushBack(dataElementValue, allocator);
    }

    dom.AddMember("data", dataValue, allocator);

    v.SetString("mapPlugin/ai");
    dom.AddMember("api", v, allocator);
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    dom.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
    // puts(sb.GetString());
    // str = sb.GetString();
    str = std::string(sb.GetString());

    spdlog::debug("ThreatResult2Js end!");
}