#pragma once
#include "../../include/FieaGameEngine/Types.h"
#include "Scope.h"

namespace Fiea::Engine::Content
{
    class Attributed : public Scope
    {
        RTTI_DECLARATIONS(Attributed, Scope);
    protected:
        struct Signature
        {
            string AttributeName;
            size_t Offset;
            Datum::Type Tag;
            size_t Count;

            Signature(string name, size_t offset, Datum::Type tag, size_t count = 1) : AttributeName(name), Offset(offset), Tag(tag), Count(count) {}
        };

        struct ClassDefinition
        {
            std::vector<Signature> Sigs;
        };
        using ClassDef = const ClassDefinition(*) (void);

    public:
        static const size_t ARRAY_SIZE = 5;

        //RO5 Methods
        Attributed() = delete;
        Attributed(const Attributed& other);
        Attributed(Attributed&& rhs) noexcept;
        Attributed& operator=(const Attributed& other);
        Attributed& operator=(Attributed&& rhs) noexcept;
        virtual ~Attributed() = default;

        //custom ctor for initiallizing Attributed child with everything it needs
        Attributed(RTTI::IdType classId, ClassDefinition fieldDefFunc);
        // Scope overrides
        [[nodiscard]] Attributed* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override; 
    private:
        void BindAtts(ClassDefinition classDef);
        ClassDefinition _Defs;
    };

}