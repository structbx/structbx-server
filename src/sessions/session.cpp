
#include "sessions/session.h"

using namespace StructBX::Sessions;

Session::Session() :
    id_("")
    ,path_("/")
    ,id_user_(-1)
    ,max_age_(3600)
{
    GenerateSessionID_();
}

Session::~Session()
{

}

void Session::GenerateNewSessionID_()
{
    GenerateSessionID_();
}

void Session::GenerateSessionID_()
{
    for(int a = 0; a < 32; a++)
    {
        Random random;
        random.seed();
        id_.push_back(random.nextChar());
    }

    GenerateMD5Hash_();
}

void Session::GenerateMD5Hash_()
{
	MD5Engine md5;
	DigestOutputStream ostr(md5);
	ostr << id_;
	ostr.flush();
	const DigestEngine::Digest& digest = md5.digest();
	id_ = DigestEngine::digestToHex(digest);
}
