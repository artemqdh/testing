#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <string.h>

void Display_Questions(std::string questions_folder) // Displays questions from folder in files
{
	for (const auto& file : std::filesystem::directory_iterator(questions_folder))
	{
		std::ifstream file1(file.path());
		if (!file1.is_open())
		{
			std::cout << "Error. Could not open file: " << file.path() << std::endl;
			continue;
		}

		std::string line;
		int num = 0;
		while (getline(file1, line))
		{
			std::cout << line << std::endl;
			++num;
			if (!(num&1)) // endl if num is 1,3,5,7,9 and no endl if 2,4,6,8
			{
				std::cout << std::endl;
			}
		}
	}
}

void StartTest(std::string answers_folder) // start test + statistic in %
{
	int right_answers = 0;
	int wrong_answers = 0;
	int total_questions = 0;

	for (const auto& file : std::filesystem::directory_iterator(answers_folder))
	{
		std::ifstream file1(file.path());
		if (!file1.is_open())
		{
			std::cout << "Error. Could not open file: " << file.path() << std::endl;
			continue;
		}
		std::string line;
		std::string answer1;
		while (getline(file1, line))
		{
			++total_questions;
			std::cout << "Your answer: ";
			std::getline(std::cin >> std::ws, answer1);
			if (line == answer1)
			{
				++right_answers;
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				++wrong_answers;
				std::cout << "Wrong!" << std::endl;
			}
		}
	}

	double percentage = 0.0;
	if (total_questions > 0)
	{
		percentage = (static_cast<double>(right_answers) / total_questions) * 100;
	}
	if (percentage > 60)
	{
		std::cout << "You have passed the test with " << percentage << " percent right answers!" << std::endl;
	}
	else
	{
		std::cout << "You have not passed the test, because you have " << percentage << " percent right answers, but you need over 60%!" << std::endl;
	}
}

class Test
{
public:
	Test(std::string name_test1, std::string name_file_test_question1, std::string name_file_test_answer1, std::string category1)
	{
		name_test = name_test1;
		name_file_test_question = name_file_test_question1;
		name_file_test_answer = name_file_test_answer1;
		category = category1;
	}

private:
	std::string name_test;
	std::string name_file_test_question;
	std::string name_file_test_answer;
	std::string category;
	std::string name_statistics_test;
};


std::map <std::string, std::vector<Test>> category_and_all_test;

class User
{
public:
	virtual void Registration() = 0;
	virtual bool Check_login() = 0;
	virtual bool Check_pass() = 0;
};

class Admin : public User
{
public:
	void Registration() override // i dont have singleton
	{
		std::ofstream file1(name_login_pass_file, std::ios::app);
		if (file1.is_open())
		{
			std::string login1, pass1;
			std::cout << "Login and password: " << std::endl;
			std::cin >> login1 >> pass1;

			file1 << GetCrypt(login1) << " " << GetCrypt(pass1) << std::endl;
			file1.close();
		}
	}
	

	bool Check_login() override // not finished
	{
		std::ifstream file1(name_login_pass_file);
		std::string line;
		std::string a;
		while (getline(file1, line))
		{
			a = line;
		}

		std::string b;
		for (int i = 0; i < a.size(); ++i)
		{
			if (a[i] != ' ')
			{
				b += a[i];
			}
			else if (a[i] == ' ')
			{
				a = b;
			}
		}
		std::string word = Encryption(a);
		return true;
	}
	
	bool Check_pass() override // not finished
	{
		std::ifstream file1(name_login_pass_file);
		std::string line;
		std::string a;
		while (getline(file1, line))
		{
			a = line;
		}

		std::string b;
		for (int i = 0; i < a.size(); ++i)
		{
			if (a[i] == ' ')
			{
				++i;
				b = a.substr(i);
			}
		}
		std::string word = Encryption(b);
		return true;
	}

	std::string Encryption(std::string word)
	{
		std::string a;
		for (int i = 0; i < word.size(); ++i)
		{
			a += word[i] - 1;
		}
		return a;
	}

	void Add_Test(std::string foldername)
	{
		std::cout << "Enter the new name: ";
		std::string new_name;
		std::getline(std::cin, new_name);

		std::cout << "Enter the new question: ";
		std::string new_question;
		std::getline(std::cin, new_question);

		std::cout << "Enter the new answer: ";
		std::string new_answer;
		std::getline(std::cin, new_answer);

		std::cout << "Enter the new category: ";
		std::string new_category;
		std::getline(std::cin, new_category);

		Test new_test(new_name, new_question, new_answer, new_category);

		category_and_all_test[new_category].push_back(new_test);

		std::string filename = foldername + "/" + new_category + ".txt";
		std::ofstream file1(filename, std::ios::app);
		if (file1.is_open())
		{
			file1 << new_name << " " << new_question << std::endl << new_answer << std::endl;
			file1.close();
			std::cout << "Test added successfully." << std::endl;
		}
		else
		{
			std::cout << "Error. Something went wrong." << std::endl;
		}
	}

