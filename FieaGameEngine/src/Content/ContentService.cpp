#include "../../include/FieaGameEngine/ContentService.h"
#include "Parser.h"
//using namespace Fiea::Engine::Content;


namespace Fiea::Engine
{
    class ContentService : public Singleton<ContentService>, public IJsonConfig, public IJsonParser, public IntHandler, public FloatHandler, public StringHandler
    {
    public:
        virtual size_t AddHandler(std::unique_ptr<IParseHandler>&& handler) override
        {
            _Handlers.push_back(std::move(handler));
            return (_Handlers.size() - 1);
        }
        virtual void RemoveHandler(size_t idx) override
        {
            std::unique_ptr<IParseHandler>& handler = _Handlers[idx];
            handler = nullptr;
        }

        /*virtual bool DeserializeStr(string& jsonTxt, ParseWriter& riter) override
        {
            return _P.DeserializeFromString(jsonTxt, riter);
        }
        virtual bool DeserializeIStm(std::istream& jsonStream, ParseWriter& riter) override
        {
            return _P.DeserializeFromIStream(jsonStream, riter);
        }
        virtual bool DeserializeFile(string& filepath, ParseWriter& riter) override
        {
            return _P.DeserializeFromFile(filepath, riter);
        }*/
    private:
        std::vector<std::unique_ptr<IParseHandler>> _Handlers;
        //Parser _P;
    };
}