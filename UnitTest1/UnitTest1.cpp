
#include "pch.h"
#include "CppUnitTest.h"
#include "C:\Users\HP\source\repos\lab 12.5\lab 12.5\source.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			char* filename = "test_companies.dat";
			

			Company* companies = nullptr;

			// ������� � �����
			int size = readFromFile(filename, companies);

			// �������� �� ��� ���� ������ ��������
			Assert::IsTrue(size > 0, L"�� ������� ��������� ��� � �����.");

			// �������� ���'��
			delete[] companies;
		}
	};
}
		


