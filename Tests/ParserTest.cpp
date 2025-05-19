//#include "Tests.h"
//#include "../../FieaGameEngine/include/FieaGameEngine/ContentService.h"
//#include "../../FieaGameEngine/src/Content/Parser.h"
//
//namespace Fiea::Engine::Tests
//{
//    using handleVect = Fiea::Engine::Content::HandleVect;
//    using Parser = Fiea::Engine::Content::Parser;
//    using namespace std::string_literals;
//
//    TEST_CLASS(ParserTester)
//    {
//        TEST_MEMCHECK;
//
//        TEST_METHOD(Part2Test)
//        {
//            handleVect vect;
//            vect.push_back(std::make_unique<IntHandler>());
//            vect.push_back(std::make_unique<FloatHandler>());
//            vect.push_back(std::make_unique<StringHandler>());
//
//            {
//                Parser p(vect);
//
//                std::istringstream simple(R"({
//                    "int":1,
//                    "float":2.0,
//                    "str":"abc"
//                })"s);
//
//                TestWriter tRiter;
//
//                // deserialize some json
//                bool success = p.DeserializeFromIStream(simple, tRiter);
//                Assert::IsTrue(success);
//                //  Test if the number of ints per depth is accurate
//                Assert::AreEqual(1_z, tRiter.CountInts(0));
//                Assert::AreEqual(0_z, tRiter.CountInts(1));
//                Assert::AreEqual(0_z, tRiter.CountInts(2));
//                //  Test if the number of floats per depth is accurate
//                Assert::AreEqual(1_z, tRiter.CountFloats(0));
//                Assert::AreEqual(0_z, tRiter.CountFloats(1));
//                Assert::AreEqual(0_z, tRiter.CountFloats(2));
//                //  Test if the number of strings per depth is accurate
//                Assert::AreEqual(1_z, tRiter.CountStrings(0));
//                Assert::AreEqual(0_z, tRiter.CountStrings(1));
//                Assert::AreEqual(0_z, tRiter.CountStrings(2));
//            }
//
//            {
//                Parser p(vect);
//
//                string simple(R"({
//                    "int":1,
//                    "float":2.0,
//                    "str":"abc"
//                })"s);
//
//                TestWriter tRiter;
//
//                // deserialize some json
//                bool success = p.DeserializeFromString(simple, tRiter);
//                Assert::IsTrue(success);
//                //  Test if the number of ints per depth is accurate
//                Assert::AreEqual(1_z, tRiter.CountInts(0));
//                Assert::AreEqual(0_z, tRiter.CountInts(1));
//                Assert::AreEqual(0_z, tRiter.CountInts(2));
//                //  Test if the number of floats per depth is accurate
//                Assert::AreEqual(1_z, tRiter.CountFloats(0));
//                Assert::AreEqual(0_z, tRiter.CountFloats(1));
//                Assert::AreEqual(0_z, tRiter.CountFloats(2));
//                //  Test if the number of strings per depth is accurate
//                Assert::AreEqual(1_z, tRiter.CountStrings(0));
//                Assert::AreEqual(0_z, tRiter.CountStrings(1));
//                Assert::AreEqual(0_z, tRiter.CountStrings(2));
//            }
//
//            {
//                Parser p(vect);
//
//                string filepath = "../../Tests/jsonSampler.openthisfileandworkformeplz";
//                TestWriter tRiter;
//
//                // deserialize some json
//                bool success = p.DeserializeFromFile(filepath, tRiter);
//                Assert::IsTrue(success);
//                //  Test if the number of ints per depth is accurate
//                Assert::AreEqual(3_z, tRiter.CountInts(0));
//                Assert::AreEqual(0_z, tRiter.CountInts(1));
//                Assert::AreEqual(0_z, tRiter.CountInts(2));
//                //  Test if the number of floats per depth is accurate
//                Assert::AreEqual(0_z, tRiter.CountFloats(0));
//                Assert::AreEqual(2_z, tRiter.CountFloats(1));
//                Assert::AreEqual(0_z, tRiter.CountFloats(2));
//                //  Test if the number of strings per depth is accurate
//                Assert::AreEqual(0_z, tRiter.CountStrings(0));
//                Assert::AreEqual(0_z, tRiter.CountStrings(1));
//                Assert::AreEqual(4_z, tRiter.CountStrings(2));
//            }
//        }
//    };
//}