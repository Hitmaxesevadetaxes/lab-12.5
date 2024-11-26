
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

			// Читання з файлу
			int size = readFromFile(filename, companies);

			// Перевірка чи дані були успішно прочитані
			Assert::IsTrue(size > 0, L"Не вдалося прочитати дані з файлу.");

			// Очищення пам'яті
			delete[] companies;
		}
	};
}
		


