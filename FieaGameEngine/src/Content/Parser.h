//#pragma once
//#include "../../include/FieaGameEngine/Types.h"
//#include "../../include/FieaGameEngine/ContentService.h"
//#include "json/json.h"
//#include <fstream>
//
//namespace Fiea::Engine::Content
//{
//    //Content Service will own handlers in step 3, so make this a reference for now
//    using HandleVect = std::vector<std::unique_ptr<IParseHandler>>;
//
//    class Parser
//    {
//    public:
//        
//        Parser(HandleVect& handler);                                   // Inputs: a sequential collection of IParseHandler
//        Parser() = delete;
//        ~Parser() = default;
//
//        bool DeserializeFromString(string& jsonTxt, ParseWriter& riter);                   // Inputs: a string of json text, and a ParseWriter
//        bool DeserializeFromIStream(std::istream& jsonStream, ParseWriter& riter);      // Inputs: a std::istream to read json from, and a ParseWriter
//        bool DeserializeFromFile(string& filepath, ParseWriter& riter);                 // Inputs: a string containing a path to a file containing json text, and a ParseWriter
//
//        //void BeginHandlers();
//        //void EndHandlers();
//    private:
//        bool DeserializeJson(std::istream& jsonStream, Json::Value& root);
//
//        bool ParseField(const Json::String& name, const Json::Value& field, ParseWriter& riter);  // could be used to wrap the processing of a single key->value pair 
//        bool ParseObject(const Json::Value& object, ParseWriter& riter);  // could be used when encountering an object (curly brace pair) to recursively process the fields within it
//        bool ParseArray(const Json::String& name, const Json::Value& array, ParseWriter& riter);  // could be used when encountering an array (square brace pair) to recursively process the elements within it
//
//        HandleVect& _Handler;
//    };
//}