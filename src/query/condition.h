
#ifndef STRUCTBX_QUERY_CONDITION
#define STRUCTBX_QUERY_CONDITION


#include "query/results.h"
#include "tools/output_logger.h"


namespace StructBX
{
    namespace Query
    {
        enum class ConditionType;
        template <typename T> class Condition;
    }
}


enum class StructBX::Query::ConditionType
{
    kWarning
    ,kError
};

template <typename T> class StructBX::Query::Condition
{
    public:
        using Ptr = std::shared_ptr<Condition>;
        using Functor = std::function<bool(T)>;

        Condition(std::string identifier, ConditionType type, Functor functor);

        ConditionType get_type() const { return type_; }
        std::string get_identifier() const { return identifier_; }
        Functor& get_functor()
        {
            auto& var = functor_;
            return var;
        }

        void set_identifier(std::string identifier) { identifier_ = identifier; }
        void set_type(ConditionType type) { type_ = type; }
        void set_functor(Functor functor) { functor_ = functor; }

        bool VerifyCondition_(T t);

    private:
        std::string identifier_;
        ConditionType type_;
        Functor functor_;
};

template <typename T> StructBX::Query::Condition<T>::Condition(std::string identifier, ConditionType type, Functor functor) :
    identifier_(identifier)
    ,type_(type)
    ,functor_(functor)
{

}

template <typename T> bool StructBX::Query::Condition<T>::VerifyCondition_(T t)
{
    return functor_(t);
}

#endif // STRUCTBX_QUERY_CONDITION
