#include "Support/AttributeBox.h"
#include "Support/AttributeBoxPlus.h"

namespace Fiea::Engine::Tests::Support
{
    TEST_CLASS(AttributeBoxTester)
    {
        //TEST_MEMCHECK;
        TEST_METHOD(ConstructorTest)
        {
            AttributeBox* abox = new AttributeBox;

            abox->ExternalInteger = 5;
            Assert::AreEqual(abox->ExternalInteger, abox->Find("int")->GetInt(0));

            delete abox;
        }


        TEST_METHOD(Construction)
        {
            {// AttributeBox* abx = new AttributeBox;


                AttributeBox abox;

                abox.ExternalInteger = 5;
                Assert::AreEqual(abox.ExternalInteger, abox.Find("int")->GetInt(0));
                abox.ExternalFloat = 5.0;
                Assert::AreEqual(abox.ExternalFloat, abox.Find("float")->GetFloat(0));
                abox.ExternalString = "wahoo!";
                Assert::AreEqual(abox.ExternalString, abox.Find("string")->GetString(0));
                Foo foo(1500);
                abox.ExternalPointer = &foo;
                Assert::AreEqual(abox.ExternalPointer, (Foo*)abox.Find("Object*")->GetRTTI(0));

                for(size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    abox.ExternalIntegerArray[i] = 6;
                    Assert::AreEqual(abox.ExternalIntegerArray[i], abox.Find("intArray")->GetInt(i));
                }
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    abox.ExternalFloatArray[i] = 6.0;
                    Assert::AreEqual(abox.ExternalFloatArray[i], abox.Find("floatArray")->GetFloat(i));
                }
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    abox.ExternalStringArray[i] = "wahoo!";
                    Assert::AreEqual(abox.ExternalStringArray[i], abox.Find("stringArray")->GetString(i));
                }
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Foo foo(1500);
                    abox.ExternalPointerArray[i] = &foo;
                    Assert::AreEqual(abox.ExternalPointerArray[i], (Foo*)abox.Find("Object*Array")->GetRTTI(i));
                }

            }
            {
                AttributeBox abox;

                Datum dat1(10);
                abox.ExternalInteger = dat1.GetInt(0);
                Assert::AreEqual(abox.ExternalInteger, dat1.GetInt(0));
                Datum dat2((float)10.0);
                abox.ExternalFloat = dat2.GetFloat(0);
                Assert::AreEqual(abox.ExternalFloat, dat2.GetFloat(0));
                Datum dat3("ten");
                abox.ExternalString = dat3.GetString(0);
                Assert::AreEqual(abox.ExternalString, dat3.GetString(0));
                Foo foo(1000);
                Datum dat4(&foo);
                abox.ExternalPointer = (Foo*)dat4.GetRTTI(0);
                Assert::AreEqual(abox.ExternalPointer, (Foo*)dat4.GetRTTI(0));

                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Datum dat(16);
                    abox.ExternalIntegerArray[i] = dat.GetInt(0);
                    Assert::AreEqual(abox.ExternalIntegerArray[i], dat.GetInt(0));
                }
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Datum dat((float)16.0);
                    abox.ExternalFloatArray[i] = dat.GetFloat(0);
                    Assert::AreEqual(abox.ExternalFloatArray[i], dat.GetFloat(0));
                }
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Datum dat("sixteen");
                    abox.ExternalStringArray[i] = dat.GetString(0);
                    Assert::AreEqual(abox.ExternalStringArray[i], dat.GetString(0));
                }
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Foo foo(1600);
                    Datum dat(&foo);
                    abox.ExternalPointerArray[i] = (Foo*)dat.GetRTTI(0);
                    Assert::AreEqual(abox.ExternalPointerArray[i], (Foo*)dat.GetRTTI(0));
                }
            }
        }

        TEST_METHOD(CopyConstructionAndOpEquals)
        {
            //Test only binding
            {
                Foo foo(1500);

                AttributeBox theBox;
                theBox.ExternalInteger = 5;
                theBox.ExternalFloat = 5.0;
                theBox.ExternalString = "wahoo!";
                theBox.ExternalPointer = &foo;
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    theBox.ExternalIntegerArray[i] = 6;
                    theBox.ExternalFloatArray[i] = 6.0f;
                    theBox.ExternalStringArray[i] = "wahoo!";
                    theBox.ExternalPointerArray[i] = &foo;
                }
                AttributeBox abox = theBox;

                Assert::IsTrue(abox.ExternalInteger == theBox.ExternalInteger);

                theBox.ExternalString = "It's a me, Mario!";

                Assert::AreEqual(theBox.ExternalString, theBox.Find("string")->GetString(0));
                Assert::IsFalse(abox.ExternalString == theBox.ExternalString);
                theBox.ExternalString = "wahoo!";

                Assert::AreEqual(abox.ExternalInteger, abox.Find("int")->GetInt(0));
                Assert::AreEqual(abox.ExternalFloat, abox.Find("float")->GetFloat(0));
                Assert::AreEqual(abox.ExternalString, abox.Find("string")->GetString(0));
                Assert::AreEqual(abox.ExternalPointer, (Foo*)abox.Find("Object*")->GetRTTI(0));
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Assert::AreEqual(abox.ExternalIntegerArray[i], abox.Find("intArray")->GetInt(i));
                    Assert::AreEqual(abox.ExternalFloatArray[i], abox.Find("floatArray")->GetFloat(i));
                    Assert::AreEqual(abox.ExternalStringArray[i], abox.Find("stringArray")->GetString(i));
                    Assert::AreEqual(abox.ExternalPointerArray[i], (Foo*)abox.Find("Object*Array")->GetRTTI(i));
                }

                Assert::IsTrue(abox.ExternalInteger == theBox.ExternalInteger);
                Assert::IsTrue(abox.ExternalFloat == theBox.ExternalFloat);
                Assert::IsTrue(abox.ExternalString == theBox.ExternalString);
                Assert::IsTrue(*abox.ExternalPointer == *theBox.ExternalPointer);
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Assert::IsTrue(abox.ExternalIntegerArray[i] == theBox.ExternalIntegerArray[i]);
                    Assert::IsTrue(abox.ExternalFloatArray[i] == theBox.ExternalFloatArray[i]);
                    Assert::IsTrue(abox.ExternalStringArray[i] == theBox.ExternalStringArray[i]);
                    Assert::IsTrue(*abox.ExternalPointerArray[i] == *theBox.ExternalPointerArray[i]);
                }
            }
        }

        TEST_METHOD(CopyAssignmentOp)
        {
            //Test only binding
            {
                Foo foo(1500);

                AttributeBox theBox;
                theBox.ExternalInteger = 5;
                theBox.ExternalFloat = 5.0;
                theBox.ExternalString = "wahoo!";
                theBox.ExternalPointer = &foo;
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    theBox.ExternalIntegerArray[i] = 6;
                    theBox.ExternalFloatArray[i] = 6.0;
                    theBox.ExternalStringArray[i] = "wahoo!";
                    theBox.ExternalPointerArray[i] = &foo;
                }

                AttributeBox abox;
                abox.ExternalInteger = 10;
                abox.ExternalString = "10";
                abox = theBox;
                abox = abox;


                Assert::AreEqual(abox.ExternalInteger, abox.Find("int")->GetInt(0));
                Assert::AreEqual(abox.ExternalFloat, abox.Find("float")->GetFloat(0));
                Assert::AreEqual(abox.ExternalString, abox.Find("string")->GetString(0));
                Assert::AreEqual(abox.ExternalPointer, (Foo*)abox.Find("Object*")->GetRTTI(0));

                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Assert::AreEqual(abox.ExternalIntegerArray[i], abox.Find("intArray")->GetInt(i));
                    Assert::AreEqual(abox.ExternalFloatArray[i], abox.Find("floatArray")->GetFloat(i));
                    Assert::AreEqual(abox.ExternalStringArray[i], abox.Find("stringArray")->GetString(i));
                    Assert::AreEqual(abox.ExternalPointerArray[i], (Foo*)abox.Find("Object*Array")->GetRTTI(i));
                }

            }
        }

        TEST_METHOD(MoveConstruction)
        {
            //Test only binding
            {
                Foo foo(1500);

                AttributeBox theBox;
                theBox.ExternalInteger = 5;
                theBox.ExternalFloat = 5.0;
                theBox.ExternalString = "wahoo!";
                theBox.ExternalPointer = &foo;
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    theBox.ExternalIntegerArray[i] = 6;
                    theBox.ExternalFloatArray[i] = 6.0;
                    theBox.ExternalStringArray[i] = "wahoo!";
                    theBox.ExternalPointerArray[i] = &foo;
                }

                AttributeBox abox = std::move(theBox);

                Assert::AreEqual(abox.ExternalInteger, abox.Find("int")->GetInt(0));
                Assert::AreEqual(abox.ExternalFloat, abox.Find("float")->GetFloat(0));
                Assert::AreEqual(abox.ExternalString, abox.Find("string")->GetString(0));
                Assert::AreEqual(abox.ExternalPointer, (Foo*)abox.Find("Object*")->GetRTTI(0));

                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Assert::AreEqual(abox.ExternalIntegerArray[i], abox.Find("intArray")->GetInt(i));
                    Assert::AreEqual(abox.ExternalFloatArray[i], abox.Find("floatArray")->GetFloat(i));
                    Assert::AreEqual(abox.ExternalStringArray[i], abox.Find("stringArray")->GetString(i));
                    Assert::AreEqual(abox.ExternalPointerArray[i], (Foo*)abox.Find("Object*Array")->GetRTTI(i));
                }

            }
        }

        TEST_METHOD(MoveAssignmentOp)
        {
            //Test only binding
            {
                Foo foo(1500);

                AttributeBox theBox;
                theBox.ExternalInteger = 5;
                theBox.ExternalFloat = 5.0;
                theBox.ExternalString = "wahoo!";
                theBox.ExternalPointer = &foo;
                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    theBox.ExternalIntegerArray[i] = 6;
                    theBox.ExternalFloatArray[i] = 6.0;
                    theBox.ExternalStringArray[i] = "wahoo!";
                    theBox.ExternalPointerArray[i] = &foo;
                }

                AttributeBox abox;
                abox.ExternalInteger = 10;
                abox.ExternalString = "10";
                abox = std::move(theBox);

                Assert::AreEqual(abox.ExternalInteger, abox.Find("int")->GetInt(0));
                Assert::AreEqual(abox.ExternalFloat, abox.Find("float")->GetFloat(0));
                Assert::AreEqual(abox.ExternalString, abox.Find("string")->GetString(0));
                Assert::AreEqual(abox.ExternalPointer, (Foo*)abox.Find("Object*")->GetRTTI(0));

                for (size_t i = 0; i < AttributeBox::ARRAY_SIZE; ++i)
                {
                    Assert::AreEqual(abox.ExternalIntegerArray[i], abox.Find("intArray")->GetInt(i));
                    Assert::AreEqual(abox.ExternalFloatArray[i], abox.Find("floatArray")->GetFloat(i));
                    Assert::AreEqual(abox.ExternalStringArray[i], abox.Find("stringArray")->GetString(i));
                    Assert::AreEqual(abox.ExternalPointerArray[i], (Foo*)abox.Find("Object*Array")->GetRTTI(i));
                }

            }
        }

    };

    TEST_CLASS(AttributeBoxPlusTester)
    {
        //TEST_MEMCHECK;

        TEST_METHOD(Construction)
        {
            {
                AttributeBoxPlus abox;

                abox.ExtraData = 5;
                abox.ExtraString = "Weegee!";
                Assert::AreEqual(abox.ExtraData, abox.Find("Extra Data")->GetInt(0));
                Assert::AreEqual(abox.ExtraString, abox.Find("Extra String")->GetString(0));
            }
            {
                AttributeBoxPlus abox;

                Datum dat1(10);
                Datum dat2("20");
                abox.ExtraData = dat1.GetInt(0);
                abox.ExtraString = dat2.GetString(0);
            }
        }

        TEST_METHOD(CopyConstructionAndOpEquals)
        {
            //Test only binding
            {
                AttributeBoxPlus theBox;
                theBox.ExtraData = 5;
                theBox.ExtraString = "Mario!";
                AttributeBoxPlus abox = theBox;

                Assert::AreEqual(abox.ExtraData, abox.Find("Extra Data")->GetInt(0));
                Assert::AreEqual(abox.ExtraString, abox.Find("Extra String")->GetString(0));
                Assert::AreEqual(abox.ExtraString, theBox.ExtraString);
                Assert::AreEqual(abox.ExtraData, theBox.ExtraData);
            }
        }

        TEST_METHOD(CopyAssignmentOp)
        {
            //Test only binding
            {
                AttributeBoxPlus theBox;
                theBox.ExtraData = 5;
                theBox.ExtraString = "Mario!";
                AttributeBoxPlus abox;

                abox.ExtraData = 10;
                abox.ExtraString = "Weegee!";
                abox = theBox;
                abox = abox;

                Assert::AreEqual(abox.ExtraData, abox.Find("Extra Data")->GetInt(0));
                Assert::AreEqual(abox.ExtraString, abox.Find("Extra String")->GetString(0));
                Assert::AreEqual(abox.ExtraString, theBox.ExtraString);
                Assert::AreEqual(abox.ExtraData, theBox.ExtraData);
            }
        }

        TEST_METHOD(MoveConstruction)
        {
            //Test only binding
            {
                AttributeBoxPlus theBox;
                theBox.ExtraData = 5;
                theBox.ExtraString = "Weegee!";
                AttributeBoxPlus abox = std::move(theBox);

                Assert::AreEqual(abox.ExtraData, abox.Find("Extra Data")->GetInt(0));
                Assert::AreEqual(abox.ExtraString, abox.Find("Extra String")->GetString(0));
            }
        }

        TEST_METHOD(MoveAssignmentOp)
        {
            //Test only binding
            {
                AttributeBoxPlus theBox;
                theBox.ExtraData = 5;
                theBox.ExtraString = "Weegee!";

                AttributeBoxPlus abox;
                abox.ExtraData = 10;
                abox.ExtraString = "Mario!";
                abox = std::move(theBox);

                Assert::AreEqual(abox.ExtraData, abox.Find("Extra Data")->GetInt(0));
                Assert::AreEqual(abox.ExtraString, abox.Find("Extra String")->GetString(0));
            }
        }

    };

}
void* operator new(size_t n)
{
    return malloc(n);
}
void operator delete(void* ptr)
{
    free(ptr);
}