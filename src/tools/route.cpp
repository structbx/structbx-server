
#include "route.h"

using namespace StructBX::Tools;

Route::Route() :
    route_("")
    ,current_route_type_(RouteType::kEntrypoint)
    ,segments_({})
{

}
Route::Route(std::string route) :
    route_(route)
    ,current_route_type_(RouteType::kEntrypoint)
{
    StringToSegment_(route);
    IdentifyRouteType_();
}

Route::Route(std::vector<std::string> segments) :
    current_route_type_(RouteType::kEntrypoint)
    ,segments_(segments)
{
    route_ = SegmentsToString_();
    IdentifyRouteType_();
}

Route::~Route()
{

}

void Route::StringToSegment_(std::string& route)
{
    URI(route).getPathSegments(segments_);
}

void Route::IdentifyRouteType_()
{
    if(segments_.size() > 0)
    {
        if(segments_.front() == "api")
            current_route_type_ = RouteType::kEndpoint;
        else
            current_route_type_ = RouteType::kEntrypoint;
    }
    else
        current_route_type_ = RouteType::kEntrypoint;
}

std::string Route::SegmentsToString_()
{
    std::string segments_to_string = "";
    for(auto it : segments_)
    {
        segments_to_string += "/" + it;
    }
    return segments_to_string;
}

bool Route::operator==(const Route& route)
{
    return route.get_route() == route_;
}