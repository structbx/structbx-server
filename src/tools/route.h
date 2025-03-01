
#ifndef STRUCTBX_TOOLS_ROUTE
#define STRUCTBX_TOOLS_ROUTE


#include <string>
#include <vector>
#include <functional>

#include "Poco/URI.h"


namespace StructBX
{
    namespace Tools
    {
        enum class RouteType;
        class Route;
    }
}

using namespace Poco;


enum class StructBX::Tools::RouteType
{
    kEntrypoint
    ,kEndpoint
};


class StructBX::Tools::Route
{
    public:
        Route();
        Route(std::string route);
        Route(std::vector<std::string> segments);
        ~Route();

        std::string get_route() const { return route_; }
        RouteType get_current_route_type() const {return current_route_type_;}
        std::vector<std::string> get_segments() const {return segments_;}

        void set_current_route_type(RouteType current_route_type) {current_route_type_ = current_route_type;}
        void set_segments(std::vector<std::string> segments) {segments_ = segments;}

        std::string SegmentsToString_();
        bool operator==(const Route& route);

    protected:
        void StringToSegment_(std::string& route);
        void IdentifyRouteType_();

    private:
        std::string route_;
        RouteType current_route_type_;
        std::vector<std::string> segments_;
};

#endif // STRUCTBX_TOOLS_ROUTE
