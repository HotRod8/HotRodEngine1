#pragma once
#include "Types.h"
#include "../RTTI.h"
#include "json/json.h"
#include <unordered_map>

namespace Fiea::Engine
{
    /// @brief To make this an abstract class w/o using pure virtual functions, we should destroy its constructors
    /// to prevent it from implementing any objects. Any derived classes from an abstract class should use virtual Destructor
    class ParseWriter : public RTTI
    {
        RTTI_DECLARATIONS(ParseWriter, RTTI);
    protected:
        using intMap = std::unordered_map<size_t, size_t>;
        using floatMap = std::unordered_map<size_t, size_t>;
        using stringMap = std::unordered_map<size_t, size_t>;

        ParseWriter() = default;
        //RO5
        virtual ~ParseWriter() = default;

        intMap _IntMap;
        floatMap _FloatMap;
        stringMap _StringMap;
    public:
        ParseWriter(const ParseWriter& other) = delete;
        ParseWriter(ParseWriter&& rhs) = delete;
        ParseWriter operator=(const ParseWriter& other) = delete;
        ParseWriter operator=(ParseWriter&& other) = delete;

        size_t IncrementDepth();       // increment a “nesting depth” counter. Called from Parser, as needed, when entering a nested json object
        size_t DecrementDepth();       // decrement a “nesting depth” counter. Called from Parser, as needed, when leaving a nested json object
        size_t Depth() const;          // return the current nesting depth
    private:
        size_t _Depth = 0;
        // Eventually, we want to store our deserialized data into a Scope object thru a ParseWriter.
        // For now, just make the deserialization structure.

    };

    class TestWriter : public ParseWriter
    {
        RTTI_DECLARATIONS(TestWriter, ParseWriter);
    public:
        // These are the endpoints for "writing"
        void RecordInt();    // No params, just keep track of how many integer values are encountered at each depth
        void RecordFloat();  // No params, just keep track of how many float values are encountered at each depth
        void RecordString(); // No params, just keep track of how many string values are encountered at each depth

        // These are the endpoints for reading the results, after deserialization
        size_t CountInts(size_t depth) const;    // Input: depth, Output: number of integers encountered at that depth
        size_t CountFloats(size_t depth) const;  // Input: depth, Output: number of floats encountered at that depth
        size_t CountStrings(size_t depth) const;  // Input: depth, Output: number of strings encountered at that depth
    };

    class IParseHandler
    {
    public:
        virtual void Begin() = 0;    // initialization code, when beginning to process a new json file, if any. consider empty "default" implementation?
        virtual void End() = 0;      // cleanup code, when beginning to process a new json file, if any. consider empty "default" implementation?

        virtual bool EnterKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal) = 0;
                // Inputs: a ParseWriter, a string for the Json key, a Json::Value associated with that key
                // Outputs: a value to let the Parser know whether the key was processed by this handler
                // Note: you may add/change params, if needed... 
        virtual void ExitKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal) = 0;
                // Inputs: A ParseWriter, a string for the Json key, a Json::Value associated with that key
                // Outputs: optional
                // Note: as with EnterKey, you can add/change params, if needed
    };

    //The user will create objects of these handlers and then we'll just have objects with this functionality
    class IntHandler : public IParseHandler
    {
    public:
        //IntHandler() = default;
        //virtual ~IntHandler() = default;

        virtual void Begin() override;
        virtual void End() override;

        virtual bool EnterKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal) override;
        virtual void ExitKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal) override;
    };
    class FloatHandler : public IParseHandler
    {
    public:
        //FloatHandler() = default;
        //virtual ~FloatHandler() = default;

        virtual void Begin() override;
        virtual void End() override;

        virtual bool EnterKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal) override;
        virtual void ExitKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal) override;
    };
    class StringHandler : public IParseHandler
    {
    public:
        //StringHandler() = default;
        //virtual ~StringHandler() = default;

        virtual void Begin() override;
        virtual void End() override;

        virtual bool EnterKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal) override;
        virtual void ExitKey(ParseWriter& riter, const Json::String& jsonKey, const Json::Value& jsonVal) override;
    };

    struct IJsonConfig
    {
    public:
        virtual size_t AddHandler(std::unique_ptr<IParseHandler>&&) = 0;     // register a handler to be considered during parsing
        // consider whether calling code can be relied upon to add handlers in the necessary processing order,
        // or whether some form of prioritization makes sense?
        virtual void RemoveHandler(size_t idx) = 0;  // remove a registered handler
    };

    struct IJsonParser
    {
    public:
        virtual bool DeserializeStr(string& jsonTxt, ParseWriter& riter) = 0;
        virtual bool DeserializeIStm(std::istream& jsonStream, ParseWriter& riter) = 0;
        virtual bool DeserializeFile(string& filepath, ParseWriter& riter) = 0;
    };
}