	void Delete_Test(std::string question)
	{
		
	}

	void Add_Tested()
	{

	}

	void Delete_Tested(std::string tested_login)
	{
		
	}

private:
	std::string GetCrypt(std::string a)
	{
		std::string a_login;
		for (int i = 0; i < a.size(); i++)
		{
			a_login.push_back(a[i] + 1);
		}
		return a_login;
	}
	std::string name_login_pass_file = "admin_login_pass.txt";
	std::string name_login_pass_file_tested = "tested_login_pass";
	std::string name_test = "new_test";
};

class Tested : public User
{
public:
	friend class Admin;
	Tested(std::string name_person_data_file1, std::string name_statistics_file1, std::string name_login_pass_file1, std::string name_file_test_incomplete1)
	{
		name_person_data_file = name_person_data_file1;
		name_statistics_file = name_statistics_file1;
		name_login_pass_file = name_login_pass_file1;
		name_file_test_incomplete = name_file_test_incomplete1;
	}

	void Registration() override
	{
		std::string name1;
		std::string adress1;
		std::int32_t phonenum1;
		std::cout << "Name (name_family), adress and phonenum(maximum 9 digits): " << std::endl;
		std::cin >> name1; 
		std::getline(std::cin >> std::ws, adress1);
		std::cin >> phonenum1;

		std::ofstream file2("all_users/" + name1 + ".txt", std::ios::app); // all_users = folder // example = all_users/artem_galimov.txt
		if (file2.is_open())
		{
			file2 << name1 << " " << adress1 << " " << phonenum1;
			file2.close();
		}
		
		std::ofstream file1("all_users/login_" + name1 + ".txt", std::ios::app); // all_users = folder // example = all_users/login_artem_galimov.txt
		if (file1.is_open())
		{
			std::string login1;
			std::string pass1;
			std::cout << "Login and password: " << std::endl;
			std::cin >> login1 >> pass1;

			file1 << GetCrypt(login1) << " " << GetCrypt(pass1) << std::endl;
			file1.close();
		}
		else
		{
			std::cout << "File not open!" << std::endl;
		}
	}

	bool Check_login() override // not finished
	{
		std::cout << "Login and password: ";
		std::string login1;
		std::string pass1;
		std::cin >> login1 >> pass1;
		
		std::ifstream file1(name_login_pass_file);
		std::string line;
		std::string a;
		while (getline(file1, line))
		{
			a = line;
		}

		std::string b;
		for (int i = 0; i < a.size(); ++i)
		{
			if (a[i] != ' ')
			{
				b += a[i];
			}
			else if (a[i] == ' ')
			{
				a = b;
			}
		}
		std::string word = Encryption(a);
		return true;
	}

	bool Check_pass() override // not finished
	{
		std::ifstream file1(name_login_pass_file);
		std::string line;
		std::string a;
		while (getline(file1, line))
		{
			a = line;
		}

		std::string b;
		for (int i = 0; i < a.size(); ++i)
		{
			if (a[i] == ' ')
			{
				++i;
				b = a.substr(i);
			}
		}
		std::string word = Encryption(b);
		return true;
	}

	std::string Encryption(std::string word)
	{
		std::string a;
		for (int i = 0; i < word.size(); ++i)
		{
			a += word[i] - 1;
		}
		return a;
	}

private:
	std::string GetCrypt(std::string a)
	{
		std::string a_login;
		for (int i = 0; i < a.size(); i++)
		{
			a_login.push_back(a[i] + 1);
		}
		return a_login;
	}
	std::string name;
	std::string address;
	std::int32_t phone_number;
	std::string name_person_data_file;
	std::string name_statistics_file;
	std::string name_login_pass_file;
	std::string name_file_test_incomplete;
};

std::vector <Tested> anyone_tested;

int main()
{
	Admin b;
	Tested a("a.txt", "b.txt", "c.txt", "d.txt");
	
	std::cout << "Welcome" << std::endl;

	std::cout << "Where do you want to login? 1 - Admin, 2 - Tested" << std::endl;
	int num1 = 0;
	std::cin >> num1;

	switch (num1)
	{
		case 1:
		{
			b.Registration();
			break;
	    }
		case 2:
		{
			a.Registration();

			std::cout << "What test do you want to start?" << std::endl;
			std::cout << "1 - basic_knowledge, 2 - mathematics and 3 - football" << std::endl;
			int num = 0;
			std::cin >> num;
			system("cls");
			switch (num)
			{
				case 1:
				{
					Display_Questions("basic_knowledge/questions");
					StartTest("basic_knowledge/answers");
					break;
				}
				case 2:
				{
					Display_Questions("mathematics/questions");
					StartTest("mathematics/answers");
					break;
				}
				case 3:
				{
					Display_Questions("football/questions");
					StartTest("football/answers");
					break;
				}
			}
		}
	}
}
