#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define UNICODE


#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <experimental/filesystem>


int main1()
{
	char filename[50];
	std::string format;
	printf("Enter file name for output: \n");
	scanf("%s", filename);
	printf("Enter file format: \n");

	freopen(filename, "a+", stdout);
	std::cin >> format;

	std::string path = "./";
	for (auto& p : std::experimental::filesystem::directory_iterator(path))
	{
		std::string filepath = p.path().u8string();
		if (filepath.find("." + format) != std::string::npos) {
			std::cout << filepath.substr(2) << std::endl;
			//printf("%s\n", filepath.substr(2));
		}
	}

	fclose(stdout);
	freopen("CON", "w", stdout);
	//std::cout << std::endl;
	printf("\n%s:\n\n", filename);

	FILE* f = fopen(filename, "r");

	char str[255];
	char* estr;
	
	while (true)
	{
		estr = fgets(str, sizeof(str), f);

		if (estr == NULL)
		{
			break;
		}

		printf("%s", str);
	}
	
	//_getch();
	return 0;
}

int main()
{
	main1();
}