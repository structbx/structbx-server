
#ifndef STRUCTBI_FUNCTIONS_FORMSDATA
#define STRUCTBI_FUNCTIONS_FORMSDATA

#include "tools/function_data.h"
#include "tools/actions_data.h"

namespace StructBI
{
    namespace Functions
    {
        class FormsData;
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Functions::FormsData : public Tools::FunctionData
{
    public:
        FormsData(FunctionData& function_data);

    protected:
        struct ParameterVerification
        {
            bool Verify(NAF::Files::FileManager::Ptr file_manager, Query::Parameter::Ptr param, Query::Field::Ptr length, Query::Field::Ptr required, Query::Field::Ptr default_value, Query::Field::Ptr column_type);
        };
        struct FileProcessing
        {
            bool Save(NAF::Files::FileManager::Ptr file_manager, Query::Parameter::Ptr param, std::string& path);
        };

        void Read_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        Tools::ActionsData actions_;
};

#endif //STRUCTBI_FUNCTIONS_FORMSDATA