#include "../../include/FieaGameEngine/ContentService.h"

namespace Fiea::Engine
{
    RTTI_DEFINITIONS(ParseWriter);
    RTTI_DEFINITIONS(TestWriter);

    // ParseWriter functions



    /// @brief 
    /// @return 
    size_t ParseWriter::IncrementDepth()
    {
        return ++_Depth;
    }
    /// @brief 
    /// @return 
    size_t ParseWriter::DecrementDepth()
    {
        return --_Depth;
    }
    /// @brief 
    /// @return 
    size_t ParseWriter::Depth() const
    {
        return _Depth;
    }

    //Derived ParseWriter functions

    /// @brief 
    void TestWriter::RecordInt()
    {
        if(_IntMap.find(Depth()) == _IntMap.end())
        {
            std::pair<size_t, size_t> intPair(Depth(), 1);
            _IntMap.emplace(intPair);
        }
        else
        {
            ++_IntMap.find(Depth())->second;
        }
    }
    /// @brief 
    void TestWriter::RecordFloat()
    {
        if (_FloatMap.find(Depth()) == _FloatMap.end())
        {
            std::pair<size_t, size_t> floatPair(Depth(), 1);
            _FloatMap.emplace(floatPair);
        }
        else
        {
            ++_FloatMap.find(Depth())->second;
        }
    }
    /// @brief 
    void TestWriter::RecordString()
    {
        if (_StringMap.find(Depth()) == _StringMap.end())
        {
            std::pair<size_t, size_t> stringPair(Depth(), 1);
            _StringMap.emplace(stringPair);
        }
        else
        {
            ++_StringMap.find(Depth())->second;
        }
    }

    /// @brief 
    /// @param depth 
    /// @return 
    size_t TestWriter::CountInts(size_t depth) const
    {
        size_t count = 0;
        if(_IntMap.find(depth) != _IntMap.end())
        {
            count = _IntMap.find(depth)->second;
        }
        return count;
    }
    /// @brief 
    /// @param depth 
    /// @return 
    size_t TestWriter::CountFloats(size_t depth) const
    {
        size_t count = 0;
        if (_FloatMap.find(depth) != _FloatMap.end())
        {
            count = _FloatMap.find(depth)->second;
        }
        return count;
    }
    /// @brief 
    /// @param depth 
    /// @return 
    size_t TestWriter::CountStrings(size_t depth) const
    {
        size_t count = 0;
        if(_StringMap.find(depth) != _StringMap.end())
        {
            count = _StringMap.find(depth)->second;
        }
        return count;
    }

    //Handler functions

    /// @brief 
    void IParseHandler::Begin()
    {
    
    }
    /// @brief 
    void IParseHandler::End()
    {
    
    }

    //Derived Handler functions

    /// @brief 
    void IntHandler::Begin()
    {
    }
    /// @brief 
    void IntHandler::End()
    {
    }
    /// @brief 
    /// @param riter 
    /// @param jsonKey 
    /// @param jsonVal 
    /// @return 
    bool IntHandler::EnterKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal)
    {
        if(jsonVal.type() == Json::intValue)
        {
            TestWriter& intWriter = static_cast<TestWriter&>(riter);
            intWriter.RecordInt();
            return true;
        }
        return false;
    }
    /// @brief 
    /// @param riter 
    /// @param jsonKey 
    /// @param jsonVal 
    void IntHandler::ExitKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal)
    {

    }

    /// @brief 
    void FloatHandler::Begin()
    {
    }
    /// @brief 
    void FloatHandler::End()
    {
    }
    /// @brief 
    /// @param riter 
    /// @param jsonKey 
    /// @param jsonVal 
    /// @return 
    bool FloatHandler::EnterKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal)
    {
        if (jsonVal.type() == Json::realValue)
        {
            TestWriter& floatWriter = static_cast<TestWriter&>(riter);
            floatWriter.RecordFloat();
            return true;
        }
        return false;
    }
    /// @brief 
    /// @param riter 
    /// @param jsonKey 
    /// @param jsonVal 
    void FloatHandler::ExitKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal)
    {
    }

    /// @brief 
    void StringHandler::Begin()
    {
    }
    /// @brief 
    void StringHandler::End()
    {
    }
    /// @brief 
    /// @param riter 
    /// @param jsonKey 
    /// @param jsonVal 
    /// @return 
    bool StringHandler::EnterKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal)
    {
        if (jsonVal.type() == Json::stringValue)
        {
            TestWriter& stringWriter = static_cast<TestWriter&>(riter);
            stringWriter.RecordString();
            return true;
        }
        return false;
    }
    /// @brief 
    /// @param riter 
    /// @param jsonKey 
    /// @param jsonVal 
    void StringHandler::ExitKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal)
    {
    }
}