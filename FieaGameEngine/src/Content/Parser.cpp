//#include "Parser.h"
//
//using namespace Fiea::Engine;
//
//namespace Fiea::Engine::Content
//{
//    class IntHandler;
//    class FloatHandler;
//    class StringHandler;
//
//    /// @brief 
//    /// @param handler 
//    Parser::Parser(HandleVect& handler) : _Handler(handler){}
//    /// @brief 
//    /// @param jsonTxt 
//    /// @param riter 
//    /// @return 
//    bool Parser::DeserializeFromString(string& jsonTxt, ParseWriter& riter)
//    {
//        Json::Value root;
//        std::istringstream jsonStream(jsonTxt);
//
//        if (DeserializeJson(jsonStream, root) == true)
//        {
//            bool tOrf = ParseObject(root, riter);
//            return tOrf;
//        }
//        else
//        {
//            return false;
//        }
//    }
//    /// @brief 
//    /// @param jsonStream 
//    /// @param riter 
//    /// @return 
//    bool Parser::DeserializeFromIStream(std::istream& jsonStream, ParseWriter& riter)
//    {
//        Json::Value root;
//
//        if (DeserializeJson(jsonStream, root) == true)
//        {
//            bool tOrf = ParseObject(root, riter);
//            return tOrf;
//        }
//        else
//        {
//            return false;
//        }
//    }
//    /// @brief 
//    /// @param filepath 
//    /// @param riter 
//    /// @return 
//    bool Parser::DeserializeFromFile(string& filepath, ParseWriter& riter)
//    {
//        Json::Value root;
//        std::fstream file(filepath, std::fstream::in);
//        if(file.is_open() == false)
//        {
//            FIEA_ASSERT(file.is_open() == true);
//            return false;
//        }
//
//        if (DeserializeJson(file, root) == true)
//        {
//            bool tOrf = ParseObject(root, riter);
//            file.close();
//            return tOrf;
//        }
//        else
//        {
//
//            file.close();
//            return false;
//        }
//    }
//
//    /// @brief 
//    //void Parser::BeginHandlers()
//    //{/*
//    //    std::unique_ptr<IntHandler> ihandle = new IntHandler();
//    //    std::unique_ptr<FloatHandler> fhandle = new FloatHandler();
//    //    std::unique_ptr<StringHandler> strhandle = new StringHandler();*/
//
//    //    if(_Handler.empty() == false)
//    //    {
//    //        _Handler.clear();
//    //    }/*
//    //    _Handler.push_back(ihandle);
//    //    _Handler.push_back(fhandle);
//    //    _Handler.push_back(strhandle);*/
//
//    //}
//    /// @brief 
//    //void Parser::EndHandlers()
//    //{
//    //    _Handler.clear();
//    //}
//
//    /// @brief 
//    /// @param jsonStream 
//    /// @param root 
//    /// @return 
//    bool Parser::DeserializeJson(std::istream& jsonStream, Json::Value& root)
//    {
//        try {
//            jsonStream >> root;
//        }
//        catch (const Json::RuntimeError&) {
//            // parse error
//            FIEA_ASSERT(root.empty() == false);
//            FIEA_ABORT();
//            return false;
//        }
//        return true;
//    }
//    /// @brief 
//    /// @param name 
//    /// @param field 
//    /// @param riter 
//    bool Parser::ParseField(const Json::String& name, const Json::Value& field, ParseWriter& riter)
//    {
//        for(size_t i = 0; i < _Handler.size(); ++i)
//        {
//            //Call on the child's EnterKey and ExitKey
//            if(_Handler[i]->EnterKey(riter, name, field) == true)
//            {
//                // shove data into ParseWriter
//                _Handler[i]->ExitKey(riter, name, field);
//                return true;
//            }
//        }
//        return false;
//    }
//    /// @brief Main iteration function. Recursively calls itself as well as other functions until we can store all the fields
//    /// @param object The root Json::Value value
//    /// @param riter The ParseWriter object
//    bool Parser::ParseObject(const Json::Value& object, ParseWriter& riter)
//    {
//        for(Json::Value::const_iterator itr = object.begin(); itr != object.end(); ++itr)
//        {
//            if(itr->isObject() == true)
//            {
//                riter.IncrementDepth();
//                ParseObject(*itr, riter);
//                riter.DecrementDepth();
//            }
//            else if(itr->isArray() == true)
//            {
//                ParseArray(itr.name(), *itr, riter);
//            }
//            else if(ParseField(itr.name(), *itr, riter) == false)
//            {
//                return false;
//            }
//        }
//        return true;
//    }
//    /// @brief Main Json array handling function
//    /// @param name 
//    /// @param array 
//    /// @param riter 
//    bool Parser::ParseArray(const Json::String& name, const Json::Value& array, ParseWriter& riter)
//    {
//        //const Json::Value& vals = array[name];
//        for (size_t i = 0; i < array.size(); ++i)
//        {
//            if(array[i].isObject() == true)
//            {
//                ParseObject(array[i], riter);
//            }
//            else if (ParseField(name, array[i], riter) == false)
//            {
//                return false;
//            }
//        }
//        return true;
//    }
//}