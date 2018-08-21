#include "stdafx.h"
#include "CppUnitTest.h"

#include "BRDF.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using Orca::Vec3f;

namespace OrcaTests
{		
    TEST_CLASS(TestAlgebra)
    {
    public:
        TEST_METHOD(TestAddition)
        {
            Vec3f accum(0);

            accum += Vec3f(1, -1, 1);
            accum += Vec3f(1, -1, 2);
            Assert::AreEqual(accum.x,  2.0f);
            Assert::AreEqual(accum.y, -2.0f);
            Assert::AreEqual(accum.z,  3.0f);
        }
    };

    TEST_CLASS(TestBRDF)
	{
	public:
        TEST_METHOD(TestSpecularReflDir)
        {
            Orca::SpecularBRDF brdfUnderTest;
            Vec3f inDir = { -1,0,-1 };
            Vec3f normalDir = { 0,0,1 };
            Orca::BRDFSample outDir = brdfUnderTest.generateOutSample(normalDir, inDir);
            Assert::IsTrue(normalDir.dotProduct(outDir.dir) > 0);

            inDir = { 1,0,-1 };
            outDir = brdfUnderTest.generateOutSample(normalDir, inDir);
            Assert::IsTrue(normalDir.dotProduct(outDir.dir) > 0);

            inDir = { 0,0,-1 };
            outDir = brdfUnderTest.generateOutSample(normalDir, inDir);
            Assert::IsTrue(normalDir.dotProduct(outDir.dir) > 0);
            Assert::AreEqual(outDir.dir.x, normalDir.x);
            Assert::AreEqual(outDir.dir.y, normalDir.y);
            Assert::AreEqual(outDir.dir.z, normalDir.z);
        }

        TEST_METHOD(TestSpecularReflNorm)
        {
            Orca::SpecularBRDF brdfUnderTest;
            Vec3f inDir = { 10, 1, -2 };
            inDir.normalize();
            Vec3f normalDir = { 0,0,1 };
            Orca::BRDFSample outDir = brdfUnderTest.generateOutSample(normalDir, inDir);
            float outNormal = outDir.dir.norm();
            Assert::IsTrue(std::abs(outDir.dir.norm() - 1.0f) < 1E-4f);
        }
	};
}